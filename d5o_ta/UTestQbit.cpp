//
//  UTestQbit.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-13.
//

#include <stdio.h>

#include <Qbit.h>

#include "UTestQbit.h"

using namespace dann5;
using namespace dann5::ocean;

void UTestQbit::runAll(ostream& out)
{
    out << endl << " --- Initialization ----" << endl;
    initialization(out);
    out << endl << " --- Bitwise Expressions ----" << endl;
    bitwise(out);
    out << endl << " --- Assignment ----" << endl;
    assignment(out);
}

void UTestQbit::initialization(ostream& out)
{
    // Default constructor initializes Q bit with an empty id and supperposition value
    // Initialize Q bit with a given id and supperposition value
    Qbit dcQbit, idQbit("id");
    
    // Initialize Q bit with a given id and value.
    // If value is different from 0 or 1, Qbit into superposition state
    Qbit aIs1("a", 1), bIsS("b", 5);
    out << "For 'a' set to value 1:" << endl;
    out << " toString() => " << aIs1.toString() << endl << " toString(false, 0) => " << aIs1.toString(false, 0) << endl << " toString(true) => " << aIs1.toString(true) << endl << " toString(true, 0) => " << aIs1.toString(true, 0) << endl;
    out << endl << "'b' was set to 5, but it is " << bIsS.toString() << endl;

    Qbit cQbit(aIs1);
    out << "A copy of 'a' has id '" << cQbit.id() << "' and value " << to_string(cQbit.value()) << endl;
    cQbit.value(4);
    out << "After copy of 'a' is set to 4 => " << cQbit.toString() << endl;
    
    cQbit.value(0);
    out << "After copy of 'a' is set to 0 => " << cQbit.toString() << endl;
    
    cQbit.value(cSuperposition);
    out << "After copy of 'a' is set to S => " << cQbit.toString() << endl;
}

void UTestQbit::assignment(ostream& out)
{
    Qbit r("r"), x("x");
    Qassign<Qbit> qbitAssign = r = x;
    
    out << "Assignment 'r = x' creats logic => " << qbitAssign << endl
        << " It's finalized (execution ready) Qubo is '" << qbitAssign.qubo() << "'" << endl;
    out << "resulting in 2 solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r &= x;
    out << endl << "Assignment 'r &= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << qbitAssign.qubo() << "'" << endl;
    out << "resulting in 2 solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r |= x;
    out << endl << "Assignment 'r |= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << qbitAssign.qubo() << "'" << endl;
    out << "resulting in 2 solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r ^= x;
    out << endl << "Assignment 'r ^= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << qbitAssign.qubo() << "'" << endl;
    out << "resulting in 2 solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;
    
    out << "The order of operations is important in following 4 assignments:" << endl;
    Qbit a("a"), b("b", 1), y("y", 5), z("z"), _0("_0_", 0), _1("_1_", 1);
    Qassign<Qbit> qbitAssign1 = _1 = (z != (b & x)) | (z != (y ^ _0));
    qbitAssign1.solve();
    out << "Assignment 1" << endl << qbitAssign1 << endl
        << " decomposed logic: " << qbitAssign1.toString(true) << endl
        << " It's generic Qubo is '" << qbitAssign1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbitAssign1.qubo() << "'" << endl
        << " resulting in :" << endl << qbitAssign1.solutions() << endl;

    Qassign<Qbit> qbitAssign2 = _1 = ((b & x) != z) | (z != (y ^ _0));
    qbitAssign2.solve();
    out << "Assignment 2" << endl << qbitAssign2 << endl
        << " decomposed logic: " << qbitAssign2.toString(true) << endl
        << " resulting in :" << endl << qbitAssign2.solutions() << endl;
    
    Qassign<Qbit> qbitAssign3 = _1 = ((b & x) != z) | ((y ^ _0) != z);
    qbitAssign3.solve();
    out << "Assignment 3" << endl << qbitAssign3 << endl
        << " decomposed logic: " << qbitAssign3.toString(true) << endl
        << " resulting in :" << endl << qbitAssign3.solutions() << endl;

    Qassign<Qbit> qbitAssign4 = _1 = (z != (b & x)) | ((y ^ _0) != z);
    qbitAssign4.solve();
    out << "Assignment 4" << endl << qbitAssign4 << endl
        << " decomposed logic: " << qbitAssign4.toString(true) << endl
        << " resulting in :" << endl << qbitAssign4.solutions() << endl;
}

void UTestQbit::bitwise(ostream& out)
{
    Qbit x("x"), y("y", 5), _0("_0_", 0), _1("_1_", 1);
    Qexpr<Qbit> qbExpr = x & y;
    out << "Expression AND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.nand(y);
    out << "Expression NAND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
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

    qbExpr = x ^ y;
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x *= y;
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (_0 *= (y | x));
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << qbExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}
