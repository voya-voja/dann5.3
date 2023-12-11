#include <D5QuboSolver.h> 
#include <thread>
#include <utility>

#include <Qstatement.h>

#include <QuboCompiler.h>

using namespace dann5;
using namespace dann5::ocean;

/**** Q Solve ****/
namespace dann5 {
	namespace ocean {

		class Qsolve
		{
		public:
			Qsolve(D5QuboSolver::RawElement* pSample, std::size_t start, std::size_t last, bool lowest = true);
			~Qsolve();

			double energy() { return mMinEnergy; };
            Qevaluations solution() { return mSolution; }
			void run();

		protected:
			void solve(std::size_t at, Qvalue value = 0);
			inline double calculateEnergy(std::size_t at);

		private:
			inline D5QuboSolver::RawElement* clone(D5QuboSolver::RawElement* pSample);

			D5QuboSolver::RawElement* mpSample;
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

Qsolve::Qsolve(D5QuboSolver::RawElement* pSample, size_t start, size_t last, bool lowest)
	: mStart(start), mLast(last), mLowest(lowest), mMinEnergy(D5QuboSolver::cMaxEnergy)
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

inline D5QuboSolver::RawElement* Qsolve::clone(D5QuboSolver::RawElement* pSample)
{
	D5QuboSolver::RawElement* pClone = new D5QuboSolver::RawElement[mLast + 1];
	for (std::size_t at = 0; at <= mLast; at++)
	{
		pClone[at].pNode = pSample[at].pNode;
		pClone[at].value = pSample[at].value;
		pClone[at].nodesEnergy = pSample[at].nodesEnergy;
	}
	return pClone;
}

/**** dann5 Qubo Solver ****/
const double D5QuboSolver::cMaxEnergy = numeric_limits<double>::max();

D5QuboSolver::D5QuboSolver(bool lowest)
    :QuboAnalyzer(Qubo()), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
    
}

D5QuboSolver::D5QuboSolver(const Qubo& qubo, bool lowest)
	:QuboAnalyzer(qubo), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
    solve();
}

D5QuboSolver::~D5QuboSolver()
{
}

Qevaluations D5QuboSolver::solution(const Qstatement& statement)
{
    if(mMinEnergy != cMaxEnergy)
        reset();
    QuboCompiler compiler;
    statement.compile(compiler);
    qubo(compiler.qubo());
    solve();
    return(mSolution);
}

void  D5QuboSolver::reset()
{
    QuboAnalyzer::reset();
    mSolution.clear();
    mMinEnergy = cMaxEnergy;
}

void  D5QuboSolver::solve()
{
	if (nodesNo() == 0) throw logic_error("Error@Qsolve: Nothing to solve!");

	std::vector<std::thread> threads;
	std::vector<Qsolve*> solvePtrs;

	// for solve threads the samples will be initialized to 0b00-0b11 and
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
    // initialize samples and start Qsolve threads ...
	for (size_t at = 0; at < noSolveThreads; ++at)		// 
	{
		// set initial sample nodes values, e.g. 0b00...0, 0b01...0, ...
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

inline D5QuboSolver::RawElement* D5QuboSolver::createSample(Qvalue init)
{
	D5QuboSolver::RawElement* sample = new D5QuboSolver::RawElement[nodesNo()];
	const Qubo& q = qubo();
	auto quboEnd = q.end();
	Nodes& nodes = QuboAnalyzer::nodes();
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
