#include <Qblock.h>

#include <Factory.h>

#include <Qsolver.h>

using namespace dann5;

/*** Qblock of quantum statements ***/

size_t Qblock::noqbs() const noexcept
{
	size_t size = 0;
	for (auto pStatement : mStatements)
        size += pStatement->noqbs();
	return(size);
}

string Qblock::toString(bool decomposed, size_t forBit) const
{
    string blockStr("");
    if(!decomposed)
    {
        blockStr = "{";
        for (auto pStatement : mStatements)
            blockStr += "\n\t" + pStatement->toString(decomposed, forBit);
        blockStr += "\n}";
    }
    else
    {
        if(forBit == 0)
            blockStr = "{";
        size_t atBit = forBit;
        for (auto pStatement : mStatements)
        {
            size_t stmntNoqbts = pStatement->noqbs();
            if(atBit < stmntNoqbts)
            {
                blockStr += " " + pStatement->toString(decomposed, atBit);
                break;
            }
            if(atBit >= stmntNoqbts)
                atBit -= stmntNoqbts;
        }
        if(forBit == noqbs() - 1)
            blockStr += " }";
    }
	return(blockStr);
}

void Qblock::compile(Qcompiler& compiler) const
{
	for (auto pStatement : mStatements)
	{
		pStatement->compile(compiler);
	}
}

void Qblock::add(const Qevaluations& samples)
{
	Qstatement::add(samples);
	for (auto pStatement : mStatements)
	{
		pStatement->add(samples);
	}
}

string Qblock::solution(size_t forBit) const
{
    size_t atBit = forBit;
	string strSol("");
	for (auto pStatement : mStatements)
	{
        size_t stmntNoqbts = pStatement->noqbs();
        if(atBit > stmntNoqbts)
            atBit -= stmntNoqbts;
        strSol += " " + pStatement->solution(atBit);
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

Qevaluations Qblock::compute()
{
	if (noSolutions() != 0)
		reset();
	Qsolver::Sp pSolver = Qsolver::Active();
    Qevaluations evltns = pSolver->solution(*this);
	return evltns;
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

Qblock Qblock::operator<<(const Qstatement& statement) const
{
	Qblock result(*this);
	result.mStatements.push_back(statement.clone());
	return(result);
}

Qblock::CommaOp Qblock::operator=(const Qstatement& statement)
{
	mStatements.push_back(statement.clone());
	return(CommaOp(this));
}

ostream& dann5::operator << (std::ostream& out, const Qblock& r)
{
	out << r.toString();
	return out;
}
