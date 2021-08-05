#include <Qbin.h>
#include <QcellOps.h>
#include <Factory.h>

using namespace dann5::ocean;

Qbin::Qbin(size_t size, const string& id) 
	:Qtype(), Qnary(size, id) 
{
	Qcells& bits = cells();
	for (size_t at = 0; at < size; at++)
	{
		bits[at] = Qcell::Sp(new Qbit(id + to_string(at)));
	}
}

Qbin::Qbin(const string& id, const Qbits& value)
	:Qtype(), Qnary(id)
{
	Qcells& bits = cells();
	for (auto bit : value)
		bits.push_back(bit);
}

Qbin::Qbin(const string& id, const Bits& value, bool asis)
	: Qtype(), Qnary(asis ? value.size() : (value.to_ullong() == 0 ? 1 : size_t(log(value.to_ullong()) / log(2)) + 1), id)
{
	Qcells& bits = cells();
	size_t size = bits.size();
	for (size_t at = 0; at < size; at++)
	{
		bits[at] = Qcell::Sp(new Qbit(id + to_string(at), value[at]));
	}
}

Qbin& Qbin::operator=(const Qbin& right)
{
	cells(right.cells());
	id(right.Qdef::id());
	return(*this);
}

Qassign<Qbin> Qbin::operator=(const Qexpr<Qbin>& right)
{
	Qassign<Qbin> assign(*this, right);
	return assign;
}

Qbin& Qbin::operator&=(const Qbin& right)
{
	size_t size = noqbs(), rSize = right.noqbs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = noqbs();
	}
	Qcells& tCells = cells(), rCells = right.cells();
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			tCells[at]->value(tCells[at]->value() & rCells[at]->value());
		else // when right is smaller all Qbits are 0
			tCells[at]->value(0);
	return (*this);
}

Qassign<Qbin> Qbin::operator&=(const Qexpr<Qbin>& right)
{
	Qexpr<Qbin> expr = *this & right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qbin& Qbin::operator|=(const Qbin& right)
{
	size_t size = noqbs(), rSize = right.noqbs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
	}
	Qcells& tCells = cells(), rCells = right.cells();
	// if right is smaller apply | operator up to last right Q bit
	for (size_t at = 0; at < rSize; at++)
		tCells[at]->value(tCells[at]->value() | rCells[at]->value());
	return (*this);
}

Qassign<Qbin> Qbin::operator|=(const Qexpr<Qbin>& right)
{
	Qexpr<Qbin> expr = *this | right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qbin& Qbin::operator^=(const Qbin& right)
{
	size_t size = noqbs(), rSize = right.noqbs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = noqbs();
	}
	Qcells& tCells = cells(), rCells = right.cells();
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			tCells[at]->value(tCells[at]->value() ^ rCells[at]->value());
		else // when right is smaller all Qbits are 0
			tCells[at]->value(tCells[at]->value() & 0);
	return (*this);
}

Qassign<Qbin> Qbin::operator^=(const Qexpr<Qbin>& right)
{
	Qexpr<Qbin> expr = *this ^ right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qexpr<Qbin> Qbin::operator~() const
{
	Qbin inverted(noqbs(), "~" + Qdef::id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(inverted.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator&(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator&(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nand(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nand(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator|(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator|(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nor(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nor(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator^(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.clone() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator^(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
	pOp->inputs({ clone(), right.rootDef() });
	Qbin out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbin::operator==(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbin::operator==(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbin::operator!=(const Qbin& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbin::operator!=(const Qexpr<Qbin>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.rootDef());

	Qexpr<Qbool> expr(pOp);
	return expr;
}