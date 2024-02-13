#include <QderivedOps.h>
#include <Factory.h>
#include <Qbit.h>

#include <Qadd.h>
#include <Qmultiply.h>

#include <Qwhole.h>

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

void QderivedOp::resize(size_t size, Qvalue value)
{
    Qcells& logic = cells();
    size_t oSize = logic.size();
    logic.resize(size);
    Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(output());
    if (pNaryOut != nullptr)
    {
        pNaryOut = dynamic_pointer_cast<Qnary>(pNaryOut->clone());
        for (size_t at = oSize; at < size; at++)
        {
            QnullCellOp::Sp pNullOp(new QnullCellOp());
            logic[at] = pNullOp;
            pNullOp->output(as_const(*pNaryOut)[at], at);
            (*pNaryOut)[at].value(cSuperposition);
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
    size_t outNoqbs = ins[0]->noqbs();
    if (pOut->noqbs() == 0)
    {
        Qnary::Sp pNaryOut = static_pointer_cast<Qnary>(pOut);
        pNaryOut->resize(outNoqbs);
    }
    Qwhole subOut(mpSubstituteOp->createOutId());
    mpSubstituteOp->operands(subOut.clone(), {pOut, ins[1]});
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

/*** Sutraction of Quantum integers

QsubtractQints::QsubtractQints()
	:Qsubtract()
{
}

QsubtractQints::QsubtractQints(const QsubtractQints& right)
	: Qsubtract(right)
{}

QsubtractQints::~QsubtractQints()
{}
***/
