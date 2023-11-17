//
//  UTestQbit.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-13.
//
#include "UTestQbit.h"

#include <Qbit.h>
#include <Qubo.h>

using namespace dann5test;

using namespace dann5;
using namespace dann5::ocean;

void UTestQbit::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Bitwise Expressions ----" << endl;
    bitwise(out);
    out << endl << " --- Comparison Expressions ----" << endl;
    comparison(out);
    out << endl << " --- Assignments ----" << endl;
    assignment(out);
    out << endl << " --- Equal Expressions vs. Assignments ----" << endl;
    eq_asign(out);
    out << endl << " --- Friends & enemies problem ----" << endl;
    friends_enemies(out);
    out << endl << " --- Vertex problem ----" << endl;
    vertex(out);
}

void UTestQbit::initialization(ostream& out)
{
    // Default constructor initializes Q bit with an empty id and supperposition value
    // Initialize Q bit with a given id and supperposition value
    Qbit dcQbit, idQbit("id");
    out << "Qbit defualt constructor: " << dcQbit.toString()
        << ", creates an object with empty id, i.e. '',\n and it is set to superposition state, i.e. S" << endl
        << "Qbit with an id only: " << idQbit.toString() << " is set to supportposition state, too." << endl;
    
    // Initialize Q bit with a given id and value.
    // If value is different from 0 or 1, Qbit into superposition state
    Qbit aIs1("a", 1), bIsS("b", 5);
    out << "Qbit with an id 'a' is set to value 1:" << endl;
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
//        << " It's finalized (execution ready) Qubo is '" << qbitAssign.qubo() << "'"
    << endl;
    out << "resulting in solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r &= x;
    out << endl << "Assignment 'r &= x' creats logic => " << qbitAssign << endl
//        << " It's Qubo is '" << qbitAssign.qubo() << "'"
    << endl;
    out << "resulting in solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r |= x;
    out << endl << "Assignment 'r |= x' creats logic => " << qbitAssign << endl
//        << " It's Qubo is '" << qbitAssign.qubo() << "'"
    << endl;
    out << "resulting in solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;

    qbitAssign = r ^= x;
    out << endl << "Assignment 'r ^= x' creats logic => " << qbitAssign << endl
//        << " It's Qubo is '" << qbitAssign.qubo() << "'"
    << endl;
    out << "resulting in solutions:"<< endl;
    qbitAssign.solve();
    out << qbitAssign.solutions() << endl;
    
    out << "The order of operations is important in following 4 assignments:" << endl;
    Qbit a("a"), b("b", 1), y("y", 5), z("z"), _0("_0", 0), _1("_1", 1);
    Qassign<Qbit> qbitAssign1 = _1 = (z != (b & x)) | (z != (y ^ _0));
    qbitAssign1.solve();
    out << "Assignment 1" << endl << qbitAssign1 << endl
        << " decomposed logic: " << qbitAssign1.toString(true) << endl
//        << " It's generic Qubo is '" << qbitAssign1.qubo(false) << "'" << endl
//        << " & finalized Qubo is '" << qbitAssign1.qubo() << "'" << endl
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

    Qassign<Qbit> qbitA4 = _1 = (z.unlike(b & x) | z.alike(y ^ _0));
    out << "Assignment 4 using un/a-like operation: " << endl << qbitA4 << endl
        << " decomposed: " << qbitA4.toString(true) << endl;
//    Qanalyzer anlyze(qbitA4.qubo());
    out << endl << " # of nodes: "
//        << anlyze.nodesNo() << " # of branches: " << anlyze.branchesNo()
        << qbitA4.solve();

    Qassign<Qbit> qbitA3 = _1 = ((b & x).unlike(z)) | ((y ^ _0).alike(z));
    out << "Assignment 3 using un/a-like operation: " << endl << qbitA3 << endl
        << " decomposed: " << qbitA3.toString(true) << endl
        << qbitA3.solve();
}

