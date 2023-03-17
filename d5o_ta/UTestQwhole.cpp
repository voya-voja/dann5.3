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


void comparisonLogic()
{
    Qbit _1("_1", 1), _0("_0", 0);
    Qwhole  y(4, "y"), r(4, "r"), x(5, "x");
    Qblock neBlck;
    {
        Qexpr<Qbit> expr;
        expr = r[1] | r[0];
        for (size_t at = 2; at < r.noqbs(); at++)
            expr = r[at] | expr;
        neBlck = y = r + x,
            _1 = expr;
    }
    Qroutine notEqual("ne", neBlck);

    cout << "Qwhole x != y " << endl << neBlck << endl
        << " decomposed logic: " << neBlck.toString(true) << endl;
    cout << " It's generic Qubo is '" << neBlck.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << neBlck.qubo() << "'" << endl;
    cout << " resulting in :" << endl << neBlck.solve() << endl;

    Qsolver::Samples solutions = neBlck.compute();
    Qbinder xyBinder(solutions);
    xyBinder = x, y;
    cout << endl << xyBinder << endl;

    Qwhole  y2(4, "y"), r2(4, "r"), x2(5, "x");
    Qassign<Qwhole> a5 = x2 = r2 + y2;
    Qexpr<Qbit> a6 = _0 != x2[4];
    Qblock blck2;
    blck2 = a5, a6;
    cout << "Qwhole x < y " << endl << blck2 << endl
        << " decomposed logic: " << blck2.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck2.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck2.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck2.solve() << endl;

    Qsolver::Samples solutions2 = blck2.compute();
    Qbinder xyBinder2(solutions2);
    xyBinder2 = x2, y2;
    cout << endl << xyBinder2 << endl;

    Qwhole  y3(3, "y"), r3(3, "_>=0"), x3(3, "x");
    Qassign<Qwhole> a7 = x3 = r3 + y3;
    Qexpr<Qbit> a8 = _0 == x3[3];
    Qblock blck3;
    blck3 = a7, a8;
    cout << "Qwhole x >= y " << endl << blck3 << endl
        << " decomposed logic: " << blck3.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck3.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck3.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck3.solve() << endl;

    Qsolver::Samples solutions3 = blck3.compute();
    Qbinder xyBinder3(solutions3);
    xyBinder3 = x3, y3;
    cout << endl << xyBinder3 << endl;

    Qwhole  y1(5, "y"), r1(4, "r"), x1(4, "x");
    Qassign<Qwhole> a3 = y1 = r1 + x1;
    Qexpr<Qbit> a4 = _0 == y1[4];
    Qblock blck1;
    blck1 = a3, a4;
    cout << "Qwhole x <= y " << endl << blck1 << endl
        << " decomposed logic: " << blck1.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck1.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck1.solve() << endl;

    Qsolver::Samples solutions1 = blck1.compute();
    Qbinder xyBinder1(solutions1);
    xyBinder1 = x1, y1;
    cout << endl << xyBinder1 << endl;

    Qwhole  y4(5, "y"), r4(4, "r"), x4(4, "x");
    Qassign<Qwhole> a9 = y4 = r4 + x4;
    Qexpr<Qbit> a10 = _0 != y4[4];
    Qblock blck4;
    blck4 = a9, a10;
    cout << "Qwhole x > y " << endl << blck4 << endl
        << " decomposed logic: " << blck4.toString(true) << endl;
    cout << " It's generic Qubo is '" << blck4.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck4.qubo() << "'" << endl;
    cout << " resulting in :" << endl << blck4.solve() << endl;

    Qsolver::Samples solutions4 = blck4.compute();
    Qbinder xyBinder4(solutions4);
    xyBinder4 = x4, y4;
    cout << endl << xyBinder4 << endl;
}

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
    out << endl << " --- Prime numbers ----" << endl;
    prime(out);
    out << endl << " --- Prime numbers (6 * Qwhole +/- 1) algorithm ----" << endl;
    prime6(out);
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
/* Issue #6
    qwExpr.reset();
    qwxExpr = x - qwExpr;
    out << "Subtraction Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;
*/

    qwExpr = qwExpr - x;
    out << "Subtraction Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl;
    out << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;
