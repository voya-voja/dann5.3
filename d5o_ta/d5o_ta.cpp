// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Qbit.h>
#include <Qbool.h>
#include <Qbin.h>
#include <Qwhole.h>
#include <Qassign.h>
#include <Qint.h>

#include <Qsolver.h>
#include <Utility.h>

using namespace std;
using namespace dann5::ocean;
using namespace dann5;


void basic_types()
{
    Qbit a0("0a", 1);
    cout << a0.toString() << " " << a0.toString(false, 0) << " " << a0.toString(true) << " " << a0.toString(true, 0) << endl;

    Qbool b("boolean", Qbool::cTrue);
    cout << b.toString() << " " << b.toString(false, 0) << " " << b.toString(true) << " " << b.toString(true, 0) << endl;

    Qbin a(3, "a");
    cout << a.toString() << " " << a.toString(false, 0) << " " << a.toString(true) << " " << a.toString(true, 0) << endl;

    Qwhole x("a", 33);
    cout << x.toString() << " " << x.toString(false, 0) << " " << x.toString(true) << " " << x.toString(true, 0) << endl;
}

void qbit_test()
{
    Qbit a("a"), r("r"), b("b", 1), x("x"), y("y", 5), z("z"), _0("_0_", 0), _1("_1_", 1);
/*
    cout << "x !& y:" << endl << (x.nand(y)).qubo() << endl << (x.nand(y)).solve() << endl;
    cout << "_1_ & (x !& y):" << endl << (_1.nand(x.nand(y))).solve() << endl;
    cout << "x !| y:" << endl << (x.nor(y)).solve() << endl;
    cout << "_0_ !| (x & y):" << endl << (_0.nor(x.nor(y))).solve() << endl;

    Qexpr<Qbit> xGtNe = z > (x != y), xGeNe = z >= (x != y), xLtNe = z < (x != y), xLeNe = z <= (x != y);
    Qexpr<Qbit> xNeGt = (x != y) > z, xNeGe = (x != y) >= z, xNeLt = (x != y) < z, xNeLe = (x != y) <= z;
    cout << xGtNe.toString(true) << endl << xGtNe.qubo() << endl << xGtNe.solve() << endl
        << xGeNe.toString(true) << endl << xGeNe.qubo() << endl << xGeNe.solve() << endl
        << xLtNe.toString(true) << endl << xLtNe.qubo() << endl << xLtNe.solve() << endl
        << xLeNe.toString(true) << endl << xLeNe.qubo() << endl << xLeNe.solve() << endl
        << xNeGt.toString(true) << endl << xNeGt.qubo() << endl << xNeGt.solve() << endl
        << xNeGe.toString(true) << endl << xNeGe.qubo() << endl << xNeGe.solve() << endl
        << xNeLt.toString(true) << endl << xNeLt.qubo() << endl << xNeLt.solve() << endl
        << xNeLe.toString(true) << endl << xNeLe.qubo() << endl << xNeLe.solve() << endl;

    Qexpr<Qbit> xEq = x == y, xAl = x *= y, xNe = x != y, xUl = x ^ y, xI = ~x;
    cout << xI << endl << xI.toString(true) << endl;

    Qassign<Qbit> aR = r = a, axEq = r = x == y, axAl = r = x *= y, axUl = _1 = x ^ y;

    cout << "Assignment\n" << aR.qubo() << "\nEqual Expression\n" << xEq.qubo() 
        << "\nEqual Assignment\n" << axEq.qubo() << "\nAlike Expression\n" 
        << xAl.qubo() << "\nAlike Assignment\n" << axAl.qubo() << endl;
    cout << "\nAssignment\n" << aR.solve() << "\nEqual Expression\n" << xEq.solve() 
        << "\nEqual Assignment\n" << axEq.solve() << "\nAlike Expression\n" 
        << xAl.solve() << "\nAlike Assignment\n" << axAl.solve() << endl;

    cout << "\nNot Equal Expression\n" << xNe.qubo() << "\nUnlike Expression\n" 
        << xUl.qubo() << "\nUnlike Assignment\n" << axUl.qubo() << endl;
    cout << "\nNot equal Expression\n" << xNe.solve() << "\nUnlike Expression\n" 
        << xUl.solve() << "\nUnlike Assignment\n" << axUl.solve() << endl;
*/
    Qassign<Qbit> qbitAssign = r = x; //_1 = (z != (b & x)) | (z != (y ^ _0));
    cout << qbitAssign << endl;
/*
    cout << endl << qbitAssign << endl << endl << replaceAll(qbitAssign.toString(true), "; ", "\n") << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qbitAssign.qubo(false) << endl 
        << endl << "*** Finalized Qubo ***" << endl << qbitAssign.qubo() << endl;
    Qanalyzer analyze(qbitAssign.qubo());
    cout << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: " << analyze.branchesNo() << endl;
*/    qbitAssign.solve();
    cout << endl << qbitAssign.solutions();

    Qassign<Qbit> qbitAssign2 = _1 = ((b & x) != z) | (z != (y ^ _0));
/*    cout << endl << qbitAssign2 << endl << endl << qbitAssign2.toString(true) << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qbitAssign2.qubo(false) << endl
        << endl << "*** Finalized Qubo ***" << endl << qbitAssign2.qubo() << endl;
    Qanalyzer analyze2(qbitAssign2.qubo());
    cout << endl << "# of nodes: " << analyze2.nodesNo() << "\t# of branches: " << analyze2.branchesNo() << endl;
*/    qbitAssign2.solve();
    cout << endl << qbitAssign2.solutions();

    Qassign<Qbit> qbitAssign3 = _1 = ((b & x) != z) | ((y ^ _0) != z);
/*    cout << endl << qbitAssign3 << endl << endl << qbitAssign3.toString(true) << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qbitAssign3.qubo(false) << endl
        << endl << "*** Finalized Qubo ***" << endl << qbitAssign3.qubo() << endl;
    Qanalyzer analyze3(qbitAssign3.qubo());
    cout << endl << "# of nodes: " << analyze3.nodesNo() << "\t# of branches: " << analyze3.branchesNo() << endl;
*/    qbitAssign3.solve();
    cout << endl << qbitAssign3.solutions();

    Qassign<Qbit> qbitAssign4 = _1 = (z != (b & x)) | ((y ^ _0) != z);
/*    cout << endl << qbitAssign4 << endl << endl << qbitAssign4.toString(true) << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qbitAssign4.qubo(false) << endl
        << endl << "*** Finalized Qubo ***" << endl << qbitAssign4.qubo() << endl;
    Qanalyzer analyze4(qbitAssign4.qubo());
    cout << endl << "# of nodes: " << analyze4.nodesNo() << "\t# of branches: " << analyze4.branchesNo() << endl;
*/    qbitAssign4.solve();
    cout << endl << qbitAssign4.solutions();
/*
    Qassign<Qbit> qbitA = _1 = ((b & x).unlike(z)) | ((y ^ _0).alike(z));
    cout << endl << qbitA << endl << endl << qbitA.toString(true) << endl;
    Qanalyzer anlyze(qbitA.qubo());
    cout << endl << "# of nodes: " << anlyze.nodesNo() << "\t# of branches: " << anlyze.branchesNo() << endl;
    cout << endl << qbitA.solve();
*/
}

