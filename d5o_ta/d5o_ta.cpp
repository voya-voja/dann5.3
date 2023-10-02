// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <typeinfo>

#include <Qwhole.h>

#include <Qassign.h>
#include <Qblock.h>
#include <Qint.h>
#include <Qbinder.h>
#include <Qroutine.h>

#include <Qsolver.h>
#include <Utility.h>

#include "UTestQbit.h"
#include "UTestQbool.hpp"
#include "UTestQbin.hpp"
#include "UTestQwhole.hpp"
#include <ULint.h>

#include <algorithm>

using namespace std;
using namespace dann5::ocean;
using namespace dann5;


void qintNegativeAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint Negative Add!\n";
    Qint a(1, "a"), b(1, "b"), c(2, "c"), d(1, "d"), A(1, "A", -2), _1_(2, "_1_", -1), _2_(2,"_2_", -2), _3_(2, "_3_", -3), _4_(2,"_4_",-4), _5_(3, "_5_", -5), _6_(3, "_6_", -6), _7_(3, "_7_", -7);
    cout << endl << "-1 = " << _1_.Qbin::toString();
    cout << endl << "-2 = " << _2_.Qbin::toString();
    cout << endl << "-3 = " << _3_.Qbin::toString();
    cout << endl << "-4 = " << _4_.Qbin::toString();
    cout << endl << "-5 = " << _5_.Qbin::toString();
    cout << endl << "-6 = " << _6_.Qbin::toString();
    cout << endl << "-7 = " << _7_.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
    Qubo q = aA.qubo();
    Qsolver s0(q, false);
    Qsolver::Samples samples = s0.solution();
    cout << endl << aA.solutions() << endl << "min Energy : " << s0.minEnergy() << endl;
    s0.solution(cout);
}

void qintPositiveAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint Positive Add!\n";
    Qint a(2, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A(2, "A", 3), _1(2, "_1", 1), _2(2, "_2", 2);
    cout << endl << "1 = " << _1.Qbin::toString();
    cout << endl << "2 = " << _2.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

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

void testSolver()
{
    ofstream fout("testSolver.txt");
    Qbit a("a"), b("b"), N("N");
    Qassign<Qbit> aN = N = a & b;
    Qsolver sN(aN.qubo(), false);
    fout << endl << "Nodes #: " << sN.nodesNo() << ", Threds #: " << size_t(pow(2, int(log2(sN.nodesNo()) - 0.1)) / 2) << endl;
    sN.solution(fout);

    Qint i1(2, "a"), i2(2, "b"), A(2, "A");
    fout << endl << endl << "A =>" << A.toString(true) << endl;
    Qassign<Qint> aA = A = i1 + i2;
    fout << "A =>" << aA.assignee()->toString(true) << endl;
    fout << endl << aA << endl << aA.qubo() << endl;
    clock_t begin_time = clock();
    aA.solve();
    clock_t end_time = clock();
    fout << endl << "Running time: " << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s" << endl;
    fout << aA.solutions();

    Qsolver sA(aA.qubo(), false);
    fout << endl << "Nodes #: " << sA.nodesNo() << ", Threds #: " << size_t(pow(2, int(log2(sA.nodesNo()) - 0.1)) / 2) << endl;
    begin_time = clock();
    sA.solution();
    end_time = clock();
    fout << endl << "Running time: " << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s" << endl;
    sA.solution(fout);
    fout.close();
}

void testPNfile()
{
    ifstream pnFstream;
    char line[800];
    vector<ULint> primes;
#ifdef _WINDOWS
    pnFstream.open("..//.//d5o_ta//d5o_py//pnCandidates.txt");
#else
    pnFstream.open("//System//Volumes//Data//Developer//Work//d5o2//d5o_ta//d5o_py//pnCandidates.txt");
#endif
    bool fo = pnFstream.is_open();
    while (pnFstream.getline(line, 800))
    {
        cout << line << endl;
        ULint prime(line);
        cout << prime.toString() << endl;
        primes.push_back(prime);
    }
    sort(primes.begin(), primes.end());
    pnFstream.close();
    ofstream spnFstream;
#ifdef _WINDOWS
    spnFstream.open("..//.//d5o_ta//d5o_py//pnsSorted.txt");
#else
    spnFstream.open("//System//Volumes//Data//Developer//Work//d5o2//d5o_ta//d5o_py//pnsSorted.txt");
#endif
    ULint zero;
    for (auto candidate : primes)
    {
        for (auto prime : primes)
        {
            if (prime == candidate)
            {
                cout << candidate.toString() << " is PRIME" << endl;
                spnFstream << candidate.toString() << endl;
                break;
            }
            if (candidate % prime == zero)
            {
                cout  << "**** NOT a PRIME: " << candidate.toString() << " **** divisable with: " << prime.toString() << endl;
                break;
            }
        }
    }
    spnFstream.close();
}

int main(int argc, const char * argv[])
{
/*
    for (size_t nQbits = 2; nQbits <= 6; nQbits += 1)
    {
        Qwhole x(nQbits, "x"), y(size_t(nQbits / 2), "y"), r("r", nQbits);
        Qassign<Qwhole> xpr = r = x * y;
        Qanalyzer a(xpr.qubo());
        cout << xpr << endl << "  nodes: " << a.nodesNo() << " branches: " << a.branchesNo() << " chain-strenght: " << a.chainStrength();
        cout << endl << xpr.qubo() << endl << xpr.solve() << endl;
    }
    Qwhole x(2, "x"), y(1, "y"), z(1, "z"), _3("_3", 3);
    Qexpr<Qwhole> qwExpr = x * y;//, z_3Expr = _3 * z, qxxExpr = qwExpr * z_3Expr;
    Qubo qubo = qwExpr.qubo();
    cout << qubo / 2 << endl;
    Qanalyzer anlyzeE(qubo);
    cout << "Expression # of nodes: " << anlyzeE.nodesNo() << " # of branches: " << anlyzeE.branchesNo() << endl;
    cout << endl << "Expression:" << endl << qwExpr << endl << qwExpr.solve() << endl;
    Qsolver::Samples samples = qwExpr.compute();
    Qbinder binder;
    binder = x, y;
    binder.add(samples);
    cout << binder << endl;
    vector<ULint> xSolutions = dynamic_pointer_cast<Qwhole>(binder["x"])->ulints();
    vector<ULint> ySolutions = dynamic_pointer_cast<Qwhole>(binder["y"])->ulints();
    cout << "sample x  y" << endl;
    for(size_t at = 0; at < samples.size(); at++)
        cout << at << ":     " << xSolutions[at].toString() << "  " << ySolutions[at].toString() << endl;

    qxxExpr.reset();
    Qwhole R("R", 6);
    Qassign<Qwhole> qxxAssign = R = qxxExpr;
    Qubo aQubo = qxxAssign.qubo();
    Qanalyzer anlyze(aQubo);
    cout << "Assignment # of nodes: " << anlyze.nodesNo() << " # of branches: " << anlyze.branchesNo() << endl;
    cout << endl << "Assignment:" << endl << qxxAssign << endl << qxxAssign.solve() << endl;

    UTestQbit utQbit;
//    utQbit.runAll(cout);
    UTestQbool utQbool;
//    utQbool.runAll(cout);
    UTestQbin utQbin;
//    utQbin.friends_enemies(cout);
//    utQbin.runAll(cout);
    UTestQwhole utQwhole;
//    utQwhole.runAll(cout); 
*/
//    pymain();

//    testSolver();

    testPNfile();

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
