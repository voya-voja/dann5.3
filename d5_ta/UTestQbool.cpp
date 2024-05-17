//
//  UTestQbool.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-23.
//
#include "UTestQbool.hpp"

#include <Qbool.h>
#include <Qvalue.h>
#include <Qubo.h>
#include <QuboCompiler.h>
#include <QuboAnalyzer.h>
#include <Qbinder.h>

using namespace dann5test;

using namespace dann5;
using namespace dann5::ocean;

void UTestQbool::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Logical Expressions ----" << endl;
    logical(out);
    out << endl << " --- Comparison Expressions ----" << endl;
    comparison(out);
    out << endl << " --- Assignments ----" << endl;
    assignment(out);
    out << endl << " --- Friends & enemies problem ----" << endl;
    friends_enemies(out);
    out << endl << " --- Vertex problem ----" << endl;
    vertex(out);
}

void UTestQbool::initialization(ostream& out)
{
    // Default constructor initializes Q bool with an empty id and
    // superposition value, plus initialize Q bool with a given id and
    // superposition value
    Qbool dcQbool, idQbool("id");
    out << "Qbool default constructor: " << dcQbool.toString()
        << ", creates an object with empty id, i.e. '',"
        << " which is set to superposition state, i.e. S" << endl
        << "Qbool with an id in quantum space as 'id' will be represented as " << idQbool.toString()
        << " is set to superposition state, too." << endl;
    

    // Initialize Q bool with a given id and value.
    // If value is different from 0 or 1, Qbool into superposition state
    Qbool aIsT("a", Qbool::cTrue), bIsS("b", 5), fIsF("f", false), tIs1("t", 1);
    out << "Qbool with an id 'a' is set to value 'Qbool::cTrue':" << endl;
    out << " toString() => " << aIsT.toString() << endl
        << " toString(false, 0) => " << aIsT.toString(false, 0) << endl
        << " toString(true) => " << aIsT.toString(true) << endl
        << " toString(true, 0) => " << aIsT.toString(true, 0) << endl;
    out << endl << "'f' was set using false, and it is "
        << fIsF.toString() << endl;
    out << endl << "'t' was set using 1, and it is " << tIs1.toString() << endl;
    out << endl << "'b' was set to 5, but it is " << bIsS.toString() << endl;

    Qbool cQbool(aIsT);
    out << "A copy of 'a' has id '" << cQbool.id() << "', value "
        << to_string(cQbool.value()) << " and representation " << cQbool << endl;
    cQbool.value(4);
    out << "After the copy of 'a' is set to 4 => " << cQbool.toString() << endl;
    
    cQbool.value(Qbool::cFalse);
    out << "After the copy of 'a' is set to 'Qbool::cFalse' => "
        << cQbool.toString() << endl;
    
    cQbool.value(cSuperposition);
    out << "After the copy of 'a' is set to S => " << cQbool.toString() << endl;
}