void qbitLt_test()
{
    Qbit x0("x0"), x1("x1"), x2("x2"), y0("y0"), y1("y1"), y2("y2"), _1("_1", 1);
    Qexpr<Qbit> xpr =  (x1 < y1) | ((x1 *= y1) & (x0 < y0));
    cout << xpr << endl << xpr.toString(true) << endl << endl;
    cout << xpr.qubo(false) << endl << xpr.qubo() << endl << endl;
    cout << xpr.solve() << endl;

    Qexpr<Qbit> xpr1 = ((x0 < y0) & (x1 *= y1)) | (x1 < y1);
    cout << xpr1 << endl << xpr1.toString(true) << endl << endl;
    cout << xpr1.qubo(false) << endl << xpr1.qubo() << endl << endl;
    cout << xpr1.solve() << endl;

    Qexpr<Qbit> e1 = x0 <= y0, e2 = x1 <= y1, e3 = x0 != y0, e4 = x1 != y1;
    Qubo q;// = e1.qubo() + e2.qubo() /* + e3.qubo()*/ + e4.qubo();// + e5.qubo() + e6.qubo();
    q[Qkey("x0", "x0")] = 1;
    q[Qkey("x1", "x1")] = 1;
    q[Qkey("y0", "y0")] = 0;
    q[Qkey("y1", "y1")] = 0;
    q[Qkey("x1", "y1")] = 1;
    q[Qkey("x0", "y1")] = -1;
    q[Qkey("x1", "y0")] = -1;
    q[Qkey("x0", "x1")] = 1;
    q[Qkey("T", "T")] = 1;
    q[Qkey("T", "y1")] = -2;
    q[Qkey("T", "x0")] = 4;

//    cout << endl << e1 << endl << e2 << endl << e3 << endl << e4 << endl;
    cout << endl << "*** Qubo ***" << endl << q << endl;
    Qsolver s(q);
    cout << endl << "# of nodes: " << s.nodesNo() << "\t# of branches: " << s.branchesNo() << endl;
    for (auto sample : s.solution())
    {
        cout << endl;
        for (auto element : sample)
            cout << element.first << ": " << to_string(element.second) << "; ";
    }
}

