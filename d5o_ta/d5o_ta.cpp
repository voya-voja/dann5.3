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

#include <Circuits.h>
#include <CircuitCompiler.h>

using namespace std;
using namespace dann5::ocean;
using namespace dann5::qiskit;
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

void prime_s2t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _2("2_", 2);
    Qblock primeNo;
    {
        Qbin _3("3_", 3), _7("7_", 7);
        Qassign<Qwhole> p_s2_2t2 = prime = s * s + _2 * t * t;
        Qassign<Qbin> p3mod8 = _3 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_2t2, p3mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 2t2, p mod 8 = 3, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
}

void prime_s4t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _4("4_", 4);
    Qblock primeNo;
    {
        Qbin _5("5_", 5), _7("7_", 7);
        Qassign<Qwhole> p_s2_4t2 = prime = s * s + _4 * t * t;
        Qassign<Qbin> p5mod8 = _5 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_4t2, p5mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 4t2, p mod 8 = 5, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
}

void prime_s8t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _8("8_", 8);
    Qblock primeNo;
    {
        Qbin _7("7_", 7);
        Qassign<Qwhole> p_s2_8t2 = prime = s * s + _8 * t * t;
        Qassign<Qbin> p1mod8 = Qbin::_1 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_8t2, p1mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    /*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
            << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    */    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 8t2, p mod 8 = 1, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
/*    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
*/
}

void prime_s16t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _16("16_", 16);
    Qblock primeNo;
    {
        Qbin _7("7_", 7);
        Qassign<Qwhole> p_s2_16t2 = prime = s * s + _16 * t * t;
        Qassign<Qbin> p1mod8 = Qbin::_1 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_16t2, p1mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    /*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
            << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    */    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 16t2, p mod 8 = 1, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
/*    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
*/
}

void prime_s4st2t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _2("2_", 2), _4("4_", 4);
    Qblock primeNo;
    {
        Qbin _7("7_", 7);
        Qassign<Qwhole> p_s2_4st_2t2 = prime = s * s + _4 * s * t + _2 * t * t;
        Qassign<Qbin> p7mod8 = _7 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_4st_2t2, p7mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    /*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
            << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    */    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 4st + 2t2, p mod 8 = 7, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
/*    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
*/
}

