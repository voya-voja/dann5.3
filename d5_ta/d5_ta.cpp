// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <typeinfo>
#include <algorithm>

#include <Logger.h>

#include <Qwhole.h>

#include <Qassign.h>
#include <Qblock.h>
#include <Qbinder.h>
#include <Qroutine.h>

#include <D5QuboSolver.h>
#include <Utility.h>

#include "UTestQbit.h"
#include "UTestQbool.hpp"
#include "UTestQbin.hpp"
#include "UTestQwhole.hpp"
#include "UTestQint.hpp"

#include <ULint.h>

#include <Qbool.h>
#include <Qint.h>
#include <Qblock.h>

#include <Qubo.h>
#include <D5QuboSolver.h>
#include <QuboCompiler.h>
#include <Qfunc.h>

#include <Circuits.h>
#include <CircuitCompiler.h>

using namespace std;
using namespace dann5::ocean;
using namespace dann5::qiskit;
using namespace dann5;
using namespace dann5test;

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

void testQbitQiskit()
{
    Qbit x("x"), y("y"), z("z", 1);
    Qexpr<Qbit> expr((x & y) ^ z);
    CircuitCompiler compiler;
    expr.compile(compiler);
    Circuit& circuit = compiler.circuit();
    cout << circuit.instructions();
    cout << circuit.draw();

    cout << endl << expr.solve() << endl;
}

void testQbinQiskit()
{
    Qbin x(2, "x"), y(2, "y"), z("z", 1);
    Qexpr<Qbin> expr((x & y) ^ z);
    CircuitCompiler compiler;
    expr.compile(compiler);
    Circuit& circuit = compiler.circuit();
    cout << circuit.instructions();
    cout << circuit.draw();

    cout << endl << expr.solve() << endl;
}

void testQwholeQiskit()
{
    Qwhole x(2, "x"), y(2, "y"), r(2, "r");
    Qassign<Qwhole> addAssign(r = x * y);
    Qexpr<Qwhole> xXeqY(x == y);
    Qblock blck; blck = addAssign;// , xXeqY;
    CircuitCompiler compiler;
    blck.compile(compiler);
    Circuit& circuit = compiler.circuit();

    cout << circuit.nodesNo() << " vs " << circuit.Circuit::nodesNo() << endl;

    cout << "INSTRUCTIONS: " << endl << circuit.instructions() << endl;
    cout << circuit.draw() << endl;

    //cout << addAssign << endl << addAssign.toString(true) << endl << addAssign.solve();
    cout << blck << endl << blck.toString(true) << endl << blck.solve();
}

void qiskitPNs()
{
    Qwhole prime(3, "p"), _6("_6", 6), factor(1, "f");
    Qassign<Qwhole> assPN(prime = _6 * factor + Qwhole::_1);
    CircuitCompiler compiler;
    assPN.compile(compiler);
    Circuit& circuit = compiler.circuit();
    cout << assPN << endl << assPN.toString(true) << endl << circuit.draw() << endl;
    cout << assPN.solve() << endl;

    Qexpr<Qwhole> pnExpr(prime + Qwhole::_1 == _6 * factor);
    compiler = CircuitCompiler();
    pnExpr.compile(compiler);
    circuit = compiler.circuit();
    cout << pnExpr << endl << pnExpr.toString(true) << endl << circuit.draw() << endl;
    cout << pnExpr.solve() << endl;

    Qwhole pP1(4, "pP1"), _6Tf(3, "_6Tf");
    Qassign<Qwhole> assPp1(pP1 = prime + Qwhole::_1),
                    ass6tF(_6Tf = _6 * factor);
    Qexpr<Qwhole> eqExpr = assPp1 == ass6tF;
    Qblock blckPN;
    blckPN << ass6tF << eqExpr << assPp1;

    //blckPN = Qblock() << prime._(_6 * factor - _1)
    compiler = CircuitCompiler();
    blckPN.compile(compiler);
    circuit = compiler.circuit();
    cout << blckPN << endl << blckPN.toString(true) << endl << circuit.draw() << endl;
    cout << blckPN.solve() << endl;
}