void qbitLe_test()
{
    Qbit x0("x0"), x1("x1"), x2("x2"), y0("y0"), y1("y1"), y2("y2"), _1("_1", 1), _0("_0", 0);
    Qexpr<Qbit> e0 = x0 <= y0, e1 = x1 <= y1, e01 = ((x0 & y1) & (~x1 & ~y0)) == _1, e2 = x2 <= y2, e12 = ((x1 & y2) & (~x2 & ~y1)) == _1, e02 = ((x0 & y2) & (~x2 & ~y0)) == _1;
    Qexpr<Qbit> eA = (x1 <= y1) & (x0 <= y0) & (((x0 & y1) & (~x1 & ~y0)) == _1);
    Qubo q = /*eA.qubo();*/e0.qubo() + e1.qubo() + e01.qubo() + e2.qubo() + e12.qubo();// +e02.qubo();
/*
    q[Qkey("_01", "y1")] = -1;
    q[Qkey("_01", "x0")] = -1;
    q[Qkey("_01", "!y0")] = -1;
    q[Qkey("_01", "!x1")] = -1;
    q[Qkey("_01", "_01")] = 4;
    q[Qkey("_=02", "y2")] = -2;
    q[Qkey("_=02", "x0")] = -2;
    q[Qkey("_=02", "_=02")] = 4;
    q[Qkey("_=12", "y2")] = -2;
    q[Qkey("_=12", "x1")] = -2;
    q[Qkey("_=12", "_=12")] = 4;
*/
    cout << endl /*<< eA << endl;*/ << e0 << endl << e1 << endl << e01 << endl << e2 << endl;
    cout << endl << "*** Qubo ***" << endl << q << endl;
    Qsolver s(q);
    cout << endl << "# of nodes: " << s.nodesNo() << "\t# of branches: " << s.branchesNo() << endl;
    for (auto sample : s.solution())
    {
        cout << endl;
        for (auto element : sample)
            cout << element.first << ": " << to_string(element.second) << "; ";
    }

}

void qbool_test()
{
    Qbool b0("0b"), b1("1b", 'F'), b2("2b", 33), b3("3b"), b4("4b"), br("br", Qbool::cTrue);
    Qassign<Qbool> qboolAssign = br = ((b3 != b4) & (b2 == b0)) | b1;
    cout << endl << qboolAssign << endl << endl << qboolAssign.toString(true) << endl;
    cout << endl << "*** Generic Qubo ***" << endl << qboolAssign.qubo(false) << endl
        << endl << "*** Finalized Qubo ***" << endl << qboolAssign.qubo() << endl;
    Qanalyzer analyze(qboolAssign.qubo());
    cout << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: " << analyze.branchesNo() << endl;
    qboolAssign.solve();
    cout << endl << qboolAssign.solutions();
}

