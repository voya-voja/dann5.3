#include <Qwhole.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qadd.h>
#include <Qmultiply.h>
#include <QwholeComp.h>
#include <Qint.h>

using namespace dann5::ocean;

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
		Qbin::resize(size, value);
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

string Qwhole::solution(size_t sampleId) const
{
	const Qcells& _cells = cells();
	size_t size = noqbs();
	Bits value;
	for (size_t at = 0; at < size; at++)
		value[at] = _cells[at]->solutionValue(sampleId);

	string valueStr = to_string(value.to_ullong());
	string id = Qdef::toString();
	return id + "\\" + to_string(size) + ":" + valueStr + "\\";
}

Qassign<Qwhole> Qwhole::operator=(const Qwhole& right)
{
	Qexpr<Qwhole> expr = right == right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator=(const Qexpr<Qwhole>& right)
{
	Qassign<Qwhole> assign(*this, right);
	return assign;
}

Qassign<Qwhole> Qwhole::operator+=(const Qwhole& right)
{
	Qexpr<Qwhole> expr = *this + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator+=(const Qexpr<Qwhole>& right)
{
	Qexpr<Qwhole> expr = *this + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator*=(const Qwhole& right)
{
	Qexpr<Qwhole> expr = *this * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator*=(const Qexpr<Qwhole>& right)
{
	Qexpr<Qwhole> expr = *this * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator-=(const Qwhole& right)
{
	// for *this = *this - right, *this is an 'assignee' after the assignment
	// so: assignee = *this - right => *this = assignee + right
	Qwhole assignee(noqbs(), "a-" + id());
	Qexpr<Qwhole> expr = assignee + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator-=(const Qexpr<Qwhole>& right)
{
	// for *this = *this - right, *this is an 'assignee' after the assignment
	// so: assignee = *this - right => *this = assignee + right
	Qwhole assignee(noqbs(), "a-" + id());
	Qexpr<Qwhole> expr = assignee + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator/=(const Qwhole& right)
{
	// for *this = *this - right, *this is an 'assignee' after the assignment
	// so: assignee = *this - right => *this = assignee + right
	Qwhole assignee(noqbs(), "a/" + id());
	Qexpr<Qwhole> expr = assignee * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qassign<Qwhole> Qwhole::operator/=(const Qexpr<Qwhole>& right)
{
	// for *this = *this - right, *this is an 'assignee' after the assignment
	// so: assignee = *this - right => *this = assignee + right
	Qwhole assignee(noqbs(), "a/" + id());
	Qexpr<Qwhole> expr = assignee * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qexpr<Qwhole> Qwhole::operator==(const Qwhole& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator==(const Qexpr<Qwhole>& right)
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ right.rootDef() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qwhole& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qexpr<Qwhole>& right)
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
	pOp->inputs({ right.rootDef() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qwhole& right) const
{
	QwholeComp comp(Factory<string, QcellOp>::Instance().create(GtQT::cMark));
	comp.inputs({ clone() });
	comp.output(right.clone());
	Qop::Sp pOp = dynamic_pointer_cast<Qop>(comp.clone());
	Qexpr<Qwhole> expr(pOp);
	
	/*
	Qwhole r(noqbs(), Qdef::id() + ">" + right.Qdef::id());
	Qexpr<Qbin> iR(!(right));
	Qexpr<Qwhole> invertRight(as_const(iR).root());
	Qexpr<Qwhole> expr(r == (*this) + invertRight);
	*/
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qwhole& right) const
{
	QwholeComp comp(Factory<string, QcellOp>::Instance().create(GeQT::cMark));
	comp.inputs({ clone() });
	comp.output(right.clone());

	Qop::Sp pOp = dynamic_pointer_cast<Qop>(comp.clone());
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qwhole& right) const
{
	QwholeComp comp(Factory<string, QcellOp>::Instance().create(LtQT::cMark));
	comp.inputs({ clone() });
	comp.output(right.clone());

	Qop::Sp pOp = dynamic_pointer_cast<Qop>(comp.clone());
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qwhole& right) const
{
/*	QwholeComp comp(Factory<string, QcellOp>::Instance().create(LeQT::cMark));
	comp.inputs({ clone() });
	comp.output(right.clone());

	Qop::Sp pOp = dynamic_pointer_cast<Qop>(comp.clone());
	Qexpr<Qwhole> expr(pOp);
	return expr;*/
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator+(const Qwhole& right) const
{
	Qop::Sp pOp(new Qadd());
	pOp->inputs({ clone(), right.clone() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator+(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qadd());
	pOp->inputs({ clone(), right.rootDef() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qwhole& right) const
{
	Qop::Sp pOp(new Qmultiply());
	pOp->inputs({ clone(), right.clone() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qmultiply());
	pOp->inputs({ clone(), right.rootDef() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator-(const Qwhole& right) const
{
	// out = *this - right in quantum world is same as *this = out - right
	Qop::Sp pOp(new Qadd());
	size_t size = noqbs();
	Qwhole out(size, pOp->outId());
	pOp->inputs({ out.clone(), right.clone() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator-(const Qexpr<Qwhole>& right) const
{
	// out = *this - right in quantum world is same as *this = out - right
	Qop::Sp pOp(new Qadd());
	size_t size = noqbs();
	Qwhole out(size, pOp->outId());
	pOp->inputs({ out.clone(), right.rootDef() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qwhole& right) const
{
	// out = *this / right in quantum world is same as *this = out * right
	Qop::Sp pOp(new Qmultiply());
	size_t size = noqbs();
	Qwhole out(size, pOp->outId());
	pOp->inputs({ out.clone(), right.clone() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qexpr<Qwhole>& right) const
{
	// out = *this / right in quantum world is same as *this = out * right
	Qop::Sp pOp(new Qmultiply());
	size_t size = noqbs();
	Qwhole out(size, pOp->outId());
	pOp->inputs({ out.clone(), right.rootDef() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}