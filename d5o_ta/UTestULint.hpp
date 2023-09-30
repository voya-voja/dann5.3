//
//  UTestULint.hpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-09-29.
//

#ifndef UTestULint_hpp
#define UTestULint_hpp

#include <stdio.h>

#include <iostream>

using namespace std;

namespace dann5
{
    // Unit tests of Qbin operations
    class UTestULint
    {
    public:
        UTestULint() {};
        
        // execute all unit tests
        void runAll(ostream& out);
    protected:
    private:
    };
};
#endif /* UTestULint_hpp */
