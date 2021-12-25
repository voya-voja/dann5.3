#include <Qcell.h>

using namespace dann5::ocean;


string Qcell::solution(size_t sampleId) const
{
	string sStr = id() + "/";
	Qvalue v = value();
	if (v == cSuperposition)
		v = mSolutions[sampleId];
	if (v > 1)
		sStr.append(1, v);
	else
		sStr += to_string(v);
	sStr += "/";
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
