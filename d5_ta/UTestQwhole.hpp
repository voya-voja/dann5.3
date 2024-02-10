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
        // use Qwhole to calculate prime numbers using the triangular theoreme
        void primeTriangular(ostream& out);

        // using p3 algorithm from the triangular theoreme calculate primes
        // Prime number p = s^2 + 2t^2, p mod 8 = 3, gcd(s,t) = 1
        void p3_s2t(ostream& out);

        // using p5 algorithm from the triangular theoreme calculate primes
        // Prime number p = s^2 + 4t^2, p mod 8 = 5, gcd(s,t) = 1
        void p5_s4t(ostream& out);

        // using p1 algorithm from the triangular theoreme calculate primes
        // Prime number p = s^2 + 8t^2, p mod 8 = 1, gcd(s,t) = 1
        void p1_s8t(ostream& out);

        // using p1 algorithm from the triangular theoreme calculate primes
        // Prime number p = s^2 + 16t^2, p mod 8 = 1, gcd(s,t) = 1
        void p1_s16t(ostream& out);

        // using p1 algorithm from the triangular theoreme calculate primes
        // Prime number p = s2 + 8st + 8t2, p mod 8 = 1, gcd(s,t) = 1
        void p1_s8st8t(ostream& out);

        // using p1 algorithm from the triangular theoreme calculate primes
        // Prime number p = s2 + 4st + 2t2, p mod 8 = 7, gcd(s,t) = 1
        void p7_s4st2t(ostream& out);

        // use p = 6 * Qwhole +/- 1 algorithms to calculate prime numbers
        void prime6factor(ostream& out);

        // use p = 6 * s - 1 algorithm to calculate prime numbers
        void p6sm1(ostream& out);

        // use p + 1 == 6 * s algorithm to calculate prime numbers
        void pp1e6s(ostream& out);

        // use p = 6 * s + 1 algorithm to calculate prime numbers
        void p6sp1(ostream& out);
    };
};
#endif /* UTestQwhole_hpp */
