#include <Qcell.h>

using namespace dann5::ocean;

string Qcell::toString(bool decomposed, size_t forBit) const
{
	string id = Qdef::toString(decomposed);
	string valueStr = "";
	Qvalue v = value();
	if (v != cSuperposition) valueStr = to_string(v);
	else valueStr.append(1, cSuperposition);
	return id + "\\" + valueStr + "\\";
}

void Qcell::add(const Qsolver::Samples& samples)
{
	if (value() != cSuperposition) return;

	string identity = id();
	for (auto sample : samples)
	{
		mSolutions.push_back(sample.mSample[identity]);
	}
}

string Qcell::solution(size_t sampleId) const
{
	string sStr = id() + "\\";
	Qvalue v = solutionValue(sampleId);
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

Qvalue Qcell::solutionValue(size_t sampleId) const 
{
	Qvalue v = value();
	if (v == cSuperposition)
		v = mSolutions[sampleId];
	return v;
}
