#pragma once

#include <pybind11/pybind11.h>

#include <iostream>

#include <Qsolver.h>
#include <Qubo.h>
#include <QuboAnalyzer.h>
using namespace std;

namespace dann5 {
	namespace ocean {
        // Qubo solver ia a quantum annealing simulator and a specialization of
        //  a Quantum analyzer. It evaluates solutions for a given QUBO.
		class D5QuboSolver : public Qsolver, public QuboAnalyzer
		{
		public:
            // a shared pointer to a specific dann5 qubo solver
            typedef shared_ptr<D5QuboSolver> Sp;

            D5QuboSolver(bool lowest = true);
            
            // Initialize quantum solver simulator with a qubo problem to be
            // solved. Optionally specify if solver should return just the
            // qubo function minimum, i.e. quantum evaluations with lowest
            // assessed energy
			D5QuboSolver(const Qubo& qubo, bool lowest = true);
            
            // destructor
			~D5QuboSolver();

            // Returns quantum evaluations for a given quantum statement,
            // Depending on the solver initialization, the returned quantum
            // evaluations can be just those with lowes assessed energy, or
            // the full set of all evaluations.
            virtual Qevaluations solution(const Qstatement&);
            
            // Returns quantum evaluations
            const Qevaluations& solution() const { return mSolution; };

            // returns minimal evaluated energy
            double minEnergy() { return mMinEnergy; };
            
            virtual void reset();

        protected:
            // runs solve algorith across multiple threads
			virtual void solve();

		private:
			static const double cMaxEnergy; // numeric_limits<double>::max()

            // RawElement coresponds to a QUBO node
			struct RawElement
			{
                // A coresponding a QUBO node, i.e. a QUBO linear element
                QuboAnalyzer::Node* pNode = nullptr;
                // The list of energies between this and all previous elements
                // (nodes) in a sample, i.e. a list of energy weight of
                // branches between this and all previous nodes concluding with
                // weight energy of this node
				vector<double>	nodesEnergy;
                // The RawElement value in a sample
				Qvalue	value = 0;
                // The total energy of this element is calcualted based on
                // values of this and previous RawElement's (nodes) within the
                // sample and coresponding energies contained in nodesEnergy
				double	valuesXenergySum = cMaxEnergy;
			};
            
            // Returns a pointer to a head RawElement array (created sample),
            // where eachRawElement instance corresponds to one of the nodes in
            // the assigned QUBO. The RawElement value will be initialized to
            // provided init value.
			inline RawElement* createSample(Qvalue init = 0);

            // implementation of quantume solve algorithm
			friend class Qsolve;

			bool	        mLowest;    // Return only evaluations with minimal energy
			double      	mMinEnergy; // Minimal evaluated energy
            Qevaluations	mSolution;  // Solution evaluations
		};
	};
};
