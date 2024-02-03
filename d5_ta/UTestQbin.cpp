//
//  UTestQbin.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-24.
//

#include "UTestQbin.hpp"

#include <Qbin.h>
#include <Qblock.h>
#include <Qbinder.h>
#include <Qubo.h>
#include <QuboCompiler.h>
#include <QuboAnalyzer.h>
#include <D5QuboSolver.h>

using namespace dann5test;

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
    out << "Qbin default constructor: " << dcQbin.toString()
        << ", creates an object with empty id, i.e. '', 0 qbits, and value 0."
        << endl << "Qbin with an id only: " << idQbin.toString()
        << " has 0 qbits and value 0, too." << endl;
    
    Qbin size3(3, "size3");
    out << "Qbin 'size3' is unknown and has 3 Qbits:" << endl;
    out << " toString() => " << size3.toString() << endl
        << " toString(false, 0) => " << size3.toString(false, 0) << endl
        << " toString(true) => " << size3.toString(true) << endl
        << " toString(true, 0) => " << size3.toString(true, 0) << endl;
        
    // Initialize Q bin with a given id and value.
    // If value is different from 0 or 1, Qbin into superposition state
    Qbin oct10("octal10", 010), dec5("dec5", 5), bin6("bin6", 0b110),
        hexA("hexA", 0xA);
    out << "For 'octal10' set to value 010:" << endl;
    out << " toString() => " << oct10.toString() << endl
        << " toString(true) => " << oct10.toString(true) << endl
        << " toString(true, 3) => " << oct10.toString(true, 3) << endl;
    out << "For 'dec5' set to value 5:" << endl;
    out << " toString() => " << dec5.toString() << endl
        << " toString(true) => " << dec5.toString(true)
        << endl << " toString(true, 1) => " << dec5.toString(true, 1) << endl;
    out << "For 'bin6' set to value 0b110:" << endl;
    out << " toString() => " << bin6.toString() << endl
        << " toString(true) => " << bin6.toString(true) << endl
        << " toString(true, 2) => " << bin6.toString(true, 2) << endl;
    out << "For 'hexA' set to value 0xA:" << endl;
    out << " toString() => " << hexA.toString() << endl
        << " toString(true) => " << hexA.toString(true) << endl
        << " toString(true, 3) => " << hexA.toString(true, 3) << endl;

    Qbin qBits("bin6qBits", Qbits(bin6));
    out << "Qbits of 'bin6' have been used to construct '"
        << qBits.toString() << endl;

    Qbin cQbin(hexA);
    out << "A copy of 'hexA' has id '" << cQbin.id() << "' and value "
        << Bits(cQbin) << endl;
}

void UTestQbin::testQnary(ostream& out)
{
    Qbin hexA("hexA", 0xA);
    hexA[1].value(4);
    out << "After 1st qbit of 'hexA' is set to 4 => " << hexA.toString()
        << endl << " decomposed: " << hexA.toString(true) << endl;
    
    hexA[1].value(0);
    out << "After 1st qbit of 'hexA' is set to 0 => " << hexA.toString()
        << endl;
    
    hexA[2].value(cSuperposition);
    out << "After 2nd qbit of 'hexA' is set to S => " << hexA.toString()
        << endl << " decomposed: " << hexA.toString(true) << endl;
    
    out << "All 'hexA' qbits are in superposition? "
        << (hexA.all() ? "true" : "false") << endl;
    out << "Any 'hexA' qbit is 0? "
        << (hexA.any(0) ? "true" : "false") << endl;
    out << "None of 'hexA' qbits is 1? "
        << (hexA.none(1) ? "true" : "false") << endl;
    out << "How many of 'hexA' qbits are 0? "
        << to_string(hexA.count(0)) << endl;
    
    out << "Shift a copy left for 2 positions: "
        << (hexA << size_t(2))->toString(true) << endl;
    out << "Shift a copy right for 1 position: "
        << (hexA >> size_t(1))->toString(true) << endl;
    
    hexA >>= size_t(2);
    out << "Shift 'hexA' right for 2 positions: "
        << hexA.toString(true) << endl;
    
    hexA <<= size_t(1);
    out << "Shift 'hexA' left for 1 position: "
        << hexA.toString(true) << endl;
    
    out << "1 shift left a copy with superposition: "
        << (hexA << cSuperposition)->toString(true) << endl;
    out << "1 shift right a copy with 1: "
        << (hexA >> Qvalue(1))->toString(true) << endl;
    
    hexA >>= Qvalue(0);
    out << "1 shift right 'hexA' with 0: " << hexA.toString(true) << endl;
    
    hexA <<= cSuperposition;
    out << "1 shift left 'hexA' with superposition: "
        << hexA.toString(true) << endl;
}

