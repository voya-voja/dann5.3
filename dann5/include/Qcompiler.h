//
//  Qcompiler.h
//  d5o_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-12.
//

#ifndef Qcompiler_h
#define Qcompiler_h
#pragma once

#include <pybind11/pybind11.h>

namespace dann5
{
    class Qop;

    // An abstration of quantum operation compiler is an integration point
    // for conversion of dann5 quantum peration to target quantum solver
    // presentation, like qiskit in case of IBM or Qubo in case of D-Wave
    class Qcompiler
    {
    public:
        // Default constructor
        Qcompiler() {};
        
        // A method to be executed by active Qsolver when solving a quantum
        // statement.
        virtual void compile(const Qop&) = 0;

    protected:
    private:
    };
};

#endif /* Qcompiler_h */
