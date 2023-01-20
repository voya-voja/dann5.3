//
//  UTestQwhole.cpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2023-01-07.
//
#include "UTestQwhole.hpp"

#include <Qwhole.h>
#include <Qblock.h>
#include <Qroutine.h>
#include <Qbinder.h>

using namespace dann5;
using namespace dann5::ocean;


void UTestQwhole::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Bitwise Expressions ----" << endl;
    bitwise(out);
    out << endl << " --- Arithmetic Expressions ----" << endl;
    arithmetic(out);
    out << endl << " --- Comparison Expressions ----" << endl;
    comparison(out);
    out << endl << " --- Assignments ----" << endl;
    assignment(out);
    out << endl << " --- Factorial ----" << endl;
    factorial(out);
}

void UTestQwhole::initialization(ostream& out)
{
    // Default constructor initializes Q whole with an empty id and supperposition value
    // Initialize Q whole with a given id and supperposition value
    Qwhole dcQwhole, idQwhole("id");
    out << "Qwhole defualt constructor: " << dcQwhole.toString()
        << ", creates an object with empty id, i.e. '', 0 qbits, and value 0." << endl
        << "Qwhole with an id only: " << idQwhole.toString() << " has 0 qbits and value 0, too." << endl;

    Qwhole size3(3, "size3");
    out << "Qwhole 'size3' is unknown and has 3 Qbits:" << endl;
    out << " toString() => " << size3.toString() << endl << " toString(false, 0) => " << size3.toString(false, 0) << endl << " toString(true) => " << size3.toString(true) << endl << " toString(true, 0) => " << size3.toString(true, 0) << endl;

    // Initialize Q whole with a given id and value.
    // If value is different from 0 or 1, Qwhole into superposition state
    Qwhole oct10("octal10", 010), dec5("dec5", 5), bin6("bin6", 0b110), hexA("hexA", 0xA);
    out << "For 'octal10' set to value 010:" << endl;
    out << " toString() => " << oct10.toString() << endl << " toString(true) => " << oct10.toString(true) << endl << " toString(true, 3) => " << oct10.toString(true, 3) << endl;
    out << "For 'dec5' set to value 5:" << endl;
    out << " toString() => " << dec5.toString() << endl << " toString(true) => " << dec5.toString(true) << endl << " toString(true, 1) => " << dec5.toString(true, 1) << endl;
    out << "For 'bin6' set to value 0b110:" << endl;
    out << " toString() => " << bin6.toString() << endl << " toString(true) => " << bin6.toString(true) << endl << " toString(true, 2) => " << bin6.toString(true, 2) << endl;
    out << "For 'hexA' set to value 0xA:" << endl;
    out << " toString() => " << hexA.toString() << endl << " toString(true) => " << hexA.toString(true) << endl << " toString(true, 3) => " << hexA.toString(true, 3) << endl;

    Qwhole qBits("bin6qBits", Qbits(Qbin(bin6)));
    out << "Qbits of 'bin6' have been used to construct '" << qBits.toString() << endl;

    Qwhole cQwhole(hexA);
    out << "A copy of 'hexA' has id '" << cQwhole.id() << "' and value " << Bits(Qbin(cQwhole)) << endl;
}

