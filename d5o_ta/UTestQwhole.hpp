#pragma once
//
//  UTestQbool.hpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2023-01-07.
//

#ifndef UTestQwhole_hpp
#define UTestQwhole_hpp

#include <stdio.h>

#include <iostream>

using namespace std;

namespace dann5
{
    // Unit tests of Qbit operations
    class UTestQwhole
    {
    public:
        UTestQwhole() {};

        // execute all unit tests
        void runAll(ostream& out);

        // test Qwhole object initialization (constructors)
        void initialization(ostream& out);
        // test Qwhole assignments
        void assignment(ostream& out);
        // test Qwhole inhereted bitwise operations and expressions
        void bitwise(ostream& out);
        // test Qwhole arithmetic operations and expressions
        void arithmetic(ostream& out);
        // test Qwhole comparison operations and expressions
        void comparison(ostream& out);
 
        // use Qwhole to solve factorial problem
        void factorial(ostream& out);
        // use Qwhole to calculate prime numbers
        void prime(ostream& out);
        // use (6 * Qwhole +/- 1) algorithm to calculate prime numbers
        void prime6(ostream& out);


    protected:
    private:
    };
};

#endif /* UTestQwhole_hpp */

