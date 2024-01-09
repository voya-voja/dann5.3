//
//  QiskitSolver.h
//  d5q
//
//  Created by Nebojsa Vojinovic on 2023-12-23.
//

#ifndef QiskitSolver_h
#define QiskitSolver_h
#pragma once

#include <pybind11/pybind11.h>

#include <Qsolver.h>
#include <Circuits.h>

namespace dann5 {
	namespace qiskit {
		// D-Wave solver is an adapter-superclass of dann5 quantum solver
		// (Qsolver) and all implementations of D-Wave quantum annealing
		// simulator and samplers. It evaluates solutions for a given 
		// QUBO or given Qstatement.
		class QiskitSolver : public Qsolver
		{
		public:
            // a shared pointer to a specific D-Wave solver
            typedef shared_ptr<QiskitSolver> Sp;

            // Defual construtor with an optinal flag to process just
            // evaluations with the lowest energy. When lowest is 'false' the
            // solver will process all the evaluation regardless of their 
            // evaluated energy
            QiskitSolver(bool lowest = true);

            // destructor
            virtual ~QiskitSolver();

            // Returns quantum evaluations for a given quantum statement,
            // Depending on the solver initialization, the returned quantum
            // evaluations can be just those with lowes assessed energy, or
            // the full set of all evaluations.
            virtual Qevaluations solution(const Qstatement&);

            // Returns quantum evaluations for a given QUBO,
            // Depending on the solver initialization, the returned quantum
            // evaluations can be just those with lowes assessed energy, or
            // the full set of all evaluations.
            virtual Qevaluations solution(const Circuits&);

            // Returns a lowest energy flag
            bool lowest() const { return mLowest; };

            // sets a lowest energy flag
            void lowest(bool lwst) { mLowest = lwst; };

            // Returns quantum evaluations
            const Qevaluations& solution() const { return mSolution; };

            // Sets quantum evaluations
            void solution(const Qevaluations& solution) { mSolution = solution; };

            // returns a minimal evaluated energy
//            double minEnergy() const { return mMinEnergy; };

            // sets a minimal evaluated energy
//            void minEnergy(double mEnergy) { mMinEnergy = mEnergy; };

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
//            static const double cMaxEnergy; // numeric_limits<double>::max()

            bool	        mLowest;    // Return only evaluations with minimal energy
//            double      	mMinEnergy; // Minimal evaluated energy
            Qevaluations	mSolution;  // Solution evaluations
        };

        class PyQiskitSolver : public QiskitSolver {
        public:
            /* Inherit the constructors */
            using QiskitSolver::QiskitSolver;

            /* Trampoline (need one for each virtual function) */
            void solve() override {
                PYBIND11_OVERRIDE_PURE(
                    //                PYBIND11_OVERRIDE(
                    void,           /* Return type */
                    QiskitSolver,    /* Parent class */
                    solve,          /* Name of function in C++ (must match Python name) */
                    /* Argument(s) */
                    );
            };
        };
	};
};

#endif /* QiskitSolver_h */