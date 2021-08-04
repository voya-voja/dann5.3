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


/**** Q Solve ****/


namespace dann5 {
	namespace ocean {

		class Qsolve
		{
		public:
			Qsolve(Qsolver::RawElement* pSample, std::size_t start, std::size_t last, Qvalue value, bool lowest = true);
			~Qsolve();

			double energy() { return mMinEnergy; };
			Qsolver::Samples solutions() { return mSolutions; }
			void run();

		protected:
			void solve(std::size_t at, Qvalue value = 0);
			inline double calculateEnergy(std::size_t at);

		private:
			inline Qsolver::RawElement* clone(Qsolver::RawElement* pSample);

			Qsolver::RawElement* mpSample;
			std::size_t		mStart;
			std::size_t		mLast;
			Qvalue			mValue;

			bool				mLowest;
			double				mMinEnergy;
			Qsolver::Samples	mSolutions;
		};
	};
};

void Qsolve::run()
{
	solve(mStart, mValue);
}

Qsolve::Qsolve(Qsolver::RawElement* pSample, size_t start, size_t last, Qvalue value, bool lowest)
	: mStart(start), mLast(last), mValue(value), mLowest(lowest), mMinEnergy(Qsolver::cMaxEnergy)
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
		if (mpSample[at].valuesXenergySum < mMinEnergy)
		{
			mMinEnergy = mpSample[at].valuesXenergySum;
			{
				mSolutions.clear();
			}
		}
		if (!mLowest || mpSample[at].valuesXenergySum == mMinEnergy)
		{
			Qsolver::Sample sample;
			for (std::size_t index = 0; index <= mLast; index++)
				sample[mpSample[index].pNode->first] = mpSample[index].value;
			{
				mSolutions.push_back(sample);
			}
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

/**** Q Solver ****/

Qsolver::Qsolver(const Qubo& qubo, bool lowest)
	:Qanalyzer(qubo), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
}

Qsolver::~Qsolver()
{
}

Qsolver::Samples Qsolver::solution()
{
	if (mSolutions.size() == 0)
	{
		solve();
	}
	return(mSolutions);
}


void  Qsolver::solve()
{
	if (nodesNo() == 0) throw logic_error("Nothing to solve!");

	std::vector<std::thread> threads;
	std::vector<Qsolve*> solvePtrs;

	size_t noSolveThreads = 2;
	if(nodesNo() > 15) 
		noSolveThreads = size_t(pow(2, int(log2(nodesNo()) - 0.1)) / 2);
	// for 8 solve threads the samples will be initialized to 0b00-0b11 and 
	// respectfully have start update value 0 or 1
	const size_t noInitNodes = size_t(noSolveThreads / 4);
	size_t last = nodesNo() - 1;
	RawElement* pSample = createSample();	// create sample 0b00...0
	for (size_t at = 0; at < noSolveThreads; ++at)		// 
	{
		size_t init = size_t(at / 2);		// sample init value in 0 (0b00) - 3 (0b11)
		Qvalue value = at % 2;				// start update value 0 or 1
		if (init > 0 && value == 0)
		{ // init
			for (size_t atNode = 0; atNode < noInitNodes; atNode++)
			{
				Qvalue value = init % 2;
				pSample[atNode].value = value;
				init >>= 1;
			}
		}
		// each solver start at nomber of initialized nodes and go to teh last node
		Qsolve* pSolve = new Qsolve(pSample, noInitNodes, last, value);
		solvePtrs.push_back(pSolve);
		threads.push_back(std::thread(&Qsolve::run, std::ref(*pSolve)));
	}

	// synchronizing all threads...
	for (auto& th : threads) th.join();

	// synchronizing all threads...
	for (auto& pSolve : solvePtrs)
	{
		double energy = pSolve->energy();
		if (energy < mMinEnergy)
		{
			mMinEnergy = energy;
			mSolutions = pSolve->solutions();
		}
		else if (energy == mMinEnergy)
		{
			Samples solutons = pSolve->solutions();
			mSolutions.insert(mSolutions.end(), solutons.begin(), solutons.end());
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