void UTestQbool::logical(ostream& out)
{
    Qbool x("x"), y("y", 5), _F("_F", false), _T("_1", 1);
    Qexpr<Qbool> xI = ~x;

    Qubo q;
    out << q;
    QuboCompiler compiler; xI.compile(compiler);
    out << "Expression '!x', NOT (invert) x is: " << xI << endl
        << " Dann5 virtual code: " << xI.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << xI.solve() << endl;
    Qexpr<Qbool> qbExpr = x & y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression AND: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x | y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression OR: " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQbool::comparison(ostream& out)
{
    Qbool x("x"), y("y", 5), z("z"), _F("_F", false), _T("_1", 1);

    Qexpr<Qbool> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
                    xXnYeZ1= (x != y) == z, xXnYeZ2 = x != (y == z);

    QuboCompiler compiler; xEq.compile(compiler);
    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEq.solve() << endl;
    compiler.reset(); xNe.compile(compiler);
    out << xNe.toString() << " decomposed: " << xNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xNe.solve() << endl;
    compiler.reset(); xXnYeZ0.compile(compiler);
    out << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xXnYeZ0.solve() << endl;
    compiler.reset(); xXnYeZ1.compile(compiler);
    out << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true)
    << endl << "Qubo: " << compiler.qubo() << endl << xXnYeZ1.solve() << endl;
    compiler.reset(); xNe.compile(compiler);
    out << xXnYeZ2.toString() << " decomposed: " << xXnYeZ2.toString(true)
        << endl << "Qubo: " << compiler.qubo() << endl
        << xXnYeZ2.solve() << endl;

    Qexpr<Qbool> qbExpr = x ^ y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
    
    qbExpr = x.alike(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = (_T & (x or y) ^ (_F.alike(y | x)));
    compiler.reset(); qbExpr.compile(compiler);
    QuboCompiler noFnlCmplr(false); qbExpr.compile(noFnlCmplr);
    out << "Complex Expression" << endl << qbExpr << endl
        << " Dann5 virtual code: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl
        << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQbool::assignment(ostream& out)
{
    Qbool r("r"), x("x");
    Qassign<Qbool> qboolAssign = r = x;
    QuboCompiler compiler; qboolAssign.compile(compiler);
    out << "Assignment 'r = x' creats logic => " << qboolAssign << endl
        << " It's finalized (execution ready) Qubo is '"
        << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qboolAssign.solve();
    out << qboolAssign.solutions() << endl;

    qboolAssign = r &= x;
    compiler.reset(); qboolAssign.compile(compiler);
    out << endl << "Assignment 'r &= x' creats logic => " << qboolAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl;
    qboolAssign.solve();
    out << qboolAssign.solutions() << endl;

    qboolAssign = r |= x;
    compiler.reset(); qboolAssign.compile(compiler);
    out << endl << "Assignment 'r |= x' creats logic => " << qboolAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'"
    << endl;
    out << "resulting in solutions:"<< endl << qboolAssign.solve() << endl;

    qboolAssign = r ^= x;
    compiler.reset(); qboolAssign.compile(compiler);
    out << endl << "Assignment 'r ^= x' creats logic => " << qboolAssign << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:"<< endl << qboolAssign.solve() << endl;
    
    Qbool b0("b0"), b1("b1", 'F'), b2("b2", 33), b3("b3"), b4("b4"),
          br("br", Qbool::cTrue);
    qboolAssign = br = ((b3 != b4) & (b2 == b0)) | b1;
    compiler.reset(); qboolAssign.compile(compiler);
    QuboCompiler noFnlCmplr(false); qboolAssign.compile(noFnlCmplr);
    out << endl << "An assignment: " << qboolAssign << endl << " has logic => "
        << qboolAssign.toString(true) << endl;
    out << endl << "*** Generic Qubo ***" << endl << noFnlCmplr.qubo()
        << endl << "*** Finalized Qubo ***" << endl << compiler.qubo() << endl;
    QuboAnalyzer analyze(compiler.qubo());
    out << endl << "# of nodes: " << analyze.nodesNo() << "\t# of branches: "
        << analyze.branchesNo() << endl;
    out << endl << "Solutions: " << endl << qboolAssign.solve();
}

void UTestQbool::friends_enemies(ostream& out)
{
    Qbool x("xaviar"), y("yolanda"), z("zeke"), w("wanda"),
          _T("_T", Qbool::cTrue);
    Qassign<Qbool> abFnEs = _T = x.alike(y) & (y.unlike(z) & z.alike(w));
    out << endl << "*** Dwave's friends & enemies problem equation:" << endl
        << abFnEs.toString() << endl;
    Qbinder fNe;
    fNe = x, y, z, w;
    fNe.add(abFnEs.compute());
    out << endl << "*** Dwave's friends & enemies problem solutions:" << endl
        << fNe << endl;
}

void UTestQbool::vertex(ostream& out)
{
    const size_t noVertices = 6;
    Qbool _T("_T", Qbool::cTrue), _F("_F", Qbool::cFalse), vertices[noVertices];
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
    Qexpr<Qbool> xbSV = ((vertices[0] ^ vertices[1])
        & (vertices[0] ^ vertices[3])
        & (vertices[2] ^ vertices[1]));
    Qassign<Qbool> axbSV = _T = xbSV;
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
    Qexpr<Qbool> xbTV = (((vertices[2] ^ vertices[3]) | (vertices[3] ^ vertices[4]))
                        & vertices[2].alike(vertices[3]).alike(vertices[4]));
    Qassign<Qbool> axbTV = _T = xbTV;
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
    Qexpr<Qbool> triangleXpr = (((vertices[2] ^ vertices[3]) | (vertices[3] ^ vertices[4]))
                            & vertices[2].alike(vertices[3]).alike(vertices[4]));
    Qexpr<Qbool> sqareXpr = ((vertices[0] ^ vertices[1])
                            & ((vertices[0] ^ vertices[3])
                                | (vertices[2] ^ vertices[1])));
    Qassign<Qbool> problemAssignment = _T = sqareXpr & triangleXpr;
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

    // pentagon
    out << "==========================================================" << endl;
    out << "a --- d" << endl
        << "|      \\" << endl
        << "|       e" << endl
        << "|      /" << endl
        << "b --- c" << endl;
    /*Qexpr<Qbool> pentagonXpr = ((vertices[0] ^ vertices[1])
                                & ((vertices[0] ^ vertices[3]) | (vertices[2] ^ vertices[1])))
                                & (((vertices[2] ^ vertices[4]) | (vertices[3] ^ vertices[4]))
                                    & vertices[2].alike(vertices[3]).alike(vertices[4]));*/
    Qexpr<Qbool> pentagonXpr = (((vertices[0] ^ vertices[1])
                                & ((vertices[0] ^ vertices[3]) | (vertices[2] ^ vertices[1])))
                                    & vertices[2].alike(vertices[3]).alike(vertices[4]));
    Qassign<Qbool> axbPV = _T = pentagonXpr;
    compiler.reset(); axbPV.compile(compiler);
    out << endl << "---- Pentagon a-b-c-d-e equation:" << endl << axbPV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl;
    QuboAnalyzer anlyzeP(compiler.qubo());
    out << endl << "---- Pentagon a-b-c-d-e equation vertex problem with ";
    out << anlyze.nodesNo() << " nodes and " << anlyze.branchesNo() 
        << " branches " << " solutions:" << endl;
    verticesBinder.reset(); verticesBinder.add(axbPV.compute());
    out << verticesBinder << endl;


    // hexagon
    out << "==========================================================" << endl;
    out << "  d -- e" << endl
        << " /      \\" << endl
        << "a        f" << endl
        << " \\      / " << endl
        << "  b -- c " << endl;

    Qexpr<Qbool> hexagonXpr = (vertices[0] ^ vertices[1]) & (vertices[1] ^ vertices[2]) 
                                & (vertices[2] ^ vertices[5]) & (vertices[5] ^ vertices[4]) 
                                & (vertices[4] ^ vertices[3]);
    Qassign<Qbool> axbHV = _T = hexagonXpr;
    compiler.reset(); axbHV.compile(compiler);
    out << endl << "---- Hexagon a-b-c-d-e-f equation:" << endl << axbHV.toString()
        << endl << "     QUBO: " << compiler.qubo() << endl;
    QuboAnalyzer anlyzeH(compiler.qubo());
    out << endl << "---- Hexagon a-b-c-d-e-f equation vertex problem with ";
    out << anlyze.nodesNo() << " nodes and " << anlyze.branchesNo()
        << " branches " << " solutions:" << endl;
    verticesBinder.reset(); verticesBinder.add(axbHV.compute());
    out << endl << verticesBinder << endl;
}
