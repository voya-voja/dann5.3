#include <math.h> 
#include <Qassign.h>
#include <Qnary.h>
#include <Utility.h>
#include <Logger.h>

using namespace dann5;

/*** Quantum Assignment ***/

Qassignment::Qassignment(const Qdef::Sp& pAssignee, const Qexpression::Sp& pExpr)
	: mpAssignee(pAssignee), mpExpr(pExpr) 
{
	bind();
}

void Qassignment::assign(const Qdef::Sp& pAssignee, const Qexpression::Sp& pExpr)
{
	mpAssignee = pAssignee;
	expression(pExpr);
}

void Qassignment::expression(const Qexpression::Sp& pExpr)
{ 
	mpExpr = pExpr; 
	if (mpAssignee != nullptr && mpExpr != nullptr)
		bind();
}

void Qassignment::bind()
{
	size_t exprSize = mpExpr->noqbs(), asgnSize = mpAssignee->noqbs();
	// ensure result Q variable and equation Q expression have same number-of-bits
	if (exprSize > asgnSize)
	{
		Qnary::Sp pAssignee = dynamic_pointer_cast<Qnary>(mpAssignee);
		if (pAssignee != nullptr)
			// if Qnary result, size result to have same # of Q bits as expression
			pAssignee->resize(exprSize);
		else
			throw std::logic_error("Error@Qassignment: The Q equation result and expression are of different type!");
	}
	else if(exprSize < asgnSize)
	{ // if expression has less Q bits, remaining Q bits of the result will become 0 bitwise logic
		QnaryOp::Sp pNaryOp = dynamic_pointer_cast<QnaryOp>(as_const(*mpExpr).root());
		if(pNaryOp != nullptr) pNaryOp->resize(asgnSize);
	}

	mpExpr->root().output(mpAssignee);
}

void Qassignment::compile(Qcompiler& compiler) const
{
    if (mpExpr == nullptr)
        throw logic_error("Error@Qassignment: Nothing to compile!");
	return(mpExpr->compile(compiler));
}

string Qassignment::toString(bool decomposed, size_t forBit) const
{
	string sAssignment = "";
	if (!decomposed)
	{
		sAssignment = mpAssignee->toString(decomposed, forBit) + " = ";
	}
	sAssignment += mpExpr->toString(decomposed, forBit);
	return sAssignment;
}

void Qassignment::add(const Qevaluations& evaluations)
{ 
	mpExpr->add(evaluations);
}

string Qassignment::solutions() const
{
	return mpExpr->solutions();
}

Qevaluations Qassignment::compute()
{
	return mpExpr->compute();
}

void Qassignment::reset()
{
	mpExpr->reset();
}

ostream& dann5::operator<<(std::ostream& out, const Qassignment& statement)
{
	out << statement.toString();
	return out;
}
