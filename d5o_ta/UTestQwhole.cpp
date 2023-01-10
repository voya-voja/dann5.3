//
//  UTestQwhole.cpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2023-01-07.
//
#include "UTestQwhole.hpp"

#include <Qwhole.h>

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

    x[3].value(cSuperposition);
    out << "**** x's 3rd qbit is set to superposition ****" << endl
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
    Qwhole x(2, "x"), y("y", 5), z(1, "z");
    Qexpr<Qwhole> qwExpr = x + y;
    out << "Addition Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    Qexpr<Qwhole> qwxExpr = z + qwExpr;
    out << "Addition Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x - y;
    out << "Subtraction Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwxExpr = z - qwExpr;
    out << "Subtraction Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x * y;
    out << "Multiplication Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwxExpr = z * qwExpr;
    out << "Multiplication Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x / y;
    out << "Division Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qwExpr.solve() << endl;

    qwxExpr = z / qwExpr;
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;
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
}

void UTestQwhole::factorial(ostream& out)
{
}