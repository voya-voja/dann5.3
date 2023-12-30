#include <DwaveSolver.h> 

#include <Qstatement.h>

#include <QuboCompiler.h>

using namespace dann5;
using namespace dann5::ocean;

/**** dann5 Dwave Solver ****/
const double DwaveSolver::cMaxEnergy = numeric_limits<double>::max();

DwaveSolver::DwaveSolver(bool lowest)
	:QuboAnalyzer(Qubo()), mLowest(lowest), mMinEnergy(cMaxEnergy)
{

}

DwaveSolver::~DwaveSolver()
{
}

Qevaluations DwaveSolver::solution(const Qstatement& statement)
{
	if (mMinEnergy != cMaxEnergy)
		reset();
	QuboCompiler compiler;
	statement.compile(compiler);
	qubo(compiler.qubo());
	solve();
	return(mSolution);
}

Qevaluations DwaveSolver::solution(const Qubo& q)
{
	if (mMinEnergy != cMaxEnergy)
		reset();
	qubo(q);
	solve();
	return(mSolution);
}

void  DwaveSolver::reset()
{
	QuboAnalyzer::reset();
	mSolution.clear();
	mMinEnergy = cMaxEnergy;
}