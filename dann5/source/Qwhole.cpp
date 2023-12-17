#include <Qwhole.h>
#include <Factory.h>
#include <Qadd.h>
#include <QderivedOps.h>
#include <Qmultiply.h>
#include <Qint.h>
#include <Qfunc.h>

using namespace dann5;

const Qwhole Qwhole::_0("_0", 0);
const Qwhole Qwhole::_1("_1", 1);

Qwhole::operator unsigned long long()
{
	return(operator const unsigned long long());
}

Qwhole::operator const unsigned long long() const
{
	if (any(cSuperposition))
		return Qwhole::cUnknown;
	unsigned long long ull = 0;
	size_t at = 0;
	for (auto qbit : cells())
	{
		ull += qbit->value() * (unsigned long long)(pow(2, at));
		at++;
	}
	return(ull);
}

void Qwhole::resize(size_t size, Qvalue value)
{
	// if unknown, resize as Q binary
	if (noqbs() == 0 || any())
		Qbin::resize(size);
	// otherwise, add 0's to the end
	else
		Qbin::resize(size, 0);
}

string Qwhole::toString(bool decomposed, size_t forBit) const
{
	if (decomposed) return Qnary::toString(decomposed, forBit);
	string vStr = Qnary::cUnknownSign;
	if(!any())
	{
		unsigned long long value = (*this);
		vStr = to_string(value);
	}
	size_t size = noqbs();
	string id = Qdef::toString(decomposed);
	return id + "\\" + to_string(size) + ":" + vStr + "\\";
}

string Qwhole::solution(size_t atEvltn) const
{
	const Qcells& _cells = cells();
	size_t size = noqbs();
	Bits value;
	for (size_t at = 0; at < size; at++)
		value[at] = _cells[at]->solutionValue(atEvltn);

	string valueStr = to_string(value.to_ullong());
	string id = Qdef::toString();
	return id + "\\" + to_string(size) + ":" + valueStr + "\\";
}

vector<unsigned long long>  Qwhole::results() const
{
	const Qcells& _cells = cells();
	size_t size = noqbs();
	size_t noSolutions = _cells[0]->solutions().size();
	vector<unsigned long long> rslts;
	for (size_t sampleId = 0; sampleId < noSolutions; sampleId++)
	{
		Bits value;
		for (size_t at = 0; at < size; at++)
		{
			value[at] = _cells[at]->solutionValue(sampleId);
		}
		rslts.push_back(value.to_ullong());
	}
	return rslts;
}

vector<ULint>  Qwhole::ulints() const
{
    const Qcells& _cells = cells();
    size_t size = noqbs();
    size_t noSolutions = _cells[0]->solutions().size();
    vector<ULint> rslts;
    for (size_t sampleId = 0; sampleId < noSolutions; sampleId++)
    {
		ULint value;
        for (size_t at = 0; at < size; at++)
        {
            value.set(at, _cells[at]->solutionValue(sampleId));
        }
        rslts.push_back(value);
    }
    return rslts;
}

Qassign<Qwhole> Qwhole::operator=(const Qwhole& right) const
{
	Qexpr<Qwhole> expr = right == right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator=(const Qexpr<Qwhole>& right) const
{
	Qassign<Qwhole> assign(*this, right);
	return assign;
}

Qassign<Qwhole> Qwhole::operator+=(const Qwhole& right) const
{
	Qexpr<Qwhole> expr = *this + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator+=(const Qexpr<Qwhole>& right) const
{
	Qexpr<Qwhole> expr = *this + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator*=(const Qwhole& right) const
{
	Qexpr<Qwhole> expr = *this * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator*=(const Qexpr<Qwhole>& right) const
{
	Qexpr<Qwhole> expr = *this * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator-=(const Qwhole& right) const
{
	Qexpr<Qwhole> expr = *this - right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator-=(const Qexpr<Qwhole>& right) const
{
	Qexpr<Qwhole> expr = *this - right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator/=(const Qwhole& right) const
{
	Qexpr<Qwhole> expr = *this / right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator/=(const Qexpr<Qwhole>& right) const
{
	Qexpr<Qwhole> expr = *this / right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qexpr<Qwhole> Qwhole::operator==(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryEq::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator==(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryEq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryNeq::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryLt::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryLt::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryLe::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryLe::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryGt::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryGt::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qwhole& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryGe::cMark);
    pOp->operands(clone(), {right.clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qexpr<Qwhole>& right) const
{
    QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(QnaryGe::cMark);
    pOp->operands(clone(), {right.rootDef()->clone()});
    Qexpr<Qwhole> expr(pOp);
    return expr;
}

Qexpr<Qwhole> Qwhole::operator+(const Qwhole& right) const
{
	Qop::Sp pOp(new Qadd());
	Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator+(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qadd());
	Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qwhole& right) const
{
	Qop::Sp pOp(new Qmultiply());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qmultiply());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator-(const Qwhole& right) const
{
	Qop::Sp pOp(new Qsubtract());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator-(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qsubtract());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qwhole& right) const
{
	Qop::Sp pOp(new Qdivide());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qdivide());
    Qwhole out(pOp->createOutId());
    pOp->operands(out.clone(), {clone(), right.rootDef()->clone()});
	Qexpr<Qwhole> expr(pOp);
	return expr;
}