void UTestQbin::assignment(ostream& out)
{
    Qbin r(2, "r"), x(3, "x");
    Qassign<Qbin> qbinAssign = r = x;
    QuboCompiler compiler; qbinAssign.compile(compiler);
    out << "Assignment 'r/2b:U/ = x/3b:U/' creats logic => " << qbinAssign
        << endl << " It's finalized (execution ready) Qubo is '"
        << compiler.qubo() << "'" << endl;
    qbinAssign.solve();
    out << "resulting in solutions:"<< endl << qbinAssign.solutions() << endl;

    qbinAssign = r &= x;
    compiler.reset(); qbinAssign.compile(compiler);
    out << endl << "Assignment 'r &= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    qbinAssign.solve();
    out << "resulting in solutions:" << endl << qbinAssign.solutions() << endl;

    qbinAssign = r |= x;
    compiler.reset(); qbinAssign.compile(compiler);
    out << endl << "Assignment 'r |= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl << qbinAssign.solve();

    qbinAssign = r ^= x;
    compiler.reset(); qbinAssign.compile(compiler);
    out << endl << "Assignment 'r ^= x' creats logic => " << qbinAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl << qbinAssign.solve();
    
    x.resize(1);
    out << "The order of operations is important in following 4 assignments:"
        << endl;
    Qbin a(1, "a"), b("b", 1), y(1, "y"), z(1, "z"), _0("_0", 0), _1("_1", 1);
    Qassign<Qbin> qbinAssign1 = _1 = (z != (b & x)) | (z != (y ^ _0));
    out << "Assignment 1" << endl << qbinAssign1 << endl
        << " decomposed logic: " << qbinAssign1.toString(true) << endl;
    QuboCompiler noFnalCmplr(false); qbinAssign1.compile(noFnalCmplr);
    out << " It's generic Qubo is '" << noFnalCmplr.qubo() << "'" << endl;
    compiler.reset(); qbinAssign1.compile(compiler);
    qbinAssign1.solve();
    out << " & finalized Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbinAssign1.solutions() << endl;

    Qassign<Qbin> qbinAssign2 = _1 = ((b & x) != z) | (z != (y ^ _0));
    out << "Assignment 2" << endl << qbinAssign2 << endl
        << " decomposed logic: " << qbinAssign2.toString(true) << endl
        << " resulting in :" << endl << qbinAssign2.solve() << endl;
    
    Qassign<Qbin> qbinAssign3 = _1 = ((b & x) != z) | ((y ^ _0) != z);
    out << "Assignment 3" << endl << qbinAssign3 << endl
        << " decomposed logic: " << qbinAssign3.toString(true) << endl
        << " resulting in :" << endl << qbinAssign3.solve() << endl;

    Qassign<Qbin> qbinAssign4 = _1 = (z != (b & x)) | ((y ^ _0) != z);
    out << "Assignment 4" << endl << qbinAssign4 << endl
        << " decomposed logic: " << qbinAssign4.toString(true) << endl
        << " resulting in :" << endl << qbinAssign4.solve() << endl;

    Qassign<Qbin> qbinA4 = _1 = (z.unlike(b & x) | z.alike(y ^ _0));
    out << "Assignment 4 using un/a-like operation: " << endl << qbinA4 << endl
        << " decomposed: " << qbinA4.toString(true) << endl;
    compiler.reset(); qbinA4.compile(compiler);
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()  << " # of branches: "
        << anlyze.branchesNo() << endl << " resulting in :" << endl
        << qbinA4.solve();

    Qassign<Qbin> qbinA3 = _1 = ((b & x).unlike(z)) | ((y ^ _0).alike(z));
    out << "Assignment 3 using un/a-like operation: " << endl << qbinA3 << endl
        << " decomposed: " << qbinA3.toString(true) << endl
        << qbinA3.solve();
}

