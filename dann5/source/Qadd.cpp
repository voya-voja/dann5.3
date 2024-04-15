#include <Qadd.h>
#include <Factory.h>
#include <Qbit.h>
#include <Qint.h>

using namespace dann5;

/*** Addition ***/

Qadd::Qadd(size_t noLastCarryBit)
	:QnaryOp(Qadd::cMark(), 2), mNoLastCarryBit(noLastCarryBit)
{}

Qadd::Qadd(const Qadd& right)
	: QnaryOp(right), mNoLastCarryBit(right.mNoLastCarryBit)
{}

Qadd::~Qadd()
{}

void Qadd::refreshOnInputs()
{
	size_t size = Qop::noqbs() + mNoLastCarryBit;
	Qcells& addCells = cells();
	addCells.resize(size);
	const Qdefs& args = Qop::inputs();
	Qnary::Sp pLeft = dynamic_pointer_cast<Qnary>(args[0]),
		pRight = dynamic_pointer_cast<Qnary>(args[1]);

	Qaddition::Carry::Sp pCarry = nullptr;
	bool checkSize = true;
	for (size_t atBit = 0; atBit < size; atBit++)
	{
		bool assignOutput = pCarry != nullptr || 
							(pLeft->noqbs() > atBit && pRight->noqbs() > atBit);
		if (checkSize && !assignOutput)
		{
			size--;
			addCells.resize(size);
			if (atBit >= size) return;
			checkSize = false;
		}
		addCells[atBit] = opAt(pLeft, pRight, atBit, pCarry, assignOutput);
	}
}

Qcell::Sp Qadd::opAt(const Qnary::Sp& pLeft, const Qnary::Sp& pRight,
			size_t atBit, Qaddition::Carry::Sp& pCarry, bool assignOutput) const
{
	size_t lSize = pLeft->noqbs(), rSize = pRight->noqbs();
	// A created Qop object is half- or full-adder Q cell operation
	QcellOp::Sp pOp = nullptr;
	if (atBit < lSize && atBit < rSize)
	{	// when both left and right operand have defined cells at the bit-level
		Qcell::Sp pLcell(as_const(*pLeft)[atBit]), 
				  pRcell(as_const(*pRight)[atBit]);
		if (atBit == 0 || pCarry == nullptr)
		{	// for bit level 0 use quantum adjusted adder circuit
            QadjustAdder adjustAdder;
			pOp = adjustAdder.process({ pLcell, pRcell } );
		}
		else
		{	// otherwise if both left and right cells are defined use adder
			// circuit
			pOp = Factory<string, QcellOp>::Instance().create(Qadder::cMark());
			pOp->inputs({ pLcell, pRcell, pCarry });
		}
	}
	else if (atBit < lSize)
	{	// when only left operand has defined cells use quantum adjusted adder  
		// circuite to add carry cell
		Qcell::Sp pLcell(as_const(*pLeft)[atBit]);
		if (pCarry == nullptr)
			pOp = dynamic_pointer_cast<QcellOp>(pLcell);
		else
        {
            QadjustAdder adjustAdder;
            pOp = adjustAdder.process({ pLcell, pCarry });
        }
	}
	else if (atBit < rSize)
	{	// when only right operand has defined cells use xor circuite to add
		// carry cell
		Qcell::Sp pRcell(as_const(*pRight)[atBit]);
		if (pCarry == nullptr)
			pOp = dynamic_pointer_cast<QcellOp>(pRcell);
		else
        {
            QadjustAdder adjustAdder;
            pOp = adjustAdder.process({ pRcell, pCarry });
        }
	}
	else // when there is no defined cells for left or right operand
	{	// return the last carry bit as a last cell of the addition operation
        return pCarry; // no need to change carry bit output
	}
	if (pOp != nullptr && assignOutput)
	{ 
        Qbit out(pOp->createOutId());
        pOp->output(out.clone());
        Qaddition::Sp pAddition = dynamic_pointer_cast<Qaddition>(pOp);
        QadjustAdder::Sp pAdjustAdder = dynamic_pointer_cast<QadjustAdder>(pOp);
        if (pAddition == nullptr || 
			(pAdjustAdder != nullptr && pAdjustAdder->isExtended()))
            pCarry = nullptr;
        else
            pCarry = as_const(*pAddition).carry();
	}
	// return the cell addition operation
	return(pOp);
}

/*** Addition of Quantum integers ***/

QaddQints::QaddQints()
	:Qadd(0)
{}

QaddQints::QaddQints(const QaddQints& right)
	: Qadd(right), mCarryoverBits(right.mCarryoverBits)
{}

QaddQints::~QaddQints()
{}

void QaddQints::add(const Qevaluations& samples)
{
	Qadd::add(samples);
	for(auto carryover: mCarryoverBits)
		carryover->add(samples);
}

string QaddQints::solution(size_t atEvltn) const
{
	string opStr(Qop::solution(atEvltn));

	Qbit& lastCarryoverBit = *dynamic_pointer_cast<Qbit>(mCarryoverBits[1]);
	Qbit& last2CarryoverBit = *dynamic_pointer_cast<Qbit>(mCarryoverBits[0]);

	// when carryover last bit != carryover 2nd last bit
	Qvalue lastValue = lastCarryoverBit.solutionValue(atEvltn);
	Qvalue last2Value = last2CarryoverBit.solutionValue(atEvltn);
	if (lastValue != last2Value)
	{
		Qint::Sp pOut = static_pointer_cast<Qint>(Qop::output());
		long long outValue = pOut->solutionValue(atEvltn);
		size_t size = pOut->noqbs();
		Bits bOut(outValue);
		// when last carryover bit == 1 add to the result (output) n-th bit = 1
		// when 2nd last carryover bit == 1 add to the result (output) n-th bit = 0
		bOut[size] = lastValue == 1;
		Qint correct(size, pOut->id(), bOut);
		// replace result value
		size_t index = opStr.find(";");
		opStr = correct.toString() + opStr.substr(index,opStr.size() - index);
	}
	return opStr;
}

void QaddQints::refreshOnInputs()
{	// NOTE: test without resizing them
	Qnaries ins = Qnaries(Qop::inputs());
	size_t nqbts0 = ins[0]->noqbs(), nqbts1 = ins[1]->noqbs();
	if (nqbts0 < nqbts1)
		ins[0]->resize(nqbts1);
	else if (nqbts0 > nqbts1)
		ins[1]->resize(nqbts0);
	Qadd::refreshOnInputs();
}
 
void QaddQints::refreshOnOutput()
{
	Qadd::refreshOnOutput();

	// Save last 2 carryover quantum bits
	Qnary::Sp pOut = static_pointer_cast<Qnary>(Qop::output());
	size_t size = pOut->noqbs();
	for (size_t at = size - 2; at < size; at++)
	{
		Qbit carryoverBit(Qaddition::Carry::Symbol(pOut->id() + to_string(at)));
		mCarryoverBits.push_back(carryoverBit.clone());
	}
}