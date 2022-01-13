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
		mValue = cSuperposition;
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

string Qbool::toString(bool decomposed, size_t forBit) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	valueStr.append(1,mValue);
	return id + "/" + valueStr + "/";
}

void Qbool::add(const Qsolver::Samples& samples)
{
	if (value() != cSuperposition) return;

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
	if (mValue == cSuperposition) 
		return Qcell::solution(sampleId);
	else
	{
		string sStr = id() + "/";
		sStr.append(1, mValue);
		sStr += "/";
		return sStr;
	}
}

Qassign<Qbool> Qbool::operator=(const Qbool& right)
{
	Qexpr<Qbool> expr = *this == right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator=(const Qexpr<Qbool>& right)
{
	Qassign<Qbool> assign(*this, right);
	return assign;
}

Qassign<Qbool> Qbool::operator&=(const Qbool& right)
{
	Qexpr<Qbool> expr = *this & right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator&=(const Qexpr<Qbool>& right)
{
	Qexpr<Qbool> expr = *this & right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator|=(const Qbool& right)
{
	Qexpr<Qbool> expr = *(this) | right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator|=(const Qexpr<Qbool>& right)
{
	Qexpr<Qbool> expr = *(this) | right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator^=(const Qbool& right)
{
	Qexpr<Qbool> expr = *(this) ^ right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator^=(const Qexpr<Qbool>& right)
{
	Qexpr<Qbool> expr = *(this) ^ right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}


Qexpr<Qbool> Qbool::operator !() const
{
	Qbool inverted("!" + id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(inverted.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator &(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator |(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator &(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator |(const Qexpr<Qbool>& right) const
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

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator==(const Qexpr<Qbool>& right) const 
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qexpr<Qbool>& right) const 
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::alike(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::alike(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::unlike(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::unlike(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbool out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}