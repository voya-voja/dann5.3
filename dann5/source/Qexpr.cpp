#include <math.h> 
#include <iostream>

#include <Qexpr.h>
#include <Qevaluation.h>
#include <Qsolver.h>
#include <Factory.h>

#include <Logger.h>

using namespace dann5;

/*** Qexpression ***/

void Qexpression::compile(Qcompiler& compiler) const
{
    mpRoot->compile(compiler);
}

string Qexpression::toString(bool decomposed, size_t forBit) const
{
	if (!decomposed) return mpRoot->toString(decomposed, forBit);
	if (forBit != cAllBits) return mpRoot->toString(decomposed, forBit);

	size_t size = mpRoot->noqbs();
	string tStr = "";
	for (size_t at = 0; at < size; at++)
	{
		tStr += mpRoot->toString(decomposed, at) + "; ";
	}
	return tStr;
}

void Qexpression::add(const Qevaluations& evaluations)
{
	mSolutions.insert(mSolutions.end(), evaluations.begin(), evaluations.end());
	root().add(evaluations);
}

string Qexpression::solutions() const
{
	string asStr("");
	size_t solutionsNo = mSolutions.size();
	for (size_t id = 0; id < solutionsNo; id++)
	{
		asStr += root()->solution(id) + "\n";
	}
	return asStr;
}

Qevaluations Qexpression::compute()
{
	if(mSolutions.size() != 0)
		reset();
    Qsolver::Sp pSolver = Qsolver::Active();
	Qevaluations evaluations = pSolver->solution(*this);
	return evaluations;
}

void Qexpression::reset()
{
	mSolutions.clear();
	root().reset();
}

std::ostream& dann5::operator << (std::ostream& out, const Qexpression& right)
{
	out << right.toString();
	return out;
}
