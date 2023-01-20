#include <QderivedOps.h>
#include <Factory.h>
#include <Qbit.h>
#include <Qubo.h>

#include <Qadd.h>
#include <Qmultiply.h>

#include <Qwhole.h>

using namespace dann5::ocean;


/*** Quantum derived operation ***/

QderivedOp::QderivedOp(const string& id, const QnaryOp& op)
	:QnaryOp(id, op.noInputs()), mEq()
{
	mEq.inputs({ op.clone() });
}

QderivedOp::QderivedOp(const QderivedOp& right)
	: QnaryOp(right), mEq(right.mEq)
{}

QderivedOp::~QderivedOp()
{}

void QderivedOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	QnaryOp::output(pOut, forBit);
	refresh();
}

void QderivedOp::refresh()
{
	// out = ins[0] - ins[1] ...
	Qdef::Sp pOut = Qop::output();
	if (pOut == nullptr) return;

	Qdefs ins = Qop::inputs();
	size_t iSize = ins.size();
	if (iSize != 2) return;

	bool reduceSize = true;
	// ... is same as ins[0] = out + ins[1]
	size_t outSize = ins[0]->noqbs();
	if (pOut->noqbs() == 0)
	{
		dynamic_pointer_cast<Qnary>(pOut)->resize(outSize);
		reduceSize = false;
	}
	QnaryOp::Sp mpSubstOp = dynamic_pointer_cast<QnaryOp>(as_const(mEq).Qop::inputs()[0]);
	mpSubstOp->releaseArguments();
	Qdef::Sp pIn = ins[1];
	mpSubstOp->inputs({ pOut, pIn });
	Qwhole out(mpSubstOp->outId());
	mpSubstOp->output(out.clone());
	pIn = ins[0];
	mEq.QcellOp::output(pIn);
	if (reduceSize)
	{
		dynamic_pointer_cast<Qnary>(pOut)->resize(pOut->noqbs() - 1);
		dynamic_pointer_cast<Qnary>(ins[0])->resize(ins[0]->noqbs() - 1);
	}
	Qcells logic = Qcells(mEq);
	Qcells& subractCells = cells();
	for (auto pCell : logic)
		subractCells.push_back(pCell);
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