//
//  QlogicCompiler.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-23.
//

#ifndef QlogicCompiler_h
#define QlogicCompiler_h
#pragma once

#include <pybind11/pybind11.h>

#include <Qcompiler.h>
#include <Qop.h>
#include <Qcell.h>

namespace dann5
{
    // Compiles a quantum operation logic and creates a list of shared-pointers
    // referencing quantum cells within the quantum operation
    class QlogicCompiler : public Qcompiler
    {
    public:
        // Default constructor
        QlogicCompiler() {};
        
        // A method to be executed to compile quantum logic of the speficified
        // operation.
        virtual void compile(const Qop& op);
        
        // return a quantum cells with the compiled logic
        const Qcells& logic() { return mLogic; };
        
    protected:
    private:
        Qcells  mLogic; // A list of shared-pointers to logic quantum cells
    };
};

#endif /* QlogicCompiler_h */
