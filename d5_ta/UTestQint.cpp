//
//  UTestQint.cpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2023-01-07.
//
#include "UTestQint.hpp"

#include <Qint.h>
#include <Qblock.h>
#include <Qroutine.h>
#include <Qbinder.h>
#include <Qubo.h>
#include <QuboCompiler.h>
#include <QuboAnalyzer.h>
#include <D5QuboSolver.h>

using namespace dann5test;

using namespace dann5;
using namespace dann5::ocean;

void UTestQint::runAll(ostream& out)
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

void UTestQint::initialization(ostream& out)
{
    // Default constructor initializes Q whole with an empty id and
    // supperposition value, plus initialize Q whole with a given id and
    // supperposition value
    Qint dcQint, idQint("id");
    out << "Qint default constructor: " << dcQint.toString()
        << ", creates an object with empty id, i.e. '',"
        << " 0 qbits, and value 0." << endl << "Qint with an id only: "
        << idQint.toString() << " has 0 qbits and value 0, too." << endl;

    Qint size3(3, "size3");
    out << "Qint 'size3' is unknown and has 3 Qbits:" << endl;
    out << " toString() => " << size3.toString() << endl
        << " toString(false, 0) => " << size3.toString(false, 0) << endl
        << " toString(true) => " << size3.toString(true) << endl
        << " toString(true, 0) => " << size3.toString(true, 0) << endl;

    // Initialize Q whole with a given id and value.
    // If value is different from 0 or 1, Qint into superposition state
    Qint oct10("octal10", 010), dec5("dec5", 5), bin6("bin6", 0b110),
           hexA("hexA", 0xA);
    out << "For 'octal10' set to value 010:" << endl;
    out << " toString() => " << oct10.toString() << endl
        << " toString(true) => " << oct10.toString(true) << endl
        << " toString(true, 3) => " << oct10.toString(true, 3) << endl;
    out << "For 'dec5' set to value 5:" << endl;
    out << " toString() => " << dec5.toString() << endl
        << " toString(true) => " << dec5.toString(true) << endl
        << " toString(true, 1) => " << dec5.toString(true, 1) << endl;
    out << "For 'bin6' set to value 0b110:" << endl;
    out << " toString() => " << bin6.toString() << endl
        << " toString(true) => " << bin6.toString(true) << endl
        << " toString(true, 2) => " << bin6.toString(true, 2) << endl;
    out << "For 'hexA' set to value 0xA:" << endl;
    out << " toString() => " << hexA.toString() << endl
        << " toString(true) => " << hexA.toString(true) << endl
        << " toString(true, 3) => " << hexA.toString(true, 3) << endl;

    Qint qBits("bin6qBits", Qbits(Qbin(bin6)));
    out << "Qbits of 'bin6' have been used to construct '"
        << qBits.toString() << endl;

    Qint cQint(hexA);
    out << "A copy of 'hexA' has id '" << cQint.id() << "' and value "
        << Bits(Qbin(cQint)) << endl;
    {
        Qint _2("_2", 2), _2_("_2_", -2), _3_("_3_", -3), _4_("_4_", -4), _5_("_5_", -5), _6_("_6_", -6), _7_("_7_", -7);
        out << endl << " 2 = " << _2.Qbin::toString();
        out << endl << " 1 = " << Qint::_1.Qbin::toString();
        out << endl << " 0 = " << Qint::_0.Qbin::toString();
        out << endl << "-1 = " << Qint::_1_.Qbin::toString();
        out << endl << "-2 = " << _2_.Qbin::toString();
        out << endl << "-3 = " << _3_.Qbin::toString();
        out << endl << "-4 = " << _4_.Qbin::toString();
        out << endl << "-5 = " << _5_.Qbin::toString();
        out << endl << "-6 = " << _6_.Qbin::toString();
        out << endl << "-7 = " << _7_.Qbin::toString() << endl;
    }
    {
        out << endl << "Initalizing quantum integers as bytes:" << endl;
        Qint _2(7, "_2", 2), _1(7, "_1", 1), _0(7, "_0", 0), _1_(7, "_1_", -1), _2_(7, "_2_", -2), _3_(7, "_3_", -3), _4_(7, "_4_", -4), _5_(7, "_5_", -5), _6_(7, "_6_", -6), _7_(7, "_7_", -7);
        out << endl << " 2 = " << _2.Qbin::toString();
        out << endl << " 1 = " << _1.Qbin::toString();
        out << endl << " 0 = " << _0.Qbin::toString();
        out << endl << "-1 = " << _1_.Qbin::toString();
        out << endl << "-2 = " << _2_.Qbin::toString();
        out << endl << "-3 = " << _3_.Qbin::toString();
        out << endl << "-4 = " << _4_.Qbin::toString();
        out << endl << "-5 = " << _5_.Qbin::toString();
        out << endl << "-6 = " << _6_.Qbin::toString();
        out << endl << "-7 = " << _7_.Qbin::toString() << endl;
    }
}