void UTestQbin::bitwise(ostream& out)
{
    Qbin x(3, "x"), y("y", 5), _0("_0", 0), _1("_1", 1);

    Qexpr<Qbin> xI = ~x;
    QuboCompiler compiler; xI.compile(compiler);
    out << "Expression '~x', INVERT (not) x is: " << xI << endl
        << " decomposed logic: " << xI.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << xI.solve() << endl;

    Qexpr<Qbin> qbExpr = x & y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression AND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.nand(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NAND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(4);
    out << "**** x has been resized to 4 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x | y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression OR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.nor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NOR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x[3].value(1);
    out << "**** x's 3rd qbit is set to 1 ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x ^ y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x *= y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(2); y.resize(2);
    out << "**** x & y have been resized to 2 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl
        << " decomposed: " << y.toString(true) << endl << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (_0 *= (y | x));
    compiler.reset(); qbExpr.compile(compiler);
    QuboCompiler noFnlCmplr(false); qbExpr.compile(noFnlCmplr);
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQbin::comparison(ostream& out)
{
    Qbin x(3, "x"), y("y", 5), z(3, "z");

    Qexpr<Qbin> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
                    xXnYeZ1= (x != y) == z, xXnYeZ2 = x != (y == z);

    QuboCompiler compiler; xEq.compile(compiler);
    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEq.solve() << endl;
    compiler.reset(); xNe.compile(compiler);
    out << xNe.toString() << " decomposed: " << xNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xNe.solve() << endl;

    Qbinder vars; vars = x, y, z;
    compiler.reset(); xXnYeZ0.compile(compiler);
    out << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl;
    vars.add(xXnYeZ0.compute());
    out << vars << endl;
    vars.reset();
    
    /*** CORECT Begin ***/
    Qblock bNeqEq; bNeqEq = x != y, x == z;
    compiler.reset(); bNeqEq.compile(compiler);
    cout << bNeqEq << endl
        << bNeqEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << bNeqEq.solve() << "CORECT!!!!"
        << endl << endl;
    Qubo qubo = compiler.qubo();
    Qbit x3("x3"), z3("z3", 0); // x3 == z3 is a problem!!!!
    compiler.reset(); (x3 == z3).compile(compiler);
    cout << endl << "Problem is: " << (x3 == z3) << " Qubo: " << compiler.qubo()
            << endl << endl;
    qubo += compiler.qubo();
    D5QuboSolver slvrNeqEq(qubo);
    Qbinder bndrNeqEq; bndrNeqEq = x, y, z;
    bndrNeqEq.add(slvrNeqEq.solution());
    cout << bndrNeqEq.solutions() << "WRONG!!!!" << endl << endl;
    
    Qexpr<Qbin> xpr((x == z) != y);
    cout << xpr << endl << xpr.toString(true) << endl;
    cout << xpr.solve() << endl << "WRONG!!!" << endl << endl;
    Qexpr<Qbin> xpr1(y != (x == z));
    cout << xpr1 << endl << xpr1.toString(true) << endl;
    cout << xpr1.solve() << endl << "WRONG!!!" << endl << endl;

    /*** CORECT End ***/

    compiler.reset(); xXnYeZ1.compile(compiler);
    out << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true)
        << endl << "Qubo: " << compiler.qubo() << endl;
    vars.add(xXnYeZ1.compute());
    out << vars << endl;
    vars.reset();
    compiler.reset(); xXnYeZ2.compile(compiler);
    out << xXnYeZ2.toString() << " decomposed: " << xXnYeZ2.toString(true)
        << endl << "Qubo: " << compiler.qubo() << endl;
    vars.add(xXnYeZ2.compute());
    out << vars << endl;
}

void UTestQbin::eq_asign(ostream& out)
{
    Qbin bn0("bn0", 03), bn1(3, "bn1"), bn2(4,"bn2"), bn3("bn3", 0b110),
         bn4(2, "bn4"), bnr("bnr", 0x5);
    Qassign<Qbin> qbinAssign = bnr = (bn0 & bn1) | ((bn2 ^ bn3) == bn4);
    
    QuboCompiler compiler; qbinAssign.compile(compiler);
    QuboCompiler noFnalCmplr(false); qbinAssign.compile(noFnalCmplr);
    out << endl << qbinAssign << endl << endl
        << qbinAssign.toString(true) << endl;
    out << endl << "*** Generic Qubo ***" << endl << noFnalCmplr.qubo() << endl
        << endl << "*** Finalized Qubo ***" << endl << compiler.qubo() << endl;
    QuboAnalyzer analyze(compiler.qubo());
    out << endl << "# of nodes: " << analyze.nodesNo()<< "\t# of branches: "
        << analyze.branchesNo() << endl;
    out << " resulting in :" << endl << qbinAssign.solve() << endl;
}

void UTestQbin::friends_enemies(ostream& out)
{
    Qbin w(2, "wolf"), x(2, "xenia"), y(2, "yael"), z(2, "zeke"), s(2, "sean"),
         t(2, "tea"), u(2, "una"), v(2, "veda"), _3("_3", 0b11);
    Qassign<Qbin> aWXfYZe = _3 = (w *= x) & (x ^ y) & (y *= z);
    out << endl << "*** Dwave's friends & enemies problem equation:" << endl
        << aWXfYZe.toString() << endl;
    out << endl << "*** Dwave's friends & enemies problem solutions:" << endl
        << aWXfYZe.solve() << endl;

    Qevaluations solutions = aWXfYZe.compute();
    Qbinder wxyzBinder(solutions);
    wxyzBinder = w, x, y, z;
    out << endl << wxyzBinder << endl;

    Qassign<Qbin> aWXfYe = _3 = (w *= x) & (w ^ y);
    Qassign<Qbin> aSTf = _3 = (s *= t);
    Qbit _1("_1", 1);
    Qassign<Qbit> aSorW = _1 = ((s[1] ^ w[1]) | (s[0] ^ w[0]));
    Qassign<Qbit> aSorY = _1 = ((s[1] ^ y[1]) | (s[0] ^ y[0]));
    Qblock prFnE;
    prFnE = aWXfYe, aSTf, aSorW, aSorY;
    out << endl << "*** Friends & enemies problem block:" << endl
        << prFnE.toString() << endl;
    QuboCompiler compiler; prFnE.compile(compiler);
    Qubo qPrFnE = compiler.qubo();
    out << endl << "*** Friends & enemies problem qubo:" << endl
        << qPrFnE << endl;
    QuboAnalyzer analysePrFnE(qPrFnE);
    out << endl << "*** Friends & enemies problem # of nodes: "
        << analysePrFnE.nodesNo() << " # of branches: "
        << analysePrFnE.branchesNo() << endl;
    Qevaluations prFnEsltns = prFnE.compute();
    Qbinder wxystBinder;
    wxystBinder = w, x, y, s, t;
    wxystBinder.add(prFnEsltns);
    out << endl << "*** Friends & enemies problem solutions:" << endl
        << wxystBinder << endl;

    Qassign<Qbin> aSTfUVe = _3 = (s *= t) & (s ^ u) & (u *= v);
    Qblock xPrFnE;
    xPrFnE = aWXfYZe, aSTfUVe, aSorW, aSorY;
    out << endl << "*** Extended friends & enemies problem block:" << endl
        << xPrFnE.toString() << endl;
    compiler.reset(); xPrFnE.compile(compiler);
    Qubo qXprFnE = compiler.qubo();
    out << endl << "*** Extended friends & enemies problem qubo:" << endl
        << qXprFnE << endl;
    QuboAnalyzer analyseXprFnE(qXprFnE);
    out << endl << "*** Extended friends & enemies problem # of nodes: "
        << analyseXprFnE.nodesNo() << ", # of branches: "
        << analyseXprFnE.branchesNo() << endl;
}

void UTestQbin::vertex(ostream& out)
{
    const size_t noVertices = 5;
    Qbit _1("_1", 1), _0("_0", 0);
    Qbin vertices(5, "a");

    Qexpr<Qbit> xbTV = (vertices[2] ^ vertices[3])
                        & (vertices[3] ^ vertices[4]) & (vertices[3] *= _0)
                    | (vertices[3] ^ vertices[2])
                        & (vertices[2] ^ vertices[4]) & (vertices[2] *= _0)
                    | (vertices[2] ^ vertices[4])
                        & (vertices[4] ^ vertices[3]) & (vertices[4] *= _0);
    Qassign<Qbit> axbTV = _1 = xbTV;
    QuboCompiler compiler; axbTV.compile(compiler);
    out << endl << "---- Triangle c-d-e equation:" << endl << axbTV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl
        << "---- Triangle c-d-e solutions:" << endl << axbTV.solve() << endl;
    // sqare
    Qexpr<Qbit> xbSV = ((vertices[0] ^ vertices[1]) & (vertices[0] ^ vertices[3])
                        & (vertices[2] ^ vertices[1]) & (vertices[2] ^ vertices[3]))
                        & (((vertices[0] *= _0) & (vertices[2] *= _0))
                           | ((vertices[1] *= _0) & (vertices[3] *= _0)));
    Qassign<Qbit> axbSV = _1 = xbSV;
    compiler.reset(); axbSV.compile(compiler);
    out << endl << "---- Square a-b-c-d equation:" << endl << axbSV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl
        << "---- Square a-b-c-d solutions:" << endl << axbSV.solve() << endl;

    Qassign<Qbit> problemAssignment = _1 = xbSV & xbTV;
    out << endl << "---- DWave square(a-b-c-d) "
        << "+ triangle (c-d-e) vertex problem equation:"
        << endl << problemAssignment.toString() << endl;
    compiler.reset(); problemAssignment.compile(compiler);
    out << endl << "     QUBO: " << compiler.qubo() << endl;
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << "---- DWave square(a-b-c-d) "
        << "+ triangle (c-d-e) vertex problem :";
    out << endl << "     # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo();
}
