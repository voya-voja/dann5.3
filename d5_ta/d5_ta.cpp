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
#include "Qbool.h"

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

void testQint()
{
    {
        Qint x(2, "x"), y(2, "y");
        Qint _3("_3_", 3);
        Qint _n3("_n3_", -3);

        Qexpr<Qint> expr = x + y;
        cout << x.toString() << ", " << _3.toString() << ", " << _n3.toString() << endl;
        cout << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        cout << Bits(-4) << ", " << Bits(-8) << ", " << Bits(-7) << ", " << Bits(-6) << ", " << Bits(-5) << endl;
        cout << "==========================================================" << endl;
        expr.reset();
        Qassign<Qint> assN3 = _n3 = expr;
        cout << assN3 << endl << assN3.toString(true) << endl << assN3.solve() << endl;
        cout << "==========================================================" << endl;
        expr.reset();
        Qassign<Qint> ass3 = _3 = expr;
        cout << ass3 << endl << ass3.toString(true) << endl << ass3.solve() << endl;
        cout << "==========================================================" << endl;
    }
    {
        Qint x(2, "x"), y(3, "y");

        Qexpr<Qint> expr = x + y;
        cout << x.toString() << endl;
        cout << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        cout << "==========================================================" << endl;
    }
    {
        Qwhole i0(1, "i0"), i1(1, "i1"), ic(1, "ic"), o(1, "o");

        Qassign<Qwhole> assign = o = i0 + i1 + ic;
        QuboCompiler compiler;
        assign.compile(compiler);
        cout << assign << endl << assign.toString(true) << endl << assign.solve() << endl;
        cout << compiler.qubo();
        cout << "==========================================================" << endl;
    }
    {
        Qint x(2, "x"), y(2, "y");

        Qexpr<Qint> expr = x * y;
        cout << x.toString() << endl;
        cout << expr << endl << expr.toString(true) << endl << expr.solve() << endl;
        cout << "==========================================================" << endl;
    }
}

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

