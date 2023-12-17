#include <Qbool.h>
#include <QcellOps.h>
#include <Factory.h>

using namespace dann5;

const Qvalue Qbool::cTrue;  // 'T' value 84
const Qvalue Qbool::cFalse; // 'F' value 71

const Qbool Qbool::_T("_T", Qbool::cTrue);
const Qbool Qbool::_F("_F", Qbool::cFalse);

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
	return id + "\\" + valueStr + "\\";
}

void Qbool::add(const Qevaluations& evaluations)
{
	if (value() != cSuperposition) return;

	Qvalues& sltns = Qcell::solutions();
	string identity = id();
	for (auto evltn : evaluations)
	{
		Qvalue v = evltn.sample()[identity];
		if(v == 0)
			sltns.push_back(Qbool::cFalse);
		else
			sltns.push_back(Qbool::cTrue);
	}
}

string Qbool::solution(size_t atEvltn) const
{
	if (mValue == cSuperposition) 
		return Qcell::solution(atEvltn);
	else
	{
		string sStr = id() + "\\";
		sStr.append(1, mValue);
		sStr += "\\";
		return sStr;
	}
}

Qassign<Qbool> Qbool::operator=(const Qbool& right) const
{Qexpr<Qbool> expr = right == right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator=(const Qexpr<Qbool>& right) const
{
	Qassign<Qbool> assign(*this, right);
	return assign;
}

Qassign<Qbool> Qbool::operator&=(const Qbool& right) const
{
	Qexpr<Qbool> expr = *this & right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator&=(const Qexpr<Qbool>& right) const
{
	Qexpr<Qbool> expr = *this & right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator|=(const Qbool& right) const
{
	Qexpr<Qbool> expr = *(this) | right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator|=(const Qexpr<Qbool>& right) const
{
	Qexpr<Qbool> expr = *(this) | right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator^=(const Qbool& right) const
{
	Qexpr<Qbool> expr = *(this) ^ right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}

Qassign<Qbool> Qbool::operator^=(const Qexpr<Qbool>& right) const
{
	Qexpr<Qbool> expr = *(this) ^ right;
	Qassign<Qbool> assign(*this, expr);
	return assign;
}


Qexpr<Qbool> Qbool::operator !() const
{
	Qbool inverted("!" + id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(inverted.clone(), {clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator &(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
	Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator &(const Qexpr<Qbool>& right) const
{
    QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
    Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});

    Qexpr<Qbool> expr(pOp);
    return expr;
}

Qexpr<Qbool> Qbool::operator |(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
    Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()->clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator |(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
    Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator==(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator==(const Qexpr<Qbool>& right) const 
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qexpr<Qbool>& right) const 
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::alike(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::alike(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::unlike(const Qbool& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
    Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::unlike(const Qexpr<Qbool>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
    Qbool out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
Qexpr<Qbool> expr(pOp);
	return expr;
}
