#pragma once

#include <pybind11/pybind11.h>

#include <map>
#include <iostream>

#include <Qstatement.h>
#include <Qevaluation.h>

using namespace std;

namespace dann5 {
    // Quantum solver abstraction evaluates solutions for a compiled quantum
    // statement.
    class Qsolver
    {
    public:
        // Qsolver's shared pointer
        typedef shared_ptr<Qsolver> Sp;
        
        // Returns an active quantum solver used as a default solver
        static Qsolver::Sp Active() { return gpActive; };
        
        // Sets an active quantum solver to be used as a default solver
        static void Active(Qsolver::Sp solver) { gpActive = solver; };

        // Default quantum solver constructor
        Qsolver() {};
        
        // destructor
        ~Qsolver() {};

        // Override to return quantum evaluations for a compiled quantum statement
        virtual Qevaluations solution(const Qstatement&) = 0;

    protected:

    private:
        static Qsolver::Sp  gpActive;   // Active instance of Qsolver
    };
};