/* Issue #6 
        qwExpr.reset();
        qxxExpr = z_3Expr - qwExpr;
        out << "Subtraction Expression" << endl << qxxExpr << endl
            << " decomposed logic: " << qxxExpr.toString(true) << endl
            << " It's generic Qubo is '" << qxxExpr.qubo(false) << "'" << endl
            << " & finalized Qubo is '" << qxxExpr.qubo() << "'" << endl;
        out << " resulting in :" << endl << qxxExpr.solve() << endl;
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
    out << "Multiplication Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qxxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qxxExpr.qubo() << "'" << endl;
    Qanalyzer anlyze(qxxExpr.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo() << endl << endl;

    qwExpr.reset();
    qwExpr = y / x;
    out << "Division Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl;
    out << " It's generic Qubo is '" << qwExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwExpr.qubo() << "'" << endl;
    Qanalyzer anlyzeD(qwExpr.qubo());
    out << endl << " # of nodes: " << anlyzeD.nodesNo()
        << " # of branches: " << anlyzeD.branchesNo() << endl << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr.reset();
    qwxExpr = qwExpr / z;
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr.reset();
    qwExpr = x + z;
    qwxExpr = y / qwExpr;
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr.reset();
    z_3Expr.reset();
    qwxExpr = z_3Expr / qwExpr;
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << qwxExpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qwxExpr.qubo() << "'" << endl;
    Qanalyzer anlyzeD2(qwxExpr.qubo());
    out << endl << " # of nodes: " << anlyzeD2.nodesNo()
        << " # of branches: " << anlyzeD2.branchesNo() << endl << endl;
//    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    Qwhole  r("r", 6), k(2, "k");
    Qexpr<Qwhole> e1 = r - x, e2 = x / k;
    Qblock blck;
    blck = e1, e2;
    out << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl;
    out << " It's generic Qubo is '" << blck.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck.qubo() << "'" << endl;
    out << " resulting in :" << endl << blck.solve() << endl;
}


void UTestQwhole::comparison(ostream& out)
{
    Qwhole x(2, "x"), y("y", 5), z(3, "z");

    Qexpr<Qwhole> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
        xXnYeZ1 = (x != y) == z, xXnYeZ2 = x != (y == z);

    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl << "Qubo: " << xEq.qubo() << endl << xEq.solve() << endl;
    out << xNe.toString() << " decomposed: " << xNe.toString(true) << endl << "Qubo: " << xNe.qubo() << endl << xNe.solve() << endl;
//ERROR ->    out << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl << "Qubo: " << xXnYeZ0.qubo() << endl << xXnYeZ0.solve() << endl;
//    out << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true) << endl << "Qubo: " << xXnYeZ1.qubo() << endl << xXnYeZ1.solve() << endl;
    out << xXnYeZ2.toString() << " decomposed: " << xXnYeZ2.toString(true) << endl << "Qubo: " << xXnYeZ2.qubo() << endl << xXnYeZ2.solve() << endl;

    Qwhole x1(2, "x"), y1(2, "y"), z1(2, "z"), w1(2, "w");
    Qexpr<Qwhole> comp = x1 + y1 == z1 * w1;
    out << endl << comp << endl << endl << comp.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() / 2 << endl;
    Qanalyzer analyzeM(comp.qubo());
    out << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    out << endl << comp.solutions();

    Qwhole x2(2, "x"), y2(2, "y"), z2(2, "z"), w2(2, "w");
    Qexpr<Qwhole> comp2 = x2 + y2 != z2 + w2;
    out << endl << comp2 << endl << endl << comp2.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl << comp2.qubo(false) << endl << comp2.qubo() / 2 << endl;
    Qanalyzer analyzeM2(comp2.qubo());
    out << endl << "# of nodes: " << analyzeM2.nodesNo() << "\t# of branches: " << analyzeM2.branchesNo() << endl;
    comp2.solve();
    out << endl << comp2.solutions() << endl << "ERROR: [QwholeExpr] != [right] issue needs to be fixed!";

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

    Qwhole  x3(3, "x"), y3(2, "y"), z3(1, "z");
    Qexpr<Qwhole> xpr = x3 <= y3 + z3;
    cout << "Qwhole x <= y + z " << endl << xpr << endl
        << " decomposed logic: " << xpr.toString(true) << endl;
    cout << " It's generic Qubo is '" << xpr.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << xpr.qubo() << "'" << endl;
    //    cout << " resulting in :" << endl << xpr.solve() << endl;
    Qanalyzer anlyze(xpr.qubo());
    cout << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo() << endl << endl;
    Qsolver::Samples solutions = xpr.compute();
    Qbinder xyBinder(solutions);
    xyBinder = x1, y1, z1;
    cout << endl << xyBinder << endl;

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
    out << endl << "Assignment 'r /= x' creats logic => " << qwholeAssign << endl
        << " It's Qubo is '" << qwholeAssign.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    Qanalyzer analyzeAd(qwholeAssign.qubo());
    out << endl << "# of nodes: " << analyzeAd.nodesNo() << "\t# of branches: " << analyzeAd.branchesNo() << endl;
//    qwholeAssign.solve();
//    out << qwholeAssign.solutions() << endl;

    Qwhole y("y", 6), z(2, "z"), k(2, "k");
    Qassign<Qwhole> a1 = y = r - x, a2 = k = y - z;
    Qblock blck;
    blck = a1, a2;
    out << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl;
    out << " It's generic Qubo is '" << blck.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << blck.qubo() << "'" << endl;
    out << " resulting in :" << endl << blck.solve() << endl;

    out << "Dann5.ocean Tests Qwhole addition!\n";
    Qwhole a(4, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A("A", 21), _1("1_", 1);
    Qassign<Qwhole> aA = A = a + b + c + d + _1;// 
    out << endl << aA << endl << endl << aA.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;
    Qanalyzer analyzeA(aA.qubo());
    out << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;
    aA.solve();
    out << endl << aA.solutions();

    out << endl << "--- Quantum assignment vs. equal expression";
    Qwhole ax(3, "a"), b1(2, "b"), A1("A", 7), c1(3, "c"), d1(2, "d"), Ae1("Ae", 7);
    Qassign<Qwhole> aA1 = A1 = ax + b1;
    out << endl << aA1.toString() << endl << aA1.solve() << endl;
    Qanalyzer analyzeA1(aA1.qubo());
    out << "Assignment # of nodes: " << analyzeA1.nodesNo() << "\t# of branches: " << analyzeA1.branchesNo() << endl;
    Qexpr<Qwhole> eA1 = Ae1 == c1 + d1;
    out << endl << eA1.toString() << endl << eA1.solve() << endl;
    Qanalyzer analyzeE1(eA1.qubo());
    out << "Equal expression # of nodes: " << analyzeE1.nodesNo() << "\t# of branches: " << analyzeE1.branchesNo() << endl;

}

void UTestQwhole::factorial(ostream& out)
{
    Qwhole M("M", 42), p(3, "p"), q(2, "q"), r(2, "r");
    Qassign<Qwhole> mM = M = p * q * r;
    out << endl << mM << endl << endl << mM.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl << mM.qubo(false) << endl << mM.qubo() << endl;
    Qanalyzer analyzeM(mM.qubo());
    out << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    const clock_t begin_time = clock();
    mM.solve();
    clock_t end_time = clock();
    cout << endl << "Running time: " << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s";
    out << endl << mM.solutions();

    out << endl << "--- Large number factorial:";
    const clock_t begin_time2 = clock();
    Qwhole M1("M", 132560640), p1(8, "p"), q1(8, "q"), r1(8, "r");
    Qassign<Qwhole> mM1 = M1 = p1 * q1 * r1;
    clock_t end_time2 = clock();
    cout << endl << "Preparation time: " << to_string(float(end_time2 - begin_time2) / CLOCKS_PER_SEC) << "s";
    out << endl << mM1 << endl;
    Qanalyzer analyzeM1(mM1.qubo());
    out << endl << "# of nodes: " << analyzeM1.nodesNo() << "\t# of branches: " << analyzeM1.branchesNo() << endl;

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
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    Qubo qPrime = primeNo.qubo();
    out << " It's generic Qubo is '" << primeNo.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse(qPrime);
    out << endl << "*** Prime number p = s2 + 2t2, p mod 8 = 3, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: " << analyse.branchesNo() << endl;
//    out << " resulting in :" << endl << primeNo.solve() << endl;
    Qsolver::Samples solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
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

    out << "Prime Number\n Code" << endl << prime6m1 << endl
        << " Logic: " << prime6m1.toString(true) << endl;
    Qubo qPrime = prime6m1.qubo();
    out << " It's generic Qubo is '" << prime6m1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse(qPrime);
    out << endl << "*** Prime number p = 6s - 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: " << analyse.branchesNo() << endl;
    Qsolver::Samples solution = prime6m1.compute();
    Qbinder pst(solution);
    pst = prime, s, t, mod;
    out << " resulting in :" << endl << pst << endl;

    Qblock prime6p1;
    {
        prime6p1 = prime = _6 * s + _1;
        //                    prime = t * _8 + mod,
        //                    mod != 5;
    }

    out << "Prime Number\n Code" << endl << prime6p1 << endl
        << " Logic: " << prime6p1.toString(true) << endl;
    qPrime = prime6p1.qubo();
    out << " It's generic Qubo is '" << prime6p1.qubo(false) << "'" << endl
        << " & finalized Qubo is '" << qPrime << "'" << endl;
    Qanalyzer analyse2(qPrime);
    out << endl << "*** Prime number p = 6s + 1 has: "
        << analyse2.nodesNo() << " # of node and # of branches: " << analyse2.branchesNo() << endl;
    Qsolver::Samples solution2 = prime6p1.compute();
    Qbinder pst2(solution2);
    pst2 = prime, s, t, mod;
    out << " resulting in :" << endl << pst2 << endl;
}
