//
//  UTestQbit.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-13.
//
#include "UTestQbit.h"

#include <Qbit.h>
#include <Qbinder.h>
#include <Qubo.h>
#include <QuboCompiler.h>
#include <QuboAnalyzer.h>

using namespace dann5test;

using namespace dann5;
using namespace dann5::ocean;

void UTestQbit::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Bitwise Expressions ----" << endl;
    bitwise(out);
    out << endl << " --- Comparison Expressions ----" << endl;
    comparison(out);
    out << endl << " --- Assignments ----" << endl;
    assignment(out);
    out << endl << " --- Equal Expressions vs. Assignments ----" << endl;
    eq_asign(out);
    out << endl << " --- Friends & enemies problem ----" << endl;
    friends_enemies(out);
    out << endl << " --- Vertex problem ----" << endl;
    vertex(out);
}

void UTestQbit::initialization(ostream& out)
{
    // Default constructor initializes Q bit with an empty id and
    // superposition value, plus initialize Q bit with a given id
    // and superposition value
    Qbit dcQbit, idQbit("id");
    out << "Qbit default constructor: " << dcQbit.toString()
        << ", creates an object with empty id, i.e. '',"
        << " which is set to superposition state, i.e. S" << endl
        << "Qbit with an id in quantum space as 'id' will be represented as " << idQbit.toString()
        << " is set to superposition state, too." << endl;
    
    // Initialize Q bit with a given id and value.
    // If value is different from 0 or 1, Qbit into superposition state
    Qbit aIs1("a", 1), bIsS("b", 5);
    out << "For a Qbit with an id 'a', which is set to value 1:" << endl;
    out << " toString() => " << aIs1.toString() << endl
        << " toString(false, 0) => " << aIs1.toString(false, 0) << endl
        << " toString(true) => " << aIs1.toString(true) << endl
        << " toString(true, 0) => " << aIs1.toString(true, 0) << endl;
    out << endl << "'b' was set to 5, but it is " << bIsS.toString()
        << ", because Qbit can have only 0 and 1 as discrete values." << endl;

    Qbit cQbit(aIs1);
    out << "A copy of 'a' has id '" << cQbit.id() << "' and value "
        << to_string(cQbit.value()) << endl;
    cQbit.value(4);
    out << "After the copy of 'a' is set to 4 => " << cQbit.toString() << endl;
    
    cQbit.value(0);
    out << "After the copy of 'a' is set to 0 => " << cQbit.toString() << endl;
    
    cQbit.value(cSuperposition);
    out << "After the copy of 'a' is set to S => " << cQbit.toString() << endl;
}

