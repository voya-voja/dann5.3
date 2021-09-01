#include <math.h> 
#include <iostream>

#include <Qexpr.h>
#include <Factory.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Qexpression ***/

Qubo Qexpression::qubo(bool finalized, size_t forBit) const
{
	if (forBit != cAllBits) return mpRoot->qubo(finalized, forBit);

	size_t size = mpRoot->noqbs();
	Qubo aQubo;
	for (size_t at = 0; at < size; at++)
	{
		aQubo += mpRoot->qubo(finalized, at);
	}
	return aQubo;
}

string Qexpression::toString(bool decomposed, size_t forBit) const
{
	if (!decomposed) return mpRoot->toString(decomposed, forBit);
	if (forBit != cAllBits) return mpRoot->toString(decomposed, forBit);

	size_t size = mpRoot->noqbs();
	string tStr = "";
	for (size_t at = 0; at < size; at++)
	{
		tStr += mpRoot->toString(decomposed, at) + "\n";
	}
	return tStr;
}

void Qexpression::add(const Qsolver::Samples& samples)
{
	mSolutions.insert(mSolutions.end(), samples.begin(), samples.end());
	root().add(samples);
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

string Qexpression::solve()
{
	if(mSolutions.size() != 0)
		clearSolutions();
	Qubo q = qubo();
	Qsolver solver(q);
	Qsolver::Samples samples = solver.solution();
	add(samples);
	return solutions();
}

void Qexpression::clearSolutions()
{
	mSolutions.clear();
	root().clearSolutions();
}

std::ostream& dann5::ocean::operator << (std::ostream& out, const Qexpression& right)
{
	out << right.toString();
	return out;
}