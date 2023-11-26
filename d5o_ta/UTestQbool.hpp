//
//  UTestQbool.hpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-23.
//

#ifndef UTestQbool_hpp
#define UTestQbool_hpp

#include <iostream>

using namespace std;

namespace dann5test
{
    // Unit tests of Qbit operations
    class UTestQbool
    {
    public:
        // Default constructor
        UTestQbool() {};
        
        // execute all unit tests
        void runAll(ostream& out);
        
        // test Qbool object initialization (constructors)
        void initialization(ostream& out);
        // test Qbool assignments
        void assignment(ostream& out);
        // test Qbool logical operations and expressions
        void logical(ostream& out);
        // test Qbool comparison operations and expressions
        void comparison(ostream& out);

        // Dwave's friends & enemies problem solved using Qbool logic
        void friends_enemies(ostream& out);
        // Based on Dwave's vertex triangle and sqare graph problem solved using Qbool logic
        void vertex(ostream& out);

    protected:
    private:
    };
};

#endif /* UTestQbool_hpp */
