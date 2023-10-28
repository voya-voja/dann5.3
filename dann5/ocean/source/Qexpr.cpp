#include <math.h> 
#include <iostream>

#include <Qexpr.h>
#include <Qsolver.h>
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

Qubos Qexpression::qubos(size_t noNodes) const
{
    size_t size = mpRoot->noqbs();
    Qubos qubos;
    size_t nodeCnt = 0;
    Qubo subQubo;
    for (size_t at = 0; at < size; at++)
    {
        Qubo qubo = mpRoot->qubo(true, at);
        Qanalyzer anlzr(qubo);
        size_t ndsNo = anlzr.nodesNo();
        nodeCnt += ndsNo;
        if(nodeCnt > noNodes)
        {
            qubos.push_back(subQubo);
            subQubo.clear();
            nodeCnt = ndsNo;
        }
        subQubo += qubo;
    }
    qubos.push_back(subQubo);
    return qubos;
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
	Qubo q = qubo();
	Qsolver solver(q);
	Qevaluations evaluations = solver.solution();
	return evaluations;
}

void Qexpression::reset()
{
	mSolutions.clear();
	root().reset();
}

std::ostream& dann5::ocean::operator << (std::ostream& out, const Qexpression& right)
{
	out << right.toString();
	return out;
}