void UTestQwhole::bitwise(ostream& out)
{
    Qwhole x(3, "x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    Qexpr<Qbin> qbExpr = x & y, xI = ~x;

    out << "Expression '~x', INVERT (not) x is: " << xI << endl
        << " decomposed logic: " << xI.toString(true) << endl
        << " It's Qubo is '" << xI.qubo() << "'" << endl
        << " resulting in :" << endl << xI.solve() << endl;

    out << "Expression AND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nand(y);
    out << "Expression NAND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(4);
    out << "**** x has been resized to 4 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x | y;
    out << "Expression OR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nor(y);
    out << "Expression NOR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x[2].value(0);
    out << "**** x's 2nd qbit is set to 0 ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x ^ y;
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nxor(y);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(2); y.resize(2);
    out << "**** x & y habe been resized to 2 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl
        << " decomposed: " << y.toString(true) << endl << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (Qbin(_0) *= (y | x));
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << qbExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQwhole::arithmetic(ostream& out)
{
    Qwhole x(2, "x"), y("y", 5), z(1, "z"), _3("_3", 3);
    Qexpr<Qwhole> qwExpr = x + y;
    out << "Addition Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y + x;
    out << "Addition Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    Qexpr<Qwhole> qwwExpr = x + y + z + _3;// +qwExpr;
    out << "Addition Expression" << endl << qwwExpr << endl
        << " decomposed logic: " << qwwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwwExpr.solve() << endl;

    qwExpr.reset();
    Qexpr<Qwhole> qxwExpr = qwExpr + z + _3;
    out << "Addition Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr = x + y;
    qxwExpr = z + qwExpr + _3;
    out << "Addition Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl;

    Qexpr<Qwhole> z_3Expr = z + _3;
    Qexpr<Qwhole> qwxExpr = x + y + z_3Expr;
    out << "Addition Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x + y;
    z_3Expr = z + _3;
    Qexpr<Qwhole> qxxExpr = qwExpr + z_3Expr;
    out << "Addition Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr = x + y;
    z_3Expr = z + _3;
    qxxExpr = z_3Expr + qwExpr;
    out << "Addition Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr = y - x;
    out << "Subtraction Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl;
    out << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;
/*
    qwExpr.reset();
    qwxExpr = x - qwExpr;
    out << "Subtraction Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;
*/
    qwExpr = x * y;
    out << "Multiplication Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr = z * qwExpr;
    out << "Multiplication Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr = qwExpr * z;
    out << "Multiplication Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x * y;
    z_3Expr = _3 * z;
    qxxExpr = qwExpr * z_3Expr;
    out << "Division Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxxExpr.qubo() << "'" << endl;
    Qanalyzer anlyze(qxxExpr.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo();

    qwExpr = x / y;
    out << "Division Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    /*
    qwExpr.reset();
    qwxExpr = z / qwExpr;
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;
*/

    Qwhole  r("r", 6), k(2, "k");
    Qexpr<Qwhole> e1 = r - x, e2 = x / k;
    Qblock blck;
    blck = e1, e2;
    cout << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck.solve() << endl;
}


void UTestQwhole::comparison(ostream& out)
{
    Qwhole x(2, "x"), y("y", 5), z(3, "z"), _F("_F", false), _T("_1", 1);

    Qexpr<Qwhole> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
        xXnYeZ1 = (x != y) == z, xXnYeZ2 = x != (y == z);

    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl << "Qubo: " << xEq.qubo() << endl << xEq.solve() << endl
        << xNe.toString() << " decomposed: " << xNe.toString(true) << endl << "Qubo: " << xNe.qubo() << endl << xNe.solve() << endl
        << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl << "Qubo: " << xXnYeZ0.qubo() << endl << xXnYeZ0.solve() << endl
        << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true) << endl << "Qubo: " << xXnYeZ1.qubo() << endl << xXnYeZ1.solve() << endl
        << xXnYeZ2.toString() << " decomposed: " << xXnYeZ2.toString(true) << endl << "Qubo: " << xXnYeZ2.qubo() << endl << xXnYeZ2.solve() << endl;

    Qexpr<Qwhole> qwExpr = x > y;
    out << "Expression > (GT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y < x;
    out << "Reversed expression > (GT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = x >= y;
    out << "Expression >= (GE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y <= x;
    out << "Reversed expression >= (GE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = x < y;
    out << "Expression < (LT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y > x;
    out << "Reversed Expression < (LT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = x <= y;
    out << "Expression <= (LE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y >= x;
    out << "Reversed expression <= (LE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;
/*
    qbExpr = (_T & (x or y) ^ (_F *= (y | x)));
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << qbExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
*/
}

void UTestQwhole::assignment(ostream& out)
{
    Qwhole r(2, "r"), x(3, "x");
    Qassign<Qwhole> qwholeAssign = r = x;

    out << "Assignment 'r = x' creats logic => " << qwholeAssign << endl
        << " It's finalized (execution ready) Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r += x;
    out << endl << "Assignment 'r += x' creats logic => " << qwholeAssign << endl
        << " It's Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r -= x;
    out << endl << "Assignment 'r -= x' creats logic => " << qwholeAssign << endl
        << " It's Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r *= x;
    out << endl << "Assignment 'r *= x' creats logic => " << qwholeAssign << endl
        << " It's Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r /= x;
    out << endl << "Assignment 'r *= x' creats logic => " << qwholeAssign << endl
        << " It's Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    Qwhole y("y", 6), z(2, "z"), k(2, "k");
    Qassign<Qwhole> a1 = y = r - x, a2 = k = y - z;
    Qblock blck;
    blck = a1, a2;
    cout << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck.solve() << endl;
}

void UTestQwhole::factorial(ostream& out)
{
}

void UTestQwhole::prime(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _1("1_", 1), _2("2_", 2);
    Qbin _3("3_", 3), _7("7_", 7);
    Qassign<Qwhole> p_s2_2t2 = prime = s * s + _2 * t * t;
    Qassign<Qbin> p3mod8 = _3 = prime & _7;
    Qassign<Qbin> s1mod2 = Qbin(_1) = s & _1;
    Qassign<Qbin> t1mod2 = Qbin(_1) = t & _1;
    Qblock gcd1;
    {
        Qwhole d(1, "d");
        gcd1 = s = s * d,
                t = t * d,
                d == _1;
    }
    Qblock primeNo;
    primeNo = p_s2_2t2, p3mod8, s1mod2, t1mod2, gcd1;
    cout << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    Qubo qPrime = primeNo.qubo();
    cout << " It's generic Qubo is '" << primeNo.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse(qPrime);
    out << endl << "*** Prime number p = s2 + 2t2, p mod 8 = 3, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: " << analyse.branchesNo() << endl;
//    cout << " resulting in :" << endl << primeNo.solve() << endl;
    Qsolver::Samples solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    cout << " resulting in :" << endl << pst << endl;
}

void UTestQwhole::prime6(ostream& out)
{
    Qwhole prime(4, "p"), s(2, "s"), t(1, "t"), mod(2, "m"), _1("1_", 1), _5("5_", 5), _3("3_", 3), _6("6_", 6), _8("8_", 8);
    Qblock prime6m1;
    {
        prime6m1 = prime = _6 * s - _1; // ,
//        prime6m1 = prime + _1 == _6 * s; // ,
//            prime = t * _8 + mod,
//                   mod != 5;
    }

    cout << "Prime Number\n Code" << endl << prime6m1 << endl
        << " Logic: " << prime6m1.toString(true) << endl;
    Qubo qPrime = prime6m1.qubo();
    cout << " It's generic Qubo is '" << prime6m1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse(qPrime);
    out << endl << "*** Prime number p = 6s - 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: " << analyse.branchesNo() << endl;
    Qsolver::Samples solution = prime6m1.compute();
    Qbinder pst(solution);
    pst = prime, s, t, mod;
    cout << " resulting in :" << endl << pst << endl;

    Qblock prime6p1;
    {
        prime6p1 = prime = _6 * s + _1;
        //                    prime = t * _8 + mod,
        //                    mod != 5;
    }

    cout << "Prime Number\n Code" << endl << prime6p1 << endl
        << " Logic: " << prime6p1.toString(true) << endl;
    qPrime = prime6p1.qubo();
    cout << " It's generic Qubo is '" << prime6p1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse2(qPrime);
    out << endl << "*** Prime number p = 6s + 1 has: "
        << analyse2.nodesNo() << " # of node and # of branches: " << analyse2.branchesNo() << endl;
    Qsolver::Samples solution2 = prime6p1.compute();
    Qbinder pst2(solution2);
    pst2 = prime, s, t, mod;
    cout << " resulting in :" << endl << pst2 << endl;
}