void prime_s8st8t(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _8("8_", 8);
    Qblock primeNo;
    {
        Qbin _7("7_", 7);
        Qassign<Qwhole> p_s2_8st_8t2 = prime = s * s + _8 * s * t + _8 * t * t;
        Qassign<Qbin> p1mod8 = Qbin::_1 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_8st_8t2, p1mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    /*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
            << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    */    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 8st + 8t2, p mod 8 = 1, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
/*    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
*/
}

void prime6m(ostream& out)
{
    Qwhole prime(4, "p"), s(2, "s"), _6("6_", 6);
    Qblock prime6m1;
    {
        prime6m1 = prime = _6 * s - Qwhole::_1;
    }
    QuboCompiler noFnlCmplr(false); prime6m1.compile(noFnlCmplr);
    QuboCompiler compiler; prime6m1.compile(compiler);
    out << "Prime Number\n Code" << endl << prime6m1 << endl
        << " Logic: " << prime6m1.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyse(compiler.qubo());
    out << endl << "*** Prime number p = 6s - 1 has: " << analyse.nodesNo()
        << " # of node and # of branches: " << analyse.branchesNo() << endl;
/*    Qevaluations solution = prime6m1.compute();
    Qbinder pst(solution);
    pst = prime, s;
    out << " resulting in :" << endl << pst << endl;
*/
    Qwhole r1(5, "r1"), r2(5, "r2");
    Qblock prime6m1X;
    {
        prime6m1X = r1 = prime + Qwhole::_1, r2= _6 * s, r1 == r2 ;
    }
    noFnlCmplr.reset(); prime6m1X.compile(noFnlCmplr);
    compiler.reset(); prime6m1X.compile(compiler);
    out << "Prime Number\n Code" << endl << prime6m1X << endl
        << " Logic: " << prime6m1X.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyseBx(compiler.qubo());
    out << endl << "*** Prime number p = 6s - 1 has: " << analyseBx.nodesNo()
        << " # of node and # of branches: " << analyseBx.branchesNo() << endl;
/*    Qevaluations solution = prime6m1.compute();
    Qbinder pst(solution);
    pst = prime, s;
    out << " resulting in :" << endl << pst << endl;
*/
    Qexpr<Qwhole> prmXpr(_6 * s == prime + Qwhole::_1);
    noFnlCmplr.reset(); prmXpr.compile(noFnlCmplr);
    compiler.reset(); prmXpr.compile(compiler);
    out << "Prime Number\n Code" << endl << prmXpr << endl
        << " Logic: " << prmXpr.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyseX(compiler.qubo());
    out << endl << "*** Prime number p = 6s - 1 has: " << analyseX.nodesNo()
        << " # of node and # of branches: " << analyseX.branchesNo() << endl;
    Qevaluations solutionX = prmXpr.compute();
    Qbinder pstX(solutionX);
    pstX = prime, s;
    out << " resulting in :" << endl << pstX << endl;
}


void prime6p(ostream& out)
{
    Qwhole prime(4, "p"), s(2, "s"), _6("6_", 6);
    Qblock prime6p1;
    {
        prime6p1 = prime = _6 * s + Qwhole::_1;
    }
    QuboCompiler noFnlCmplr(false); prime6p1.compile(noFnlCmplr);
    QuboCompiler compiler; prime6p1.compile(compiler);
    out << "Prime Number\n Code" << endl << prime6p1 << endl
        << " Logic: " << prime6p1.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyse2(compiler.qubo());
    out << endl << "*** Prime number p = 6s + 1 has: "
        << analyse2.nodesNo() << " # of node and # of branches: " << analyse2.branchesNo() << endl;
/*    Qevaluations solution2 = prime6p1.compute();
    Qbinder pst2(solution2);
    pst2 = prime, s;
    out << " resulting in :" << endl << pst2 << endl;
*/
    Qassign<Qwhole> prmXpr(prime = _6 * s + Qwhole::_1);
    noFnlCmplr.reset(); prmXpr.compile(noFnlCmplr);
    compiler.reset(); prmXpr.compile(compiler);
    out << "Prime Number\n Code" << endl << prmXpr << endl
        << " Logic: " << prmXpr.toString(true) << endl;
/*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
*/    QuboAnalyzer analyseX(compiler.qubo());
    out << endl << "*** Prime number p = 6s + 1 has: " << analyseX.nodesNo()
        << " # of node and # of branches: " << analyseX.branchesNo() << endl;
    Qevaluations solutionX = prmXpr.compute();
    Qbinder pstX(solutionX);
    pstX = prime, s;
    out << " resulting in :" << endl << pstX << endl;
}



void prime_s2t_prime(ostream& out)
{
    Qwhole prime(5, "p"), s(1, "s"), t(2, "t"), _2("2_", 2);
    Qblock primeNo;
    {
        Qbin _3("3_", 3), _7("7_", 7);
        Qassign<Qwhole> p_s2_2t2 = prime = s * s + _2 * t * t;
        Qassign<Qbin> p3mod8 = _3 = prime & _7;
        Qassign<Qbin> s1mod2 = Qbin::_1 = s & Qwhole::_1;
        Qassign<Qbin> t1mod2 = Qbin::_1 = t & Qwhole::_1;
        Qblock gcd1;
        {
            Qwhole d(1, "d");
            gcd1 = s = s * d,
                t = t * d,
                d == Qwhole::_1;
        }
        primeNo = p_s2_2t2, p3mod8, s1mod2, t1mod2, gcd1;
    }
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    /*    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
            << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    */    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 2t2, p mod 8 = 3, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
}

void testQiskit()
{
    Qbit x("x"), y("y"), z("z", 1);
    Qexpr<Qbit> expr((x & y) ^ z);
    CircuitCompiler compiler;
    expr.compile(compiler);
    Circuit& circuit = compiler.circuit();
    cout << circuit.instructions();

    cout << endl << expr.solve() << endl;
}

int main(int argc, const char * argv[])
{
    Qsolver::Active(D5QuboSolver::Sp(new D5QuboSolver()));

    testQiskit();
/*
    prime_s2t(cout);
    prime_s4t(cout);
    prime_s8t(cout);
    prime_s16t(cout);
    prime_s4st2t(cout);
    prime_s8st8t(cout);
    prime6m(cout);
    prime6p(cout);
    prime_s2t_prime(cout);

    UTestQbit utQbit;
    utQbit.runAll(cout);
    UTestQbool utQbool;
    utQbool.runAll(cout);
    UTestQbin utQbin;
    utQbin.runAll(cout);
    UTestQwhole utQwhole;
    utQwhole.runAll(cout);
    utQwhole.prime(cout);
    utQwhole.prime6(cout);
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
