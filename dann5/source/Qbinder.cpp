#include <Qbinder.h>

#include <Factory.h>

using namespace dann5;

/*** Qbinder of quantum statements ***/


size_t Qbinder::noqbs() const noexcept
{
	size_t size = 0;
	for (auto pElement : mElements)
	{
		size_t atSize = pElement->noqbs();
		if (size < atSize) size = atSize;
	}
	return(size);
}

string Qbinder::toString(bool decomposed, size_t forBit) const
{
	string blockStr("");
	for (auto pElement : mElements)
	{
		blockStr += pElement->toString(decomposed, forBit) + " ";
	}
	string sltnsSrt = solutions();
	if(!sltnsSrt.empty())
		blockStr += "{\n" + sltnsSrt +"}";
	return(blockStr);
}

void Qbinder::add(const Qevaluations& evltns)
{
	mSolutions.insert(mSolutions.end(), evltns.begin(), evltns.end());
	for (auto pElement : mElements)
	{
		pElement->add(evltns);
	}
}

string Qbinder::solution(size_t atEvltn) const
{
	string strSol("");
	for (auto pElement : mElements)
	{
		strSol += pElement->solution(atEvltn) + " ";
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
	for (auto pElement : mElements)
	{
		pElement->reset();
	}
    mSolutions.clear();
}

Qbinder& Qbinder::operator<<(const Qdef& right)
{
	addElement(right);
	return(*this);
}

Qbinder Qbinder::operator<<(const Qdef& statement) const
{
	Qbinder result(*this);
	result.mElements.push_back(statement.clone());
	return(result);
}

Qbinder::CommaOp Qbinder::operator=(const Qdef& right)
{
	addElement(right);
	return(CommaOp(this));
}

Qdef::Sp Qbinder::operator[](string id) const
{
	for (auto pElement : mElements)
		if (pElement->id() == id)
			return pElement;
	return nullptr;
}

ostream& dann5::operator << (std::ostream& out, const Qbinder& r)
{
	out << r.solutions();
	return out;
}


void Qbinder::addElement(const Qdef& right)
{
	Qdef::Sp pElement = right.clone();
	mElements.push_back(pElement);
	if (mSolutions.size() > 0)
		pElement->add(mSolutions);
}
