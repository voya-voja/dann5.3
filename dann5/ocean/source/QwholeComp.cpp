#include <QwholeComp.h>
#include <Factory.h>
#include <Qbit.h>
#include <Qubo.h>

using namespace dann5::ocean;

/*** Addition ***/

QwholeComp::QwholeComp(const QcellOp::Sp& pOp)
	:QnaryOp(pOp->id(), 1), mpOp(pOp)
{
	if (Qdef::id() != GtQT::cMark && Qdef::id() != GeQT::cMark && Qdef::id() != LtQT::cMark && Qdef::id() != LeQT::cMark)
		throw std::logic_error("The Q whole comparison operator cannot be " + Qdef::id() + " .It can be only > or >= or < or <=");
}

QwholeComp::QwholeComp(const QwholeComp& right)
	: QnaryOp(right), mpOp(right.mpOp)
{}

QwholeComp::~QwholeComp()
{}

void QwholeComp::output(const Qdef::Sp& out, size_t forBit)
{
	setOutput(out);
	refresh();
}

string QwholeComp::toString(bool decomposed, size_t forBit) const
{
	return(mpOp->toString(decomposed));
}

Qubo QwholeComp::qubo(bool finalized, size_t forBit) const
{
	return(mpOp->qubo(finalized));
}

void QwholeComp::refresh()
{
	size_t size = Qop::noqbs();
	Qnary::Sp pLeft = dynamic_pointer_cast<Qnary>(Qop::inputs()[0]),
		pRight = dynamic_pointer_cast<Qnary>(Qop::output());
	if (pLeft == nullptr || pRight == nullptr) return;

	// refresh processing logic once left (input) and right (output) operand are set
	string oprtr = LtQT::cMark;
	if (Qdef::id() == GtQT::cMark || Qdef::id() == GeQT::cMark)
		oprtr = GtQT::cMark;
	mpOp->inputs( { as_const(*pLeft)[0] } );
	mpOp->output( as_const(*pRight)[0], 0 );
	for (size_t atBit = 1; atBit < size; atBit++)
	{
		Qcell::Sp pL(as_const(*pLeft)[atBit]), pR(as_const(*pRight)[atBit]);
		QcellOp::Sp pCompOp = Factory<string, QcellOp>::Instance().create(oprtr);
		pCompOp->inputs({ pL });
		pCompOp->output(pR, atBit);
		QcellOp::Sp pEqOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
		pEqOp->inputs({ pL });
		pEqOp->output(pR, atBit);
		QcellOp::Sp pAndOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
		pAndOp->inputs({ pEqOp, mpOp });
		Qbit andOut(pAndOp->outId());
		pAndOp->output(andOut.clone());
		mpOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
		mpOp->inputs({ pCompOp, pAndOp });
		Qbit orOut(mpOp->outId());
		mpOp->output(orOut.clone());
	}
}