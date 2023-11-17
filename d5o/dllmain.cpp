// dllmain.cpp : Defines the entry point for the DLL application.

// pybind11 � Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <ULint.h>

#include <Qbit.h>
#include <Qbool.h>
#include <Qbin.h>
#include <Qint.h>
#include <Qwhole.h>

#include <Qexpr.h>
#include <Qassign.h>
#include <Qblock.h>
#include <Qroutine.h>
#include <Qbinder.h>

#include <Qevaluation.h>
#include <Qsolver.h>
#include <Qcompiler.h>

#include <pybind11/stl.h>
#include <pybind11/operators.h>
//#include <pybind11/eigen.h>
//#include <pybind11/embed.h>

using namespace dann5;

#define VERSION_INFO "3.0.0"

namespace py = pybind11;

#ifdef _DEBUG
PYBIND11_MODULE(d5o_d, m) {
#else
PYBIND11_MODULE(d5o2, m) {
#endif

	m.doc() = R"pbdoc(
        d5o plugin
        -----------------------

        .. currentmodule:: d5o

        .. autosummary::
           :toctree: _generate

			Qbit
			Qbool
			Qbin
			Qwhole
			Oint
			Qexpression
			Qassignment
    )pbdoc";

	m.def("version", []() { return VERSION_INFO; }, R"pbdoc(Returns version of dann5 d5o2 library.)pbdoc");


/*--- Qwhole.h definitions ---*/
    py::class_<ULint>(m, "ULint",
        R"pbdoc( Unsigned Long Integer supports manipulation of whole numbers larger than operating system maximum, e.g. 64 bits.)pbdoc")

        .def(py::init<>())
        .def(py::init<const ULint&>())
        .def(py::init<unsigned long long>(), "Constructs unsigned long integer object initialized with the unsigned long long value.")
        .def(py::init<string>(), "Constructs unsigned long integer object by converting a string argument using base 10.")
        .def(py::init<string, Byte>(), "Constructs unsigned long integer object by converting a string argument for given base.")

        .def("set", &ULint::set, "Sets bit at position 'at' to 1 if 'bit' is true, otherwise 0")

        .def("toString", &ULint::toString, "Converts unsigned long integer to string presentation for base 10")
        .def("toString", [](ULint& o, unsigned base ) { return o.toString(base); }, "Converts unsigned long integer to string presentation for given base")
		.def("__str__", [](ULint& o) { return o.toString(); })
        .def("__repr__", [](ULint& o) { return o.toString(); })

		.def("noBytes", &ULint::noBytes, "Returns number of containing bytes.")
		.def("noBits", &ULint::noBits, "Returns number of containing bits.")

        .def("at", (unsigned char&(ULint::*)(size_t)) & ULint::at, "Returns a reference of a byte (unsigned char) at position 'at'")
        .def("at", (const unsigned char&(ULint::*)(size_t) const) & ULint::at, "Returns a constant reference of a byte (unsigned char) at position 'at'")
    
        .def("__setitem__", [](ULint& self, size_t index, unsigned char val) { self[index] = val; }, "Sets a a byte (unsigned char) at position 'index'")
        .def("__getitem__", [](ULint& self, size_t index) { return self[index]; }, "Returns a value of a byte (unsigned char) at position 'index'")
		
		/*** Assignment operattors ***/
		.def("_", static_cast<ULint & (ULint::*)(const ULint&)>(&ULint::operator=), "Assigns the value on the right to the variable on the left")
		.def("as", static_cast<ULint & (ULint::*)(const ULint&)>(&ULint::operator=), "Assigns the value on the right to the variable on the left")

		.def(py::self >>= size_t(), "Shifts the value to right by the number of bits specified by the right operand and then assigns the result to the variable on the left.")
		.def(py::self <<= size_t(), "Shifts the value to left by the number of bits specified by the right operand and then assigns the result to the variable on the left.")

		.def(py::self += py::self, "First adds the current value of the variable on left to the value on the right and then assigns the result to the variable on the left.")
		.def(py::self -= py::self, "First subtracts the value on the right from the current value of the variable on left and then assign the result to the variable on the left.")
		.def(py::self *= py::self, "First multiplies the current value of the variable on left to the value on the right and then assign the result to the variable on the left.")
		.def(py::self /= py::self, "First divides the current value of the variable on left by the value on the right and then assign the result to the variable on the left.")
		.def(py::self %= py::self, "First calculates mode of the current value of the variable on left by the value on the right and then assign the result to the variable on the left.")

		/*** Bitwise operattors ***/
		.def(py::self >> size_t(), "Shifts the value to right by the number of bits specified by the right operand.")
		.def(py::self << size_t(), "Shifts the value to left by the number of bits specified by the right operand.")

		/*** Arithmetic operattors ***/
		.def(py::self + py::self, "Returns addition of two operands.")
		.def(py::self - py::self, "Returns subtraction of second operand from the first.")
		.def(py::self * py::self, "Returns multiplication of two operands.")
		.def(py::self / py::self, "Returns division of the first operand by the second operand.")
		.def(py::self % py::self, "Returns calculated mode, i.e. returns the remainder an integer division of first operand by the second operand.")

		/*** Relational operattors ***/
		.def(py::self == py::self, "Checks if both operands are equal.")
		.def(py::self != py::self, "Checks if both operands are not equal.")
		.def(py::self > py::self, "Checks if first operand is greater than the second operand.")
		.def(py::self >= py::self, "Checks if first operand is greater than or equal to the second operand.")
		.def(py::self < py::self, "Checks if first operand is lesser than the second operand.")
		.def(py::self <= py::self, "Checks if first operand is lesser than or equal to the second operand.");

/*--- Qvalue.h definitions ---*/
	m.def("Qvalue", []() { return char(); }, R"pbdoc( Q value is unsigned char. Valid values are {0, 1, S(uperposition)}.)pbdoc");

	m.def("Superposition", []() { return cSuperposition; }, R"pbdoc(Default quantum superposition value. 
		Though any Q bit value except 0 and 1 will be considered superposition.)pbdoc");

	py::class_<Qvalues>(m, "Qvalues", R"pbdoc(An array of Quantum values)pbdoc");

/*--- Qexpr.h definitions ---*/
	py::class_<Qexpression>(m, "Qexpression",
		R"pbdoc( Quantum expression of arithmetic Q operations with Q defined symbols as operands)pbdoc")
		.def("root", static_cast<Qop& (Qexpression::*)()>(&Qexpression::root), "returns a reference to the Q expression root Q operaton")
		.def("rootDef", static_cast<Qdef& (Qexpression::*)()>(&Qexpression::rootDef), "returns a reference to the Q expression root Q definition")

		.def("noqbs", &Qexpression::noqbs, "Returns the number of Q bits that the Q expression holds")

		.def("compile", &Qexpression::compile,
			"Compiles this Q expression to generate quantum solver code")

		.def("toString", &Qexpression::toString, "returns string presentation of this Q expression object")
		.def("toString", [](Qexpression& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qexpression& o) { return o.toString(); })
		.def("__str__", [](Qexpression& o) { return o.toString(); })

		.def("add", &Qexpression::add, "Add a quantum evaluation to this quantum expression.")
		.def("solutions", &Qexpression::solutions, "For existing samples, returns a string with all solutions of this Q expression")
		.def("solve", &Qexpression::solve, "Solve this Q expression and return a string with all solutions")
		.def("compute", &Qexpression::compute, "Returns computed sample set with all solutions for the Q expression logic")
		.def("reset", &Qexpression::reset, "Clear all solution samples");


	/*=== QbitExpr ===*/
	py::class_<Qexpr<Qbit>, Qexpression>(m, "QbitExpr", R"pbdoc( Instantiation of quantum bit expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qop::Sp&>())
		.def(py::init<const Qexpr<Qbit>&>())

		/*** Bitwise ***/
		.def(~py::self, "instantiate Q bit expression with inversion logic, e.g. for Qbit with id 'x' the expression is '~x' != 'x'")

		.def(py::self & py::self, "instantiate Q expression with and logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self & Qbit(), "instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbit>&(Qexpr<Qbit>::*)(const Qbit&)>(&Qexpr<Qbit>::nand), "instantiate Q expression with nand logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbit>&(Qexpr<Qbit>::*)(const Qexpr<Qbit>&)>(&Qexpr<Qbit>::nand), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qbit(), "instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbit>&(Qexpr<Qbit>::*)(const Qbit&) >(&Qexpr<Qbit>::nor), "instantiate Q expression with nor logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbit>&(Qexpr<Qbit>::*)(const Qexpr<Qbit>&) >(&Qexpr<Qbit>::nor), "instantiate Q expression with nor logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' | [right])")

		.def("unlike", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qbit&) const>(&Qexpr<Qbit>::unlike), "instantiate Q expression with xor logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' ^ 'y'")
		.def("unlike", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qexpr<Qbit>&) const>(&Qexpr<Qbit>::unlike), "instantiate Q expression with xor logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' ^ [right]")
		.def(py::self ^ Qbit(), "instantiate Q expression with xor logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' ^ 'y'")
		.def(py::self ^ py::self, "instantiate Q expression with xor logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' ^ [right]")

		.def("alike", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qbit&) const>(&Qexpr<Qbit>::alike), "instantiate Q expression with nxor logic, e.g. for Qexpr with id 'x' and 'y' the expression is !('x' ^ 'y')")
		.def("alike", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qexpr<Qbit>&) const>(&Qexpr<Qbit>::alike), "instantiate Q expression with nxor logic, e.g. for Qexpr id 'x' and [right] object the expression is !('x' ^ [right])")
		.def("nxor", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qbit&) const>(&Qexpr<Qbit>::nxor), "instantiate Q expression with nxor logic, e.g. for Qexpr with id 'x' and 'y' the expression is !('x' ^ 'y')")
		.def("nxor", static_cast<Qexpr<Qbit>(Qexpr<Qbit>::*)(const Qexpr<Qbit>&) const>(&Qexpr<Qbit>::nxor), "instantiate Q expression with nxor logic, e.g. for Qexpr id 'x' and [right] object the expression is !('x' ^ [right])")

		/*** Comparison ***/
		.def(py::self == Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' > 'y'")
		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' >= 'y'")
		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self < Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' < 'y'")
		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= Qbit(), "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' <= 'y'")
		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]");


	/*=== QboolExpr ===*/
	py::class_<Qexpr<Qbool>, Qexpression>(m, "QboolExpression", R"pbdoc( Instantiation of quantum boolean expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qop::Sp&>())
		.def(py::init<const Qexpr<Qbool>&>())

		/*** Logical ***/
		.def(!py::self, "instantiate Q boolean expression with inversion logic, e.g. for Qbool with id 'x' the expression is '!x' != 'x'")

		.def(py::self & Qbool(), "instantiate Q expression with and logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self & py::self, "instantiate Q expression with and logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' & [right]")

		.def(py::self | Qbool(), "instantiate Q expression with or logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' | [right]")

//		.def("unlike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::unlike), "instantiate Q expression with nand logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' ^ 'y'")
//		.def("unlike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::unlike), "instantiate Q expression with nand logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' ^ [right]")
//		.def(py::self^ Qbool(), "instantiate Q expression with xor logic, e.g. for Qexpr with id 'x' and 'y' the expression is 'x' ^ 'y'")
//		.def(py::self^ py::self, "instantiate Q expression with xor logic, e.g. for Qexpr id 'x' and [right] object the expression is 'x' ^ [right]")

//		.def("alike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::alike), "instantiate Q expression with nand logic, e.g. for Qexpr with id 'x' and 'y' the expression is !('x' ^ 'y')")
//		.def("alike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::alike), "instantiate Q expression with nand logic, e.g. for Qexpr id 'x' and [right] object the expression is !('x' ^ [right])")
//		.def("nxor", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::nxor), "instantiate Q expression with nand logic, e.g. for Qexpr with id 'x' and 'y' the expression is !('x' ^ 'y')")
//		.def("nxor", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::nxor), "instantiate Q expression with nand logic, e.g. for Qexpr id 'x' and [right] object the expression is !('x' ^ [right])")

		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qbool(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qbool(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");


	/*=== QbinExpr ===*/
	py::class_<Qexpr<Qbin>, Qexpression>(m, "QbinExpr", R"pbdoc( Instantiation of quantum binary expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qop::Sp&>())
		.def(py::init<const Qexpr<Qbin>&>())

		/*** Bitwise ***/
		.def(~py::self, "instantiate Q bit expression with inversion logic, e.g. for Qbin with id 'x' the expression is '~x' != 'x'")

		.def(py::self& py::self, "instantiate Q expression with and logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self& Qbin(), "instantiate Q expression with and logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbin>& (Qexpr<Qbin>::*)(const Qbin&)>(&Qexpr<Qbin>::nand), "instantiate Q expression with nand logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbin>& (Qexpr<Qbin>::*)(const Qexpr<Qbin>&)>(&Qexpr<Qbin>::nand), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qbin(), "instantiate Q expression with or logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbin>& (Qexpr<Qbin>::*)(const Qbin&)>(&Qexpr<Qbin>::nor), "instantiate Q expression with nor logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbin>& (Qexpr<Qbin>::*)(const Qexpr<Qbin>&)>(&Qexpr<Qbin>::nor), "instantiate Q expression with nor logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' | [right])")


		.def(py::self^ py::self, "instantiate Q expression with xor logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' ^ 'y'")
		.def(py::self^ Qbin(), "instantiate Q expression with xor logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' ^ [right]")

		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbin with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qbin(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbin with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qbin(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");


	/*=== QwholeExpr ===*/
	py::class_<Qexpr<Qwhole>, Qexpression>(m, "QwholeExpr", R"pbdoc( Instantiation of quantum whole expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qop::Sp&>())
		.def(py::init<const Qexpr<Qwhole>&>())

		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' > 'y'")
		.def(py::self > Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' >= 'y'")
		.def(py::self >= Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' >= [right root]")

		.def(py::self < py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' < 'y'")
		.def(py::self >= Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' <= 'y'")
		.def(py::self >= Qwhole(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] Qwhole the expression is 'x' <= [right root]")

		/*** Arithmetic ***/
		.def(py::self + py::self, "instantiate Q expression with and logic, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' + 'y'")
		.def(py::self + Qwhole(), "instantiate Q expression with and logic, e.g. for Qwhole expression with id 'x' and [right] Qwhole object the expression is 'x' + [right]")

		.def(py::self - py::self, "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' - 'y'")
		.def(py::self - Qwhole(), "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and [right] Qwhole object the expression is 'x' - [right]")

		.def(py::self * py::self, "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self * Qwhole(), "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and [right] Qwhole object the expression is 'x' * [right]")

		.def(py::self / py::self, "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self / Qwhole(), "instantiate Q expression with or logic, e.g. for Qwhole expression with id 'x' and [right] Qwhole object the expression is 'x' * [right]");


/*--- Qassign.h definitions ---*/
	py::class_<Qassignment>(m, "Qassignment",
		R"pbdoc( Quantum assignment of an expression to a result)pbdoc")
		.def("_", &Qassignment::assign, "Assign a Q expression to Q defintion assignee. Use without inputs to remove (one or boath) Q assignment members")
		.def("as", &Qassignment::assign, "Assign a Q expression to Q defintion assignee. Use without inputs to remove (one or boath) Q assignment members")

		.def("assignee", static_cast<const Qdef::Sp& (Qassignment::*)() const>(&Qassignment::assignee), "returns a reference to the Q assignee")
		.def("assignee", static_cast<void (Qassignment::*)(const Qdef::Sp&)>(&Qassignment::assignee), "set a Q definition pointer to a new assignee")

		.def("expression", static_cast<const Qexpression::Sp& (Qassignment::*)() const>(&Qassignment::expression), "returns a reference to the assignment's Q expression")
		.def("expression", static_cast<void (Qassignment::*)(const Qexpression::Sp&)>(&Qassignment::expression), "set assignment's new Q expression")

		.def("noqbs", &Qassignment::noqbs, "Returns the number of Q bits that the Q assignment holds")

        .def("compile", &Qassignment::compile,
            "Compiles this quantum assignment to generate quantum solver code")

		.def("toString", &Qassignment::toString, "returns string presentation of this Q assignment object")
		.def("toString", [](Qassignment& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qassignment& o) { return o.toString(); })
		.def("__str__", [](Qassignment& o) { return o.toString(); })

		.def("add", &Qassignment::add, "Add a quantum evaluation to this Q assignment.")
		.def("solutions", &Qassignment::solutions, "For existing samples, returns a string with all solutions of this Q assignment")
		.def("solve", &Qassignment::solve, "Solve this Q assignment and return a string with all solutions")
		.def("compute", &Qassignment::compute, "Returns computed sample set with all solutions for the Q assignment logic")
		.def("reset", &Qassignment::reset, "Clear all solution samples");


	/*=== QbitAssign ===*/
	py::class_<Qassign<Qbit>, Qassignment>(m, "QbitAssign", R"pbdoc( Instantiation of quantum bit expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qbit&>())
		.def(py::init<const Qbit&, const Qexpr<Qbit>&>())
		.def(py::init<const Qassign<Qbit>&>());


	/*=== QboolAssign ===*/
	py::class_<Qassign<Qbool>, Qassignment>(m, "QboolAssign", R"pbdoc( Instantiation of quantum bool expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qbool&>())
		.def(py::init<const Qbool&, const Qexpr<Qbool>&>())
		.def(py::init<const Qassign<Qbool>&>());


	/*=== QbinAssign ===*/
	py::class_<Qassign<Qbin>, Qassignment>(m, "QbinAssign", R"pbdoc( Instantiation of quantum bin expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qbin&>())
		.def(py::init<const Qbin&, const Qexpr<Qbin>&>())
		.def(py::init<const Qassign<Qbin>&>());


	/*=== QwholeAssign ===*/
	py::class_<Qassign<Qwhole>, Qassignment>(m, "QwholeAssign", R"pbdoc( Instantiation of quantum whole expression)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qwhole&>())
		.def(py::init<const Qwhole&, const Qexpr<Qwhole>&>())
		.def(py::init<const Qassign<Qwhole>&>());


/*--- Qblock.h definitions---*/
	py::class_<Qblock>(m, "Qblock",
		R"pbdoc( A Q block is a lexical structure of quantum code, which is grouped
				 together. Qblock consist of one or more quantum statements. Qblock 
				 is a quantum statement, too, permitting block-structured quantum 
				 programming through the creation of blocks nested within other blocks)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qblock&>())

		.def("noqbs", &Qblock::noqbs, "Returns the number of Q bits that the Q block holds")

        .def("compile", &Qblock::compile,
            "Compiles this quantum block to generate quantum solver code")

		.def("toString", &Qblock::toString, "returns string presentation of this Q block object")
		.def("toString", [](Qblock& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qblock& o) { return o.toString(); })
		.def("__str__", [](Qblock& o) { return o.toString(); })

		.def("add", &Qblock::add, "Add a quantum evaluation to this Q block.")
		.def("solutions", &Qblock::solutions, "For existing samples, returns a string with all solutions of this Q block")
		.def("solve", &Qblock::solve, "Solve this Q block and return a string with all solutions")
		.def("compute", &Qblock::compute, "Returns computed sample set with all solutions for the Q block logic")
		.def("reset", &Qblock::reset, "Clear all solution samples")
		
		.def(py::self << Qexpr<Qbit>())
		.def(py::self << Qexpr<Qbool>())
		.def(py::self << Qexpr<Qbin>())
		.def(py::self << Qexpr<Qwhole>())
		.def("_a", [](Qblock& o, const Qexpr<Qwhole>& right) { o << right; return o; })

		.def(py::self << Qassign<Qbit>())
		.def(py::self << Qassign<Qbool>())
		.def(py::self << Qassign<Qbin>())
		.def(py::self << Qassign<Qwhole>())
		.def("_a", [](Qblock& o, const Qassign<Qwhole>& right) { o << right; return o; });


/*--- Qdef.h definitions---*/
	m.def("AllBits", []() { return cAllBits; }, R"pbdoc(A constant indicating all Q bits should be processed.)pbdoc");

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
*/
/*--- Qbit.h definitions ---*/
	py::class_<Qbit, std::shared_ptr<Qbit> >(m, "Qbit",
		R"pbdoc( Quantum bit is in superposition state for any value except 0 and 1)pbdoc")

		.def(py::init<>())
		.def(py::init<const Qbit&>())
		.def(py::init<const string&>())
		.def(py::init<const string&, Qvalue>())

		.def("id", static_cast<void(Qbit::*)(const string&)>(&Qbit::id), "sets ID")
		.def("id", static_cast<string(Qbit::*)() const>(&Qbit::id), "const get ID")
		.def("noqbs", &Qbit::noqbs, "always returns 1")
		.def("value", static_cast<void(Qbit::*)(Qvalue)>(&Qbit::value), "sets Qvalue")
		.def("value", static_cast<Qvalue(Qbit::*)()>(&Qbit::value), "get Qvalue")
		.def("value", static_cast<Qvalue(Qbit::*)() const>(&Qbit::value), "const get Qvalue")

		.def("toString", &Qbit::toString, "returns string presentation of this Q bit object")
		.def("toString", [](Qbit& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qbit& o) { return o.toString(); })
		.def("__str__", [](Qbit& o) { return o.toString(); })

		.def("add", &Qbit::add, "adds solutions to this object")
		.def("solution", &Qbit::solution, "returns a solution for this object identified by id")

	/*** Assignments ***/
		.def("_", static_cast<Qassign<Qbit>(Qbit::*)(const Qbit&)>(&Qbit::operator=), "assigns a value of right Q bit to this Q bit")
		.def("as", static_cast<Qassign<Qbit>(Qbit::*)(const Qbit&)>(&Qbit::operator=), "assigns a value of right Q bit to this Q bit")
		.def("_", static_cast<Qassign<Qbit>(Qbit::*)(const Qexpr<Qbit>&)>(&Qbit::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")
		.def("as", static_cast<Qassign<Qbit>(Qbit::*)(const Qexpr<Qbit>&)>(&Qbit::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")


	/*** Compound Assignments ***/
		.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bit")
		.def(py::self &= Qexpr<Qbit>(), "'and assignment' of an Q expression creates a following Q bit assignment [this] = [this] & [right]")

		.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bit")
		.def(py::self |= Qexpr<Qbit>(), "'or assignment' of an Q expression creates a following Q bit assignment [this] = [this] | [right]")

		.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bit")
		.def(py::self ^= Qexpr<Qbit>(), "'xor assignment' of an Q expression creates a following Q bit assignment [this] = [this] ^ [right]")
		
	/*** Bitwise ***/
		.def(~py::self, "instantiate Q bit expression with inversion logic, e.g. for Qbit with id 'x' the expression is '!x' != 'x'")

		.def(py::self & py::self, "instantiate Q expression with and logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self & Qexpr<Qbit>(), "instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::nand), "instantiate Q expression with nand logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::nand), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qexpr<Qbit>(), "instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::nor), "instantiate Q expression with nor logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::nor), "instantiate Q expression with nor logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' | [right])")

		.def("unlike", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::unlike), "instantiate Q expression with nand logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' ^ 'y'")
		.def("unlike", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::unlike), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' ^ [right]")
		.def(py::self ^ py::self, "instantiate Q expression with xor logic, e.g. for Qbit with id 'x' and 'y' the expression is 'x' ^ 'y'")
		.def(py::self ^ Qexpr<Qbit>(), "instantiate Q expression with xor logic, e.g. for Qbit id 'x' and [right] object the expression is 'x' ^ [right]")

		.def("alike", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::alike), "instantiate Q expression with nand logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' ^ 'y')")
		.def("alike", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::alike), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' ^ [right])")
		.def("nxor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qbit&) const>(&Qbit::nxor), "instantiate Q expression with nand logic, e.g. for Qbit with id 'x' and 'y' the expression is !('x' ^ 'y')")
		.def("nxor", static_cast<Qexpr<Qbit>(Qbit::*)(const Qexpr<Qbit>&) const>(&Qbit::nxor), "instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right] object the expression is !('x' ^ [right])")

	/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' > 'y'")
		.def(py::self > Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' >= 'y'")
		.def(py::self >= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self < py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' < 'y'")
		.def(py::self < Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qbit with id 'x' and 'y' the expression is 'x' <= 'y'")
		.def(py::self <= Qexpr<Qbit>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]");

	
	/*=== Qbits ===*/
//	py::class_<Qbits>(m, "Qbits", R"pbdoc(Quantum vector of Q cell references)pbdoc");


/*--- Qbool.h definitions ---*/
		py::class_<Qbool>(m, "Qbool",
			R"pbdoc( Quantum boolean is in superposition state for any value except 'F' and 'T')pbdoc")
			.def("true", []() { return Qbool::cTrue; })
			.def("false", []() { return Qbool::cFalse; })

			.def(py::init<const Qbool&>())
			.def(py::init<const string&>())
			.def(py::init<const string&, Qvalue>())

			.def("id", static_cast<void(Qbool::*)(const string&)>(&Qbool::id), "sets ID")
			.def("id", static_cast<string(Qbool::*)() const>(&Qbool::id), "const get ID")
			.def("noqbs", &Qbool::noqbs, "always returns 1")
			.def("value", static_cast<void(Qbool::*)(Qvalue)>(&Qbool::value), "sets Qvalue")
			.def("value", static_cast<Qvalue(Qbool::*)()>(&Qbool::value), "get Qvalue")
			.def("value", static_cast<Qvalue(Qbool::*)() const>(&Qbool::value), "const get Qvalue")

			.def("toString", &Qbool::toString, "returns string presentation of this Q bit object")
			.def("toString", [](Qbool& o, bool decomposed) { return o.toString(decomposed); })
			.def("toString", [](Qbool& o) { return o.toString(); })
			.def("__str__", [](Qbool& o) { return o.toString(); })

			.def("add", &Qbool::add, "adds solutions to this object")
			.def("solution", &Qbool::solution, "returns a solution for this object identified by id")

			/*** Assignments ***/
			.def("_", static_cast<Qassign<Qbool>(Qbool::*)(const Qbool&)>(&Qbool::operator=), "assigns a value of right Q bit to this Q bit")
			.def("as", static_cast<Qassign<Qbool>(Qbool::*)(const Qbool&)>(&Qbool::operator=), "assigns a value of right Q bit to this Q bit")
			.def("_", static_cast<Qassign<Qbool>(Qbool::*)(const Qexpr<Qbool>&)>(&Qbool::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")
			.def("as", static_cast<Qassign<Qbool>(Qbool::*)(const Qexpr<Qbool>&)>(&Qbool::operator=), "assignment of an Q expression creates a Q bit assignment where this Q bit is an assignee")


			/*** Compound Assignments ***/
			.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bit")
			.def(py::self &= Qexpr<Qbool>(), "'and assignment' of an Q expression creates a following Q bit assignment [this] = [this] & [right]")

			.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bit")
			.def(py::self |= Qexpr<Qbool>(), "'or assignment' of an Q expression creates a following Q bit assignment [this] = [this] | [right]")

			.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bit")
			//		.def(py::self ^= Qexpr<Qbool>(), "'xor assignment' of an Q expression creates a following Q bit assignment [this] = [this] ^ [right]")

			/*** Logical ***/
			.def(!py::self, "instantiate Q boolean expression with inversion logic, e.g. for Qbool with id 'x' the expression is '!x' != 'x'")

			.def(py::self& py::self, "instantiate Q expression with and logic, e.g. for Qbool with id 'x' and 'y' the expression is 'x' && 'y'")
			.def(py::self& Qexpr<Qbool>(), "instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' && [right]")

			.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbool with id 'x' and 'y' the expression is 'x' || 'y'")
			.def(py::self | Qexpr<Qbool>(), "instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' || [right]")

			.def("unlike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::unlike), "instantiate Q expression with nand logic, e.g. for Qbool with id 'x' and 'y' the expression is 'x' ^ 'y'")
			.def("unlike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::unlike), "instantiate Q expression with nand logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' ^ [right]")
			.def(py::self^ py::self, "instantiate Q expression with xor logic, e.g. for Qbool with id 'x' and 'y' the expression is 'x' ^ 'y'")
			.def(py::self^ Qexpr<Qbool>(), "instantiate Q expression with xor logic, e.g. for Qbool id 'x' and [right] object the expression is 'x' ^ [right]")

			.def("alike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::alike), "instantiate Q expression with nand logic, e.g. for Qbool with id 'x' and 'y' the expression is !('x' ^ 'y')")
			.def("alike", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::alike), "instantiate Q expression with nand logic, e.g. for Qbool id 'x' and [right] object the expression is !('x' ^ [right])")
			.def("nxor", static_cast<Qexpr<Qbool>(Qbool::*)(const Qbool&) const>(&Qbool::nxor), "instantiate Q expression with nand logic, e.g. for Qbool with id 'x' and 'y' the expression is !('x' ^ 'y')")
			.def("nxor", static_cast<Qexpr<Qbool>(Qbool::*)(const Qexpr<Qbool>&) const>(&Qbool::nxor), "instantiate Q expression with nand logic, e.g. for Qbool id 'x' and [right] object the expression is !('x' ^ [right])")

			/*** Comparison ***/
			.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' == 'y'")
			.def(py::self == Qexpr<Qbool>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

			.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' != 'y'")
			.def(py::self != Qexpr<Qbool>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");


/*--- Qnary.h definitions ---*/
		py::class_<Bits>(m, "Bits", R"pbdoc(coresponds to a bitset of the same size and unsigned long long (64 bits))pbdoc")
			.def(py::init<>())
			.def(py::init<unsigned long long>())

			.def(py::self == py::self, "compares the contents")

			.def("__setitem__", [](Bits& self, size_t index, bool val) { self[index] = val; }, "Sets a value at the position 'index'")
			.def("__getitem__", [](Bits& self, size_t index) { return bool(self[index]); }, "Returns bit value at 'index' position as a boolean.")
			.def("__len__", [](Bits& self) { return self.size(); }, "Returns size (lenght) of the bitset.")
			.def("test", &Bits::test, "Returns the value of the bit at the position pos (counting from 0). Unlike operator[], performs a bounds check and throws std::out_of_range if pos does not correspond to a valid position in the bitset.")

			.def("all", &Bits::all, "checks if all of the bits are set to true")
			.def("any", &Bits::any, "checks if any of the bits are set to true")
			.def("none", &Bits::none, "checks if none of the bits are set to true")
			.def("count", &Bits::count, "returns the number of bits set to true")

			.def("set", static_cast<Bits& (Bits::*)(size_t, bool)>(&Bits::set), "sets bits to true or given value")
			.def("reset", static_cast<Bits& (Bits::*)(size_t)>(&Bits::reset), "sets bits to false")
			.def("flip", static_cast<Bits& (Bits::*)(size_t)>(&Bits::flip), "toggles the values of bits")

			.def("to_ulong", &Bits::to_ulong, "returns an unsigned long integer representation of the data")
			.def("to_ullong", &Bits::to_ullong, "returns an unsigned long long integer representation of the data")
			.def("to_string", [](Bits& o) { return o.to_string(); });

/*--- Qbin.h definitions ---*/
	py::class_<Qbin>(m, "Qbin",
		R"pbdoc( Quantum binary is in unknown state, if any of its quantum bits is in superposition state)pbdoc")

		.def("Unknown", []() { return Qnary::cUnknown; }, R"pbdoc(Any binary container or number is unknown, if any of its Q bits is in superposition state.)pbdoc")

		.def(py::init<>())
		.def(py::init<const Qbin&>())
		.def(py::init<const string&>())
		.def(py::init<size_t, const string&>())
		.def(py::init<const string&, const Qbits&>())
		.def(py::init<const string&, const Bits&>())
		.def(py::init<const string&, const Bits&, bool>())

		.def("id", static_cast<void(Qbin::*)(const string&)>(&Qbin::id), "sets ID")
		.def("id", static_cast<string(Qbin::*)() const>(&Qbin::id), "const get ID")
		.def("resize", &Qbin::resize)
		.def("resize", [](Qbin& o, size_t size) { return o.resize(size); })
		.def("noqbs", &Qbin::noqbs, "always returns the number of Qbits contained by the object")

		.def("value", static_cast<Bits(Qbin::*)()>(&Qbin::operator Bits), "get Bits value")
		.def("value", static_cast<const Bits(Qbin::*)() const>(&Qbin::operator const Bits), "get Bits value")

		.def("toString", &Qbin::toString, "returns string presentation of this Q bin object")
		.def("toString", [](Qbin& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qbin& o) { return o.toString(); })
		.def("__str__", [](Qbin& o) { return o.toString(); })

		.def("toQbits", [](Qbin& o) { return o.operator Qbits(); })
		.def("toBits", [](Qbin& o) { return o.operator Bits(); })

		.def("add", &Qbin::add, "adds solutions to this object")
		.def("solution", &Qbin::solution, "returns a solution for this object identified by id")
        .def("reset", &Qbin::reset, "Reset the quantum variable into its initial state without solutions by clearing all solution samples")

        .def("__setitem__", [](Qbin &self, size_t index, Qvalue val) { self[index].value(val); })
		.def("__getitem__", [](Qbin& self, size_t index) { return self[index]; }, "Returns Qbit object at 'index' position.")

		/*** Assignments ***/
		.def("_", static_cast<Qassign<Qbin>(Qbin::*)(const Qbin&)>(&Qbin::operator=), "assigns a value of right Q creates a Q bin assignment where this Q bin is an assignee")
		.def("as", static_cast<Qassign<Qbin>(Qbin::*)(const Qbin&)>(&Qbin::operator=), "assigns a value of right Q creates a Q bin assignment where this Q bin is an assignee")
		.def("_", static_cast<Qassign<Qbin>(Qbin::*)(const Qexpr<Qbin>&)>(&Qbin::operator=), "assignment of an Q expression creates a Q bin assignment where this Q bin is an assignee")
		.def("as", static_cast<Qassign<Qbin>(Qbin::*)(const Qexpr<Qbin>&)>(&Qbin::operator=), "assignment of an Q expression creates a Q bin assignment where this Q bin is an assignee")

		/*** Compound Assignments ***/
		.def(py::self &= py::self, "change value of this object as result of and operation between this and right Q bin")
		.def(py::self &= Qexpr<Qbin>(), "'and assignment' of an Q expression creates a following Q bin assignment [this] = [this] & [right]")

		.def(py::self |= py::self, "change value of this object as result of or operation between this and right Q bin")
		.def(py::self |= Qexpr<Qbin>(), "'or assignment' of an Q expression creates a following Q bin assignment [this] = [this] | [right]")

		.def(py::self ^= py::self, "change value of this object as result of xor operation between this and right Q bin")
		.def(py::self ^= Qexpr<Qbin>(), "'xor assignment' of an Q expression creates a following Q bin assignment [this] = [this] ^ [right]")

		/*** Bitwise ***/
		.def(~py::self, "instantiate Q binary expression with inversion logic, e.g. for Qbin with id 'x' the expression is '~x' != 'x'")

		.def(py::self& py::self, "instantiate Q expression with and logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' & 'y'")
		.def(py::self& Qexpr<Qbin>(), "instantiate Q expression with and logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' & [right]")

		.def("nand", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::nand), "instantiate Q expression with nand logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' & 'y')")
		.def("nand", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::nand), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' & [right])")

		.def(py::self | py::self, "instantiate Q expression with or logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' | 'y'")
		.def(py::self | Qexpr<Qbin>(), "instantiate Q expression with or logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' | [right]")

		.def("nor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::nor), "instantiate Q expression with nor logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' | 'y')")
		.def("nor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::nor), "instantiate Q expression with nor logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' | [right])")


        .def("unlike", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::unlike), "instantiate Q expression with nand logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' ^ 'y'")
        .def("unlike", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::unlike), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' ^ [right]")
        .def(py::self ^ py::self, "instantiate Q expression with xor logic, e.g. for Qbin with id 'x' and 'y' the expression is 'x' ^ 'y'")
        .def(py::self ^ Qexpr<Qbin>(), "instantiate Q expression with xor logic, e.g. for Qbin id 'x' and [right] object the expression is 'x' ^ [right]")

        .def("alike", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::alike), "instantiate Q expression with nand logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' ^ 'y')")
        .def("alike", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::alike), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' ^ [right])")
        .def("nxor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qbin&) const>(&Qbin::nxor), "instantiate Q expression with nand logic, e.g. for Qbin with id 'x' and 'y' the expression is !('x' ^ 'y')")
        .def("nxor", static_cast<Qexpr<Qbin>(Qbin::*)(const Qexpr<Qbin>&) const>(&Qbin::nxor), "instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right] object the expression is !('x' ^ [right])")
		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qexpr<Qbin>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qbool with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qexpr<Qbin>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]");


/*--- Qwhole.h definitions ---*/
	py::class_<Qwhole, Qbin>(m, "Qwhole",
		R"pbdoc( Quantum whole is in unknown state, if any of its quantum bits is in superposition state)pbdoc")

		.def("Unknown", []() { return Qwhole::cUnknown; }, R"pbdoc(Qwhole is unknown, if any of its Q bits is in superposition state.)pbdoc")

		.def(py::init<>())
		.def(py::init<const Qwhole&>())
		.def(py::init<const string&>())
		.def(py::init<size_t, const string&>())
		.def(py::init<const string&, const Qbits&>())
		.def(py::init<const string&, unsigned long long>())
		.def(py::init<const string&, const Bits&, bool>())

		.def("value", static_cast<unsigned long long (Qwhole::*)()>(&Qwhole::operator unsigned long long), "get unsigned long long value")
		.def("value", static_cast<const unsigned long long (Qwhole::*)() const>(&Qwhole::operator const unsigned long long), "get const unsigned long long value")

		.def("toString", &Qwhole::toString, "returns string presentation of this Q whole object")
		.def("toString", [](Qwhole& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qwhole& o) { return o.toString(); })
		.def("__str__", [](Qwhole& o) { return o.toString(); })

		.def("solution", &Qwhole::solution, "returns a solution for this object identified by id")
		.def("results", &Qwhole::results, "Returns the list of assigned solutions as unsigned long long numbers")
        .def("ulints", &Qwhole::ulints, "Returns the list of assigned solutions as unsigned long integer")


		/*** Assignments ***/
		.def("_", static_cast<Qassign<Qwhole>(Qwhole::*)(const Qwhole&) const>(&Qwhole::operator=), "assigns a value of right Q whole to this Q whole")
		.def("as", static_cast<Qassign<Qwhole>(Qwhole::*)(const Qwhole&) const>(&Qwhole::operator=), "assigns a value of right Q whole to this Q whole")
		.def("_", static_cast<Qassign<Qwhole>(Qwhole::*)(const Qexpr<Qwhole>&) const>(&Qwhole::operator=), "assignment of an Q expression creates a Q whole assignment where this Q whole is an assignee")
		.def("as", static_cast<Qassign<Qwhole>(Qwhole::*)(const Qexpr<Qwhole>&) const>(&Qwhole::operator=), "assignment of an Q expression creates a Q whole assignment where this Q whole is an assignee")

		/*** Compound Assignments ***/
		.def(py::self += py::self, "change value of this object as result of add operation between this and right Q whole")
		.def(py::self += Qexpr<Qwhole>(), "'add assignment' of an Q expression creates a following Q whole assignment [this] = [this] + [right]")

		.def(py::self *= py::self, "change value of this object as result of multiply operation between this and right Q whole")
		.def(py::self *= Qexpr<Qwhole>(), "'multiply assignment' of an Q expression creates a following Q whole assignment [this] = [this] * [right]")

		.def(py::self -= py::self, "change value of this object as result of subtract operation between this and right Q whole")
		.def(py::self -= Qexpr<Qwhole>(), "'subtract assignment' of an Q expression creates a following Q whole assignment [this] = [this] - [right]")

		.def(py::self /= py::self, "change value of this object as result of divide operation between this and right Q whole")
		.def(py::self /= Qexpr<Qwhole>(), "'divide assignment' of an Q expression creates a following Q whole assignment [this] = [this] / [right]")


	/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' == 'y'")
		.def(py::self == Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' != 'y'")
		.def(py::self != Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' > 'y'")
		.def(py::self > Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' >= 'y'")
		.def(py::self >= Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self < py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' < 'y'")
		.def(py::self < Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' <= 'y'")
		.def(py::self <= Qexpr<Qwhole>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]")

	/*** Arithmetic ***/

		.def(py::self + py::self, "instantiate Q expression with and logic, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' + 'y'")
		.def(py::self + Qexpr<Qwhole>(), "instantiate Q expression with add logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' + [right]")

		.def(py::self * py::self, "instantiate Q expression with multiply logic, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self * Qexpr<Qwhole>(), "instantiate Q expression with multiply logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' * [right]")

		.def(py::self - py::self, "instantiate Q expression with and logic, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' + 'y'")
		.def(py::self - Qexpr<Qwhole>(), "instantiate Q expression with add logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' + [right]")

		.def(py::self / py::self, "instantiate Q expression with multiply logic, e.g. for Qwhole with id 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self / Qexpr<Qwhole>(), "instantiate Q expression with multiply logic, e.g. for Qwhole id 'x' and [right] object the expression is 'x' * [right]");


	/*--- Qint.h definitions ---*/
	py::class_<Qint, Qbin>(m, "Qint",
		R"pbdoc( Quantum integer is in unknown state, if any of its quantum bits is in superposition state)pbdoc")

		.def("Unknown", []() { return Qint::cUnknown; }, R"pbdoc(Qint is unknown, if any of its Q bits is in superposition state.)pbdoc")

		.def(py::init<>())
		.def(py::init<const Qint&>())
		.def(py::init<const string&>())
		.def(py::init<size_t, const string&>())
		.def(py::init<const string&, const Qbits&>())
		//		.def(py::init<const string&, long long>())
		.def(py::init<const string&, const Bits&, bool>())
		.def(py::init<size_t, const string&, const Bits&>())

		.def("value", static_cast<long long (Qbin::*)()>(&Qint::operator long long), "get long long value")
		.def("value", static_cast<const long long (Qbin::*)() const>(&Qint::operator const long long), "get long long value")

		.def("toString", &Qint::toString, "returns string presentation of this Q integer object")
		.def("toString", [](Qint& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qint& o) { return o.toString(); })
		.def("__str__", [](Qint& o) { return o.toString(); })

		.def("solution", &Qint::solution, "returns a solution for this object identified by id")


		/*** Assignments ***/
		.def("_", static_cast<Qint& (Qint::*)(const Qint&)>(&Qint::operator=), "assigns a value of right Q integer to this Q bin")
		.def("as", static_cast<Qint& (Qint::*)(const Qint&)>(&Qint::operator=), "assigns a value of right Q integer to this Q bin")
		.def("_", static_cast<Qassign<Qint>(Qint::*)(const Qexpr<Qint>&)>(&Qint::operator=), "assignment of an Q expression creates a Q integer assignment where this Q bin is an assignee")
		.def("as", static_cast<Qassign<Qint>(Qint::*)(const Qexpr<Qint>&)>(&Qint::operator=), "assignment of an Q expression creates a Q integer assignment where this Q bin is an assignee")

		/*** Compound Assignments ***/
		.def(py::self += py::self, "change value of this object as result of add operation between this and right Q integer")
		.def(py::self += Qexpr<Qint>(), "'add assignment' of an Q expression creates a following Q integer assignment [this] = [this] + [right]")

		.def(py::self *= py::self, "change value of this object as result of multiply operation between this and right Q integer")
		.def(py::self *= Qexpr<Qint>(), "'multiply assignment' of an Q expression creates a following Q integer assignment [this] = [this] * [right]")

		.def(py::self -= py::self, "change value of this object as result of subtract operation between this and right Q integer")

		.def(py::self /= py::self, "change value of this object as result of divide operation between this and right Q integer")


		/*** Comparison ***/
		.def(py::self == py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' == 'y'")
		//		.def(py::self == Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' == [right root]")

		.def(py::self != py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' != 'y'")
		//		.def(py::self != Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' != [right root]")

		.def(py::self > py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' > 'y'")
		//		.def(py::self > Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' > [right root]")

		.def(py::self >= py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' >= 'y'")
		//		.def(py::self >= Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' >= [right root]")

		.def(py::self < py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' < 'y'")
		//		.def(py::self >= Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' < [right root]")

		.def(py::self <= py::self, "instantiate Q comparison expression, e.g. for Qint with id 'x' and 'y' the expression is 'x' <= 'y'")
		//		.def(py::self >= Qexpr<Qint>(), "instantiate Q comparison expression, e.g. for arguments 'x' and [right] the expression is 'x' <= [right root]")

		/*** Arithmetic ***/
		.def(py::self + py::self, "instantiate Q expression with and logic, e.g. for Qint with id 'x' and 'y' the expression is 'x' + 'y'")
		.def(py::self + Qexpr<Qint>(), "instantiate Q expression with add logic, e.g. for Qint id 'x' and [right] object the expression is 'x' + [right]")

		.def(py::self* py::self, "instantiate Q expression with multiply logic, e.g. for Qint with id 'x' and 'y' the expression is 'x' * 'y'")
		.def(py::self* Qexpr<Qint>(), "instantiate Q expression with multiply logic, e.g. for Qint id 'x' and [right] object the expression is 'x' * [right]");


/*--- Qroutine.h definitions---*/
	py::class_<Qroutine>(m, "Qroutine",
		R"pbdoc( Quantum routine is a Q definition with a sequence of logical 
				 quantum statements organized as Q block)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qroutine&>())
		.def(py::init<size_t>())
		.def(py::init<const string&>())
		.def(py::init<const string&, size_t>())
		.def(py::init<const string&, const Qblock&>())
		.def(py::init<const string&, const Qblock&, size_t>())

		.def("noqbs", &Qroutine::noqbs, "Returns the number of Q bits that the Q routine holds")

        .def("compile", &Qroutine::compile,
            "Compiles this quantum block to generate quantum solver code")

		.def("toString", &Qroutine::toString, "returns string presentation of this Q routine object")
		.def("toString", [](Qroutine& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qroutine& o) { return o.toString(); })
		.def("__str__", [](Qroutine& o) { return o.toString(); })

		.def("add", &Qroutine::add, "Add a quantum evaluation to this Q routine.")
		.def("solution", &Qroutine::solution, "For added sample set(s), returns a string represnting 'at' solution of operands of statements within this Q routine")
		.def("compute", &Qroutine::compute, "Returns computed sample set with all solutions for the Q routine logic")
		.def("reset", &Qroutine::reset, "Clear all solution samples")

		.def(py::self << Qexpr<Qbit>())
		.def(py::self << Qexpr<Qbool>())
		.def(py::self << Qexpr<Qbin>())
		.def(py::self << Qexpr<Qwhole>())
		.def("_a", [](Qblock& o, const Qexpr<Qwhole>& right) { o << right; return o; })

		.def(py::self << Qassign<Qbit>())
		.def(py::self << Qassign<Qbool>())
		.def(py::self << Qassign<Qbin>())
		.def(py::self << Qassign<Qwhole>())
		.def("_a", [](Qblock& o, const Qassign<Qwhole>& right) { o << right; return o; });


/*--- Qbinder.h definitions---*/
	py::class_<Qbinder>(m, "Qbinder",
		R"pbdoc( Quantum binder is a Q definition with a sequence of logical 
				 quantum statements organized as Q block)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qbinder&>())
		.def(py::init<const Qevaluations&>())

		.def("noqbs", &Qbinder::noqbs, "Returns the number of Q bits that the Q binder holds")

		.def("toString", &Qbinder::toString, "returns string presentation of this Q binder object")
		.def("toString", [](Qbinder& o, bool decomposed) { return o.toString(decomposed); })
		.def("toString", [](Qbinder& o) { return o.toString(); })
		.def("__str__", [](Qbinder& o) { return o.toString(); })

		.def("add", &Qbinder::add, "Add a quantum evaluation to this Q binder the combination of node values should be different for each sample")
		.def("solution", &Qbinder::solution, "For added sample set(s), returns a string represnting 'at' solution of operands of statements within this Q binder")
		.def("solutions", &Qbinder::solutions, "For added sample set(s), returns a string represnting all solutions of operands contained in the Q binder")
		.def("reset", &Qbinder::reset, "Clear all solution samples")

		.def(py::self << Qbit())
		.def(py::self << Qbool())
		.def(py::self << Qbin())
		.def(py::self << Qwhole())
		
//		.def("__getitem__", [](Qbinder& self, size_t at) { return self[at].get(); })
//		.def("__getitem__", [](Qbinder& self, string id) { return self[id].get(); });
		.def("__getitem__", [](Qbinder& self, size_t at) { return &(*self[at]); })
		.def("__getitem__", [](Qbinder& self, string id) { return &(*self[id]); });


/*--- Qevaluation.h definitions ---*/
    py::class_<Qevaluation>(m, "Qevaluation",
                            R"pbdoc(A quantum sample is defined as a dictionary (map) of definition nodes and their values. The node names are defined by coresponding compiled quantum statement object sent to a quantum solver)pbdoc")
        .def(py::init<>())
        .def(py::init<const Qevaluation&>())
        .def(py::init<const Qsample&, double>())
        .def(py::init<double, const Qsample&>())
    
        .def("reset", &Qevaluation::reset, "Reset this evaluation to initial values")
        .def("isValid", &Qevaluation::isValid, "The evaluation is valid if the sample set has elements")
    
        .def(py::self += py::self, "Merge this and right evaluation with same or different elements")
        .def(py::self + py::self, "Return a quantum evaluation with merge elements from this and right object")

        .def("sample", [](const Qevaluation& o) { return o.sample(); })
        .def("energy", [](const Qevaluation& o) { return o.energy(); });

/*--- Qsolver.h definitions ---*/
	py::class_<Qsolver>(m, "Qsolver",
                        R"pbdoc(Quantum solver abstraction evaluates solutions for a compiled quantum statement.)pbdoc")
    .def_static("Active", static_cast<void (*)(Qsolver::Sp)>(&Qsolver::Active),
                "Sets an active quantum solver to be used as a default solver")
    .def_static("Active", static_cast<Qsolver::Sp (*)()>(&Qsolver::Active),
                "Returns an active quantum solver used as a default solver")
    .def("solution", &Qsolver::solution,
         "Override to return quantum evaluations for a compiled quantum statement");

/*--- Qcompiler.h definitions ---*/
    py::class_<Qcompiler>(m, "Qcompiler",
                          R"pbdoc(An abstration of quantum operation compiler is an integration point for conversion of dann5 quantum peration to target quantum solver presentation, like qiskit in case of IBM or Qubo in case of D-Wave)pbdoc")
        .def("compile", &Qcompiler::compile, "A method to be executed by active Qsolver when solving a quantum statement.");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}