void friends_enemies()
{
    Qbool xaviar("x"), yolanda("y"), zeke("z"), wanda("w"), _T_("_T_", true);
    Qexpr<Qbool> xFnEs = (xaviar == yolanda) & (!yolanda == zeke) & (zeke == wanda);
    cout << endl << xFnEs.solve() << endl;
    Qassign<Qbool> aFnEs = _T_ = (xaviar & yolanda) & (!yolanda & !zeke) & (zeke & wanda);
//    cout << endl << aFnEs.solve() << endl;

    Qbit x("x"), y("y"), z("z"), w("w"), _1_("_1_", 1);
    Qexpr<Qbit> xbFnEs = (x == y) & ((~y == z) & (z == w));
    cout << endl << xbFnEs.toString(true) << endl;
    cout << endl << xbFnEs.solve() << endl;

    Qassign<Qbit> abFnEs = _1_ = (x *= y) & ((y ^ z) & (z *= w));
    cout << endl << abFnEs.toString(true) << endl;
    cout << endl << abFnEs.solve() << endl;

}

void vertex()
{
/*    Qbool xaviar("x"), yolanda("y"), zeke("z"), wanda("w"), _T_("_T_", true);
    Qexpr<Qbool> xFnEs = (xaviar == yolanda) & (!yolanda == zeke) & (zeke == wanda);
    cout << endl << xFnEs.solve() << endl;
    Qassign<Qbool> aFnEs = _T_ = (xaviar & yolanda) & (!yolanda & !zeke) & (zeke & wanda);
    //    cout << endl << aFnEs.solve() << endl;
*/
    Qbit v1("v1"), v2("v2"), v3("v3"), v4("v4"), v5("v5"), _1_("_1_", 1);
    Qexpr<Qbit> xbV = (v1 == v2) & (v1 == v3) & (v2 == v4) & (v3 == v4) & (v3 == v5) & (v4 == v5);
    cout << endl << xbV.toString(true) << endl;
    cout << endl << xbV.solve() << endl;
/*
    Qassign<Qbit> abFnEs = _1_ = (x *= y) & ((y ^ z) & (z *= w));
    cout << endl << abFnEs.toString(true) << endl;
    cout << endl << abFnEs.solve() << endl;
*/
}

void qbin_test()
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

