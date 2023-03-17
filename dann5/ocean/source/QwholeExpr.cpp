#include <QwholeExpr.h>
#include <Factory.h>
#include <Qbit.h>
#include <Qroutine.h>
#include <QderivedOps.h>

using namespace dann5::ocean;

/*** Q whole expression ***/

QwholeExpr QwholeExpr::operator!=(const Qwhole& right) const
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
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator!=(const QwholeExpr& right) const
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
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator>(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine(">");
	{
		Qwhole rght(right), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = rght = out + *this,
			_0 != rght[rght.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator>(const QwholeExpr& right) const
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
		*pRtn = subA,
			_0 != (*pSubXout)[pSubXout->noqbs() - 1];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator>=(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine(">=");
	{
		Qwhole ths/*(*this)*/, out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 == ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator>=(const QwholeExpr& right) const
{
	Qroutine* pRtn = new Qroutine(">=");
	{
		Qwhole ths/*(*this)*/, out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 == ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator<(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine("<");
	{
		Qwhole ths/*(*this)*/, out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 != ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator<(const QwholeExpr& right) const
{
	Qroutine* pRtn = new Qroutine("<");
	{
		Qwhole ths/*(*this)*/, out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = ths = out + right,
			_0 != ths[ths.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator<=(const Qwhole& right) const
{
	Qroutine* pRtn = new Qroutine("<=");
	{
		Qwhole rght(right), out(noqbs(), pRtn->outId());
		Qbit _0("_0", 0);
		*pRtn = rght = out + *this,
			_0 == rght[rght.noqbs()];
	}
	Qop::Sp pOp = static_pointer_cast<Qop>(Qroutine::Sp(pRtn));
	QwholeExpr expr(pOp);
	return expr;
}

QwholeExpr QwholeExpr::operator<=(const QwholeExpr& right) const
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
	QwholeExpr expr(pOp);
	return expr;
}