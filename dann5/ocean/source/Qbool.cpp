#include <Qbool.h>
#include <QcellOps.h>
#include <Factory.h>

using namespace dann5::ocean;

void Qbool::value(Qvalue v) 
{ 
	if (v == 1 || v == cTrue)
		mValue = cTrue;
	else if (v == 0 || v == cFalse)
		mValue = cFalse;
	else
		mValue = gSuperposition;
}

Qvalue Qbool::value() const 
{
	if (mValue == cTrue)
		return 1;
	else if (mValue == cFalse)
		return 0;
	return mValue; 
}

Qvalue Qbool::value() 
{ 
	return as_const(*this).value(); 
}

string Qbool::toString(bool decomposed, size_t bitLevel) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	valueStr.append(1,mValue);
	return id + "/" + valueStr + "/";
}

void Qbool::solutions(const Qsolver::Samples& samples)
{
	if (value() != gSuperposition) return;

	Qvalues& sltns = Qcell::solutionValues();
	string identity = id();
	for (auto sample : samples)
	{
		Qvalue v = sample[identity];
		if(v == 0)
			sltns.push_back(Qbool::cFalse);
		else
			sltns.push_back(Qbool::cTrue);
	}
}

string Qbool::solution(size_t sampleId) const
{
	if (mValue == gSuperposition) 
		return Qcell::solution(sampleId);
	else
	{
		string sStr = id() + "/";
		sStr.append(1, mValue);
		sStr += "/";
		return sStr;
	}
}

Qbool& Qbool::operator=(const Qbool& right)
{
	mValue = right.mValue;
	id(right.id());
	return(*this);
}

Qassign<Qbool> Qbool::operator=(const Qexpr<Qbool>& right)
{
	Qassign<Qbool> assign(*this, right);
	return assign;
}

Qbool& Qbool::operator&=(const Qbool& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else if (mValue == cTrue && right.mValue == cTrue)
		mValue = cTrue;
	else
		mValue = cFalse;

	return(*this);
}

Qassign<Qbool> Qbool::operator&=(const Qexpr<Qbool>& right)
{
	Qexpr<Qbool> expr = *this and right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qbool& Qbool::operator|=(const Qbool& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else if (mValue == cTrue || right.mValue == cTrue)
		mValue = cTrue;
	else
		mValue = cFalse;
	return(*this);
}

Qassign<Qbool> Qbool::operator|=(const Qexpr<Qbool>& right)
{
	Qexpr<Qbool> expr = *(this) or right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qbool& Qbool::operator^=(const Qbool& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else if (mValue == right.mValue)
		mValue = cTrue;
	else
		mValue = cFalse;
	return(*this);
}

Qexpr<Qbool> Qbool::operator not() const
{
	Qbool inverted("~" + id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(inverted.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator and(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator or(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator and(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator or(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator==(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}