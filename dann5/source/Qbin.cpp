#include <Qbin.h>
#include <QnaryOp.h>
#include <Factory.h>

using namespace dann5;

const Qbin Qbin::_0("_0", 0);
const Qbin Qbin::_1("_1", 1);

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

Qbin::operator Qbits()
{
    Qcells cells = Qcells(*this);
    Qbits bits;
    for(auto pCell: cells)
    {
        bits.push_back(static_pointer_cast<Qbit>(pCell));
    }
    return bits;
}

Qbin::operator const Qbits() const
{
    Qcells cells = Qcells(*this);
    Qbits bits;
    for(auto pCell: cells)
    {
        bits.push_back(static_pointer_cast<Qbit>(pCell));
    }
    return bits;
}

Qbit& Qbin::operator[](size_t pos)
{
	Qbit& qbit = dynamic_cast<Qbit&>(Qnary::operator[](pos));
    return qbit;
}

Qassign<Qbin> Qbin::operator=(const Qbin& right) const
{
    Qexpr<Qbin> expr = right == right;
    Qassign<Qbin> assign(*this, expr);
    return assign;
}

Qassign<Qbin> Qbin::operator=(const Qexpr<Qbin>& right) const
{
	Qassign<Qbin> assign(*this, right);
	return assign;
}

Qassign<Qbin> Qbin::operator&=(const Qbin& right) const
{
    Qexpr<Qbin> expr = *this & right;
    Qassign<Qbin> assign(*this, expr);
    return assign;
}

Qassign<Qbin> Qbin::operator&=(const Qexpr<Qbin>& right) const
{
	Qexpr<Qbin> expr = *this & right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qassign<Qbin> Qbin::operator|=(const Qbin& right) const
{
    Qexpr<Qbin> expr = *this | right;
    Qassign<Qbin> assign(*this, expr);
    return assign;
}

Qassign<Qbin> Qbin::operator|=(const Qexpr<Qbin>& right) const
{
	Qexpr<Qbin> expr = *this | right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qassign<Qbin> Qbin::operator^=(const Qbin& right) const
{
    Qexpr<Qbin> expr = *this ^ right;
    Qassign<Qbin> assign(*this, expr);
    return assign;
}

Qassign<Qbin> Qbin::operator^=(const Qexpr<Qbin>& right) const
{
	Qexpr<Qbin> expr = *this ^ right;
	Qassign<Qbin> assign(*this, expr);
	return assign;
}

Qexpr<Qbin> Qbin::operator~() const
{
	Qbin inverted(noqbs(), QnaryInvert::cMark + Qdef::id());
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryInvert::cMark);
	pOp->operands(inverted.clone(), {clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator&(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qand::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator&(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qand::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nand(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnand::cMark);
	Qbin out(pOp->createOutId());
	pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nand(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnand::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator|(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qor::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator|(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qor::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nor(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnor::cMark);
	Qbin out(pOp->createOutId());
	pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::nor(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnor::cMark);
	Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::unlike(const Qbin& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qxor::cMark);
    Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});

    Qexpr<Qbin> expr(pOp);
    return expr;
}

Qexpr<Qbin> Qbin::unlike(const Qexpr<Qbin>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qxor::cMark);
    Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});

    Qexpr<Qbin> expr(pOp);
    return expr;
}

Qexpr<Qbin> Qbin::alike(const Qbin& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnxor::cMark);
    Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});

    Qexpr<Qbin> expr(pOp);
    return expr;
}

Qexpr<Qbin> Qbin::alike(const Qexpr<Qbin>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qnxor::cMark);
    Qbin out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});

    Qexpr<Qbin> expr(pOp);
    return expr;
}

Qexpr<Qbin> Qbin::operator==(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator==(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator!=(const Qbin& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}

Qexpr<Qbin> Qbin::operator!=(const Qexpr<Qbin>& right) const
{
	QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qbin> expr(pOp);
	return expr;
}
