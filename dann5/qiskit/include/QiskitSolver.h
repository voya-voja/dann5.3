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
		// Qiskit solver is an adapter-superclass of dann5 quantum solver
		// (Qsolver) and all implementations of Qiskit quantum simulators and
        // backends. It evaluates solutions for a given Circuit or a given
        // Qstatement.
		class QiskitSolver : public Qsolver
		{
		public:
            // a shared pointer to a specific Qiskit solver
            typedef shared_ptr<QiskitSolver> Sp;

            // Defual construtor.
            QiskitSolver(bool lowest = true);

            // destructor
            virtual ~QiskitSolver();

            // Returns quantum evaluations for a given quantum statement.
            virtual Qevaluations solution(const Qstatement&);

            // Returns quantum evaluations for a given dann5 circuit.
            virtual Qevaluations solution(const D5circuit&);
/*
            // Returns a lowest energy flag
            bool lowest() const { return mLowest; };

            // sets a lowest energy flag
            void lowest(bool lwst) { mLowest = lwst; };
*/
            // Returns quantum evaluations
            const Qevaluations& solution() const { return mSolution; };

            // Sets quantum evaluations
            void solution(const Qevaluations& solution) { mSolution = solution; };

            // returns a minimal evaluated energy
//            double minEnergy() const { return mMinEnergy; };

            // sets a minimal evaluated energy
//            void minEnergy(double mEnergy) { mMinEnergy = mEnergy; };

            // returns a circuite being solved
            const D5circuit& circuit() const { return mCircuit; };

            // Resets evaluated solutions and the coresponding circuit. 
            virtual void reset();

            // Override to run solve algorith on a coresponding Qiskit quantum
		    // simulator or backend
            virtual void solve() = 0;

        protected:
        private:
//            static const double cMaxEnergy; // numeric_limits<double>::max()

            bool	        mLowest;    // Return only evaluations with minimal energy
//            double      	mMinEnergy; // Minimal evaluated energy
            Qevaluations	mSolution;  // Solution evaluations
            D5circuit       mCircuit;
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