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
            // Depending on the solver initializatio, the returned quantum
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
			static const double cMaxEnergy;

			struct RawElement
			{
                QuboAnalyzer::Node* pNode = nullptr;
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
