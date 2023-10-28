#include <Qsolver.h> 
#include <iostream>


using namespace dann5::ocean;

const double Qsolver::cMaxEnergy = numeric_limits<double>::max();


/**** Q Analyzer ****/

Qanalyzer::Nodes& Qanalyzer::nodes()
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

Qanalyzer::Branches& Qanalyzer::branches()
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

void Qanalyzer::calculateChainStrength()
{
	for(auto element: mQubo)
		if (abs(element.second) > mChainStrength)
			mChainStrength = abs(element.second);
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qanalyzer::Nodes& nodes)
{
	cout << "[";
	for(auto node : nodes)
		out << "(" << node.first << " : " << to_string(node.second) << "), ";
	cout << "]";
	return out;
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qanalyzer::Branches& branches)
{
	cout << "[";
	for (auto branch : branches)
		out << "((" << branch.first.first << ", " << branch.first.second << ") : " << to_string(branch.second) << "), ";
	cout << "]";
	return out;
}

/**** Q Solve ****/


namespace dann5 {
	namespace ocean {

		class Qsolve
		{
		public:
			Qsolve(Qsolver::RawElement* pSample, std::size_t start, std::size_t last, bool lowest = true);
			~Qsolve();

			double energy() { return mMinEnergy; };
            Qevaluations solution() { return mSolution; }
			void run();

		protected:
			void solve(std::size_t at, Qvalue value = 0);
			inline double calculateEnergy(std::size_t at);

		private:
			inline Qsolver::RawElement* clone(Qsolver::RawElement* pSample);

			Qsolver::RawElement* mpSample;
			std::size_t		mStart;
			std::size_t		mLast;

			bool				mLowest;
			double				mMinEnergy;
			Qevaluations	mSolution;
		};
	};
};

void Qsolve::run()
{
	if(!mLowest)
		mSolution.clear();
	solve(mStart, 0);
	solve(mStart, 1);
}

Qsolve::Qsolve(Qsolver::RawElement* pSample, size_t start, size_t last, bool lowest)
	: mStart(start), mLast(last), mLowest(lowest), mMinEnergy(Qsolver::cMaxEnergy)
{
	mpSample = clone(pSample);
	for (size_t at = 0; at < mStart; at++)
	{
		mpSample[at].valuesXenergySum = calculateEnergy(at);
	}
}

Qsolve::~Qsolve()
{
	delete[] mpSample;
}

void Qsolve::solve(std::size_t at, Qvalue value)
{
	mpSample[at].value = value;
	mpSample[at].valuesXenergySum = calculateEnergy(at);

	if (at == mLast)
	{
		if (mLowest && mpSample[at].valuesXenergySum < mMinEnergy)
		{
			mMinEnergy = mpSample[at].valuesXenergySum;
			{
				mSolution.clear();
			}
		}
		if (!mLowest || mpSample[at].valuesXenergySum == mMinEnergy)
		{
			Qevaluation evltn;
			for (std::size_t index = 0; index <= mLast; index++)
                evltn.sample()[mpSample[index].pNode->first] = mpSample[index].value;
            evltn.energy() = mpSample[at].valuesXenergySum;
			mSolution.push_back(evltn);
		}
	}
	else
	{
		solve(at + 1, 0);
		solve(at + 1, 1);
	}
}

inline double Qsolve::calculateEnergy(std::size_t at)
{
	double energy = 0;
	if (at > 0)	// 
		energy = mpSample[at - 1].valuesXenergySum;

	for (std::size_t prev = 0; prev <= at; prev++)
	{
		energy += mpSample[prev].value * mpSample[at].value * mpSample[at].nodesEnergy[prev];
	}
	return(energy);
}

inline Qsolver::RawElement* Qsolve::clone(Qsolver::RawElement* pSample)
{
	Qsolver::RawElement* pClone = new Qsolver::RawElement[mLast + 1];
	for (std::size_t at = 0; at <= mLast; at++)
	{
		pClone[at].pNode = pSample[at].pNode;
		pClone[at].value = pSample[at].value;
		pClone[at].nodesEnergy = pSample[at].nodesEnergy;
	}
	return pClone;
}

/**** Q evaluation ****/

Qevaluation& Qevaluation::operator+=(const Qevaluation& right)
{
    mEnergy += right.mEnergy;
    bool mergeable = true;
    Qsample::const_iterator dEnd = mSample.cend();
    for(auto elmntR : right.sample())
    {   // if left and right evaluation have the same element their values should be the same
        Qsample::const_iterator at = mSample.find(elmntR.first);
        bool dffrntElmnt =  at == dEnd;
        mergeable = dffrntElmnt || (*at).second == elmntR.second;
        if(!mergeable)
        {
            reset();
            break;
        }
        if(dffrntElmnt) // when mergable and different element
            mSample[elmntR.first] = elmntR.second;
    }
    return(*this);
}

