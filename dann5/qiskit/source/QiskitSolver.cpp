#include <QiskitSolver.h> 

#include <Qstatement.h>

#include <CircuitCompiler.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** dann5 Qiskit Solver ****/

QiskitSolver::QiskitSolver(bool lowest)
	:CircuitAnalyzer(Circuit()), mLowest(lowest), mMinEnergy(cMaxEnergy)
{

}

QiskitSolver::~QiskitSolver()
{
}

Qevaluations QiskitSolver::solution(const Qstatement& statement)
{
	if (mMinEnergy != cMaxEnergy)
		reset();
	CircuitCompiler compiler;
	statement.compile(compiler);
	qubo(compiler.qubo());
	solve();
	return(mSolution);
}

Qevaluations QiskitSolver::solution(const Circuits& q)
{
	if (mMinEnergy != cMaxEnergy)
		reset();
	qubo(q);
	solve();
	return(mSolution);
}

void  QiskitSolver::reset()
{
	mSolution.clear();
	mMinEnergy = cMaxEnergy;
}
/*
void  QiskitSolver::solve()
{
	throw logic_error("Error@QiskitSolver::solve(): Implementation is required!");
}
*/