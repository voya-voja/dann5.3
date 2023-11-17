#pragma once

#include <pybind11/pybind11.h>

#include <map>
#include <iostream>

#include <Qstatement.h>
#include <Qevaluation.h>

using namespace std;

namespace dann5 {
    // Quantum solver abstraction is a specialization of a Quantum analyzer.
    // It evaluates solutions for a compiled quantum statement.
    class Qsolver
    {
    public:
        // Qsolver's shared pointer
        typedef shared_ptr<Qsolver> Sp;
        
        static Qsolver::Sp Active() { return gpActive; };
        
        static void Active(Qsolver::Sp solver) { gpActive = solver; };

        // Default quantum solver constructor
        Qsolver() {};
        
        // destructor
        ~Qsolver() {};

        // Override to return quantum evaluations for a compiled quantum statement
        virtual Qevaluations solution(const Qstatement&) = 0;

    protected:

    private:
        static Qsolver::Sp  gpActive;
    };
};
