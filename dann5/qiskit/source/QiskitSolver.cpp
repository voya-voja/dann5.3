#include <QiskitSolver.h> 

#include <Qstatement.h>

#include <CircuitCompiler.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** dann5 Qiskit Solver ****/

QiskitSolver::QiskitSolver(bool lowest)
	:mLowest(lowest)//, mMinEnergy(cMaxEnergy)
{
}

QiskitSolver::~QiskitSolver()
{
}

Qevaluations QiskitSolver::solution(const Qstatement& statement)
{
	reset();
	CircuitCompiler compiler;
	statement.compile(compiler);
	mCircuit = compiler.circuit();
	solve();
	return(mSolution);
}

Qevaluations QiskitSolver::solution(const D5circuit& crct)
{
	reset();
	mCircuit = crct;
	solve();
	return(mSolution);
}

void  QiskitSolver::reset()
{
	mSolution.clear();
	mCircuit.reset();
//	mMinEnergy = cMaxEnergy;
}