int main(int argc, const char* argv[])
{
    _lf("main");
    Qsolver::Active(D5QuboSolver::Sp(new D5QuboSolver()));
    // testQint();

//    testQbitQiskit();
//    testQbinQiskit();
//    testQwholeQiskit();
//    qiskitPNs();
    {
        Qwhole x(2, "x"), y(2, "y");
        Qexpr<Qwhole> xpr = x > y;
        cout << xpr << endl << xpr.toString(true) << endl << xpr.solve();
    }

    { // WORKING when x is 3 or more qbits
        Qwhole x(3, "x"), y("y", 5), z(1, "z");
        Qexpr<Qwhole> xpr = (y + z) > x;
        cout << xpr << endl << xpr.toString(true) << endl << xpr.solve();

        Qwhole w(3, "w");
        Qassign<Qwhole> ass = w = y + z;
        Qblock blck; blck = ass, w > x;
        cout << blck << endl << blck.toString(true) << endl << blck.solve();
    }

    { // WORKING when x is of proper size, e.g. 4 qbits when operands are 3 qbits and 1 qbit 
        Qwhole x(4, "x"), y("y", 5), z(1, "z");
        Qexpr<Qwhole> xpr = x > (y + z);
        cout << xpr << endl << xpr.toString(true) << endl << xpr.solve();

        Qwhole w(4, "w");
        Qassign<Qwhole> ass = w = y + z;
        Qblock blck; blck = ass, x > w;
        cout << blck << endl << blck.toString(true) << endl << blck.solve();
    }

    { // WORKING 
        Qwhole x(4, "x"), y("y", 5), z(1, "z"), o(2, "o"), p(2, "p");
        Qexpr<Qwhole> xpr = (o + p) > (y + z);
        cout << xpr << endl << xpr.toString(true) << endl << xpr.solve();

        Qwhole w(4, "w");
        Qassign<Qwhole> ass = w = y + z;
        Qassign<Qwhole> ass2 = x = o + p;
        Qblock blck; blck = ass, ass2, x > w;
        cout << blck << endl << blck.toString(true) << endl << blck.solve();
    }

    //Qexpr<Qwhole> qwExpr(y - x), qxwExpr = qwExpr + z + _3;
    //QuboCompiler noFnlCmplr(true); qxwExpr.compile(noFnlCmplr);
    //QuboCompiler compiler; qxwExpr.compile(compiler);
    //cout << "Addition Expression" << endl << qxwExpr << endl
    //    << " decomposed logic: " << qxwExpr.toString(true) << endl
    //    << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    //cout << endl << " & finalized Qubo is '"
    //    << compiler.qubo() << "'" << endl;
    //cout << endl << " resulting in :" << endl << qxwExpr.solve() << endl;


    /*UTestQbit utQbit;
    utQbit.runAll(cout);
    UTestQbool utQbool;
    utQbool.runAll(cout);
    UTestQbin utQbin;
    utQbin.runAll(cout);
    UTestQwhole utQwhole;
    utQwhole.runAll(cout);*/

    // boolean statement for all the possible variables called Nik and Shawnie
   /* {
        Qbool nik("nik"), shawnie("shawnie");

        Qexpr<Qbool> expression = nik & shawnie;

        cout << nik << endl;
        cout << shawnie << endl;

        cout << expression << endl;
        cout << expression.toString(true) << endl;
        cout << expression.solve() << endl;

        cout << "====================" << endl;

        Qassign<Qbool> assignment(Qbool::_T = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;
    }
    cout << "===================" << endl;
    {
        Qbit jim("jim"), pam("pam");

        Qexpr<Qbit> expression = jim & pam;

        cout << jim << endl;
        cout << pam << endl;

        cout << expression << endl;
        cout << expression.toString(true) << endl;
        cout << expression.solve() << endl;

        cout << "====================" << endl;

        Qassign<Qbit> assignment(Qbit::_0 = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;
    }
    cout << "===================" << endl;
    try
    {
        Qbin jim(2, "jim"), pam(2, "pam");

        Qexpr<Qbin> expression = jim & pam;

        cout << jim << endl;
        cout << pam << endl;

        cout << expression << endl;
        cout << expression.toString(true) << endl;
        cout << expression.solve() << endl;

        cout << "====================" << endl;

        Qbin _3("_3_", 3);
        Qassign<Qbin> assignment(_3 = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;

        cout << "====================" << endl;

        Qbin _i_(3, "_i_");
        assignment = (_i_ = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }

    cout << "===================" << endl;
    try
    {
        Qwhole kurt(3, "kurt"), john(3, "john");

        Qexpr<Qbin> expression = kurt & john;

        cout << kurt << endl;
        cout << john << endl;

        cout << expression << endl;
        cout << expression.toString(true) << endl;
        cout << expression.solve() << endl;

        cout << "====================" << endl;

        Qbin _10("_10_", 0b1010);
        Qassign<Qbin> assignment(_10 = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;

        cout << "====================" << endl;
        assignment.reset();
        Qbin _i_(3, "_i_");
        assignment = (_i_ = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;

        cout << "====================" << endl;

        Qwhole _11("_11_", 11);
        Qassign<Qwhole> assignment1 = (_11 = kurt + john);
        cout << assignment1 << endl;
        cout << assignment1.toString(true) << endl;
        cout << assignment1.solve() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }

    cout << "===================" << endl;
    try
    {
        Qint curtis(3, "curtis"), jimmy(3, "jimmy");

        Qexpr<Qint> expression = curtis + jimmy;

        cout << curtis << endl;
        cout << jimmy << endl;

        cout << expression << endl;
        cout << expression.toString(true) << endl;
        cout << expression.solve() << endl;

        cout << "====================" << endl;

        Qint _n8(4, "_n8_", -8);
        Qassign<Qint> assignment = (_n8 = expression);
        cout << assignment << endl;
        cout << assignment.toString(true) << endl;
        cout << assignment.solve() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }*/
    //    pymain();

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
