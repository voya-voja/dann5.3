#include <math.h> 
#include <Qstatement.h>
#include <Utility.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qstatement::Qstatement()
{
}

Qstatement::Qstatement(const Qstatement& right)
	:mNoSolutions(right.mNoSolutions)
{
}

Qstatement::~Qstatement()
{
}

void Qstatement::add(const Qsolver::Samples& samples)
{
	mNoSolutions += samples.size();
}

string Qstatement::solve()
{
	Qsolver::Samples samples = compute();
	add(samples);
	return solutions();
}

void Qstatement::reset()
{
	mNoSolutions = 0;
}
