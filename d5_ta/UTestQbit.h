//
//  UTestQbit.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-13.
//

#ifndef UTestQbit_h
#define UTestQbit_h

#include <iostream>

using namespace std;

namespace dann5test
{
    // Unit tests of Qbit operations
    class UTestQbit
    {
    public:
        // Default constructor
        UTestQbit() {};
        
        // execute all unit tests
        void runAll(ostream& out);
        
        // test Qbit object initialization (constructors)
        void initialization(ostream& out);
        // test Qbit assignments
        void assignment(ostream& out);
        // test Qbit bitwise operations and expressions
        void bitwise(ostream& out);
        // test Qbit comparison operations and expressions
        void comparison(ostream& out);

        // test compares Qbit equal expressions and assignments
        void eq_asign(ostream& out);
        // Dwave's friends & enemies problem solved using Qbit logic
        void friends_enemies(ostream& out);
        // Based on Dwave's vertex triangle and sqare graph problem solved using Qbit logic
        void vertex(ostream& out);

    protected:
    private:
    };
};

#endif /* UTestQbit_h */
