#include <QuboAnalyzer.h>

using namespace dann5;
using namespace dann5::ocean;


/**** Q Analyzer ****/
void QuboAnalyzer::reset()
{
    mQubo.clear();
    mNodes.clear();
    mBranches.clear();
    mChainStrength = 0;
}

QuboAnalyzer::Nodes& QuboAnalyzer::nodes()
{
	if (mNodes.size() == 0)
	{
		for (auto element : mQubo)
		{
			if (element.first.first == element.first.second)
			{
				Node node(element.first.first, element.second);
				mNodes.push_back(node);;
			}
		}
	}
	return(mNodes);
}

QuboAnalyzer::Branches& QuboAnalyzer::branches()
{
	if (mBranches.size() == 0)
	{
		for (auto element : mQubo)
		{
			if (element.first.first != element.first.second)
			{
				Element branch(element);
				mBranches.push_back(branch);
			}
		}
	}
	return(mBranches);
}

void QuboAnalyzer::calculateChainStrength()
{
	for(auto element: mQubo)
		if (abs(element.second) > mChainStrength)
			mChainStrength = abs(element.second);
}

ostream& dann5::ocean::operator<<(std::ostream& out, const QuboAnalyzer::Nodes& nodes)
{
	cout << "[";
	for(auto node : nodes)
		out << "(" << node.first << " : " << to_string(node.second) << "), ";
	cout << "]";
	return out;
}

ostream& dann5::ocean::operator<<(std::ostream& out, const QuboAnalyzer::Branches& branches)
{
	cout << "[";
	for (auto branch : branches)
		out << "((" << branch.first.first << ", " << branch.first.second << ") : " << to_string(branch.second) << "), ";
	cout << "]";
	return out;
}
