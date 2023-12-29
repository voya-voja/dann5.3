//
//  Qtype.h
//  d5
//
//  Created by Nebojsa Vojinovic on 2022-11-12.
//

#ifndef Qtype_h
#define Qtype_h
#pragma once

#include <pybind11/pybind11.h>

#include <utility>

using namespace std;

namespace dann5 {
    // A Quantum type enforces Q type definitions
    class Qtype
    {
    public:
        // Q type shared pointer
        typedef shared_ptr<Qtype> Sp;

        // default constructor
        Qtype() {};

        // Copy constructor
        Qtype(const Qtype& right) {};

        // destruct the Q type
        ~Qtype() {};

    protected:
    private:
    };
};

#endif /* Qtype_h */