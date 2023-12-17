#include <Qint.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qadd.h>
#include <Qmultiply.h>

using namespace dann5;

const Qint Qint::_0("_0", 0);
const Qint Qint::_1("_1", 1);
const Qint Qint::_1_("_1_", -1);

Qint::Qint(const string& id, const Bits& value, bool asis)
	: Qbin(id, value, asis)
{
	if(!asis && noqbs() < 64)
		Qbin::resize(noqbs() + 1);
}

Qint::Qint(size_t size, const string& id, const Bits& value)
	: Qbin(size + 1, id)
{
	Qcells& bits = cells();
	size_t iSize = bits.size();
	for (size_t at = 0; at < iSize; at++)
	{
		bits[at] = Qcell::Sp(new Qbit(id + to_string(at), value[at]));
	}
}

Qint::operator long long()
{
	return(operator const long long());
}

Qint::operator const long long() const
{
	if (any(cSuperposition))
		return Qint::cUnknown;
	Bits bits = (*this);
	size_t iSize = noqbs();
	if (bits[iSize - 1] == 1)
		for (size_t at = iSize; at < bits.size(); at++)
			bits[at] = 1;
	long long ll = (long long)bits.to_ullong();
	return(ll);
}

void Qint::resize(size_t size, Qvalue value)
{
	// if unknown, resize as Q binary
	if (noqbs() == 0 || any())
		Qbin::resize(size, cSuperposition);
	// if last bit is negative, add 1's to the end
	else if ((*this)[noqbs() - 1] == 1) 
		Qbin::resize(size, 1);
	// otherwise, add 0's to the end
	else 
		Qbin::resize(size);
}

string Qint::toString(bool decomposed, size_t forBit) const
{
	if (decomposed) return Qnary::toString(decomposed, forBit);
	string vStr = Qnary::cUnknownSign;
	if (!any())
	{
		long long value = (*this);
		vStr = to_string(value);
	}
	size_t size = noqbs();
	string id = Qdef::toString(decomposed);
	return id + "\\" + to_string(size) + ":" + vStr + "\\";
}

string Qint::solution(size_t atEvltn) const
{
	const Qcells& _cells = cells();
	size_t size = noqbs();
	Bits value;
	for (size_t at = 0; at < size; at++)
		value[at] = _cells[at]->solutionValue(atEvltn);

	if (value[size - 1] == 1)
		for (size_t at = size; at < value.size(); at++)
			value[at] = 1;
	long long v = (long long)value.to_ullong();
	string valueStr = to_string(v);
	string id = Qdef::toString();
	return id + "\\" + to_string(size) + ":" + valueStr + "\\";
}

Qassign<Qint > Qint ::operator=(const Qint & right) const
{
    Qexpr<Qint > expr = right == right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator=(const Qexpr<Qint >& right) const
{
    Qassign<Qint > assign(*this, right);
    return assign;
}

Qassign<Qint > Qint ::operator+=(const Qint & right) const
{
    Qexpr<Qint > expr = *this + right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator+=(const Qexpr<Qint >& right) const
{
    Qexpr<Qint > expr = *this + right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator*=(const Qint & right) const
{
    Qexpr<Qint > expr = *this * right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator*=(const Qexpr<Qint >& right) const
{
    Qexpr<Qint > expr = *this * right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator-=(const Qint & right) const
{
    Qexpr<Qint > expr = *this - right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator-=(const Qexpr<Qint >& right) const
{
    Qexpr<Qint > expr = *this - right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator/=(const Qint & right) const
{
    Qexpr<Qint > expr = *this / right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qassign<Qint > Qint ::operator/=(const Qexpr<Qint >& right) const
{
    Qexpr<Qint > expr = *this / right;
    Qassign<Qint > assign(*this, expr);
    return assign;
}

Qexpr<Qint> Qint::operator==(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator!=(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator>(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qgt::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator>=(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qge::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator<(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qlt::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator<=(const Qint& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qle::cMark);
    pOp->operands(clone(), {right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator+(const Qint& right) const
{
	Qop::Sp pOp(new QaddQints());
	Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator+(const Qexpr<Qint>& right) const
{
	Qop::Sp pOp(new QaddQints());
	Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator*(const Qint& right) const
{
	Qop::Sp pOp(new Qmultiply());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator*(const Qexpr<Qint>& right) const
{
	Qop::Sp pOp(new Qmultiply());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});

	Qexpr<Qint> expr(pOp);
	return expr;
}

Qexpr<Qint> Qint::operator-(const Qint& right) const
{
    Qop::Sp pOp(new Qsubtract());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qint> expr(pOp);
    return expr;
}

Qexpr<Qint> Qint::operator-(const Qexpr<Qint>& right) const
{
    Qop::Sp pOp(new Qsubtract());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
    Qexpr<Qint> expr(pOp);
    return expr;
}

Qexpr<Qint> Qint::operator/(const Qint& right) const
{
    Qop::Sp pOp(new Qdivide());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
    Qexpr<Qint> expr(pOp);
    return expr;
}

Qexpr<Qint> Qint::operator/(const Qexpr<Qint>& right) const
{
    Qop::Sp pOp(new Qdivide());
    Qint out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
    Qexpr<Qint> expr(pOp);
    return expr;
}

