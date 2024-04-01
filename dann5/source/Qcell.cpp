#include <Qcell.h>

using namespace dann5;

string Qcell::toString(bool decomposed, size_t forBit) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	Qvalue v = value();
	if (v != cSuperposition) valueStr = to_string(v);
	else valueStr.append(1, cSuperposition);
	return id + "\\" + valueStr + "\\";
}

void Qcell::add(const Qevaluations& evaluations)
{
	if (value() != cSuperposition) return;

	string identity = id();
	for (auto evltn : evaluations)
	{
		Qsample& sample = evltn.sample();
		Qsample::const_iterator at = sample.find(identity);
		if (at != sample.cend())
		{
			mSolutions.push_back(evltn.sample()[identity]);
		}
	}
}

string Qcell::solution(size_t atEvltn) const
{

	string sStr = id() + "\\";
	Qvalue v = solutionValue(atEvltn);
	if (v > 1)
		sStr.append(1, v);
	else
		sStr += to_string(v);
	sStr += "\\";
	return sStr;
}

void Qcell::reset()
{
	mSolutions.clear();
}

Qvalue Qcell::solutionValue(size_t atEvltn) const
{
	Qvalue v = value();
	if(v == cSuperposition)
	{
		size_t size = mSolutions.size();
		if (size > 0 && atEvltn < size)
			v = mSolutions[atEvltn];
	}
	return v;
}