void qwholeAdd_test()
{
    std::cout << "Dann5.ocean Tests Qwhole!\n";
    Qwhole a(4, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A("A", 21), _1("1_", 1);
    Qassign<Qwhole> aA = A = a + b + c + d + _1;// 
    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;
    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;
    aA.solve();
    cout << endl << aA.solutions();
}

void qwholeX_test()
{   
    Qwhole M("M", 42), p(3, "p"), q(2, "q"), r(2, "r");
    Qassign<Qwhole> mM = M = p * q *r;
    cout << endl << mM << endl << endl << mM.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << mM.qubo(false) << endl << mM.qubo() << endl;
    Qanalyzer analyzeM(mM.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    mM.solve();
    cout << endl << mM.solutions();
    Qsolver solver(mM.qubo());
    Qsolver::Samples solutions = solver.solution();
    mM.add(solutions);
    cout << endl << mM.solutions();
}


void qwholeXlarge_test()
{
    Qwhole M("M", 132560640), p(8, "p"), q(8, "q"), r(8, "r");
    Qassign<Qwhole> mM = M = p * q * r;
    cout << endl << mM << endl;
    Qanalyzer analyzeM(mM.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
//    mM.solve();
//    cout << endl << mM.solutions();
}

void qwholeEq_test()
{
    Qwhole x(2, "x"), y(2, "y"), z(2,"z"), w(2, "w");
    Qexpr<Qwhole> comp = x + y == z * w;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() / 2 << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qwholeNe_test()
{
    Qwhole x(2, "x"), y(2, "y"), z(2, "z"), w(2, "w");
    Qexpr<Qwhole> comp = x + y != z + w;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() / 2 << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qwholeLt_test()
{
    Qwhole x(2, "x"), y(2, "y");
    Qexpr<Qwhole> comp = x < y;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() / 2 << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qwholeLe_test()
{
    Qwhole x(2, "x"), y(2, "y");
    Qexpr<Qwhole> comp = x <= y;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qwholeGe_test()
{
    Qwhole x(3, "x"), y(3, "y");
    Qexpr<Qwhole> comp = x >= y;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qwholeGt_test()
{
    Qwhole x(3, "x"), y(3, "y");
    Qexpr<Qwhole> comp = x > y;
    cout << endl << comp << endl << endl << comp.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << comp.qubo(false) << endl << comp.qubo() << endl;
    Qanalyzer analyzeM(comp.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    cout << endl << comp.solutions();
}

void qintNegativeAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint!\n";
    Qint a(3, "a"), b(3, "b"), c(2, "c"), d(1, "d"), A(2, "A", -3), _1_(2, "_1_", -1), _2_(2,"_2_", -2), _4_(3,"_4_",-4), _5_(3, "_5_", -5), _6_(3, "_6_", -6), _7_(3, "_7_", -7);
    cout << endl << "-1 = " << _1_.Qbin::toString();
    cout << endl << "-2 = " << _2_.Qbin::toString();
    cout << endl << "-4 = " << _4_.Qbin::toString();
    cout << endl << "-5 = " << _5_.Qbin::toString();
    cout << endl << "-6 = " << _6_.Qbin::toString();
    cout << endl << "-7 = " << _7_.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << dynamic_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
    cout << endl << aA.solutions();

    Qubo q = aA.qubo();
    q[Qkey("a3", "a3")] = 0;
    q[Qkey("b3", "b3")] = 0;
    q[Qkey("a3", "b3")] = 1;
    cout << endl << "*** Qubo ***" << endl << q << endl;
    Qsolver solver(q);
    Qsolver::Samples samples = solver.solution();
    aA.add(samples);
    cout << endl << aA.solutions() << endl << "min Energy : " << solver.minEnergy() << endl;
}

void qintPositiveAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint!\n";
    Qint a(3, "a"), b(3, "b"), c(2, "c"), d(1, "d"), A(2, "A", 3), _1(2, "_1", 1), _2(2, "_2", 2);
    cout << endl << "1 = " << _1.Qbin::toString();
    cout << endl << "2 = " << _2.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << dynamic_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
    cout << endl << aA.solutions();
}

#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

int pymain() {
    py::scoped_interpreter python;

    auto math = py::module::import("math");
    double root_two = math.attr("sqrt")(2.0).cast<double>();

    std::cout << "The square root of 2 is: " << root_two << "\n";

    return 0;
}

void qassignVSqexpr()
{
    Qwhole a(3, "a"), b(2, "b"), A("A", 7), c(3, "c"), d(2, "d"), Ae("Ae", 7);
    Qassign<Qwhole> aA = A = a + b;
    cout << aA.toString() << endl << aA.solve() << endl;
    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;
    Qexpr<Qwhole> eA = Ae == c + d;
    cout << eA.toString() << endl << eA.solve() << endl;
    Qanalyzer analyzeE(eA.qubo());
    cout << endl << "# of nodes: " << analyzeE.nodesNo() << "\t# of branches: " << analyzeE.branchesNo() << endl;
}

void eqVSneq()
{
    Qwhole c(3, "c"), d(2, "d"), We("We", 7), e(3, "e"), f(2, "f"), Wne("Wne", 7);
    Qexpr<Qwhole> eW = We == c + d;
    cout << eW.toString() << endl << eW.solve() << endl;
    Qanalyzer analyzeEw(eW.qubo());
    cout << endl << "# of nodes: " << analyzeEw.nodesNo() << "\t# of branches: " << analyzeEw.branchesNo() << endl;
    Qexpr<Qwhole> neW = Wne != e + f;
    cout << neW.toString() << endl << neW.solve() << endl;
    Qanalyzer analyzeNEw(neW.qubo());
    cout << endl << "# of nodes: " << analyzeNEw.nodesNo() << "\t# of branches: " << analyzeNEw.branchesNo() << endl;

    Qbin a(3, "a"), b(2, "b"), Be("Be", 7), x(2, "x"), y(3, "y"), Bne("Bne", 7);
    Qexpr<Qbin> eB = Be == (a ^ b);
    cout << eB.toString() << endl << eB.solve() << endl;
    Qanalyzer analyzeEb(eB.qubo());
    cout << endl << "# of nodes: " << analyzeEb.nodesNo() << "\t# of branches: " << analyzeEb.branchesNo() << endl;
    Qexpr<Qbin> neB = Bne != (x ^ y);
    cout << neB.toString() << endl << neB.solve() << endl;
    Qanalyzer analyzeNEb(neB.qubo());
    cout << endl << "# of nodes: " << analyzeNEb.nodesNo() << "\t# of branches: " << analyzeNEb.branchesNo() << endl;
}

void qbinXorTest()
{
    Qbin a(3, "a"), b(3, "b"), Be("Be", 7), x(3, "x"), y(3, "y"), Bne("Bne", 7);
    Qassign<Qbin> aB = Be = (a ^ b);
    cout << aB.toString() << endl << aB.toString(true) << endl << aB.solve() << endl;
    Qanalyzer analyzeAb(aB.qubo());
    cout << endl << "# of nodes: " << analyzeAb.nodesNo() << "\t# of branches: " << analyzeAb.branchesNo() << endl;

    Qexpr<Qbin> eB = Bne == (x ^ y);
    cout << eB.toString() << endl << endl << eB.toString(true) << eB.solve() << endl;
    Qanalyzer analyzeEb(eB.qubo());
    cout << endl << "# of nodes: " << analyzeEb.nodesNo() << "\t# of branches: " << analyzeEb.branchesNo() << endl;
}

void routineTest()
{
    Qwhole c(3, "c"), d(2, "d"), We("We", 7), _3("3_", 3), _1("1_", 1);
    Qexpr<Qwhole> xW = We == c + d;
    Qanalyzer xWa(xW.qubo(false)), xWfa(xW.qubo());
    cout << "\n\nAddition:\n" << xWa.nodes() << endl << xWa.branches() << endl;
    cout << xW.toString() << endl << xW.solve() << endl;
    Qexpr<Qwhole> xEqW = d == _3;
    Qanalyzer xEqWa(xEqW.qubo(false));
    cout << "\n\nEqual:\n" << xEqWa.nodes() << endl << xEqWa.branches() << endl;
    cout << xEqW.toString() << endl << xEqW.solve() << endl;
    Qanalyzer rWa(xW.qubo(false) + xEqW.qubo(false));
    cout << "\n\nUnion:\n" << rWa.nodes() << endl << rWa.branches() << endl;
    Qubo rQ = xW.qubo() + xEqW.qubo();
    Qsolver solver(rQ);
    xW.reset();
    xW.add(solver.solution());
    cout << xW.toString() << endl << xW.solve() << endl;
    cout << "\n\n---------- FINALIZED -------\n\nUnion:\n" << solver.nodes() << endl << solver.branches() << endl;
    cout << "\n\nAddition:\n" << xWfa.nodes() << endl << xWfa.branches() << endl;

    Qexpr<Qwhole> xaneW = (We == c + d) & (d == _3), xareW = (We == c + d) | (d == _3);
    cout << xaneW.toString() << endl << xaneW.solve() << endl;
    cout << xareW.toString() << endl << xareW.solve() << endl;
}

int main()
{
/*    string answer;
    cout << "should I start?";
    cin >> answer;
*/
    basic_types();
//    qbit_test();
//    qbitLt_test();
//    qbitLe_test();
//    qbool_test();
//    friends_enemies();
//    vertex();
//    qbin_test();
/*
    const clock_t begin_time = clock();
    qwholeAdd_test();
    clock_t addition_end_time = clock();
    cout << endl << "Running time: " << to_string(float(addition_end_time - begin_time) / CLOCKS_PER_SEC) << "s";

    qwholeX_test();
    clock_t multiplication_end_time = clock();
    cout << endl << "Running time: " << to_string(float(multiplication_end_time - addition_end_time) / CLOCKS_PER_SEC) << "s";

    qwholeXlarge_test();
*/
//    qwholeEq_test();
//    qwholeNe_test();
//    qwholeLt_test();
//    qwholeLe_test();
//    qwholeGe_test();
//    qwholeGt_test();

    qintNegativeAdd_test();
    qintPositiveAdd_test();

//    pymain();

//    qassignVSqexpr();
//    eqVSneq();
//    qbinXorTest();
//    routineTest();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
