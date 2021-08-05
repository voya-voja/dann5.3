#include <Qbit.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qbool.h>

using namespace dann5::ocean;

string Qbit::toString(bool decomposed, size_t bitLevel) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	if (mValue != gSuperposition) valueStr = to_string(mValue);
	else valueStr.append(1, gSuperposition);
	return id + "/" + valueStr + "/";
}

void Qbit::solutions(const Qsolver::Samples& samples)
{
	if (value() != gSuperposition) return;

	Qvalues& sltns = Qcell::solutionValues();
	string identity = id();
	for (auto sample : samples)
	{
		sltns.push_back(sample[identity]);
	}
}

Qbit& Qbit::operator=(const Qbit& right)
{
	mValue = right.mValue;
	id(right.id());
	return(*this);
}

Qassign<Qbit> Qbit::operator=(const Qexpr<Qbit>& right)
{
	Qassign<Qbit> assign(*this, right);
	return assign;
}

Qbit& Qbit::operator&=(const Qbit& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else
		mValue &= right.mValue;
	return(*this);
}

Qassign<Qbit> Qbit::operator&=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *this & right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qbit& Qbit::operator|=(const Qbit& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else
		mValue |= right.mValue;
	return(*this);
}

Qassign<Qbit> Qbit::operator|=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *(this) | right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qbit& Qbit::operator^=(const Qbit& right)
{
	if (mValue == gSuperposition || right.mValue == gSuperposition)
		mValue = gSuperposition;
	else
		mValue ^= right.mValue;
	return(*this);
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

Qexpr<Qbit> Qbit::operator^(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator^(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbit out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator==(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator==(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator!=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator!=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator>(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator>(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}
Qexpr<Qbool> Qbit::operator>=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator>=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator<(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator<(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator<=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbit::operator<=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}