Qevaluation Qevaluation::operator+(const Qevaluation& right) const
{
    Qevaluation rslt(*this);
    rslt += right;
    return(rslt);
}

Qevaluations dann5::ocean::operator+(const Qevaluations& left, const Qevaluations& right)
{
    Qevaluations evaltns;
    for(auto evltnL : left)
        for(auto evltnR : right)
        {
            Qevaluation evltn = evltnL + evltnR;
            if(evltn.isValid())
                evaltns.push_back(evltn);
        }
    return evaltns;
}

std::ostream& dann5::ocean::operator << (std::ostream& out, const Qevaluations& evaluations)
{
    bool tableHeader = true;
    for (auto evltn : evaluations)
    {
        out << endl;
        if (tableHeader)
        {
            for (auto element : evltn.sample())
                out << element.first << " ";
            out << endl;
            tableHeader = false;
        }
        for (auto element : evltn.sample())
            out << to_string(element.second) << " ";
        out << "--> " << evltn.energy();
    }
    return out;
}

/**** Q Solver ****/

Qsolver::Qsolver(const Qubo& qubo, bool lowest)
	:Qanalyzer(qubo), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
}

Qsolver::~Qsolver()
{
}

Qevaluations Qsolver::solution()
{
	if (mSolution.size() == 0)
	{
		solve();
	}
	return(mSolution);
}


void  Qsolver::solve()
{
	if (nodesNo() == 0) throw logic_error("Error@Qsolve: Nothing to solve!");

	std::vector<std::thread> threads;
	std::vector<Qsolve*> solvePtrs;

	// for 4 solve threads the samples will be initialized to 0b00-0b11 and 
	// respectfully have start update value 0 or 1
	size_t noInitNodes = size_t(log2(nodesNo()) - 0.1);
	size_t noSolveThreads = 1;
	if (noInitNodes > 0)
	{
		noInitNodes--;
		noSolveThreads = size_t(pow(2, noInitNodes));
	}
	size_t last = nodesNo() - 1;
	RawElement* pSample = createSample();	// create sample 0b00...0
	for (size_t at = 0; at < noSolveThreads; ++at)		// 
	{
		// set initial sample nodes values
		size_t atNoBuffer = at;
		for (size_t atNode = 0; atNode < noInitNodes; atNode++)
		{
			Qvalue value = atNoBuffer % 2;
			pSample[atNode].value = value;
			atNoBuffer >>= 1;
		}
		// each solver start at nomber of initialized nodes and go to the last node
		Qsolve* pSolve = new Qsolve(pSample, noInitNodes, last, mLowest);
		solvePtrs.push_back(pSolve);
		threads.push_back(std::thread(&Qsolve::run, std::ref(*pSolve)));
	}

	// synchronizing all threads...
	for (auto& th : threads) th.join();

	// merge solutions from all Qsolve threads...
	for (auto& pSolve : solvePtrs)
	{
		double energy = pSolve->energy();
		if (mLowest && energy < mMinEnergy)
		{
			mMinEnergy = energy;
			mSolution = pSolve->solution();
		}
		else if (!mLowest || energy == mMinEnergy)
		{
			Qevaluations solutons = pSolve->solution();
			mSolution.insert(mSolution.end(), solutons.begin(), solutons.end());
		}
		delete pSolve;
	}
	delete [] pSample;
}

inline Qsolver::RawElement* Qsolver::createSample(Qvalue init)
{
	Qsolver::RawElement* sample = new Qsolver::RawElement[nodesNo()];
	const Qubo& q = qubo();
	auto quboEnd = q.end();
	Nodes& nodes = Qanalyzer::nodes();
	for (std::size_t at = 0; at < nodesNo(); at++)
	{
		sample[at].pNode = &nodes[at];
		sample[at].value = init;
		for (size_t prev = 0; prev <= at; prev++)
		{
			Qkey key(sample[prev].pNode->first, sample[at].pNode->first);
			auto pElement = q.find(key);
			if (pElement != quboEnd)
			{
				sample[at].nodesEnergy.push_back(pElement->second);
			}
			else
			{
				key = Qkey(sample[at].pNode->first, sample[prev].pNode->first);
				pElement = q.find(key);
				if (pElement != quboEnd)
				{
					sample[at].nodesEnergy.push_back(pElement->second);
				}
				else
				{
					sample[at].nodesEnergy.push_back(0);
				}
			}
		}
	}
	return sample;
}

void Qsolver::solution(ostream& out)
{
	if (mSolution.size() == 0)
		solution();
    out << mSolution;
}
