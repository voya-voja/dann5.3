#pragma once

#include <pybind11/pybind11.h>

#include <iostream>

#include <Qubo.h>
#include <Qsolver.h>

using namespace std;

namespace dann5 {
	namespace ocean {
        // Qubo analyzer of a given qubo
		class QuboAnalyzer
		{
		public:
            // a logical QUBO node
			typedef std::pair<string, double> Node;
            
            // a list of linear nodes in a QUBO
			typedef std::vector<Node> Nodes;
            
            // a logical QUBO branch connecting two nodes
			typedef std::pair<Qkey, double> Element;
            
            // a list of binary QUBO elements
			typedef std::vector<Element> Branches;

            // Quantum analyzer is initialized with a QUBO instance
            QuboAnalyzer(const Qubo& qubo) : mQubo(qubo) {};

            // returns a list of linear nodes
			Nodes& nodes();
            
            // returns a list of branches, i.e. binary lements
			Branches& branches();

            // returns a number of linear nodes in a given QUBO instance
			inline std::size_t nodesNo() { return nodes().size(); };
            
            // returns a number of branches, i.e. binary QUBO elements
			inline std::size_t branchesNo() { return branches().size(); };

            // returns the given QUBO instance
			inline const Qubo& qubo() const { return mQubo; };

            // returns suggested chain streinght for a given QUBO instance
			inline double chainStrength()
			{
				if (mChainStrength == 0)
					calculateChainStrength();
				return mChainStrength;
			};
            
            virtual void reset();

		protected:
            void qubo(const Qubo& q) { mQubo = q; };
            
            // algorithm to calcualte the chain streingth
			void calculateChainStrength();

		private:
			Qubo		mQubo;              // QUBO instance
			Nodes		mNodes;             // linear nodes
			Branches	mBranches;          // binary elements
			double		mChainStrength = 0; // calculated chain streinght
		};

		// Send a string representation of Nodes or Branches into an output stream
		std::ostream& operator << (std::ostream&, const QuboAnalyzer::Nodes&);
		std::ostream& operator << (std::ostream&, const QuboAnalyzer::Branches&);
	};
};
