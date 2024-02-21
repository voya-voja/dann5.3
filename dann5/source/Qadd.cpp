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
	: Qadd(right)
{}

QaddQints::~QaddQints()
{}

string QaddQints::solution(size_t atEvltn) const
{
	string opStr(Qadd::solution(atEvltn));
	size_t valueStart = opStr.find(':') + 1;
	string valueStr = opStr.substr(valueStart, opStr.find('\\', valueStart) - valueStart);
	int result = stoi(valueStr);
	valueStart = opStr.find(':', valueStart) + 1;
	valueStr = opStr.substr(valueStart, opStr.find('\\', valueStart) - valueStart);
	int argument0 = stoi(valueStr);
	valueStart = opStr.find(':', valueStart) + 1;
	valueStr = opStr.substr(valueStart, opStr.find('\\', valueStart) - valueStart);
	int argument1 = stoi(valueStr);
	if (argument0 + argument1 != result)
	{
		Bits resultBits(result);
		string updatedStr = opStr.substr(0, opStr.find(':') + 1);
		updatedStr += to_string(argument0 + argument1);
		size_t restStart = opStr.find("\\; ");
		updatedStr += opStr.substr(restStart, opStr.size() - restStart);
		opStr = updatedStr;
 	}
	return opStr;
}

void QaddQints::refreshOnInputs()
{
	Qnaries ins = Qnaries(Qop::inputs());
	size_t nqbts0 = ins[0]->noqbs(), nqbts1 = ins[1]->noqbs();
	if (nqbts0 < nqbts1)
		ins[0]->resize(nqbts1);
	else if (nqbts0 > nqbts1)
		ins[1]->resize(nqbts0);
	Qadd::refreshOnInputs();
}
// 
//void QaddQints::refreshOnOutput()
//{
//	Qadd::refreshOnOutput();
//	Qint::Sp pIntOut = dynamic_pointer_cast<Qint>(Qop::output());
//	if (pIntOut == nullptr)
//		throw logic_error("ERROR @QaddQints: Output is not Qint or is not defined!");
//	size_t size = pIntOut->noqbs() + 1;
//	pIntOut->resize(size);
//}