void UTestQbit::bitwise(ostream& out)
{
    Qbit x("x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    Qexpr<Qbit> qbExpr = x & y, xI = ~x;

    out << "Expression '~x', NOT (invert) x is: " << xI << endl
        << " decomposed logic: " << xI.toString(true) << endl
//        << " It's Qubo is '" << xI.qubo() << "'" << endl
        << " resulting in :" << endl << xI.solve() << endl;

    out << "Expression AND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.nand(y);
    out << "Expression NAND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x | y;
    out << "Expression OR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.nor(y);
    out << "Expression NOR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x ^ y;
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x *= y;
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (_0 *= (y | x));
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
//        << " It's generic Qubo is '" << qbExpr.qubo(false) << "'" << endl
//        << " & finalized Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}


void UTestQbit::comparison(ostream& out)
{
    Qbit a("a"), r("r"), b("b"), x("x"), y("y"), z("z"), _0("_0", 0), _1("_1", 1);

    Qexpr<Qbit> xGtNe = z > (x != y), xGeNe = z >= (x != y), xLtNe = z < (x != y), xLeNe = z <= (x != y);

    Qexpr<Qbit> xNeGt = (x != y) > z, xNeGe = (x != y) >= z, xNeLt = (x != y) < z, xNeLe = (x != y) <= z;

    out << xGtNe.toString() << " decomposed: " << xGtNe.toString(true) << endl //<< "Qubo: " << xGtNe.qubo()
        << endl << xGtNe.solve() << endl
        << xGeNe.toString() << " decomposed: " << xGeNe.toString(true) << endl //<< "Qubo: " << xGeNe.qubo()
        << endl << xGeNe.solve() << endl
        << xLtNe.toString() << " decomposed: " << xLtNe.toString(true) << endl //<< "Qubo: " << xLtNe.qubo()
        << endl << xLtNe.solve() << endl
        << xLeNe.toString() << " decomposed: " << xLeNe.toString(true) << endl //<< "Qubo: " << xLeNe.qubo()
        << endl << xLeNe.solve() << endl
        << xNeGt.toString() << " decomposed: " << xNeGt.toString(true) << endl //<< "Qubo: " << xNeGt.qubo()
        << endl << xNeGt.solve() << endl
        << xNeGe.toString() << " decomposed: " << xNeGe.toString(true) << endl //<< "Qubo: " << xNeGe.qubo()
        << endl << xNeGe.solve() << endl
        << xNeLt.toString() << " decomposed: " << xNeLt.toString(true) << endl //<< "Qubo: " << xNeLt.qubo()
        << endl << xNeLt.solve() << endl
        << xNeLe.toString() << " decomposed: " << xNeLe.toString(true) << endl //<< "Qubo: " << xNeLe.qubo()
        << endl << xNeLe.solve() << endl;
}

void UTestQbit::eq_asign(ostream& out)
{
    Qbit a("a"), r("r"), b("b"), x("x"), y("y"), z("z"), _0("_0", 0), _1("_1", 1);

    Qexpr<Qbit> xEq = x == y, xAl = x *= y, xNe = x != y, xUl = x ^ y;

    Qassign<Qbit> aR = r = a, axEq = r = x == y, axAl = r = x *= y, axNe = r = x != y, axUl = _1 = x ^ y;

    out << "Assignment " << aR //<< "\n qubo: " << aR.qubo()
        << endl << aR.solve() << endl
        << "Equal Expression " << xEq //<< "\n qubo: " << xEq.qubo()
        << endl << xEq.solve() << endl
        << "Equal Assignment " << axEq //<< "\n qubo: " << axEq.qubo()
        << endl << axEq.solve() << endl
        << "Alike Expression " << xAl //<< "\n qubo: " << xAl.qubo()
        << endl << xAl.solve() << endl
        << "Alike Assignment " << axAl //<< "\n qubo: " << axAl.qubo()
        << endl << axAl.solve() << endl;

    out << "Not Equal Expression " << xNe //<< "\n qubo: " << xNe.qubo()
        << endl << xNe.solve() << endl
        << "Not Equal Assignment " << axNe //<< "\n qubo: " << axNe.qubo()
        << endl << axNe.solve() << endl
        << "Unlike Expression " << xUl //<< "\n qubo: " << xUl.qubo()
        << endl << xUl.solve() << endl
        << "Unlike Assignment " << axUl //<< "\n qubo: " << axUl.qubo()
        << endl << axUl.solve() << endl;
}

void UTestQbit::friends_enemies(ostream& out)
{
    Qbit x("xaviar"), y("yolanda"), z("zeke"), w("wanda"), _1("_1", 1);
    Qassign<Qbit> abFnEs = _1 = (x *= y) & ((y ^ z) & (z *= w));
    out << endl << "*** Dwave's friends & enemies problem equation:" << endl << abFnEs.toString() << endl;
    out << endl << "*** Dwave's friends & enemies problem solutions:" << endl << abFnEs.solve() << endl;
}

void UTestQbit::vertex(ostream& out)
{
    const size_t noVertices = 5;
    Qbit _1("_1", 1), _0("_0", 0), vertices[noVertices];
    for(int at = 0; at < noVertices; at++)
        vertices[at].id(string(1, char('a' + at)));


    Qexpr<Qbit> xbTV = (vertices[2] ^ vertices[3]) & (vertices[3] ^ vertices[4]) & (vertices[3] *= _0)
                    | (vertices[3] ^ vertices[2]) & (vertices[2] ^ vertices[4]) & (vertices[2] *= _0)
                    | (vertices[2] ^ vertices[4]) & (vertices[4] ^ vertices[3]) & (vertices[4] *= _0);
    Qassign<Qbit> axbTV = _1 = xbTV;
    out << endl << "---- Triangle c-d-e equation:" << endl << axbTV.toString() << endl;
    out << endl << "---- Triangle c-d-e solutions:" << endl << axbTV.solve() << endl;
    // sqare
    Qexpr<Qbit> xbSV = ((vertices[0] ^ vertices[1]) & (vertices[0] ^ vertices[3])
                        & (vertices[2] ^ vertices[1]) & (vertices[2] ^ vertices[3]))
                        & (((vertices[0] *= _0) & (vertices[2] *= _0))
                           | ((vertices[1] *= _0) & (vertices[3] *= _0)));
    Qassign<Qbit> axbSV = _1 = xbSV;
    out << endl << "---- Square a-b-c-d equation:" << endl << axbSV.toString() << endl;
    out << endl << "---- Square a-b-c-d solutions:" << endl << axbSV.solve() << endl;
    
    Qassign<Qbit> problemAssignment = _1 = xbSV & xbTV;
    out << endl << "---- DWave square(a-b-c-d) + triangle (c-d-e) vertex problem equation:"
        << endl << axbSV.toString() << endl;
    out << endl << "---- DWave square(a-b-c-d) + triangle (c-d-e) vertex problem qubo:"
//        << endl << axbSV.qubo()
    << endl;
}
