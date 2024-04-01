#include <QderivedOps.h>
#include <Factory.h>
#include <Qbit.h>

#include <Qadd.h>
#include <Qmultiply.h>

#include <Qwhole.h>
#include <Qint.h>

using namespace dann5;


/*** Quantum derived operation ***/

QderivedOp::QderivedOp(const string& id, const QnaryOp& op)
	:QnaryOp(id, op.noInputs()), mEq()
{
    mpSubstituteOp = dynamic_pointer_cast<QnaryOp>(op.clone());
}

QderivedOp::QderivedOp(const QderivedOp& right)
	: QnaryOp(right), mEq(right.mEq), mpSubstituteOp(right.mpSubstituteOp)
{}

size_t QderivedOp::noqbs() const noexcept
{ 
    const Qdefs& ins = Qop::inputs();
    if (ins.size() == 0)
        return 0;
    return ins[1]->noqbs(); 
};


void QderivedOp::resize(size_t size, Qvalue value)
{
    Qcells& logic = cells();
    size_t oSize = logic.size();
    logic.resize(size);
    Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(output());
    if (pNaryOut != nullptr)
    {
        pNaryOut = dynamic_pointer_cast<Qnary>(pNaryOut->clone());
        size_t outSize = pNaryOut->noqbs();
        for (size_t at = oSize; at < size; at++)
        {
            QnullCellOp::Sp pNullOp(new QnullCellOp());
            logic[at] = pNullOp;
            if(at < outSize)
            {
                pNullOp->output(as_const(*pNaryOut)[at], at);
                (*pNaryOut)[at].value(cSuperposition);
            }
        }
    }
}

void QderivedOp::refreshOnInputs()
{
    Qdefs ins = Qop::inputs();
    Qdef::Sp pOut = Qop::output();
    size_t iSize = ins.size();
    if (iSize != 2) return;
    // ... is same as ins[0] = out + ins[1]
    if (pOut->noqbs() == 0)
    {
        size_t outNoqbs = ins[1]->noqbs();
        Qnary::Sp pNaryOut = static_pointer_cast<Qnary>(pOut);
        pNaryOut->resize(outNoqbs);
    }
    //Qwhole subOut(mpSubstituteOp->createOutId());
    Qnary::Sp pSubOpOut = dynamic_pointer_cast<Qnary>(ins[0]->clone());
    pSubOpOut->resize(0);
    pSubOpOut->id(mpSubstituteOp->createOutId());
    mpSubstituteOp->operands(pSubOpOut, {pOut, ins[1]});
    mEq.operands(ins[0], {mpSubstituteOp});
    resize(mEq.noqbs());
}

void QderivedOp::refreshOnOutput()
{
    Qdef::Sp pOut = Qop::output();
    Qnary::Sp pSubOpOut = dynamic_pointer_cast<Qnary>(mpSubstituteOp->Qop::output());
    Qdefs ins = Qop::inputs();
    mpSubstituteOp->releaseOperands();
    pSubOpOut->resize(0);
    mpSubstituteOp->operands(pSubOpOut, {pOut, ins[1]});
    mEq.releaseOperands();
    mEq.operands(ins[0], {mpSubstituteOp});
}

/*** Substraction ***/

Qsubtract::Qsubtract()
	:QderivedOp(Qsubtract::cMark(), Qadd())
{}


/*** Division ***/

Qdivide::Qdivide()
	:QderivedOp(Qdivide::cMark(), Qmultiply())
{}

/*** Sutraction of Quantum integers ***/

QsubtractQints::QsubtractQints()
	:QderivedOp(Qsubtract::cMark(), QaddQints())
{
}

QsubtractQints::QsubtractQints(const QsubtractQints& right)
	: QderivedOp(right), mCarryoverBits(right.mCarryoverBits)
{}

QsubtractQints::~QsubtractQints()
{}


void QsubtractQints::add(const Qevaluations& samples)
{
    QderivedOp::add(samples);
    for (auto carryover : mCarryoverBits)
        carryover->add(samples);
}

string QsubtractQints::solution(size_t atEvltn) const
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
        bOut[size] = lastValue == 0;
        Qint correct(size, pOut->id(), bOut);
        // replace result value
        size_t index = opStr.find(";");
        opStr = correct.toString() + opStr.substr(index, opStr.size() - index);
    }
    return opStr;
}

void QsubtractQints::refreshOnInputs()
{
    //Qnaries ins = Qnaries(Qop::inputs());
    //size_t nqbts0 = ins[0]->noqbs(), nqbts1 = ins[1]->noqbs();
    //if (nqbts0 < nqbts1)
    //    ins[0]->resize(nqbts1);
    //else if (nqbts0 > nqbts1)
    //    ins[1]->resize(nqbts0);
    QderivedOp::refreshOnInputs();
}

void QsubtractQints::refreshOnOutput()
{
    QderivedOp::refreshOnOutput();

    // Save last 2 carryover quantum bits from substitute operation
    Qnary::Sp pOut = static_pointer_cast<Qnary>(substituteOp()->Qop::output());
    size_t size = pOut->noqbs();
    for (size_t at = size - 2; at < size; at++)
    {
        Qbit carryoverBit(Qaddition::Carry::Symbol(pOut->id() + to_string(at)));
        mCarryoverBits.push_back(carryoverBit.clone());
    }
}