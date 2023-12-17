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

void QderivedOp::refreshOnInputs()
{
    Qdefs ins = Qop::inputs();
    Qdef::Sp pOut = Qop::output();
    size_t iSize = ins.size();
    if (iSize != 2) return;
    // ... is same as ins[0] = out + ins[1]
    size_t outNoqbs = ins[0]->noqbs();
//    if(outNoqbs > 2) outNoqbs--;
    if (pOut->noqbs() == 0)
    {
        Qnary::Sp pNaryOut = static_pointer_cast<Qnary>(pOut);
        pNaryOut->resize(outNoqbs);
    }
    Qwhole out(mpSubstituteOp->createOutId());
    mpSubstituteOp->operands(out.clone(), {pOut, ins[1]});
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
	:QderivedOp(Qsubtract::cMark, Qadd() )
{}


/*** Division ***/

Qdivide::Qdivide()
	:QderivedOp(Qdivide::cMark, Qmultiply())
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
