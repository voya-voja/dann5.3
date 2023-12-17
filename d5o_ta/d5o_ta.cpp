// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <typeinfo>
#include <algorithm>

#include <Qwhole.h>

#include <Qassign.h>
#include <Qblock.h>
#include <Qint.h>
#include <Qbinder.h>
#include <Qroutine.h>

#include <D5QuboSolver.h>
#include <Utility.h>

#include "UTestQbit.h"
#include "UTestQbool.hpp"
#include "UTestQbin.hpp"
#include "UTestQwhole.hpp"
#include <ULint.h>

#include <Qubo.h>
#include <D5QuboSolver.h>
#include <QuboCompiler.h>
#include <Qfunc.h>

using namespace std;
using namespace dann5::ocean;
using namespace dann5;
using namespace dann5test;

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
//    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

//    Qanalyzer analyzeA(aA.qubo());
//    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
//    Qubo q = aA.qubo();
//    D5QuboSolver s0(q, false);
//    Qevaluations samples = s0.solution();
//    cout << endl << aA.solutions() << endl << "min Energy : " << s0.minEnergy() << endl;
//    s0.solution(cout);
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
//    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

//    Qanalyzer analyzeA(aA.qubo());
//    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

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

void testPNcandidates()
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

Qexpr<Qwhole> minusF(const Qdef::Sp& pLeft,const Qdef::Sp& pRight)
{
    Qminus::Sp pMinus = Qminus::Sp(new Qminus());
    Qwhole out(pRight->noqbs(), pMinus->createOutId());
    pMinus->operands(out.clone(), {pLeft, pRight});
    Qexpr<Qwhole> xMinus(pMinus);
    return xMinus;
}

void testQminus()
{
    QuboCompiler compiler, fullCmplr(false);
    Qwhole x(2, "x"), y(2, "y"), z("z", 5);

    Qnary::Sp pY = dynamic_pointer_cast<Qnary>(y.clone());
    pY->resize(3);
    Qexpr<Qwhole> xFy_x = minusF(pY->clone(), x.clone()); // _-0 = (y - x);

    Qexpr<Qwhole> xFz_y_x(minusF(z.clone(), xFy_x.rootDef().clone())); // z - _-0; _-0 = (y - x);
    cout << xFz_y_x << endl
    << xFz_y_x.toString(true) << endl;
    xFz_y_x.compile(fullCmplr);
    xFz_y_x.compile(compiler);
    cout << fullCmplr.qubo() << endl
         << compiler.qubo() << endl
         << xFz_y_x.solve() << endl;
}

void testAddFunc()
{
    Qwhole x(2, "x"), y(2, "y"), z(2, "z");
    Qexpr<Qwhole> xP(x + y), xM(minusF(x.clone(), y.clone()));
    cout << xP << endl << xP.toString(true) << endl;
    cout << xM << endl << xM.toString(true) << endl;
    Qexpr<Qwhole> xPxP(z + xP), xPxM(z + xM);
    cout << xPxP << endl << xPxP.toString(true) << endl;
    cout << xPxM << endl << xPxM.toString(true) << endl;
    cout << xPxM.solve() << endl;
}

int main(int argc, const char * argv[])
{
    Qsolver::Active(D5QuboSolver::Sp(new D5QuboSolver()));

//    testQminus();
//    testAddFunc();
    
    Qbin x(4, "x"),z(3, "z"), y("y", 0b101);
    Qexpr<Qbin> xpr((x == z) != y);
    cout << xpr << endl << xpr.toString(true) << endl;
    cout << xpr.solve();
    Qexpr<Qbin> xpr1(y != (x == z));
    cout << xpr1 << endl << xpr1.toString(true) << endl;
    cout << xpr1.solve();

/*
    UTestQbit utQbit;
    utQbit.runAll(cout);
    UTestQbool utQbool;
    utQbool.runAll(cout);
    UTestQbin utQbin;
    utQbin.runAll(cout);
    UTestQwhole utQwhole;
    utQwhole.runAll(cout);
*/
//    pymain();

//    testPNcandidates();

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
