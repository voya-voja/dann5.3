//
//  UTestQbin.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-24.
//

#include "UTestQbin.hpp"

#include <Qbin.h>

using namespace dann5;
using namespace dann5::ocean;

void UTestQbin::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Test Qnary ----" << endl;
    testQnary(out);
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

void UTestQbin::initialization(ostream& out)
{
    // Default constructor initializes Q bin with an empty id and supperposition value
    // Initialize Q bin with a given id and supperposition value
    Qbin dcQbin, idQbin("id");
    out << "Qbin defualt constructor: " << dcQbin.toString()
    << ", creates an object with empty id, i.e. '', 0 qbits, and value 0." << endl
    << "Qbin with an id only: " << idQbin.toString() << " has 0 qbits and value 0, too." << endl;
    
    
    Qbin size3(3, "size3");
    out << "Qbin 'size3' is undefined and has 3 Qbits:" << endl;
    out << " toString() => " << size3.toString() << endl << " toString(false, 0) => " << size3.toString(false, 0) << endl << " toString(true) => " << size3.toString(true) << endl << " toString(true, 0) => " << size3.toString(true, 0) << endl;
    
    
    Qbin bn0("bn0", 03), bn1(3, "bn1"), bn2(4,"bn2"), bn3("bn3", 0b110), bn4(2, "bn4"), bnr("bnr", 0x5);
    
    // Initialize Q bin with a given id and value.
    // If value is different from 0 or 1, Qbin into superposition state
    Qbin oct10("octal10", 010), dec5("dec5", 5), bin6("bin6", 0b110), hexA("hexA", 0xA);
    out << "For 'octal10' set to value 010:" << endl;
    out << " toString() => " << oct10.toString() << endl << " toString(true) => " << oct10.toString(true) << endl << " toString(true, 3) => " << oct10.toString(true, 3) << endl;
    out << "For 'dec5' set to value 5:" << endl;
    out << " toString() => " << dec5.toString() << endl << " toString(true) => " << dec5.toString(true) << endl << " toString(true, 1) => " << dec5.toString(true, 1) << endl;
    out << "For 'bin6' set to value 0b110:" << endl;
    out << " toString() => " << bin6.toString() << endl << " toString(true) => " << bin6.toString(true) << endl << " toString(true, 2) => " << bin6.toString(true, 2) << endl;
    out << "For 'hexA' set to value 0xA:" << endl;
    out << " toString() => " << hexA.toString() << endl << " toString(true) => " << hexA.toString(true) << endl << " toString(true, 3) => " << hexA.toString(true, 3) << endl;

    Qbin qBits("bin6qBits", Qbits(bin6));
    out << "Qbits of 'bin6' have been used to construct '" << qBits.toString() << endl;

    Qbin cQbin(hexA);
    out << "A copy of 'hexA' has id '" << cQbin.id() << "' and value " << Bits(cQbin) << endl;
}

void UTestQbin::testQnary(ostream& out)
{
    Qbin hexA("hexA", 0xA);
    hexA[1].value(4);
    out << "After 1st qbit of 'hexA' is set to 4 => " << hexA.toString() << endl
        << " decomposed: " << hexA.toString(true) << endl;
    
    hexA[1].value(0);
    out << "After 1st qbit of 'hexA' is set to 0 => " << hexA.toString() << endl;
    
    hexA[2].value(cSuperposition);
    out << "After 2nd qbit of 'hexA' is set to S => " << hexA.toString() << endl
    << " decomposed: " << hexA.toString(true) << endl;
    
    out << "All 'hexA' qbits are in superposition? " << (hexA.all() ? "true" : "false") << endl;
    out << "Any 'hexA' qbit is 0? " << (hexA.any(0) ? "true" : "false") << endl;
    out << "None of 'hexA' qbits is 1? " << (hexA.none(1) ? "true" : "false") << endl;
    out << "How many of 'hexA' qbits are 0? " << to_string(hexA.count(0)) << endl;
    
    out << "Shift a copy left for 2 positions: " << (hexA << size_t(2))->toString(true) << endl;
    out << "Shift a copy right for 1 position: " << (hexA >> size_t(1))->toString(true) << endl;
    hexA >>= size_t(2);
    out << "Shift 'hexA' right for 2 positions: " << hexA.toString(true) << endl;
    hexA <<= size_t(1);
    out << "Shift 'hexA' left for 1 position: " << hexA.toString(true) << endl;
    
    
    out << "1 shift left a copy with superposition: " << (hexA << cSuperposition)->toString(true) << endl;
    out << "1 shift right a copy with 1: " << (hexA >> Qvalue(1))->toString(true) << endl;
    hexA >>= Qvalue(0);
    out << "1 shift right 'hexA' with 0: " << hexA.toString(true) << endl;
    hexA <<= cSuperposition;
    out << "1 shift left 'hexA' with superposition: " << hexA.toString(true) << endl;
}

