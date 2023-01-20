#include <Qblock.h>

#include <Factory.h>
#include <Qubo.h>

using namespace dann5::ocean;

/*** Qblock of quantum statements ***/


size_t Qblock::noqbs() const noexcept
{
	size_t size = 0;
	for (auto pStatement : mStatements)
	{
		size_t atSize = pStatement->noqbs();
		if (size < atSize) size = atSize;
	}
	return(size);
}


string Qblock::toString(bool decomposed, size_t forBit) const
{
	string blockStr("{");
	for (auto pStatement : mStatements)
	{
		blockStr += "\n\t" + pStatement->toString(decomposed, forBit);
	}
	blockStr += "\n}";
	return(blockStr);
}

Qubo Qblock::qubo(bool finalized, size_t forBit) const
{
	Qubo qubo;
	for (auto pStatement : mStatements)
	{
		qubo += pStatement->qubo(finalized, forBit);
	}
	return(qubo);
}

void Qblock::add(const Qsolver::Samples& samples)
{
	Qstatement::add(samples);
	for (auto pStatement : mStatements)
	{
		pStatement->add(samples);
	}
}

string Qblock::solution(size_t forBit) const
{
	string strSol("");
	for (auto pStatement : mStatements)
	{
		strSol += " " + pStatement->solution(forBit);
	}
	return strSol;
}

string Qblock::solutions() const
{
	string strSols("");
	size_t solutionsNo = noSolutions();
	for (size_t forBit = 0; forBit < solutionsNo; forBit++)
	{
		strSols += solution(forBit) + "\n";
	}
	return(strSols);
}

Qsolver::Samples Qblock::compute()
{
	if (noSolutions() != 0)
		reset();
	Qubo qubo;
	for (auto pStatement : mStatements)
	{
		qubo += pStatement->qubo();
	}
	Qsolver solver(qubo);
	Qsolver::Samples samples = solver.solution();
	return samples;
}

void Qblock::reset()
{
	for (auto pStatement : mStatements)
	{
		pStatement->reset();
	}
}

Qblock& Qblock::operator<<(const Qstatement& statement)
{
	mStatements.push_back(statement.clone());
	return(*this);
}

Qblock::CommaOp Qblock::operator=(const Qstatement& statement)
{
	mStatements.push_back(statement.clone());
	return(CommaOp(this));
}

ostream& dann5::ocean::operator << (std::ostream& out, const Qblock& r)
{
	out << r.toString();
	return out;
}