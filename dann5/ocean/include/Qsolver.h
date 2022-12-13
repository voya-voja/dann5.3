#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <thread>

#include <Qubo.h>


using namespace std;


namespace dann5 {
	namespace ocean {
		class Qanalyzer
		{
		public:
			typedef std::pair<string, double> Node;
			typedef std::vector<Node> Nodes;
			typedef std::pair<Qkey, double> Element;
			typedef std::vector<Element> Branches;

			Qanalyzer(const Qubo& qubo) : mQubo(qubo) {};

			Nodes& nodes();
			Branches& branches();

			inline std::size_t nodesNo() { return nodes().size(); };
			inline std::size_t branchesNo() { return branches().size(); };

			inline const Qubo& qubo() const { return mQubo; };

		protected:
		private:
			Qubo		mQubo;
			Nodes		mNodes;
			Branches	mBranches;
		};

		// Send a string representation of Nodes or Branches into an output stream
		std::ostream& operator << (std::ostream&, const Qanalyzer::Nodes&);
		std::ostream& operator << (std::ostream&, const Qanalyzer::Branches&);

		class Qsolver : public Qanalyzer
		{
		public:
			// A semple is defined as a dictionary (map) of definition nodes and their values.
			// The node names are defined by qubo() for each Q equation
			typedef map<string, Qvalue> Sample;
			struct SampleEng
			{
				Sample mSample;
				double mEnergy;
			};

			// A list of samples with the same list of nodes and different combination of values
			typedef vector<SampleEng> Samples;

			Qsolver(const Qubo& qubo, bool lowest = true);
			~Qsolver();

			Samples solution();
			double minEnergy() { return mMinEnergy; };
			void solution(ostream& out);

		protected:
			void solve();

		private:
			static const double cMaxEnergy;

			struct RawElement
			{
				Qanalyzer::Node* pNode = nullptr;
				vector<double>	nodesEnergy;
				Qvalue	value = 0;
				double	valuesXenergySum = cMaxEnergy;
			};

			inline RawElement* createSample(Qvalue init = 0);

			friend class Qsolve;

			bool	mLowest;
			double	mMinEnergy;
			Samples	mSolution;
		};
	};
};
