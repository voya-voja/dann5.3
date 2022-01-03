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

using namespace std;
using namespace dann5::ocean;

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
    Qbit a0("0a", 1), a1("1a"), a2("2a", 5), a3("3a", 0), a4("4a"), ar("ar",1);
    Qassign<Qbit> qbitAssign = ar = (a0 & a1) | ((a2 ^ a3) == a4);
    cout << endl << qbitAssign << endl << endl << qbitAssign.toString(true) << endl;
    cout << endl << "*** Logic Qubo ***" << endl << qbitAssign.qubo(false) << endl 
        << endl << "*** Reduced discrete values Qubo ***" << endl << qbitAssign.qubo() << endl;
    Qanalyzer analyze(qbitAssign.qubo());
    cout << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: " << analyze.branchesNo() << endl;
    qbitAssign.solve();
    cout << endl << qbitAssign.solutions();
}

void qbool_test()
{
    Qbool b0("0b"), b1("1b", 'F'), b2("2b", 33), b3("3b"), b4("4b"), br("br", Qbool::cTrue);
    Qassign<Qbool> qboolAssign = br = ((b3 != b4) & (b2 == b0)) | b1;
    cout << endl << qboolAssign << endl << endl << qboolAssign.toString(true) << endl;
    cout << endl << "*** Logic Qubo ***" << endl << qboolAssign.qubo(false) << endl
        << endl << "*** Reduced discrete values Qubo ***" << endl << qboolAssign.qubo() << endl;
    Qanalyzer analyze(qboolAssign.qubo());
    cout << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: " << analyze.branchesNo() << endl;
    qboolAssign.solve();
    cout << endl << qboolAssign.solutions();
}

void qbin_test()
{
    Qbin bn0("bn0", 03), bn1(3, "bn1"), bn2(4,"bn2"), bn3("bn3", 0b110), bn4(2, "bn4"), bnr("bnr", 0x5);
    Qassign<Qbin> qbinAssign = bnr = (bn0 & bn1) | ((bn2 ^ bn3) == bn4);
    cout << endl << qbinAssign << endl << endl << qbinAssign.toString(true) << endl;
    cout << endl << "*** Logic Qubo ***" << endl << qbinAssign.qubo(false) << endl
        << endl << "*** Reduced discrete values Qubo ***" << endl << qbinAssign.qubo() << endl;
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

    Qbit x0("x0"), x1("x1"), x2("x2"), y0("y0"), y1("y1"), y2("y2"), _1("_1", 1);
    Qexpr<Qbit> e1 = x0 <= y0, e2 = x1 <= y1, e3 = (x0 != y0) == _1, e4 = (x1 != y1) == _1;
    Qubo q = e1.qubo() + e2.qubo() /* + e3.qubo()*/ + e4.qubo();// + e5.qubo() + e6.qubo();

    cout << endl << e1 << endl << e2 << endl << e3 << endl << e4 << endl;
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

    Qbit x0("x0"), x1("x1"), x2("x2"), y0("y0"), y1("y1"), y2("y2"), _1("_1", 1), _0("_0", 0);
    Qexpr<Qbit> e0 = x0 <= y0, e1 = x1 <= y1, e01 = ((x0 & y1) & (~x1 & ~y0)) == _1, e2 = x2 <= y2, e12 = ((x1 & y2) & (~x2 & ~y1)) == _1, e02 = ((x0 & y2) & (~x2 & ~y0)) == _1;
    Qexpr<Qbit> eA = (x1 <= y1) & (x0 <= y0) & (((x0 & y1) & (~x1 & ~y0)) == _1);
    Qubo q = /*eA.qubo();*/e0.qubo() + e1.qubo() + e01.qubo() + e2.qubo() + e12.qubo();// +e02.qubo();
/*  q[Qkey("_01", "y1")] = -1;
    q[Qkey("_01", "x0")] = -1;
    q[Qkey("_01", "~y0")] = -1;
    q[Qkey("_01", "~x1")] = -1;
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

void qintAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint!\n";
    Qint a(3, "a"), b(3, "b"), c(2, "c"), d(1, "d"), A(2, "A", -3), _1_(2, "_1_", -1), _2_(2,"_2_", -2);
    cout << endl << "A = " << A.Qbin::toString();
    cout << endl << "-1 = " << _1_.Qbin::toString();
    cout << endl << "-2 = " << _2_.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString();
    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << dynamic_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString();
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

int main()
{
/*    string answer;
    cout << "should I start?";
    cin >> answer;
    basic_types();
    qbit_test();
    qbool_test();
    qbin_test();
    const clock_t begin_time = clock();
    qwholeAdd_test();
    clock_t addition_end_time = clock();
    cout << endl << "Running time: " << to_string(float(addition_end_time - begin_time) / CLOCKS_PER_SEC) << "s";

    qwholeX_test();
    clock_t multiplication_end_time = clock();
    cout << endl << "Running time: " << to_string(float(multiplication_end_time - addition_end_time) / CLOCKS_PER_SEC) << "s";

    qwholeXlarge_test();
*/
//    qwholeLt_test();
//    qwholeLe_test();
//    qwholeGe_test();
//    qwholeGt_test();

//    qintAdd_test();
//    pymain();

//    qassignVSqexpr();
    eqVSneq();
//    qbinXorTest();
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
