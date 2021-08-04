#include <math.h> 
#include <Qequation.h>
#include <Qnary.h>
#include <Utility.h>
#include <Logger.h>

using namespace dann5::ocean;

/*** Quantum Equation ***/


Qequation::Qequation(const Qdef& result)
	: Qstatement(), mpResult(Qdef::Sp(result.clone()))
{
}

Qequation::Qequation(const Qdef& result, const Qexpression& expr)
	: Qstatement(expr), mpResult(Qdef::Sp(result.clone()))
{
	bindExpression();
}


Qequation::Qequation(const Qequation& right)
	: Qstatement(right), mpResult(right.mpResult)
{
/*	reduct(createReduct());
	reduct().init();
*/
}
/*
Qequation& Qequation::operator=(const Qequation& right)
{
	if (right.nobs() > mResult.nobs())
	{
		mResult.resize(right.nobs(), 0);
	}
	Qstatement::operator=(right);
	return(*this);
}
*/
Qequation& Qequation::operator=(const Qexpression& right)
{
	// ensure result Q variable and equation Q expression have same number-of-bits
	expression(right);
	bindExpression();
	return(*this);
}

void Qequation::bindExpression()
{
	// ensure result Q variable and equation Q expression have same number-of-bits
	if (noqbs() > mpResult->noqbs())
	{
		Qnary::Sp pResult = dynamic_pointer_cast<Qnary>(mpResult);
		if (pResult != nullptr)
			// if Qnary result, size result to have same # of Q bits as expression
			pResult->resize(noqbs());
		else
			throw std::logic_error("The Q equation result and expression are of different type!");;
	} // if expression has less Q bits, remaining Q bits or result will result 0 bitwise logic, e.g. they are removed

	expression().root().output(mpResult);
}

string Qequation::toString(bool decomposed, size_t level) const
{
	string sEquation = "";
	if (!decomposed)
	{
		sEquation = mpResult->toString(decomposed, level) + " = ";
	}
	sEquation += Qstatement::toString(decomposed, level);
	return sEquation;
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qequation& statement)
{
	out << statement.toString();
	return out;
}

/*
Qequation Qequation::operator&(const Qvar& right) const
{
	Qequation result(*this);
	result &= right;
	return(result);
}

Qequation Qequation::operator&(const Qequation& right) const
{
	Qequation result(*this);
	result &= right;
	return(result);
}

Qequation& Qequation::operator&=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() &= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("&=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator&=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult &= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() &= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("&= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator|(const Qvar& right) const
{
	Qequation result(*this);
	result |= right;
	return(result);
}

Qequation Qequation::operator|(const Qequation& right) const
{
	Qequation result(*this);
	result |= right;
	return(result);
}

Qequation& Qequation::operator|=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() |= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("|=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator|=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult |= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() |= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("|= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::nand(const Qvar& right) const
{
	Qequation result(*this);
	result.nandMutable(right);
	return(result);
}

Qequation Qequation::nand(const Qequation& right) const
{
	Qequation result(*this);
	result.nandMutable(right);
	return(result);
}

Qequation& Qequation::nandMutable(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression().nandMutable(right.definition());
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~&=\n", toString(true));
	return(*this);
}

Qequation& Qequation::nandMutable(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult.nandMutable(right.mResult);
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression().nandMutable(right.expression());
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~&= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::nor(const Qvar& right) const
{
	Qequation result(*this);
	result.norMutable(right);
	return(result);
}

Qequation Qequation::nor(const Qequation& right) const
{
	Qequation result(*this);
	result.norMutable(right);
	return(result);
}

Qequation& Qequation::norMutable(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression().norMutable(right.definition());
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~|=\n", toString(true));
	return(*this);
}

Qequation& Qequation::norMutable(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult.norMutable(right.mResult);
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression().norMutable(right.expression());
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~|= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator^(const Qvar& right) const
{
	Qequation result(*this);
	result ^= right;
	return(result);
}

Qequation Qequation::operator^(const Qequation& right) const
{
	Qequation result(*this);
	result ^= right;
	return(result);
}

Qequation& Qequation::operator^=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() ^= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("^=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator^=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult ^= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() ^= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("^= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator+(const Qvar& right) const
{
	Qequation result(*this);
	result += right;
	return(result);
}

Qequation Qequation::operator+(const Qequation& right) const
{
	Qequation result(*this);
	result += right;
	return(result);
}

Qequation& Qequation::operator+=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{	
		expression() += right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat( "+=\n", toString(true) );
	return(*this);
}

Qequation& Qequation::operator+=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult += right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() += right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+= Reducted: \n", toString(true));
	return(*this);
}

Qroutine Qequation::operator-(const Qvar& subtrahend) const
{
	// define difference variable with the same size as subtrahend argument
	Qvar difference(subtrahend.definition().size(), "dfrnc");

	// for this.result as minuend and subtrahend argment: difference = minuend - subtrahend
	// create Q equation: minuend = difference + subtrahend
	Qequation minuend(mResult);
	minuend = difference + subtrahend;

	// create subtraction routine
	string rName = "dfrnc = " + mResult.definition().name() + " - " + subtrahend.definition().name();
	Qroutine subtraction(nobs(), rName);
	subtraction << (*this) << minuend;
	return subtraction;
}

Qequation Qequation::operator*(const Qvar& right) const
{
	Qequation result(*this);
	result *= right;
	return(result);
}

Qequation Qequation::operator*(const Qequation& right) const
{
	Qequation result(*this);
	result *= right;
	return(result);
}

Qequation& Qequation::operator*=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() *= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator*=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult *= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() *= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+= Reducted: \n", toString(true));
	return(*this);
}

Qroutine Qequation::operator/(const Qvar& divisor) const
{
	// define quatient variable with same size as divisor 
	Qvar quotient(divisor.definition().size(), "qtnt");

	// for this.result as dividend and divisor argment: quotient = dividend / divisor
	// create Q equation: dividend = quotient * divisor
	Qequation dividend(mResult);
	dividend = quotient * divisor;

	// create multiplication routine
	string rName = "qtnt = " + mResult.definition().name() + " / " + divisor.definition().name();
	Qroutine multiplication(nobs(), rName);
	multiplication << (*this) << dividend;
	return multiplication;
}
*/



/*** Qequation::Reduct code***/
/*
Qequation::Reduct::Reduct(Qequation& equation)
	:Qstatement::Reduct(equation), mEquation(equation)
{
	_lc;
}

void Qequation::Reduct::init()
{
	Qstatement::Reduct::init();

	string definition(""), expression("");
	for (Index at = 0; at < mEquation.nobs(); at++)
	{
		// capture result expression and reduction
		Qoperand::Sp pOperand = mEquation.expression()(at);
		if (pOperand != nullptr)
		{
			expression = pOperand->toString(false);
			definition = as_const(mEquation.mResult).definition()(at)->identity();
			Qbit value = as_const(mEquation.mResult).value()(at);
			Reduction reduction(definition, value);
			reductions()[expression] = reduction;

			// capture expression of carry forward operands in expression
			expression = Qaddition::Carry::Symbol(expression);
			definition = Qaddition::Carry::Symbol(definition);
			value = Qbit::gSuperposition;
			Reduction carryCorrect(definition, value);
			reductions()[expression] = carryCorrect;
		}
	}
}
*/