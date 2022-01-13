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
		ull += qbit->value() * unsigned long long(pow(2, at));
		at++;
	}
	return(ull);
}

void Qwhole::resize(size_t size, Qvalue value)
{
	// if unknown, resize as Q binary
	if (noqbs() == 0 || any())
		Qbin::resize(size, value);
	// otherwise, add 0's to the end
	else
		Qbin::resize(size);
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
	return id + "/" + to_string(size) + ":" + vStr + "/";
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
	return id + "/" + to_string(size) + ":" + valueStr + "/";
}

Qwhole& Qwhole::operator=(const Qwhole& right)
{
	Qbin::operator=(right);
	return(*this);
}

Qassign<Qwhole> Qwhole::operator=(const Qexpr<Qwhole>& right)
{
	Qassign<Qwhole> assign(*this, right);
	return assign;
}

Qwhole& Qwhole::operator+=(const Qwhole& right)
{
/*	Index tSize(noqbs()), rSize(right.noqbs());
	Index size(tSize < rSize ? rSize : tSize);
	if (tSize < size)
		resize(size);

	unsigned char carry = 0;
	bool superposition = false;
	Qbits& tQbits = value(), rQbits = right.value();
	for (Index at = 0; at < size; at++)
	{
		// if right is smaller apply + operator up to last right Q bit
		Qvalue t(tQbits[at]), r(at < rSize ? rQbits[at] : 0);
		superposition = (t > 1) || (r > 1);
		if (superposition)
			tQbits[at].value(Qbit::cSuperposition);
		else
		{
			carry += t + r;
			tQbits[at].value(carry % 2);
			carry >>= 1;
		}
	}
	if (!superposition && carry == 1)
	{
		resize(size + 1);
		tQbits[size].value(carry);
	}
*/	return (*this);
}

Qassign<Qwhole> Qwhole::operator+=(const Qexpr<Qwhole>& right)
{
	Qexpr<Qwhole> expr = *this + right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qwhole& Qwhole::operator*=(const Qwhole& right)
{
/*	QcellVector tVector(this->operator const dann5::ocean::QcellVector()), rVector(right.operator const dann5::ocean::QcellVector());
	QcellMatrix xMatrix = tVector * rVector.transpose();
	QcellMatrix reversedXMatrix = xMatrix.rowwise().reverse();

	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = reversedXMatrix.rows() + reversedXMatrix.cols();
	resize(size);
	Index last = size - 1;

	int carry = 0;
	bool superpositon = false;
	// Value of 0 bit is the sum of right-most diagonal 
	Index atDiagonal = reversedXMatrix.cols() - 1;
	Qbits& qbits = value();
	for (Index at = 0; at < last; at++)
	{
		QcellVector diagonal = reversedXMatrix.diagonal(atDiagonal);
		for (Index atD = 0; atD < diagonal.rows() && !superpositon; atD++)
		{
			QcellVector qb = diagonal(atD);
			superpositon = qb->value() == Qcell::cSuperposition;
			carry += qb;
		}
		if (superpositon)
			qbits[at]->value(Qcell::cSuperposition);
		else
		{
			qbits[at]->value(carry % 2);
			carry >>= 1;
		}
		--atDiagonal;
	}
	if (superpositon)
		qbits[last].value(Qbit::cSuperposition);
	else
		qbits[last].value(carry);
*/	return *this;
}

Qassign<Qwhole> Qwhole::operator*=(const Qexpr<Qwhole>& right)
{
	Qexpr<Qwhole> expr = *this * right;
	Qassign<Qwhole> assign(*this, expr);
	return assign;
}

Qwhole& Qwhole::operator-=(const Qwhole& right)
{
/*	unsigned long long tValue(*this), rValue(right);
	if (rValue > tValue)
		throw invalid_argument("The right operand (" + to_string(rValue)
			+ ") has to be lower than value of the left operand ("
			+ to_string(tValue) + ")");

	unsigned long long result = tValue - rValue;
	size_t size = size_t(log(result) / log(2) + 1);
	resize(size);
	Qbits& qbits = value();
	for (size_t at = 0; at < size; at++)
	{
		qbits[at].value(result % 2);
		result >>= 1;
	}
*/	return *this;
}

Qwhole& Qwhole::operator/=(const Qwhole& right)
{
/*	unsigned long long tValue(*this), rValue(right);
	if (rValue > tValue)
		throw invalid_argument("The right operand (" + to_string(rValue)
			+ ") has to be lower than value of the left operand ("
			+ to_string(tValue) + ")");

	unsigned long long result = tValue / rValue;
	size_t size = size_t(log(result) / log(2) + 1);
	resize(size);
	Qbits & qbits = value();
	for (size_t at = 0; at < size; at++)
	{
		qbits[at].value(result % 2);
		result >>= 1;
	}
*/	return *this;
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