void UTestQbin::assignment(ostream& out)
{
    Qbin r(2, "r"), x(3, "x");
    Qassign<Qbin> qbinAssign = r = x;
    
    out << "Assignment 'r/2b:U/ = x/3b:U/' creats logic => " << qbinAssign << endl
        << " It's finalized (execution ready) Qubo is '" << qbinAssign.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qbinAssign.solve();
    out << qbinAssign.solutions() << endl;

    qbinAssign = r &= x;
    out << endl << "Assignment 'r &= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << qbinAssign.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qbinAssign.solve();
    out << qbinAssign.solutions() << endl;

    qbinAssign = r |= x;
    out << endl << "Assignment 'r |= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << qbinAssign.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qbinAssign.solve();
    out << qbinAssign.solutions() << endl;

    qbinAssign = r ^= x;
    out << endl << "Assignment 'r ^= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << qbinAssign.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qbinAssign.solve();
    out << qbinAssign.solutions() << endl;
    
    x.resize(1);
    out << "The order of operations is important in following 4 assignments:" << endl;
    Qbin a(1, "a"), b("b", 1), y(1, "y"), z(1, "z"), _0("_0", 0), _1("_1", 1);
    Qassign<Qbin> qbinAssign1 = _1 = (z != (b & x)) | (z != (y ^ _0));
    qbinAssign1.solve();
    out << "Assignment 1" << endl << qbinAssign1 << endl
        << " decomposed logic: " << qbinAssign1.toString(true) << endl
        << " It's generic Qubo is '" << qbinAssign1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbinAssign1.qubo() << "'" << endl
        << " resulting in :" << endl << qbinAssign1.solutions() << endl;

    Qassign<Qbin> qbinAssign2 = _1 = ((b & x) != z) | (z != (y ^ _0));
    qbinAssign2.solve();
    out << "Assignment 2" << endl << qbinAssign2 << endl
        << " decomposed logic: " << qbinAssign2.toString(true) << endl
        << " resulting in :" << endl << qbinAssign2.solutions() << endl;
    
    Qassign<Qbin> qbinAssign3 = _1 = ((b & x) != z) | ((y ^ _0) != z);
    qbinAssign3.solve();
    out << "Assignment 3" << endl << qbinAssign3 << endl
        << " decomposed logic: " << qbinAssign3.toString(true) << endl
        << " resulting in :" << endl << qbinAssign3.solutions() << endl;

    Qassign<Qbin> qbinAssign4 = _1 = (z != (b & x)) | ((y ^ _0) != z);
    qbinAssign4.solve();
    out << "Assignment 4" << endl << qbinAssign4 << endl
        << " decomposed logic: " << qbinAssign4.toString(true) << endl
        << " resulting in :" << endl << qbinAssign4.solutions() << endl;

    Qassign<Qbin> qbinA4 = _1 = (z.unlike(b & x) | z.alike(y ^ _0));
    out << "Assignment 4 using un/a-like operation: " << endl << qbinA4 << endl
        << " decomposed: " << qbinA4.toString(true) << endl;
    Qanalyzer anlyze(qbinA4.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo()
        << qbinA4.solve();

    Qassign<Qbin> qbinA3 = _1 = ((b & x).unlike(z)) | ((y ^ _0).alike(z));
    out << "Assignment 3 using un/a-like operation: " << endl << qbinA3 << endl
        << " decomposed: " << qbinA3.toString(true) << endl
        << qbinA3.solve();
}

void UTestQbin::bitwise(ostream& out)
{
    Qbin x(3, "x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    Qexpr<Qbin> qbExpr = x & y, xI = ~x;

    out << "Expression '~x', NOT (invert) x is: " << xI << endl
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

    qbExpr = x *= y;
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(2); y.resize(2);
    out << "**** x & y habe been resized to 2 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl
        << " decomposed: " << y.toString(true) << endl << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (_0 *= (y | x));
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << qbExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qbExpr.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}


void UTestQbin::comparison(ostream& out)
{
    Qbin x(3, "x"), y("y", 5), z(3, "z");

    Qexpr<Qbin> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
                    xXnYeZ1= (x != y) == z, xXnYeZ2 = x != (y == z);

    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl << "Qubo: " << xEq.qubo() << endl << xEq.solve() << endl
        << xNe.toString() << " decomposed: " << xNe.toString(true) << endl << "Qubo: " << xNe.qubo() << endl << xNe.solve() << endl
        << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl << "Qubo: " << xXnYeZ0.qubo() << endl << xXnYeZ0.solve() << endl
        << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true) << endl << "Qubo: " << xXnYeZ1.qubo() << endl << xXnYeZ1.solve() << endl
        << xXnYeZ2.toString() << " decomposed: " << xXnYeZ2.toString(true) << endl << "Qubo: " << xXnYeZ2.qubo() << endl << xXnYeZ2.solve() << endl;
}

void UTestQbin::eq_asign(ostream& out)
{
    Qbin bn0("bn0", 03), bn1(3, "bn1"), bn2(4,"bn2"), bn3("bn3", 0b110), bn4(2, "bn4"), bnr("bnr", 0x5);
    Qassign<Qbin> qbinAssign = bnr = (bn0 & bn1) | ((bn2 ^ bn3) == bn4);
    cout << endl << qbinAssign << endl << endl << qbinAssign.toString(true) << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qbinAssign.qubo(false) << endl
        << endl << "*** Finalized Qubo ***" << endl << qbinAssign.qubo() << endl;
    Qanalyzer analyze(qbinAssign.qubo());
    cout << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: " << analyze.branchesNo() << endl;
    qbinAssign.solve();
    cout << endl << qbinAssign.solutions();
}

void UTestQbin::friends_enemies(ostream& out)
{
    Qbin x(2, "xaviar"), y(2, "yolanda"), z(2, "zeke"), w(2, "wanda"), r(2, "ramon"), _3("_3", 0b11);
    Qassign<Qbin> abFnEs = _3 = (x *= y) & (y ^ z) & (z *= w);
    out << endl << "*** Dwave's friends & enemies problem equation:" << endl << abFnEs.toString() << endl;
    out << endl << "*** Dwave's friends & enemies problem solutions:" << endl << abFnEs.solve() << endl;
}

void UTestQbin::vertex(ostream& out)
{
    const size_t noVertices = 5;
    Qbit _1("_1", 1), _0("_0", 0);
    Qbin vertices(5, "a");

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
        << endl << axbSV.qubo() << endl;
}
