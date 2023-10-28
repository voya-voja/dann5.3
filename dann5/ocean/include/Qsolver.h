#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <thread>

#include <Qubo.h>

#include <iostream>

using namespace std;


namespace dann5 {
	namespace ocean {
        // Quantum analyzer of a given qubo
		class Qanalyzer
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
			Qanalyzer(const Qubo& qubo) : mQubo(qubo) {};

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

		protected:
            // algorithm to calcualte the chain streingth
			void calculateChainStrength();

		private:
			Qubo		mQubo;              // QUBO instance
			Nodes		mNodes;             // linear nodes
			Branches	mBranches;          // binary elements
			double		mChainStrength = 0; // calculated chain streinght
		};

		// Send a string representation of Nodes or Branches into an output stream
		std::ostream& operator << (std::ostream&, const Qanalyzer::Nodes&);
		std::ostream& operator << (std::ostream&, const Qanalyzer::Branches&);

        // A quantum sample is defined as a dictionary (map) of definition
        // nodes and their values. The node names are defined by coresponding
        // Qubo object sent to a quantum solver
        typedef map<string, Qvalue> Qsample;
        
        // A quantum evaluation is a pair of a sample and its assessed energy
        // by quantum solver for a given Qubo
        class Qevaluation
        {
        public:
            constexpr static const double cOoRenergy = MAXFLOAT;   // Out of Range Energy
            
            // default constructor
            Qevaluation() :mEnergy(cOoRenergy) {};
            
            // copy constructor
            Qevaluation(const Qevaluation& right): mSample(right.mSample), mEnergy(right.mEnergy) {};
            
            // construct evaluation by pairing a sample and assessed energy
            Qevaluation(const Qsample& sample, double energy): mSample(sample), mEnergy(energy) {};
            Qevaluation(double energy, const Qsample& sample): mSample(sample), mEnergy(energy) {};
            
            // reset this evaluation to initial values
            void reset() {
                mEnergy = cOoRenergy;
                mSample.clear();
            }
            
            // the evaluation is valid if the sample set has elements
            bool isValid() { return mSample.size() > 0; };

            // merge this and right evaluation with same or different elements
            Qevaluation& operator+=(const Qevaluation& right);
            
            // return a quantum evaluation with merge elements from this and
            // right object
            Qevaluation operator+(const Qevaluation& right) const;
            
            // return sample object
            Qsample& sample() { return mSample; };
            const Qsample& sample() const { return mSample; };
            
            // return energy value
            double& energy() { return mEnergy; };
            double energy() const { return mEnergy; };

        protected:
        private:
            Qsample mSample;    // the evaluation sample
            double mEnergy;     // the assessed energy 
        };

        // A list of quantum evaluations by quantum solver for a coresponding Qubo
        typedef vector<Qevaluation> Qevaluations;
    
        // merges two quantum evaluations vectors with same and/or different elements
        Qevaluations operator+(const Qevaluations& left, const Qevaluations& right);
        
        // Sends a representation of Qevaluations into output stream
        std::ostream& operator << (std::ostream&, const Qevaluations&);

        // Quantum solver simulator is a specialization of a Quantum analyzer.
        // It evaluates solutions for a given QUBO.
		class Qsolver : public Qanalyzer
		{
		public:
            // Initialize quantum solver simulator with a qubo problem to be
            // solved. Optionally specify if solver should return just the
            // qubo function minimum, i.e. quantum evaluations with lowest
            // assessed energy
			Qsolver(const Qubo& qubo, bool lowest = true);
            
            // destructor
			~Qsolver();

            // return quantum evaluations, the Qevaluations can be just
            // those with lowes assessed energy or the full set of all
            // evaluations, depending on Qsolver initialization
            virtual Qevaluations solution();
            
            // returns minimal evaluated energy
			double minEnergy() { return mMinEnergy; };
            
            // send the string representation of the solution to output stream
			void solution(ostream& out);

		protected:
            // runs solve algorith across multiple threads
			virtual void solve();

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

            // implementation of quantume solve algorithm
			friend class Qsolve;

			bool	        mLowest;
			double      	mMinEnergy;
            Qevaluations	mSolution;
		};
	};
};
