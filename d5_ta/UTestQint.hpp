//
// UTestQint.hpp
// d5o_ta
//

#ifndef UTestQint_hpp
#define UTestQint_hpp

#include "UTest.h"

using namespace std;

namespace dann5test
{
    // Unit tests of Qint operations
    class UTestQint : public UTest
    {
    public:
        // Default constructor
        UTestQint() {};

        // execute all unit tests
        void runAll(ostream& out);

        // test Qint object initialization (constructors)
        void initialization(ostream& out);
        // test Qint assignments
        void assignment(ostream& out);
        // test Qint inhereted bitwise operations and expressions
        void bitwise(ostream& out);
        // test Qint arithmetic operations and expressions
        void arithmetic(ostream& out);
        // test Qint comparison operations and expressions
        void comparison(ostream& out);
 
        // use Qint to solve factorial problem
        void factorial(ostream& out);
    protected:
        // Provides a unique prefix of the test file name
        virtual string filePrefix() { return "qint"; };

    private:
    };
};
#endif /* UTestQint_hpp */
