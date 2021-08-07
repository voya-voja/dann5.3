// dllmain.cpp : Defines the entry point for the DLL application.

// pybind11 — Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <Qubo.h>
#include <Qbit.h>
#include <Qbool.h>
#include <Qbin.h>
#include <Qwhole.h>

#include <Qexpr.h>
#include <Qassign.h>
#include <Qsolver.h>

#include <Qstatement.h>

#include <pybind11/stl.h>
#include <pybind11/operators.h>
//#include <pybind11/eigen.h>
//#include <pybind11/embed.h>

using namespace dann5::ocean;

#define VERSION_INFO "2.0.0"

namespace py = pybind11;


PYBIND11_MODULE(d5o, m) {
	m.doc() = R"pbdoc(
        d5o plugin
        -----------------------

        .. currentmodule:: d5o

        .. autosummary::
           :toctree: _generate

			Qubo
			QuboTable
			Qwhole
			Qdef
			Qexpression
			Qvar
			Qequation
    )pbdoc";
/*--- Qubo.h definitions ---*/
	py::class_<Qkey>(m, "Qkey", R"pbdoc(Qubo key corresponds to an element of DWave's dimod.BinaryQuadraticModel class where
		key pair with the same Quantum node name is a linear node and one with different Quantum node names is a quadratic element)pbdoc");

	py::class_<Qubo>(m, "Qubo", R"pbdoc(Qubo model dictionar of a solution graph for a binary objective
		function where a node pair is a Qubo key mapped into its bias, which
		corespondes to Quantum energy values for that node 
		Qubo corresponds to DWave's dimod.BinaryQuadraticModel class, represented 
		as an upper-diagonal matrix Q, where diagonal terms are the linear
		coefficients and the nonzero off-diagonal terms the quadratic coefficients (graph branches)pbdoc");

	// specify C++ class->baseclass specialization
	m.def("Qvalue", []() { return unsigned char(); }, R"pbdoc( Q value is unsigned char. Valid values are {0, 1, S}.)pbdoc");

	py::class_<Qvalues>(m, "Qvalues", R"pbdoc(An array of Quantum values)pbdoc");

	py::class_<QuboTable>(m, "QuboTable", R"pbdoc(A Qubo table abstraction)pbdoc")
		.def("qubo", (Qubo(QuboTable::*)() const) & QuboTable::qubo, "Default Qubo qubo")
		.def("qubo", (Qubo(QuboTable::*)(const QuboTable::IoPorts&, bool) const) & QuboTable::qubo, "Qubo qubo with the input list of arguments");

	// specify C++ class->baseclass specialization
	py::class_<EqQT, QuboTable>(m, "EqQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NeqQT, QuboTable>(m, "NeqQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LtQT, QuboTable>(m, "LtQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LeQT, QuboTable>(m, "LeQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GtQT, QuboTable>(m, "GtQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GeQT, QuboTable>(m, "GeQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<AndQT, QuboTable>(m, "AndQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NandQT, QuboTable>(m, "NandQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<OrQT, QuboTable>(m, "OrQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NorQT, QuboTable>(m, "NorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotLeftOrRightQT, QuboTable>(m, "NotLeftOrRightQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<DwNotLeftOrRightQT, QuboTable>(m, "DwNotLeftOrRightQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NxorQT, QuboTable>(m, "NxorQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<XorQT, QuboTable>(m, "XorQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<Adder05QT, XorQT>(m, "Adder05QT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<AdderQT, QuboTable>(m, "AdderQT")
		.def(py::init<>());

/*--- Qexpr.h definitions ---*/
	py::class_<Qexpression>(m, "Qexpression",
		R"pbdoc( Quantum expression of arithmetic Q operations with Q defined symbols as operands)pbdoc")
		.def("root", static_cast<Qop& (Qexpression::*)()>(&Qexpression::root), "returns a reference to the Q expression root Q operaton")
		.def("rootDef", static_cast<Qdef& (Qexpression::*)()>(&Qexpression::rootDef), "returns a reference to the Q expression root Q definition")

		.def("noqbs", &Qexpression::noqbs, "Returns the number of Q bits that the Q expression holds")

		.def("qubo", &Qexpression::qubo, 
			"Returns a qubo representation of this Q expression, if not finalized, returns a full qubo definition representation if finalized, returns an expression that replaces symbols with values of Q bits in deterministic state")

		.def("toString", &Qexpression::toString, "returns string presentation of this Q expression object")

		.def("solutions", static_cast<void (Qexpression::*)(const Qsolver::Samples&)>(&Qexpression::solutions), 
			"Set a sample set with a node list defined by qubo() of this Q expression the combination of node values should be different for each sample")
		.def("solutions", static_cast<string(Qexpression::*)() const>(&Qexpression::solutions), "For existing samples, returns a string with all solutions of this Q expression")

		.def("solve", static_cast<string(Qexpression::*)()>(&Qexpression::solve), "Solve this Q expression and return a string with all solutions");

/*--- Qassign.h definitions ---*/
	py::class_<Qassignment>(m, "Qassignment",
		R"pbdoc( Quantum assignment of an expression to a result)pbdoc")
		.def("assign", &Qassignment::assign, "Assign a Q expression to Q defintion assignee. Use without inputs to remove (one or boath) Q assignment members")

		.def("assignee", static_cast<const Qdef::Sp& (Qassignment::*)() const>(&Qassignment::assignee), "returns a reference to the Q assignee")
		.def("assignee", static_cast<void (Qassignment::*)(const Qdef::Sp&)>(&Qassignment::assignee), "set a Q definition pointer to a new assignee")

		.def("expression", static_cast<const Qexpression::Sp& (Qassignment::*)() const>(&Qassignment::expression), "returns a reference to the assignment's Q expression")
		.def("expression", static_cast<void (Qassignment::*)(const Qexpression::Sp&)>(&Qassignment::expression), "set assignment's new Q expression")

		.def("noqbs", &Qassignment::noqbs, "Returns the number of Q bits that the Q assignment holds")

		.def("qubo", &Qassignment::qubo,
			"Returns a qubo representation of this Q assignment, if not finalized, returns a full qubo definition representation if finalized, returns an expression that replaces symbols with values of Q bits in deterministic state")

		.def("toString", &Qassignment::toString, "returns string presentation of this Q assignment object")

		.def("solutions", static_cast<void (Qassignment::*)(const Qsolver::Samples&)>(&Qassignment::solutions),
			"Set a sample set with a node list defined by qubo() of this Q assignment the combination of node values should be different for each sample")
		.def("solutions", static_cast<string(Qassignment::*)() const>(&Qassignment::solutions), "For existing samples, returns a string with all solutions of this Q expression")

		.def("solve", static_cast<string(Qassignment::*)()>(&Qassignment::solve), "Solve this Q assignment and return a string with all solutions");
/*--- Qbit.h definitions ---*/
	py::class_<Qbit>(m, "Qbit",
		R"pbdoc( Quantum bit is in superposition state for any value except 0 and 1)pbdoc")

		.def(py::init<>())
		.def(py::init<const Qbit&>())
		.def(py::init<const string&>())
		.def(py::init<const string&, Qvalue>())

		.def("noqbs", &Qbit::noqbs, "always returns 1")
		.def("value", static_cast<void(Qbit::*)(Qvalue)>(&Qbit::value), "sets Qvalue")
		.def("value", static_cast<Qvalue(Qbit::*)()>(&Qbit::value), "get Qvalue")
		.def("value", static_cast<Qvalue(Qbit::*)() const>(&Qbit::value), "const get Qvalue")

		.def("toString", &Qbit::toString, "returns string presentation of this Q bit object")
		.def("solutions", &Qbit::solutions, "returns solutions for this object")
		.def("solution", &Qbit::solution, "returns a solution for this object identified by id")

	/*** Assignments ***/
		.def("assign", static_cast<Qbit& (Qbit::*)(const Qbit&)>(&Qbit::operator=), "assigns a value of right Q bit to this Q bit")
		.def("assign", static_cast<Qassign<Qbit>(Qbit::*)(const Qexpr<Qbit>&)>(&Qbit::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")


	/*** Compound Assignments ***/
		.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bit")
		.def(py::self &= Qexpr<Qbit>(), "'and assignment' of an Q expression creates a following Q bit assignment [this] = [this] & [right]")

		.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bit")
		.def(py::self |= Qexpr<Qbit>(), "'or assignment' of an Q expression creates a following Q bit assignment [this] = [this] | [right]")

		.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bit")
		.def(py::self ^= Qexpr<Qbit>(), "'xor assignment' of an Q expression creates a following Q bit assignment [this] = [this] ^ [right]")
		
	/*** Bitwise ***/
		.def(~py::self, "instantiate Q bit expression with inversion logic, e.g. for Qbit with id 'x' the expression is '~x' != 'x'")

		.def(py::self & py::self, "instantiate Q expression with and logic, e.g. for Qbit ids 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self & Qexpr<Qbit>(), "instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::nand), "instantiate Q expression with nand logic, e.g. for Qbit ids 'x' and 'y' the expression is ~('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::nand), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is ~('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbit ids 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qexpr<Qbit>(), "instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::nor), "instantiate Q expression with nor logic, e.g. for Qbit ids 'x' and 'y' the expression is ~('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::nor), "instantiate Q expression with nor logic, e.g. for Qbit id 'x' and [right] object the expression is ~('x' | [right])")

		
		.def(py::self ^ py::self, "instantiate Q expression with xor logic, e.g. for Qbit ids 'x' and 'y' the expression is 'x' ^ 'y'")
		.def(py::self ^ Qexpr<Qbit>(), "instantiate Q expression with xor logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' ^ [right]")

	/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' > 'y'")
		.def(py::self > Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' >= 'y'")
		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self< py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' < 'y'")
		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qbit ids 'x' and 'y' the expression is 'x' <= 'y'")
		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]");

/*--- Qbool.h definitions ---*/
		py::class_<Qbool>(m, "Qbool",
			R"pbdoc( Quantum boolean is in superposition state for any value except 'F' and 'T')pbdoc")

			.def(py::init<const Qbool&>())
			.def(py::init<const string&>())
			.def(py::init<const string&, Qvalue>())
			.def("noqbs", &Qbool::noqbs, "always returns 1")
			.def("value", static_cast<void(Qbool::*)(Qvalue)>(&Qbool::value), "sets Qvalue")
			.def("value", static_cast<Qvalue(Qbool::*)()>(&Qbool::value), "get Qvalue")
			.def("value", static_cast<Qvalue(Qbool::*)() const>(&Qbool::value), "const get Qvalue")

			.def("toString", &Qbool::toString, "returns string presentation of this Q bit object")
			.def("solutions", &Qbool::solutions, "returns solutions for this object")
			.def("solution", &Qbool::solution, "returns a solution for this object identified by id")

			/*** Assignments ***/
			.def("assign", static_cast<Qbool& (Qbool::*)(const Qbool&)>(&Qbool::operator=), "assigns a value of right Q bit to this Q bit")
			.def("assign", static_cast<Qassign<Qbool>(Qbool::*)(const Qexpr<Qbool>&)>(&Qbool::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")


			/*** Compound Assignments ***/
			.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bit")
			.def(py::self &= Qexpr<Qbool>(), "'and assignment' of an Q expression creates a following Q bit assignment [this] = [this] & [right]")

			.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bit")
			.def(py::self |= Qexpr<Qbool>(), "'or assignment' of an Q expression creates a following Q bit assignment [this] = [this] | [right]")

			.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bit")
			//		.def(py::self ^= Qexpr<Qbool>(), "'xor assignment' of an Q expression creates a following Q bit assignment [this] = [this] ^ [right]")

					/*** Logical ***/
			.def(!py::self, "instantiate Q boolean expression with inversion logic, e.g. for Qbool with id 'x' the expression is '!x' != 'x'")

			.def(py::self& py::self, "instantiate Q expression with and logic, e.g. for Qbool ids 'x' and 'y' the expression is 'x' && 'y'")
			.def(py::self& Qexpr<Qbool>(), "instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' && [right]")

			.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbool ids 'x' and 'y' the expression is 'x' || 'y'")
			.def(py::self | Qexpr<Qbool>(), "instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' || [right]")


			/*** Comparison ***/
			.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbool ids 'x' and 'y' the expression is 'x' == 'y'")
			.def(py::self == Qexpr<Qbool>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

			.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbool ids 'x' and 'y' the expression is 'x' != 'y'")
			.def(py::self != Qexpr<Qbool>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");

/*--- Qbin.h definitions ---*/
	py::class_<Qbin>(m, "Qbin",
		R"pbdoc( Quantum binary is in unknown state, if any of its quantum bits is in superposition state)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qbin&>())
		.def(py::init<const string&>())
		.def(py::init<size_t, const string&>())
		.def(py::init<const string&, const Qbits&>())
		.def(py::init<const string&, const Bits&>())
		.def(py::init<const string&, const Bits&, bool>())

		.def("resize", &Qbin::resize)
		.def("noqbs", &Qbin::noqbs, "always returns 1")

		.def("value", static_cast<Bits(Qbin::*)()>(&Qbin::operator Bits), "get Bits value")
		.def("value", static_cast<const Bits(Qbin::*)() const>(&Qbin::operator const Bits), "get Bits value")

		.def("toString", &Qbin::toString, "returns string presentation of this Q bin object")
		.def("solutions", &Qbin::solutions, "returns solutions for this object")
		.def("solution", &Qbin::solution, "returns a solution for this object identified by id")

		/*** Assignments ***/
		.def("assign", static_cast<Qbin& (Qbin::*)(const Qbin&)>(&Qbin::operator=), "assigns a value of right Q bin to this Q bin")
		.def("assign", static_cast<Qassign<Qbin>(Qbin::*)(const Qexpr<Qbin>&)>(&Qbin::operator=), "assignment of an Q expression creates a Q bin assignment where this Q bin is an assignee")

		/*** Compound Assignments ***/
		.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bin")
		.def(py::self &= Qexpr<Qbin>(), "'and assignment' of an Q expression creates a following Q bin assignment [this] = [this] & [right]")

		.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bin")
		.def(py::self |= Qexpr<Qbin>(), "'or assignment' of an Q expression creates a following Q bin assignment [this] = [this] | [right]")

		.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bin")
		.def(py::self ^= Qexpr<Qbin>(), "'xor assignment' of an Q expression creates a following Q bin assignment [this] = [this] ^ [right]")

		/*** Bitwise ***/
		.def(~ py::self, "instantiate Q binary expression with inversion logic, e.g. for Qbin with id 'x' the expression is '~x' != 'x'")

		.def(py::self& py::self, "instantiate Q expression with and logic, e.g. for Qbin ids 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self& Qexpr<Qbin>(), "instantiate Q expression with and logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::nand), "instantiate Q expression with nand logic, e.g. for Qbin ids 'x' and 'y' the expression is ~('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::nand), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is ~('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbin ids 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qexpr<Qbin>(), "instantiate Q expression with or logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::nor), "instantiate Q expression with nor logic, e.g. for Qbin ids 'x' and 'y' the expression is ~('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::nor), "instantiate Q expression with nor logic, e.g. for Qbin id 'x' and [right] object the expression is ~('x' | [right])")

		.def(py::self^ py::self, "instantiate Q expression with xor logic, e.g. for Qbin ids 'x' and 'y' the expression is 'x' ^ 'y'")
		.def(py::self^ Qexpr<Qbin>(), "instantiate Q expression with xor logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' ^ [right]")

		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbool ids 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qexpr<Qbin>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbool ids 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qexpr<Qbin>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");


	py::class_<Qwhole, Qbin>(m, "Qwhole",
		R"pbdoc( Quantum whole is in unknown state, if any of its quantum bits is in superposition state)pbdoc")
		.def(py::init<const Qwhole&>())
		.def(py::init<const string&>())
		.def(py::init<size_t, const string&>())
		.def(py::init<const string&, const Qbits&>())
		.def(py::init<const string&, const Bits&>())
		.def(py::init<const string&, const Bits&, bool>())

		.def("value", static_cast<unsigned long long (Qbin::*)()>(&Qwhole::operator unsigned long long), "get unsigned long long value")
		.def("value", static_cast<const unsigned long long (Qbin::*)() const>(&Qwhole::operator const unsigned long long), "get unsigned long long value")

		.def("toString", &Qwhole::toString, "returns string presentation of this Q whole object")
		.def("solution", &Qwhole::solution, "returns a solution for this object identified by id")


		/*** Assignments ***/
		.def("assign", static_cast<Qwhole& (Qwhole::*)(const Qwhole&)>(&Qwhole::operator=), "assigns a value of right Q whole to this Q bin")
		.def("assign", static_cast<Qassign<Qwhole>(Qwhole::*)(const Qexpr<Qwhole>&)>(&Qwhole::operator=), "assignment of an Q expression creates a Q whole assignment where this Q bin is an assignee")

		/*** Compound Assignments ***/
		.def(py::self += py::self, "change value of this object as result of add operation between this and right Q whole")
		.def(py::self += Qexpr<Qwhole>(), "'add assignment' of an Q expression creates a following Q whole assignment [this] = [this] + [right]")

		.def(py::self *= py::self, "change value of this object as result of multiply operation between this and right Q whole")
		.def(py::self *= Qexpr<Qwhole>(), "'multiply assignment' of an Q expression creates a following Q whole assignment [this] = [this] * [right]")

		.def(py::self -= py::self, "change value of this object as result of subtract operation between this and right Q whole")

		.def(py::self /= py::self, "change value of this object as result of divide operation between this and right Q whole")


	/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' == 'y'")
//		.def(py::self == Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' != 'y'")
//		.def(py::self != Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' > 'y'")
//		.def(py::self > Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' >= 'y'")
//		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self < py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' < 'y'")
//		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' <= 'y'")
//		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]")

	/*** Arithmetic ***/

		.def(py::self + py::self, "instantiate Q expression with and logic, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' + 'y'")
		.def(py::self + Qexpr<Qwhole>(), "instantiate Q expression with add logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' + [right]")

		.def(py::self + py::self, "instantiate Q expression with multiply logic, e.g. for Qwhole ids 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self + Qexpr<Qwhole>(), "instantiate Q expression with multiply logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' * [right]");

/*
	py::class_<Qdef>(m, "Qdef",
		R"pbdoc( Quantum definition of a definition defines Q bit definition names in a vector)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qdef&>())
		.def(py::init<Index>())
		.def(py::init<Index, const std::string&>())
		.def("nobs", &Qdef::noqbs)
		.def(py::self + py::self)
		.def(py::self * py::self);

	py::class_<Qstatement>(m, "Qstatement")
		//		.def(py::init<>())
		//		.def(py::init<const Qstatement&>())
		//		.def(py::init<Index>())
		//		.def(py::init<const Qexpression&, const Qvars&>())
		.def("nobs", &Qstatement::noqbs)
		//.def("expression", static_cast<const Qexpression & (Qstatement::*)() const>(&Qstatement::expression), "return const Qexpression&")
		.def("expression", static_cast<Qexpression& (Qstatement::*)()>(&Qstatement::expression), "return Qexpression&");
		//.def("arguments", static_cast<const Qvars & (Qstatement::*)() const>(&Qstatement::arguments), "return const Qvars&")
		//.def("arguments", static_cast<Qvars & (Qstatement::*)()>(&Qstatement::arguments), "return Qvars&");
*/
/*
	py::class_<Qequation, Qstatement>(m, "Qequation",
		R"pbdoc( Quantum equation is a coupling of result Q variable via Q expression with Qvar arguments
			Default constructor creates Q equation witout a result Q variable so the result definition is an empty string
				fx = Qeqation()

			Overloaded constructors: create Q equation with a given Q variable as an expected result,
				r = Qvar(4, "R"), e.g. result variable with definition name "R" and 4 Q bits in superposition state
				fR = Qeauation(r), e.g. a Q eauation with a result defined and without expression and arguments 

			Overloaded constructors with 3 arguments: result Q variable, resulting Q expression and corresponding
				Q variables as arguments of the expression:
				e = Qvar("e", 8), e.g. result variable "e" is set to a value 8 (b1000)
				args = [Qvar(3, "a"), Qvar(4, "b")], e.g. there are 2 arguments of the equation
				xPrsn = args[0].definition() + args[1].definition(), e.g. the equation expresion is "a + b"
				fe = Qequation(e, xPrsn, args), e.g. the equation contains the result, its expression and 
												corresponding arguemnts
 
			Copy constructor: creates a quantum variable with a same name and value as right object:
				myEquation = Qequation(...)
				...
				aCopy = Qequation(myEquation)

			assignment(Qequation right) - an assignment operator returns a reference of this Q equation with Q result,
				expression and arguments same as in right Qequation
				a = Qvar(2, "a"), e.g. Q variable named a with 2 bits in superposition state
				b = Qvar("b", 2), e.g. Q variable named b with value of 2
				c = Qvar(3, "c"), e.g. Q variable named c with 3 bits in superposition state
				e = Qvar("e", 6), e.g. Q variable named e with value of 6
				anE = Qequation(e), e.g. a Q equation with result variable e
				anE.assignment( a * b * c ), e.g. to anE equation assign multiplication of a,b and c variable

			Addition operator returns a new Qequation object with added Q variable to this Q equation
				fx = anE + a, e.g. where existing Qequation anE and variable a, create a new Qequation fx

			Addition operator returns a new Qequation object resulting from addition of this and right Q equations
				fy = fx + anE, e.g. where fy adds results, expressions and arguments of existing equations fx and anE

			Addition operator returns a reference to this Qequation object with added Q variable
				fy += b, .e.g. where b is existing Q varaible, and fy is an existing Qequation

			Addition operator returns a reference to this Qequation object with added right Q equation
				fy += anE, .e.g. where both fy and anE are existing Qequation's

			Multiplication operator returns a new Qequation object with multiplied Q variable with this Q equation
				fx = anE * a, e.g. where existing Qequation anE and variable a, create a new Qequation fx

			Multiplication operator returns a new Qequation object resulting from multiplication of this and right Q equations
				fy = fx * anE, e.g. where fy multiplies results and expressions, 
									and adds arguments of existing equations fx and anE

			Multiplication operator returns a reference to this Qequation object with multiplied Q variable
				fy *= b, .e.g. where b is existing Q varaible, and fy is an existing Qequation

			Multiplication operator returns a reference to this Qequation object with multiplied right Q equation
				fy *= anE, .e.g. where both fy and anE are existing Qequation's

			size() - returns a number of bit level expressions in this Q equation

			result() - returns a constant reference to the result of this Q equation

			expression() - returns a constant reference to the expression of this Q equation

			arguments() - returns a constant reference to the expression arguments of this Q equation

			qubo(bool finalized) - Returns a qubo representation of this Q equation, 
				if not finalized, returns a full qubo definition representation of this Q equation
				if finalized, returns an expression that replaces symbols with values of
				Qbits in deterministic states for all the Q variables, i.e. result and expression arguments

			add(Sample& sample) Add a sample with a node list defined by qubo() of this Q equation
				A Semple is defined as a dictionary (map) of definition nodes and their values.
				The node names are defined by qubo() for each Q equation

			set(Samples& samples) - Set a sample list with a node list defined by qubo() of this Q equation
				the combination of node values should be different for each sample

			solutions() - for existing samples, returns a string representation of all solutions of this Q euation

			toString(bool decomposed) - Returns a string representation of this Q equation, 
				if not decomposed, returns an equation line per each Qbit level
				if decomposed, returns a line per Qbit operational expression

		)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qequation&>())
		.def(py::init<const Qvar&>())
		.def(py::init<const Qvar&, const Qexpression&, const Qvars&>())
		.def("solutions", &Qequation::solutions)
		.def("add", &Qequation::add)
		.def("set", &Qequation::set)
		//		.def("toString", &Qequation::toString) 		//	virtual string toString(bool decomposed = false, Index level = Eigen::Infinity) const;
		.def("toString", static_cast<string(Qequation::*)() const>(&Qequation::toString), "return string representation")
		.def("toString", static_cast<string(Qequation::*)(bool) const>(&Qequation::toString), "overload: return string with decomposed representation")
		.def("toString", static_cast<string(Qequation::*)(bool, Index) const>(&Qequation::toString), "overload: return string with decomposed representation of a specified Qbit level")
		.def("result", &Qequation::result)
		//		.def("qubo", &Qequation::qubo)
		.def("qubo", static_cast<Qubo(Qequation::*)() const>(&Qequation::qubo), "return finalized qubo representation")
		.def("qubo", static_cast<Qubo(Qequation::*)(bool) const>(&Qequation::qubo), "overload: return qubo representation")
		.def("qubo", static_cast<Qubo(Qequation::*)(bool, Index) const>(&Qequation::qubo), "overload: return qubo representation of a specified Qbit level")
		.def("assign", static_cast<Qequation & (Qequation::*)(const Qequation&)>(&Qequation::operator=), "assign Qequation")
		.def("assign", static_cast<Qequation & (Qequation::*)(const Qvar&)>(&Qequation::operator=), "assign Qequation")
		.def(py::self & py::self)
		.def(py::self &= py::self)
		.def(py::self & Qvar())
		.def(py::self &= Qvar())
		.def("nand", static_cast<Qequation(Qequation::*)(const Qequation&) const>(&Qequation::nand), "operator ~&")
		.def("nandMutable", static_cast<Qequation & (Qequation::*)(const Qequation&)>(&Qequation::nandMutable), "operator ~&=")
		.def("nand", static_cast<Qequation(Qequation::*)(const Qvar&) const>(&Qequation::nand), "operator ~&")
		.def("nandMutable", static_cast<Qequation & (Qequation::*)(const Qvar&)>(&Qequation::nandMutable), "operator ~&=")
		.def(py::self | py::self)
		.def(py::self |= py::self)
		.def(py::self | Qvar())
		.def(py::self |= Qvar())
		.def("nor", static_cast<Qequation(Qequation::*)(const Qequation&) const>(&Qequation::nor), "operator ~|")
		.def("norMutable", static_cast<Qequation & (Qequation::*)(const Qequation&)>(&Qequation::norMutable), "operator ~|=")
		.def("nor", static_cast<Qequation(Qequation::*)(const Qvar&) const>(&Qequation::nor), "operator ~|")
		.def("norMutable", static_cast<Qequation & (Qequation::*)(const Qvar&)>(&Qequation::norMutable), "operator ~|=")
		.def(py::self ^ py::self)
		.def(py::self ^= py::self)
		.def(py::self ^ Qvar())
		.def(py::self ^= Qvar())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self + Qvar())
		.def(py::self += Qvar())
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self * Qvar())
		.def(py::self *= Qvar());
*/
	// specify C++ class->baseclass specialization
	py::class_<Qsolver>(m, "Qsolver")
		.def(py::init<const Qubo&>())
		.def(py::init<const Qubo&, bool>())
		.def("solution", &Qsolver::solution);

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}


