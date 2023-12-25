//
//  DwaveSolver.h
//  d5o
//
//  Created by Nebojsa Vojinovic on 2023-12-23.
//

#ifndef DwaveSolver_h
#define DwaveSolver_h
#pragma once

#include <pybind11/pybind11.h>

#include <Qsolver.h>
#include <Qubo.h>
#include <QuboAnalyzer.h>


namespace dann5 {
	namespace ocean {
		// D-Wave solver is an adapter-superclass of dann5 quantum solver
		// (Qsolver) and all implementations of D-Wave quantum annealing
		// simulator and samplers. It evaluates solutions for a given 
		// QUBO or given Qstatement.
		class DwaveSolver : public Qsolver, public QuboAnalyzer
		{
		public:
            // a shared pointer to a specific D-Wave solver
            typedef shared_ptr<DwaveSolver> Sp;

            // Defual construtor with an optinal flag to process just
            // evaluations with the lowest energy. When lowest is 'false' the
            // solver will process all the evaluation regardless of their 
            // evaluated energy
            DwaveSolver(bool lowest = true);

            // destructor
            virtual ~DwaveSolver();

            // Returns quantum evaluations for a given quantum statement,
            // Depending on the solver initialization, the returned quantum
            // evaluations can be just those with lowes assessed energy, or
            // the full set of all evaluations.
            virtual Qevaluations solution(const Qstatement&);

            // Returns quantum evaluations for a given QUBO,
            // Depending on the solver initialization, the returned quantum
            // evaluations can be just those with lowes assessed energy, or
            // the full set of all evaluations.
            virtual Qevaluations solution(const Qubo&);

            // Returns quantum evaluations
            const Qevaluations& solution() const { return mSolution; };

            // Sets quantum evaluations
            void solution(const Qevaluations& solution) { mSolution = solution; };

            // returns minimal evaluated energy
            double minEnergy() const { return mMinEnergy; };

            // sets a minimal evaluated energy
            void minEnergy(double mEnergy) { mMinEnergy = mEnergy; };

            // reset solutions and assessed minimum energy, 
            // calls Qanalyzer::reset() to reset qubo, and nodes and branches
            // of analyses
            virtual void reset();

            // Override to run solve algorith on a coresponding D-Wave quantum
		    // annealing simulator or sampler
            virtual void solve() = 0;

        protected:
            void init();

        private:
            static const double cMaxEnergy; // numeric_limits<double>::max()

            bool	        mLowest;    // Return only evaluations with minimal energy
            double      	mMinEnergy; // Minimal evaluated energy
            Qevaluations	mSolution;  // Solution evaluations
        };

        class PyDwaveSolver : public DwaveSolver {
        public:
            /* Inherit the constructors */
            using DwaveSolver::DwaveSolver;

            /* Trampoline (need one for each virtual function) */
            void solve() override {
                PYBIND11_OVERRIDE_PURE(
                    //                PYBIND11_OVERRIDE(
                    void,           /* Return type */
                    DwaveSolver,    /* Parent class */
                    solve,          /* Name of function in C++ (must match Python name) */
                    /* Argument(s) */
                    );
            };
        };
	};
};

#endif /* DwaveSolver_h */