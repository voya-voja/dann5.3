//
// UTestQwhole.hpp
// d5o_ta
//

#ifndef UTestQwhole_hpp
#define UTestQwhole_hpp

#include <iostream>

using namespace std;

namespace dann5test
{
    // Unit tests of Qwhole operations
    class UTestQwhole
    {
    public:
        // Default constructor
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
        void comparisonLogic(ostream& out);
        void comparison(ostream& out);
 
        // use Qwhole to solve factorial problem
        void factorial(ostream& out);
        // use Qwhole to calculate prime numbers
        void prime(ostream& out);
        // use (6 * Qwhole +/- 1) algorithm to calculate prime numbers
        void prime6(ostream& out);
    };
};
#endif /* UTestQwhole_hpp */