void UTestQbit::assignment(ostream& out)
{
    Qbit r("r"), x("x");
    Qassign<Qbit> qbitAssign = r = x;
    QuboCompiler compiler; qbitAssign.compile(compiler);
    out << "Assignment 'r = x' creats logic => " << qbitAssign << endl
        << " It's finalized (execution ready) Qubo is '" << compiler.qubo()
        << "'" << endl;
    qbitAssign.solve();
    out << "resulting in solutions:"<< endl << qbitAssign.solutions() << endl;

    qbitAssign = r &= x;
    compiler.reset(); qbitAssign.compile(compiler);
    out << endl << "Assignment 'r &= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    qbitAssign.solve();
    out << "resulting in solutions:"<< endl << qbitAssign.solutions() << endl;

    qbitAssign = r |= x;
    compiler.reset(); qbitAssign.compile(compiler);
    out << endl << "Assignment 'r |= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl << qbitAssign.solve();

    qbitAssign = r ^= x;
    compiler.reset(); qbitAssign.compile(compiler);
    out << endl << "Assignment 'r ^= x' creats logic => " << qbitAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl << qbitAssign.solve();
    
    out << "The order of operations is important in following 4 assignments:"
        << endl;
    Qbit a("a"), b("b", 1), y("y", 5), z("z"), _0("_0", 0), _1("_1", 1);
    Qassign<Qbit> qbitAssign1 = _1 = (z != (b & x)) | (z != (y ^ _0));
    qbitAssign1.solve();
    QuboCompiler noFnlCmplr(false); qbitAssign1.compile(noFnlCmplr);
    out << "Assignment 1" << endl << qbitAssign1 << endl
        << " Dann5 virtual code: " << qbitAssign1.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    compiler.reset(); qbitAssign1.compile(compiler);
    out << " & finalized Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbitAssign1.solutions() << endl;

    Qassign<Qbit> qbitAssign2 = _1 = ((b & x) != z) | (z != (y ^ _0));
    qbitAssign2.solve();
    out << "Assignment 2: != (not-equal) is not commutative: " << endl << qbitAssign2 << endl
        << " Dann5 virtual code: " << qbitAssign2.toString(true) << endl
        << " resulting in :" << endl << qbitAssign2.solutions() << endl
        << "2 resalts only , because '_&4' and 'z' have to be different";
    
    Qassign<Qbit> qbitAssign3 = _1 = ((b & x) != z) | ((y ^ _0) != z);
    qbitAssign3.solve();
    out << "Assignment 3 different than, Assignment 1 and 2: " << endl << qbitAssign3 << endl
        << " Dann5 virtual code: " << qbitAssign3.toString(true) << endl
        << " resulting in :" << endl << qbitAssign3.solutions() << endl
        << "1 resalt only , as '_&5' and '_^5' have to be different from 'z'";

    Qassign<Qbit> qbitAssign4 = _1 = (z != (b & x)) | ((y ^ _0) != z);
    qbitAssign4.solve();
    out << "Assignment 4 has same solutions as Assignmnet 2" << endl << qbitAssign4 << endl
        << " Dann5 virtual code: " << qbitAssign4.toString(true) << endl
        << " resulting in :" << endl << qbitAssign4.solutions() << endl;

    Qassign<Qbit> qbitA4 = _1 = (z.unlike(b & x) | z.unlike(y ^ _0));
    out << "Assignment 4 using unlike operation: " << endl << qbitA4 << endl
        << " decomposed: " << qbitA4.toString(true) << endl;
    compiler.reset(); qbitA4.compile(compiler);
    QuboAnalyzer anlyze4(compiler.qubo());
    out << endl << " # of nodes: "
        << anlyze4.nodesNo() << " # of branches: " << anlyze4.branchesNo()
        << endl << qbitA4.solve();
    Qassign<Qbit> qbitA3 = _1 = ((b & x).unlike(z)) | ((y ^ _0).unlike(z));
    out << "Assignment 3 using unlike operation: " << endl << qbitA3 << endl
        << " decomposed: " << qbitA3.toString(true) << endl;
    compiler.reset(); qbitA3.compile(compiler);
    QuboAnalyzer anlyze3(compiler.qubo());
    out << endl << " # of nodes: "
        << anlyze3.nodesNo() << " # of branches: " << anlyze3.branchesNo()
        << endl << qbitA3.solve();
}

