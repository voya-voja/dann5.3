// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Qbit.h>
#include <Qbool.h>
#include <Qbin.h>
#include <Qwhole.h>
#include <Qassign.h>

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
    Qwhole M("M", 49), p(3, "p"), q(3, "q"), r(1, "r");
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
    Qwhole M("M", 132560640), p(16, "p"), q(8, "q"), r(8, "r");
    Qassign<Qwhole> mM = M = p * q * r;
    cout << endl << mM << endl;
    Qanalyzer analyzeM(mM.qubo());
    cout << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: " << analyzeM.branchesNo() << endl;
//    mM.solve();
//    cout << endl << mM.solutions();
}

int main()
{
    string answer;
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

//    qwholeXlarge_test();
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
