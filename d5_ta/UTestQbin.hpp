//
//  UTestQbin.hpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2022-12-24.
//

#ifndef UTestQbin_hpp
#define UTestQbin_hpp

#include "UTest.h"

using namespace std;

namespace dann5test
{
    // Unit tests of Qbin operations
    class UTestQbin : public UTest
    {
    public:
        // Default constructor
        UTestQbin() {};
        
        // execute all unit tests
        void runAll(ostream& out);
        
        // test Qbin object initialization (constructors)
        void initialization(ostream& out);
        // test inherented Qnary functionality
        void testQnary(ostream& out);
        // test Qbin assignments
        void assignment(ostream& out);
        // test Qbin binwise operations and expressions
        void bitwise(ostream& out);
        // test Qbin comparison operations and expressions
        void comparison(ostream& out);

        // test compares Qbin equal expressions and assignments
        void eq_asign(ostream& out);
        // Dwave's friends & enemies problem solved using Qbin logic
        void friends_enemies(ostream& out);
        // Based on Dwave's vertex triangle and sqare graph problem solved using Qbin logic
        void vertex(ostream& out);

    protected:
        // Provides a unique prefix of the test file name
        virtual string filePrefix() { return "qbin"; };

    private:
    };
};

#endif /* UTestQbin_hpp */
