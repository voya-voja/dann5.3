#include <math.h> 
#include <Qstatement.h>
#include <Utility.h>

using namespace dann5;

/*** Qstatement ***/
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

void Qstatement::add(const Qevaluations& samples)
{
	mNoSolutions += samples.size();
}

string Qstatement::solve()
{
    Qevaluations evltns = compute();
	add(evltns);
	return solutions();
}

void Qstatement::reset()
{
	mNoSolutions = 0;
}
