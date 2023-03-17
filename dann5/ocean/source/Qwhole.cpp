#include <Qwhole.h>
#include <Factory.h>
#include <Qadd.h>
#include <QderivedOps.h>
#include <Qmultiply.h>
#include <Qint.h>
#include <Qroutine.h>

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
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ clone() });
	pOp->output(right.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator==(const Qexpr<Qwhole>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
	pOp->inputs({ right.rootDef() });
	pOp->output(clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine("!=");
	{
		Qwhole out(noqbs(), pRtn->outId());
		size_t size = out.noqbs();
		if (size < 2)
			out.resize(2);
		Qexpr<Qbit> qbXpr;
		qbXpr = out[1] | out[0];
		for (size_t at = 2; at < size; at++)
			qbXpr = out[at] | qbXpr;
		Qbit _1("_1", 1);
		*pRtn = Qwhole(right) = out + *this,
			_1 = qbXpr;
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qexpr<Qwhole>& right) const
{
	Qroutine* pRtn = new Qroutine("!=");
	{
		Qwhole out(noqbs(), pRtn->outId());
		size_t size = out.noqbs();
		if (size < 2)
			out.resize(2);
		Qexpr<Qbit> qbXpr;
		qbXpr = out[1] | out[0];
		for (size_t at = 2; at < size; at++)
			qbXpr = out[at] | qbXpr;
		Qbit _1("_1", 1);
		*pRtn = out = right - *this,
			_1 = qbXpr;
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine(">");
	{
		Qwhole rght(right), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = rght = out + *this,
			_0 != rght[rght.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qexpr<Qwhole>& right) const
{
	Qroutine* pRtn = new Qroutine(">");
	{
		Qwhole out(noqbs(), pRtn->outId());
		Qwhole::Sp pRout = dynamic_pointer_cast<Qwhole>(right.root()->outputs()[0]);
		Qbit _0("_0", 0);
		Qassign<Qwhole> subA = out = right - *this;
		Qexpression& subX = *subA.expression();
		Qsubtract::Sp pSubtract = static_pointer_cast<Qsubtract>(as_const(subX).root());
		QnaryOp::Sp pSbstttOp = pSubtract->substituteOp();
		Qwhole::Sp pSubXout = static_pointer_cast<Qwhole>(pSbstttOp->outputs()[0]);
		*pRtn =  subA,
			_0 != (*pSubXout)[pSubXout->noqbs() - 1];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine(">=");
	{
		Qwhole ths(*this), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 == ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qexpr<Qwhole>& right) const
{
	Qroutine* pRtn = new Qroutine(">=");
	{
		Qwhole ths(*this), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 == ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine("<");
	{
		Qwhole ths(*this), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 != ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qexpr<Qwhole>& right) const
{
	Qroutine* pRtn = new Qroutine("<");
	{
		Qwhole ths(*this), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 != ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine("<=");
	{
		Qwhole rght(right), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = rght = out + *this,
			_0 == rght[rght.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qexpr<Qwhole>& right) const
{
	Qroutine* pRtn = new Qroutine("<=");
	{
		Qwhole out(noqbs(), pRtn->outId());
		Qwhole::Sp pRout = dynamic_pointer_cast<Qwhole>(right.root()->outputs()[0]);
		Qbit _0("_0", 0);
		Qassign<Qwhole> subA = out = right - *this;
		Qexpression& subX = *subA.expression();
		Qsubtract::Sp pSubtract = static_pointer_cast<Qsubtract>(as_const(subX).root());
		QnaryOp::Sp pSbstttOp = pSubtract->substituteOp();
		Qwhole::Sp pSubXout = static_pointer_cast<Qwhole>(pSbstttOp->outputs()[0]);
		*pRtn = subA,
			_0 == (*pSubXout)[pSubXout->noqbs() - 1];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
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
	Qop::Sp pOp(new Qsubtract());
	pOp->inputs({ clone(), right.clone() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator-(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qsubtract());
	pOp->inputs({ clone(), right.rootDef() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qwhole& right) const
{
	Qop::Sp pOp(new Qdivide());
	pOp->inputs({ clone(), right.clone() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator/(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp(new Qdivide());
	pOp->inputs({ clone(), right.rootDef() });
	Qwhole out(pOp->outId());
	pOp->output(out.clone());

	Qexpr<Qwhole> expr(pOp);
	return expr;
}