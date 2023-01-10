#include <Qbit.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qbool.h>

using namespace dann5::ocean;

string Qbit::toString(bool decomposed, size_t forBit) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	if (mValue != cSuperposition) valueStr = to_string(mValue);
	else valueStr.append(1, cSuperposition);
	return id + "\\" + valueStr + "\\";
}

Qassign<Qbit> Qbit::operator=(const Qbit& right)
{
	Qexpr<Qbit> expr = right == right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator=(const Qexpr<Qbit>& right)
{
	Qassign<Qbit> assign(*this, right);
	return assign;
}

Qassign<Qbit> Qbit::operator&=(const Qbit& right)
{
	Qexpr<Qbit> expr = *this & right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator&=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *this & right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator|=(const Qbit& right)
{
	Qexpr<Qbit> expr = *(this) | right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator|=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *(this) | right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator^=(const Qbit& right)
{
	Qexpr<Qbit> expr = *this ^ right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator^=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *this ^ right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qexpr<Qbit> Qbit::operator~() const
{
	Qbit inverted("~" + id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(inverted.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}
Qexpr<Qbit> Qbit::operator>=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}