#include <Qbinder.h>

#include <Factory.h>
#include <Qubo.h>

using namespace dann5::ocean;

/*** Qbinder of quantum statements ***/


size_t Qbinder::noqbs() const noexcept
{
	size_t size = 0;
	for (auto pOperand : mOperands)
	{
		size_t atSize = pOperand->noqbs();
		if (size < atSize) size = atSize;
	}
	return(size);
}

string Qbinder::toString(bool decomposed, size_t forBit) const
{
	string blockStr("");
	for (auto pOperand : mOperands)
	{
		blockStr += pOperand->toString(decomposed, forBit) + " ";
	}
	string sltnsSrt = solutions();
	if(!sltnsSrt.empty())
		blockStr += "{\n" + sltnsSrt +"}";
	return(blockStr);
}

void Qbinder::add(const Qevaluations& evltns)
{
	mSolutions.insert(mSolutions.end(), evltns.begin(), evltns.end());
	for (auto pOperand : mOperands)
	{
		pOperand->add(evltns);
	}
}

string Qbinder::solution(size_t forBit) const
{
	string strSol("");
	for (auto pOperand : mOperands)
	{
		strSol += pOperand->solution(forBit) + " ";
	}
	return strSol;
}

string Qbinder::solutions() const
{
	string strSols("");
	size_t solutionsNo = mSolutions.size();
	for (size_t forBit = 0; forBit < solutionsNo; forBit++)
	{
		strSols += solution(forBit) + "\n";
	}
	return(strSols);
}

void Qbinder::reset()
{
	for (auto pOperand : mOperands)
	{
		pOperand->reset();
	}
}

Qbinder& Qbinder::operator<<(const Qdef& right)
{
	addOperand(right);
	return(*this);
}

Qbinder Qbinder::operator<<(const Qdef& statement) const
{
	Qbinder result(*this);
	result.mOperands.push_back(statement.clone());
	return(result);
}

Qbinder::CommaOp Qbinder::operator=(const Qdef& right)
{
	addOperand(right);
	return(CommaOp(this));
}

Qdef::Sp Qbinder::operator[](string id)
{
	for (auto pOperand : mOperands)
		if (pOperand->id() == id)
			return pOperand;
	return nullptr;
}

ostream& dann5::ocean::operator << (std::ostream& out, const Qbinder& r)
{
	out << r.solutions();
	return out;
}


void Qbinder::addOperand(const Qdef& right)
{
	Qdef::Sp pOperand = right.clone();
	mOperands.push_back(pOperand);
	if (mSolutions.size() > 0)
		pOperand->add(mSolutions);
}