void UTestQint::bitwise(ostream& out)
{
    Qint x(3, "x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    Qexpr<Qbin> xI = ~x;
 
    QuboCompiler compiler; xI.compile(compiler);
    out << "Expression '~x', INVERT (not) x is: " << xI << endl
        << " Dann5 virtual code: " << xI.toString(true) << endl;
    out << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << xI.solve() << endl;

    Qexpr<Qbin> qbExpr = x & y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression AND: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nand(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NAND: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(4);
    out << "**** x has been resized to 4 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x | y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression OR: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NOR: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x[2].value(0);
    out << "**** x's 2nd qbit is set to 0 ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x ^ y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nxor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(2); y.resize(2);
    out << "**** x & y have been resized to 2 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl
        << " decomposed: " << y.toString(true) << endl << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (Qbin(_0).alike(y | x));
    QuboCompiler noFnlCmplr(false); qbExpr.compile(noFnlCmplr);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Complex Expression" << endl << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQint::arithmetic(ostream& out)
{
    Qint x(2, "x"), y("y", -2), z(1, "z"), _3("_3", 3);
    Qexpr<Qint> qwExpr = x + y;
    QuboCompiler noFnlCmplr(false); qwExpr.compile(noFnlCmplr);
    QuboCompiler compiler; qwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwExpr << endl
        << " Dann5 virtual code: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr.reset();  qwExpr = y + x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwExpr << endl
        << " Dann5 virtual code: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    Qexpr<Qint> qwwExpr = x + y + z + _3;// +qwExpr;
    noFnlCmplr.reset(); qwwExpr.compile(noFnlCmplr);
    compiler.reset(); qwwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwwExpr << endl
        << " Dann5 virtual code: " << qwwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwwExpr.solve() << endl;

    qwExpr.reset();
    Qexpr<Qint> qxwExpr = qwExpr + z + _3;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Addition Expression" << endl << qxwExpr << endl
        << " Dann5 virtual code: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr.reset(); qwExpr = x + y;
    qxwExpr.reset(); qxwExpr = z + qwExpr + _3;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Addition Expression" << endl << qxwExpr << endl
        << " Dann5 virtual code: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl;

    Qexpr<Qint> z_3Expr = z + _3;
    Qexpr<Qint> qwxExpr = x + y + z_3Expr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Addition Expression" << endl << qwxExpr << endl
        << " Dann5 virtual code: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr.reset(); qwExpr = x + y;
    z_3Expr.reset(); z_3Expr = z + _3;
    Qexpr<Qint> qxxExpr = qwExpr + z_3Expr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Addition Expression" << endl << qxxExpr << endl
        << " Dann5 virtual code: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr.reset(); z_3Expr.reset(); qxxExpr.reset();
    qwExpr = x + y;
    z_3Expr = z + _3;
    qxxExpr = z_3Expr + qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Addition Expression" << endl << qxxExpr << endl
        << " Dann5 virtual code: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr.reset();
    qwExpr = y - x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qwExpr << endl
        << " Dann5 virtual code: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;
/* Issue #6 */
    qwxExpr.reset();
    qwxExpr = z - qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qwxExpr << endl
        << " Dann5 virtual code: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;
// Issue #13
    /*qwExpr.reset(); qxwExpr.reset();
    qxwExpr = qwExpr - x;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qxwExpr << endl
        << " Dann5 virtual code: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl << "WRONG!!!"
        << endl << endl;*/
    
    Qint w(2, "w");
    Qblock blckSub; blckSub = z = w - x, w = y - x;
    noFnlCmplr.reset(); blckSub.compile(noFnlCmplr);
    compiler.reset(); blckSub.compile(compiler);
    out << "Subtraction Expression" << endl << blckSub << endl
        << " Dann5 virtual code: " << blckSub.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blckSub.solve() << endl;
/* Issue #13  */
    /*qwExpr.reset(); z_3Expr.reset(); qxxExpr.reset();
    qxxExpr = z_3Expr - qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qxxExpr << endl
        << " Dann5 virtual code: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;*/

    qwExpr.reset(); qwExpr = x * y;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qwExpr << endl
        << " Dann5 virtual code: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr.reset();
    qwxExpr.reset();
    qwxExpr = z * qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qwxExpr << endl
        << " Dann5 virtual code: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze2(compiler.qubo());
    out << endl << " # of nodes: " << anlyze2.nodesNo()
        << " # of branches: " << anlyze2.branchesNo() << endl << endl;
    //out << endl << " resulting in :" << endl << qwxExpr.solve() << endl;

    qxwExpr.reset(); qwExpr.reset();
    qxwExpr = qwExpr * z;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qxwExpr << endl
        << " Dann5 virtual code: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze1(compiler.qubo());
    out << endl << " # of nodes: " << anlyze1.nodesNo()
        << " # of branches: " << anlyze1.branchesNo() << endl << endl;
    // out << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr.reset();  qwExpr = x * y;
    z_3Expr.reset();  z_3Expr = _3 * z;
    qxxExpr.reset();  qxxExpr = qwExpr * z_3Expr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qxxExpr << endl
        << " Dann5 virtual code: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo() << endl << endl;

    qwExpr.reset();
    qwExpr = y / x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Division Expression" << endl << qwExpr << endl
        << " Dann5 virtual code: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyzeD(compiler.qubo());
    out << endl << " # of nodes: " << anlyzeD.nodesNo()
        << " # of branches: " << anlyzeD.branchesNo() << endl << endl;
    //out << " resulting in :" << endl << qwExpr.solve() << endl;
    
    qwExpr.reset();
    Qblock byDx; byDx = qwExpr, x != Qint::_0;
    noFnlCmplr.reset(); byDx.compile(noFnlCmplr);
    compiler.reset(); byDx.compile(compiler);
    out << "Division Expression" << endl << byDx << endl
        << " Dann5 virtual code: " << byDx.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyzeDne(compiler.qubo());
    out << endl << " # of nodes: " << anlyzeDne.nodesNo()
        << " # of branches: " << anlyzeDne.branchesNo() << endl << endl;
    out << " resulting in :" << endl << byDx.solve() << endl;
// Issue #13
    /*qwExpr.reset();
    qxwExpr.reset();
    qxwExpr = qwExpr / z;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Division Expression" << endl << qxwExpr << endl
        << " Dann5 virtual code: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze4(compiler.qubo());
    out << endl << " # of nodes: " << anlyze4.nodesNo()
        << " # of branches: " << anlyze4.branchesNo() << endl << endl;*/
    //out << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr.reset();  qwExpr = x + z;
    qwxExpr.reset();  qwxExpr = y / qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Division Expression" << endl << qwxExpr << endl
        << " Dann5 virtual code: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze5(compiler.qubo());
    out << endl << " # of nodes: " << anlyze5.nodesNo()
        << " # of branches: " << anlyze5.branchesNo() << endl << endl;
    //out << " resulting in :" << endl << qwxExpr.solve() << endl;
// Issue #13
    /*qxxExpr.reset();  qxxExpr = z_3Expr / qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Division Expression" << endl << qxxExpr << endl
        << " Dann5 virtual code: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyzeD2(compiler.qubo());
    out << endl << " # of nodes: " << anlyzeD2.nodesNo()
        << " # of branches: " << anlyzeD2.branchesNo() << endl << endl;*/
//    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    Qint  r("r", 6), k(2, "k");
    Qexpr<Qint> e1 = r - x, e2 = x / k;
    Qblock blck;
    blck = e1, e2;
    noFnlCmplr.reset(); blck.compile(noFnlCmplr);
    compiler.reset(); blck.compile(compiler);
    out << "Subtraction Expression" << endl << blck << endl
        << " Dann5 virtual code: " << blck.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze6(compiler.qubo());
    out << endl << " # of nodes: " << anlyze6.nodesNo()
        << " # of branches: " << anlyze6.branchesNo() << endl << endl;
//    out << " resulting in :" << endl << blck.solve() << endl;
}

void UTestQint::comparison(ostream& out)
{
    QuboCompiler compiler;
    Qint x(2, "x"), y("y", -2), z(2, "z"), w(2, "w");

    Qexpr<Qint>   xGt = x > z,
                    xGe = x >= z,
                    xLt = x < z,
                    xLe = x <= z,
                    xNeq = x != z,
                    xEq = x == z;
    compiler.reset(); xEq.compile(compiler);
    out << xEq.toString() << endl << " decomposed: " << endl
        << xEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEq.solve() << endl;

    compiler.reset(); xNeq.compile(compiler);
    out << xNeq.toString() << endl << " decomposed: " << endl
        << xNeq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xNeq.solve() << endl;

    compiler.reset(); xGt.compile(compiler);
    out << xGt.toString() << endl << " decomposed: " << endl
        << xGt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xGt.solve() << endl;

    compiler.reset(); xGe.compile(compiler);
    out << xGe.toString() << endl << " decomposed: " << endl
        << xGe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xGe.solve() << endl;

    compiler.reset(); xLt.compile(compiler);
    out << xLt.toString() << endl << " decomposed: " << endl
        << xLt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xLt.solve() << endl;

    compiler.reset(); xLe.compile(compiler);
    out << xLe.toString() << endl << " decomposed: " << endl
        << xLe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xLe.solve() << endl;

    xGt.reset(); xGe.reset(); xLt.reset(); xLe.reset(); xNeq.reset(); xEq.reset();
    Qexpr<Qint>   xGtEq = xGt == y,
                    xGeEq = xGe == y,
                    xLtEq = xLt == y,
                    xLeEq = xLe == y,
                    xNeqEq = xNeq == y,
                    xEqEq = xEq == y;
    
    compiler.reset(); xEqEq.compile(compiler);
    xEqEq.reset();
    out << xEqEq << endl << " decomposed: " << endl
        << xEqEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqEq.solve() << endl;
    
    compiler.reset(); xNeqEq.compile(compiler);
    out << xNeqEq << endl << " decomposed: " << endl
        << xNeqEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xNeqEq.solve() << endl
        << "WRONG!!!" << endl << endl;

    /*** CORECT Begin ***/
    Qblock bNeqEq; bNeqEq = x != z, x == y;
    compiler.reset(); bNeqEq.compile(compiler);
    out << bNeqEq << endl
        << bNeqEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << bNeqEq.solve() << "CORECT!!!!"
        << endl << endl;
    Qubo qubo = compiler.qubo();
    Qbit x3("x3"), y3("y3", 0); // x3 == y3 is a problem!!!!
    compiler.reset(); (x3 == y3).compile(compiler);
    out << endl << "Problem is: " << (x3 == y3) << " Qubo: " << compiler.qubo()
            << endl << endl;
    qubo += compiler.qubo();
    D5QuboSolver slvrNeqEq(qubo);
    Qbinder bndrNeqEq; bndrNeqEq = x, z, y;
    bndrNeqEq.add(slvrNeqEq.solution());
    out << bndrNeqEq.solutions() << "WRONG!!!!" << endl << endl;
    /*** CORECT End ***/

    compiler.reset(); xGtEq.compile(compiler);
    out << xGtEq << endl << " decomposed: " << endl
        << xGtEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xGtEq.solve() << endl;

    compiler.reset(); xGeEq.compile(compiler);
    out << xGeEq << endl << " decomposed: " << endl
        << xGeEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xGeEq.solve() << endl;

    compiler.reset(); xLtEq.compile(compiler);
    out << xLtEq.toString() << endl << " decomposed: " << endl
        << xLtEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xLtEq.solve() << "WRONG!!!"
        << endl << endl;

    /*** CORECT Begin ***/
    Qblock bLtEq; bLtEq = x < z, x == y;
    compiler.reset(); bLtEq.compile(compiler);
    out << bLtEq << endl
        << bLtEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << bLtEq.solve() << "CORECT!!!!"
        << endl << endl;
    qubo = compiler.qubo();
    compiler.reset(); (x3 == y3).compile(compiler);
    out << endl << "Problem is: " << (x3 == y3) << " Qubo: " << compiler.qubo()
            << endl << endl;
    qubo += compiler.qubo();
    D5QuboSolver slvrLtEq(qubo);
    Qbinder bndrLtEq; bndrLtEq = x, z, y;
    bndrLtEq.add(slvrLtEq.solution());
    out << bndrLtEq.solutions() << "WRONG!!!!" << endl << endl;
    /*** CORECT End ***/

    compiler.reset(); xLeEq.compile(compiler);
    out << xLeEq << endl << " decomposed: " << endl
        << xLeEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xLeEq.solve() << endl;

    xGt.reset(); xGe.reset(); xLt.reset(); xLe.reset(); xNeq.reset(); xEq.reset();
    Qexpr<Qint>   xEqGt = y == xGt,
                    xEqGe = y == xGe,
                    xEqLt = y == xLt,
                    xEqLe = y == xLe,
                    xEqNeq = y == xNeq,
                    xEEq = y == xEq;
    
    compiler.reset(); xEEq.compile(compiler);
    xEEq.reset();
    out << xEEq << endl << " decomposed: " << endl
        << xEEq.toString(true) << endl
    << "Qubo: " << compiler.qubo() << endl << xEEq.solve() << endl;
    
    compiler.reset(); xEqNeq.compile(compiler);
    out << xEqNeq << endl << " decomposed: " << endl
        << xEqNeq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqNeq.solve() << endl << "WRONG!!!"
        << endl << endl;;

    compiler.reset(); xEqGt.compile(compiler);
    out << xEqGt << endl << " decomposed: " << endl
        << xEqGt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqGt.solve() << endl;

    compiler.reset(); xEqGe.compile(compiler);
    out << xEqGe << endl << " decomposed: " << endl
        << xEqGe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqGe.solve() << endl;

    compiler.reset(); xEqLt.compile(compiler);
    out << xEqLt.toString() << endl << " decomposed: " << endl
        << xEqLt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqLt.solve() << endl << "WRONG!!!"
        << endl << endl;;

    compiler.reset(); xEqLe.compile(compiler);
    out << xEqLe << endl << " decomposed: " << endl
        << xEqLe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqLe.solve() << endl;
    
    Qexpr<Qint> xxEyAz(x == y + z);
    compiler.reset(); xxEyAz.compile(compiler);
    out << xxEyAz << endl << " decomposed: " << endl
        << xxEyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEqLe.solve() << endl;

    Qexpr<Qint> xxNEyAz(x != y + z);
    compiler.reset(); xxNEyAz.compile(compiler);
    out << xxNEyAz.toString() << endl << " decomposed: " << endl
        << xxNEyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xxNEyAz.solve() << endl << "WRONG!!!"
        << endl << endl;

    Qexpr<Qint> xxGTyAz(x > y + z);
    compiler.reset(); xxGTyAz.compile(compiler);
    out << xxGTyAz.toString() << endl << " decomposed: " << endl
        << xxGTyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xxGTyAz.solve() << endl << "WRONG!!!"
        << endl << endl;

    Qexpr<Qint> xxGEyAz(x >= y + z);
    compiler.reset(); xxGEyAz.compile(compiler);
    out << xxGEyAz.toString() << endl << " decomposed: " << endl
        << xxGEyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xxGEyAz.solve() << endl;

    Qexpr<Qint> xxLTyAz(x < y + z);
    compiler.reset(); xxLTyAz.compile(compiler);
    out << xxLTyAz.toString() << endl << " decomposed: " << endl
        << xxLTyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xxLTyAz.solve() << endl;

    Qexpr<Qint> xxLEyAz(x <= y + z);
    compiler.reset(); xxLEyAz.compile(compiler);
    out << xxLEyAz.toString() << endl << " decomposed: " << endl
        << xxLEyAz.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xxLEyAz.solve() << endl << "WRONG!!!"
        << endl << endl;
    
    Qexpr<Qint> xyAzEx(y + z == x);
    compiler.reset(); xyAzEx.compile(compiler);
    out << xyAzEx << endl << " decomposed: " << endl
        << xyAzEx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzEx.solve() << endl;

    Qexpr<Qint> xyAzNEx(y + z != x);
    compiler.reset(); xyAzNEx.compile(compiler);
    out << xyAzNEx.toString() << endl << " decomposed: " << endl
        << xyAzNEx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzNEx.solve() << endl << "WRONG!!!"
        << endl << endl;

    Qexpr<Qint> xyAzGTx(y + z > x);
    compiler.reset(); xyAzGTx.compile(compiler);
    out << xyAzGTx.toString() << endl << " decomposed: " << endl
        << xyAzGTx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzGTx.solve() << endl;

    Qexpr<Qint> xyAzGEx(y + z >= x);
    compiler.reset(); xyAzGEx.compile(compiler);
    out << xyAzGEx.toString() << endl << " decomposed: " << endl
        << xyAzGEx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzGEx.solve() << endl << "WRONG!!!"
    << endl << endl;

    Qexpr<Qint> xyAzLTx(y + z < x);
    compiler.reset(); xyAzLTx.compile(compiler);
    out << xyAzLTx.toString() << endl << " decomposed: " << endl
        << xyAzLTx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzLTx.solve() << endl << "WRONG!!!"
    << endl << endl;

    Qexpr<Qint> xyAzLEx(y + z <= x);
    compiler.reset(); xyAzLEx.compile(compiler);
    out << xyAzLEx.toString() << endl << " decomposed: " << endl
        << xyAzLEx.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xyAzLEx.solve() << endl;
}

void UTestQint::assignment(ostream& out)
{
    Qint r(2, "r"), x(2, "x");
    Qassign<Qint> QintAssign = r = x;

    QuboCompiler compiler; QintAssign.compile(compiler);
    out << "Assignment 'r = x' creats logic => " << QintAssign << endl
        << " It's finalized (execution ready) Qubo is '"
        << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    QintAssign.solve();
    out << QintAssign.solutions() << endl;

    QintAssign = r += x;
    compiler.reset(); QintAssign.compile(compiler);
    out << endl << "Assignment 'r += x' creats logic => " << QintAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    QintAssign.solve();
    out << QintAssign.solutions() << endl;

    QintAssign = r -= x;
    compiler.reset(); QintAssign.compile(compiler);
    out << endl << "Assignment 'r -= x' creats logic => " << QintAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl << QintAssign.solve() << endl;

    QintAssign = r *= x;
    compiler.reset(); QintAssign.compile(compiler);
    out << endl << "Assignment 'r *= x' creats logic => " << QintAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl << QintAssign.solve();

    QintAssign = r /= x;
    compiler.reset(); QintAssign.compile(compiler);
    out << endl << "Assignment 'r /= x' creats logic => " << QintAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeAd(compiler.qubo());
    out << endl << "# of nodes: " << analyzeAd.nodesNo()
        << "\t# of branches: " << analyzeAd.branchesNo() << endl;
//    out << "resulting in solutions:" << endl << QintAssign.solve()<< "WRONG!!!"
//    << endl << endl;

    Qint y("y", 6), z(2, "z"), k(2, "k");
    Qassign<Qint> a1 = y = r - x, a2 = k = y - z;
    Qblock blck;
    blck = a1, a2;
    QuboCompiler noFnlCmplr(false); blck.compile(noFnlCmplr);
    compiler.reset(); blck.compile(compiler);
    out << "Subtraction Expression" << endl << blck << endl
        << " Dann5 virtual code: " << blck.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    out  << " resulting in :" << endl << blck.solve() << endl;

    out << "Dann5.ocean Tests Qint addition!\n";
    Qint a(2, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A("A", 10), _1("1_", 1);
    Qassign<Qint> aA = A = a + b + c + d + _1;
    noFnlCmplr.reset(); aA.compile(noFnlCmplr);
    compiler.reset(); aA.compile(compiler);
    out << endl << aA << endl << endl << aA.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeA(compiler.qubo());
    out << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: "
        << analyzeA.branchesNo() << endl;
    out << " resulting in :" << endl << aA.solve();

    out << endl << "--- Quantum assignment vs. equal expression";
    Qint ax(3, "a"), b1(2, "b"), A1("A", 7), c1(3, "c"), d1(2, "d"),
            Ae1("Ae", 7);
    Qassign<Qint> aA1 = A1 = ax + b1;
    compiler.reset(); aA1.compile(compiler);
    out << endl << aA1.toString() << endl << aA1.solve() << endl;
    QuboAnalyzer analyzeA1(compiler.qubo());
    out << "Assignment # of nodes: " << analyzeA1.nodesNo()
        << "\t# of branches: " << analyzeA1.branchesNo() << endl;
    
    Qexpr<Qint> eA1 = Ae1 == c1 + d1;
    compiler.reset(); eA1.compile(compiler);
    out << endl << eA1.toString() << endl << eA1.solve() << endl;
    QuboAnalyzer analyzeE1(compiler.qubo());
    out << "Equal expression # of nodes: " << analyzeE1.nodesNo()
        << "\t# of branches: " << analyzeE1.branchesNo() << endl;

    {
        out << "==========================================================" << endl;
        Qint x(2, "x"), y(2, "y");
        Qint _3("_3_", 3);
        Qint _n3("_n3_", -3);

        Qexpr<Qint> expr = x + y;
        out << x.toString() << ", " << _3.toString() << ", " << _n3.toString() << endl;
        out << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        out << Bits(-4) << ", " << Bits(-8) << ", " << Bits(-7) << ", " << Bits(-6) << ", " << Bits(-5) << endl;
        out << "==========================================================" << endl;
        expr.reset();
        Qassign<Qint> assN3 = _n3 = expr;
        out << assN3 << endl << assN3.toString(true) << endl << assN3.solve() << endl;
        out << "==========================================================" << endl;
        expr.reset();
        Qassign<Qint> ass3 = _3 = expr;
        out << ass3 << endl << ass3.toString(true) << endl << ass3.solve() << endl;
        out << "==========================================================" << endl;
    }
    {
        Qint x(2, "x"), y(3, "y");

        Qexpr<Qint> expr = x + y;
        out << x.toString() << endl;
        out << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        out << "==========================================================" << endl;
    }
    {
        Qint x(2, "x"), y(2, "y");

        Qexpr<Qint> expr = x * y;
        out << x.toString() << endl;
        out << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        out << "==========================================================" << endl;
    }
    {
        Qint a(1, "a"), b(1, "b"), c(2, "c"), d(1, "d"), A(1, "A", -2);
        out << endl << "A = " << A.Qbin::toString() << endl;

        Qassign<Qint> aA = A = a + b + c + d + Qint::_1_;
        out << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

        out << endl << aA << endl << endl << aA.toString(true) << endl;

        aA.solve();
        out << endl << aA.solutions() << endl;
        out << "==========================================================" << endl;
    }
    {
        out << "Dann5.ocean Tests Qint Positive Add!\n";
        Qint a(2, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A(2, "A", 3), _1(2, "_1", 1), _2(2, "_2", 2);

        Qassign<Qint> aA = A = a + b + c + d + Qint::_1;
        out << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

        out << endl << aA << endl << endl << aA.toString(true) << endl;
        out << endl << aA.solve() << endl;
        out << "==========================================================" << endl;
    }
}

void UTestQint::factorial(ostream& out)
{
    Qint M("M", -4), p(2, "p"), q(1, "q"), r(1, "r");
    Qassign<Qint> mM = M = p * q * r;
    QuboCompiler noFnlCmplr(false); mM.compile(noFnlCmplr);
    QuboCompiler compiler; mM.compile(compiler);
    out << endl << mM << endl << endl << mM.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeM(compiler.qubo());
    out << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: "
        << analyzeM.branchesNo() << endl;
    const clock_t begin_time = clock();
    mM.solve();
    clock_t end_time = clock();
    out << endl << "Running time: "
        << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s";
    out << endl << mM.solutions();

    out << endl << "--- Large number factorial:";
    const clock_t begin_time2 = clock();
    Qint M1("M", 132560640), p1(8, "p"), q1(8, "q"), r1(8, "r");
    Qassign<Qint> mM1 = M1 = p1 * q1 * r1;
    clock_t end_time2 = clock();
    out << endl << "Preparation time: "
        << to_string(float(end_time2 - begin_time2) / CLOCKS_PER_SEC) << "s";
    out << endl << mM1 << endl;
    compiler.reset(); mM1.compile(compiler);
    QuboAnalyzer analyzeM1(compiler.qubo());
    out << endl << "# of nodes: " << analyzeM1.nodesNo() << "\t# of branches: "
        << analyzeM1.branchesNo() << endl;

}