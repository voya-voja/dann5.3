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
	:QcellOp(id, op.noInputs()), mEq()
{
	mEq.inputs({ op.clone() });
}

QderivedOp::QderivedOp(const QderivedOp& right)
	: QcellOp(right), mEq(right.mEq)
{}

void QderivedOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	QcellOp::output(pOut, forBit);
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

	// ... is same as ins[0] = out + ins[1]
	size_t outSize = ins[0]->noqbs();
	if (pOut->noqbs() == 0)
	{
		static_pointer_cast<Qnary>(pOut)->resize(outSize);
	}
	QnaryOp::Sp mpSubstOp = static_pointer_cast<QnaryOp>(as_const(mEq).Qop::inputs()[0]);
	Qwhole out(mpSubstOp->outId());
	mpSubstOp->releaseArguments();
	QnaryOp::Sp mpNaryOp = dynamic_pointer_cast<QnaryOp>(ins[1]);
	if(mpNaryOp != nullptr)
	{
		Qdefs insNaryOp = as_const(*mpNaryOp).Qop::inputs();
		mpNaryOp->releaseArguments();
		mpNaryOp->inputs(insNaryOp);
	}
	mpSubstOp->inputs({ pOut, ins[1] });
	mpSubstOp->output(out.clone());
	mEq.QcellOp::output(ins[0]);
}

Qvalue QderivedOp::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(mEq.Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] == pOut->value()));
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
