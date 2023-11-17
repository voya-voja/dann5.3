#include <Qbit.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qbool.h>

using namespace dann5;

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
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
	pOp->inputs({ clone() });
	pOp->output(inverted.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnand::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnand::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnor::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnor::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qgt::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qgt::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}
Qexpr<Qbit> Qbit::operator>=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qge::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qge::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qlt::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qlt::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qle::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qle::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbit> expr(pOp);
	return expr;
}