void basicQint()
{
    Qint q_int;
    cout << endl << q_int << endl;
    q_int.id("id");
    cout << endl << q_int << endl;

    Qint qint("qint");
    cout << endl << qint << endl;
    qint.resize(2);
    cout << endl << qint << endl;

    Qint malfoy(2, "malfoy"), harry(2, "harry");
    cout << endl << malfoy << endl << harry << endl;

    Qexpr<Qint> xpr = malfoy + harry;
    cout << endl << xpr << endl << xpr.toString(true) << endl;
    cout << endl << xpr.compute() << endl;
    cout << endl << xpr.solve() << endl;

    Qint result(2, "result");
    Qassign<Qint> ass(result = malfoy - harry);
    cout << endl << ass << endl << ass.toString(true) << endl;
    cout << endl << ass.compute() << endl;
    cout << endl << ass.solve() << endl;

    ass.reset();
    ass = result = malfoy * harry;
    cout << endl << ass << endl << ass.toString(true) << endl;
    cout << endl << ass.compute() << endl;
    cout << endl << ass.solve() << endl;

    ass.reset();
    ass = result = malfoy / harry;
    cout << endl << ass << endl << ass.toString(true) << endl;
    cout << endl << ass.compute() << endl;
    cout << endl << ass.solve() << endl;

    /*Qint x(2, "x"), y("y", 6), z(2, "z");
    Qassign<Qint> qwExpr = z = y - x;
    Qblock byDx; byDx = qwExpr, x != Qint::_0;
    QuboCompiler noFnlCmplr(false), compiler;
    compiler.reset(); byDx.compile(compiler);
    cout << "Division Expression" << endl << byDx << endl
        << " decomposed logic: " << byDx.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    cout << endl << " & finalized Qubo is '"
        << compiler.qubo() << "'" << endl;
    QuboAnalyzer anlyzeDne(compiler.qubo());
    cout << endl << " # of nodes: " << anlyzeDne.nodesNo()
        << " # of branches: " << anlyzeDne.branchesNo() << endl << endl;
    cout << " resulting in :" << endl << byDx.solve() << endl;*/
}

void unitTests()
{
    UTestQbit utQbit;
    cout << utQbit.run() << endl;
    UTestQbool utQbool;
    cout << utQbool.run() << endl;
    UTestQbin utQbin;
    cout << utQbin.run() << endl;
    UTestQwhole utQwhole;
    cout << utQwhole.run() << endl;
    UTestQint utQint;
    cout << utQint.run() << endl;
}

void neTest()
{
    Qwhole w(2, "w"), c(2, "c"), w1("w1", 1), w0("w0", 0), w2("w2", 2);
    Qexpr<Qwhole> ltXpr = c != w1, lt1Xpr = w1 != c;
    QuboCompiler compiler, fullC(false);
    cout << ltXpr << endl << ltXpr.toString(true) << endl << endl;
    cout << ltXpr.solve() << endl << endl;
    cout << lt1Xpr << endl << lt1Xpr.toString(true) << endl << endl;
    cout << lt1Xpr.solve() << endl << endl;

    ltXpr.compile(fullC);
    cout << fullC.qubo() << endl << endl;
    ltXpr.compile(compiler);
    cout << compiler.qubo() << endl << endl;

    fullC.reset(); 
    compiler.reset();

    lt1Xpr.compile(fullC);
    cout << fullC.qubo() << endl << endl;
    lt1Xpr.compile(compiler);
    cout << compiler.qubo() << endl << endl;
}

int main(int argc, const char * argv[])
{
    _lf("main");
    Qsolver::Active(D5QuboSolver::Sp(new D5QuboSolver()));

    neTest();
//    unitTests();

//    testQbitQiskit();
//    testQbinQiskit();
//    testQwholeQiskit();
//    qiskitPNs();

    //Qexpr<Qwhole> qwExpr(y - x), qxwExpr = qwExpr + z + _3;
    //QuboCompiler noFnlCmplr(true); qxwExpr.compile(noFnlCmplr);
    //QuboCompiler compiler; qxwExpr.compile(compiler);
    //cout << "Addition Expression" << endl << qxwExpr << endl
    //    << " decomposed logic: " << qxwExpr.toString(true) << endl
    //    << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    //cout << endl << " & finalized Qubo is '"
    //    << compiler.qubo() << "'" << endl;
    //cout << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    //    testPNcandidates();
    _lft("main", "*** END ***");
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