void UTestQbit::bitwise(ostream& out)
{
    Qbit x("x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    
    Qexpr<Qbit> xI = ~x;
    QuboCompiler compiler; xI.compile(compiler);
    out << "Expression '~x', NOT (invert) x is: " << xI << endl
        << " Dann5 virtual code: " << xI.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << xI.solve() << endl;
    Qexpr<Qbit> qbExpr = x & y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression AND: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x.nand(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NAND: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x | y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression OR: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x.nor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NOR: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x ^ y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = x.alike(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    qbExpr = (_1 & (x.nand(y))) ^ (_0.alike(y | x));
    compiler.reset(); qbExpr.compile(compiler);
    out << "Complex Expression" << endl << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "''" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQbit::comparison(ostream& out)
{
    Qbit a("a"), r("r"), b("b"), x("x"), y("y"), z("z"),
        _0("_0", 0), _1("_1", 1);

    Qexpr<Qbit> xGtNe = z > (x != y), xGeNe = z >= (x != y),
                xLtNe = z < (x != y), xLeNe = z <= (x != y);

    Qexpr<Qbit> xNeGt = (x != y) > z, xNeGe = (x != y) >= z,
                xNeLt = (x != y) < z, xNeLe = (x != y) <= z;

    QuboCompiler compiler; xGtNe.compile(compiler);
    out << xGtNe.toString() << " decomposed: " << xGtNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xGtNe.solve() << endl;
    compiler.reset(); xGeNe.compile(compiler);
    out << xGeNe.toString() << " decomposed: " << xGeNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xGeNe.solve() << endl;
    compiler.reset(); xLtNe.compile(compiler);
    out << xLtNe.toString() << " decomposed: " << xLtNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xLtNe.solve() << endl;
    compiler.reset(); xLeNe.compile(compiler);
    out << xLeNe.toString() << " decomposed: " << xLeNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xLeNe.solve() << endl;
    compiler.reset(); xNeGt.compile(compiler);
    out << xNeGt.toString() << " decomposed: " << xNeGt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xNeGt.solve() << endl;
    compiler.reset(); xNeGe.compile(compiler);
    out << xNeGe.toString() << " decomposed: " << xNeGe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xNeGe.solve() << endl;
    compiler.reset(); xNeLt.compile(compiler);
    out << xNeLt.toString() << " decomposed: " << xNeLt.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xNeLt.solve() << endl;
    compiler.reset(); xNeLe.compile(compiler);
    out << xNeLe.toString() << " decomposed: " << xNeLe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl
        << xNeLe.solve() << endl;
}

void UTestQbit::eq_asign(ostream& out)
{
    Qbit a("a"), r("r"), b("b"), x("x"), y("y"), z("z"),
        _0("_0", 0), _1("_1", 1);

    Qexpr<Qbit> xEq = x == y, xAl = x.alike(y), xNe = x != y, xUl = x ^ y;

    Qassign<Qbit> aR = r = a, axEq = r = x == y, axAl = r = x.alike(y),
                    axNe = r = x != y, axUl = _1 = x ^ y;

    QuboCompiler compiler; aR.compile(compiler);
    out << "Assignment 'r = a' is " << aR << "\n qubo: " << compiler.qubo()
        << endl << aR.solve() << endl;
    compiler.reset(); xEq.compile(compiler);
    out << "Equal Expression 'x == y' is " << xEq << "\n qubo: " << compiler.qubo()
        << endl << xEq.solve() << endl;
    compiler.reset(); axEq.compile(compiler);
    out << "Equal Assignment 'r = x == y' is " << axEq << "\n qubo: " << compiler.qubo()
        << endl << axEq.solve() << endl;
    compiler.reset(); xAl.compile(compiler);
    out << "Alike Expression 'x.alike(y)' is " << xAl << "\n qubo: " << compiler.qubo()
        << endl << xAl.solve() << endl;
    compiler.reset(); axAl.compile(compiler);
    out << "Alike Assignment 'r = x.alike(y)' is " << axAl << "\n qubo: " << compiler.qubo()
        << endl << axAl.solve() << endl;
    compiler.reset(); xNe.compile(compiler);
    out << "Not Equal Expression 'x != y' is " << xNe << "\n qubo: " << compiler.qubo()
        << endl << xNe.solve() << endl;
    compiler.reset(); axNe.compile(compiler);
    out << "Not Equal Assignment 'r = x != y' is " << axNe << "\n qubo: " << compiler.qubo()
        << endl << axNe.solve() << endl;
    compiler.reset(); xUl.compile(compiler);
    out << "Unlike Expression 'x ^ y' is " << xUl << "\n qubo: " << compiler.qubo()
        << endl << xUl.solve() << endl;
    compiler.reset(); axUl.compile(compiler);
    out << "Unlike Assignment '_1 = x ^ y' is " << axUl << "\n qubo: " << compiler.qubo()
        << endl << axUl.solve() << endl;
}

void UTestQbit::friends_enemies(ostream& out)
{
    Qbit x("xaviar"), y("yolanda"), z("zeke"), w("wanda");
    Qassign<Qbit> abFnEs = Qbit::_1 = (x.alike(y) & (y ^ z) & z.alike(w));
    out << endl << "*** Dwave's friends & enemies problem equation:" << endl
        << abFnEs.toString() << endl << abFnEs.toString(true) << endl;
    Qbinder fNe;
    fNe = x, y, z, w;
    fNe.add(abFnEs.compute());
    out << endl << "*** Dwave's friends & enemies problem solutions:" << endl
        << fNe << endl;
}

void UTestQbit::vertex(ostream& out)
{
    const size_t noVertices = 5;
    Qbit _1("_1", 1), _0("_0", 0), vertices[noVertices];
    Qbinder verticesBinder;
    for(int at = 0; at < noVertices; at++)
    {
        vertices[at].id(string(1, char('a' + at)));
        verticesBinder << vertices[at];
    }

    // sqare
    out << "==========================================================" << endl;
    out << "a --- d" << endl
        << "|     |" << endl
        << "|     |" << endl
        << "|     |" << endl
        << "b --- c" << endl;
    Qexpr<Qbit> xbSV = ((vertices[0] ^ vertices[1])
                        & (vertices[0] ^ vertices[3])
                        & (vertices[2] ^ vertices[1]));
    Qassign<Qbit> axbSV = _1 = xbSV;
    QuboCompiler compiler; axbSV.compile(compiler);
    out << endl << "---- Square a-b-c-d equation:" << endl << axbSV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl;
    verticesBinder.reset(); verticesBinder.add(axbSV.compute());
    out << "---- Square a-b-c-d solutions:" << endl << verticesBinder << endl;

    // triangle
    out << "==========================================================" << endl;
    out << "d" << endl
        << "|\\" << endl
        << "| e" << endl
        << "|/" << endl
        << "c" << endl;
    Qexpr<Qbit> xbTV = (((vertices[2] ^ vertices[3]) | (vertices[3] ^ vertices[4]))
                        & vertices[2].alike(vertices[3]).alike(vertices[4]));
    Qassign<Qbit> axbTV = _1 = xbTV;
    compiler.reset(); axbTV.compile(compiler);
    out << endl << "---- Triangle c-d-e equation:" << endl << axbTV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl;
    verticesBinder.reset(); verticesBinder.add(axbTV.compute());
    out << "---- Triangle c-d-e solutions:" << endl << verticesBinder << endl;

    // squer & triangle
    out << "==========================================================" << endl;
    out << "a --- d" << endl
        << "|     |\\" << endl
        << "|     | e" << endl
        << "|     |/" << endl
        << "b --- c" << endl;
    Qexpr<Qbit> triangleXpr = (((vertices[2] ^ vertices[3]) | (vertices[3] ^ vertices[4]))
        & vertices[2].alike(vertices[3]).alike(vertices[4]));
    Qexpr<Qbit> sqareXpr = ((vertices[0] ^ vertices[1])
        & ((vertices[0] ^ vertices[3])
            | (vertices[2] ^ vertices[1])));
    Qassign<Qbit> problemAssignment = _1 = sqareXpr & triangleXpr;
    out << endl << "---- Square(a-b-c-d) "
        << "+ triangle (c-d-e) vertex problem equation:"
        << endl << problemAssignment.toString() << endl;
    compiler.reset(); problemAssignment.compile(compiler);
    out << endl << "     QUBO: " << compiler.qubo() << endl;
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << "---- Square(a-b-c-d) "
        << "+ triangle (c-d-e) vertex problem with ";
    out << anlyze.nodesNo() << " nodes and " << anlyze.branchesNo()
        << " branches " << " solutions:" << endl;
    verticesBinder.reset(); verticesBinder.add(problemAssignment.compute());
    out << verticesBinder << endl;
}
