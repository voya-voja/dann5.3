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

		// Quntum solve algorithm is private class in dann5::ocean namespace
		class Qsolve
		{
		public:
			// Construct Qsolve algorithm with an inital sample chain of 
			// SampleElements, starting and last SampleElement position, and
			// lowest energy mode calculation.
			// The constructor uses a clone of provided pSample
			Qsolve(D5QuboSolver::SampleElement* pSample, std::size_t start, 
									std::size_t last, bool lowest = true);

			// Destructor of Qsolve delets cloned sample
			~Qsolve();

			// return calculated minimal energy for a given sample
			double energy() { return mMinEnergy; };

			// return calculated evaluations based on mode set with 'lowest'
			// argument at the time of construction
            Qevaluations solution() { return mSolution; }

			// run calculation of solution quantum evaluations and 
			// minimal energy
			void run();

		protected:
			// A recursive method walks the tree of all possible samples to identify
			// all the evaluations and to calculate their energies
			void solve(std::size_t at, Qvalue value = 0);

			// calculates a energy up to a node at position 'at'
			inline double calculateEnergy(std::size_t at);

		private:
			// returs a clone of provided pSample
			inline D5QuboSolver::SampleElement* clone(D5QuboSolver::SampleElement* pSample);

			// a pointer to sample head SampleElement
			D5QuboSolver::SampleElement* mpSample;	
			std::size_t		mStart;	// start SampleElement position for calcuations
			std::size_t		mLast;	// last SampleElement position for calcuations

			bool			mLowest;	// mode of calcualtions of Qevaluations
			double			mMinEnergy;	// minimal energy calcualtes 
			Qevaluations	mSolution;	// calaculated quantum evaluations
		};
	};
};

void Qsolve::run()
{
	if(!mLowest)
		mSolution.clear();
	// solve by setting SampleElement of a node at start position to 0 value
	solve(mStart, 0);
	// solve by setting SampleElement of a node at start position to 1 value
	solve(mStart, 1);
}

Qsolve::Qsolve(D5QuboSolver::SampleElement* pSample, size_t start, size_t last, bool lowest)
	: mStart(start), mLast(last), mLowest(lowest), mMinEnergy(D5QuboSolver::cMaxEnergy)
{
	mpSample = clone(pSample);
	for (size_t at = 0; at < mStart; at++)
	{	// calculates values x energy sums for the initialized nodes 
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
			if (mLowest)
			{
				mSolution.clear();
			}
		}
		if (!mLowest || mpSample[at].valuesXenergySum == mMinEnergy)
		{ // convert sample of SampleElements into a quantum evaluation
			Qevaluation evltn;
			for (std::size_t index = 0; index <= mLast; index++)
                evltn.sample()[mpSample[index].pNode->first] = mpSample[index].value;
            evltn.energy() = mpSample[at].valuesXenergySum;
			// save the solved quantum evaluation as a valid solution
			mSolution.push_back(evltn);
		}
	}
	else
	{
		// solve by setting SampleElement value to 0 for a node at + 1
		solve(at + 1, 0);
		// solve by setting SampleElement value to 1 for a node at + 1
		solve(at + 1, 1);
	}
}

inline double Qsolve::calculateEnergy(std::size_t at)
{
	double energy = 0;
	if (at > 0)	// start with the sum calculated for the previous node
		energy = mpSample[at - 1].valuesXenergySum;
	// by walking through all branches of this node calculate the energy based on
	// values of the node at and all previous nodes
	for (std::size_t prev = 0; prev <= at; prev++)
	{	
		energy += mpSample[prev].value * mpSample[at].value 
										* mpSample[at].nodesEnergy[prev];
	}
	return(energy);
}

inline D5QuboSolver::SampleElement* Qsolve::clone(D5QuboSolver::SampleElement* pSample)
{
	D5QuboSolver::SampleElement* pClone = new D5QuboSolver::SampleElement[mLast + 1];
	for (std::size_t at = 0; at <= mLast; at++)
	{	// Qsolve only reads the value of pNode and all of Qsolve instances
		// point to the same node
		pClone[at].pNode = pSample[at].pNode;
		pClone[at].nodesEnergy = pSample[at].nodesEnergy;
		// Qsolve instance only manipulates its value 
		pClone[at].value = pSample[at].value;
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
	// solve the provided qubo
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
    return(solution(compiler.qubo()));
}

Qevaluations D5QuboSolver::solution(const Qubo& q)
{
	if (mMinEnergy != cMaxEnergy)
		reset();
	qubo(q);
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

	// for solve threads the samples will be initialized first noInitNodes 
	// (number of initialising nodes) to 0b0...0...0 - 0b1...1...0
	size_t noInitNodes = size_t(log2(nodesNo()) - 0.1);
	size_t noSolveThreads = 1;
	if (noInitNodes > 0)
	{
 		noInitNodes--;
		noSolveThreads = size_t(pow(2, noInitNodes));
	}
	size_t last = nodesNo() - 1;
	SampleElement* pSample = createSample();	// create sample 0b00...0
    // initialize samples and start Qsolve threads ...
	for (size_t at = 0; at < noSolveThreads; ++at)		// 
	{
		// set initial sample nodes values, e.g. 
		// between 0b0...00...0 to 0b1...10...0
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

inline D5QuboSolver::SampleElement* D5QuboSolver::createSample(Qvalue init)
{
	Nodes& nodes = QuboAnalyzer::nodes();
	size_t noNodes = nodes.size();

	const Qubo& q = qubo();
	auto quboEnd = q.end();

	// create a sample as an array of SampleElements coresponding to nodes
	D5QuboSolver::SampleElement* pSample = new D5QuboSolver::SampleElement[noNodes];

	for (std::size_t at = 0; at < noNodes; at++)
	{
		pSample[at].pNode = &nodes[at];
		pSample[at].value = init;
		for (size_t prev = 0; prev <= at; prev++)
		{
			Qkey key(pSample[prev].pNode->first, pSample[at].pNode->first);
			auto pElement = q.find(key);
			if (pElement != quboEnd)
			{	// add energy weight between this and a previous node
				pSample[at].nodesEnergy.push_back(pElement->second);
			}
			else
			{ // if a key is not found, try switching the order of nodes
				key = Qkey(pSample[at].pNode->first, pSample[prev].pNode->first);
				pElement = q.find(key);
				if (pElement != quboEnd)
				{ // add the energy weight of the found branch, between this and
				  // previoud element
					pSample[at].nodesEnergy.push_back(pElement->second);
				}
				else
				{	// if the branch does not exist, the energy weight is 0
					pSample[at].nodesEnergy.push_back(0);
				}
			}
		}
	}
	return pSample;
}
