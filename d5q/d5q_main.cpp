// dllmain.cpp : Defines the entry point for the DLL application.

// pybind11 ? Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>


#include <Circuits.h>

#include <CircuitCompiler.h>
#include <QiskitSolver.h>

#include <pybind11/stl.h>

using namespace dann5;
using namespace dann5::qiskit;


#define VERSION_INFO "3.0.0"

namespace py = pybind11;

#ifdef _DEBUG
PYBIND11_MODULE(d5q_d, m) {
#else
PYBIND11_MODULE(d5q, m) {
#endif

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif

	m.doc() = R"pbdoc(
        d5o plugin
        -----------------------

        .. currentmodule:: d5o

        .. autosummary::
           :toctree: _generate
    )pbdoc";

	/*--- Circuits.h definitions ---*/
	py::class_<QuReg>(m, "QuReg",
		R"pbdoc(A QuantumRegister wraper.)pbdoc")

		.def(py::init<>(), 
			R"pbdoc(default constructor  creates 0 size instance
					 with an empty name)pbdoc")
		.def(py::init<size_t, const string& >(),
			R"pbdoc(Construct an instance by specifying size and name
					 of a quantum register.)pbdoc")
		.def(py::init<const QuantumRegister&>(),
			R"pbdoc(Construct an instance of a wraper for the quantum register.
					)pbdoc")

		.def("numQubits", [](QuReg& self) { return self.mNumQubits; },
			R"pbdoc(returns a size of the quantum register)pbdoc")

		.def("name", [](QuReg& self) { return self.mName; },
			R"pbdoc(returns a name of the quantum register)pbdoc")

		.def("QuantumRegister", [](QuReg& self)
			{ return QuantumRegister(self); },
			R"pbdoc(returns a QuantumRegister instance with same size and name.
					)pbdoc")

		.def("__str__", [](QuReg& self) { return string(self); })
		.def("__repr__", [](QuReg& self) { return string(self); });

	py::class_<QuantumBit>(m, "QuantumBit",
		R"pbdoc(A Qu(antum)-Bit wraper.)pbdoc")

		.def(py::init<>(), 
			R"pbdoc(default constructor creates an instance with 
					an default quantum register and position set to 0)pbdoc")
		.def(py::init<const QuReg &, size_t >(),
			R"pbdoc(Construct an instance by specifying a quantum register and 
					the position of the quantum bit within the register.)pbdoc")
		.def(py::init<const Qubit&>(),
			R"pbdoc(Construct an instance of a wraper for the quantum bit.)pbdoc")

		.def("register", [](QuantumBit& self) { return QuReg(self.mRegister); },
			R"pbdoc(returns a quantum bit register as a QuReg instance.)pbdoc")

		.def("at", [](QuantumBit& self) { return self.mAt; },
			R"pbdoc(returns a quantum bit position within the register.)pbdoc")

		.def("Qubit", [](QuantumBit& self) { return Qubit(self); },
			R"pbdoc(returns a Qubit instance with the same register and
					the same position.)pbdoc")

		.def("__str__", [](QuantumBit& self) { return string(self); })
		.def("__repr__", [](QuantumBit& self) { return string(self); });

	py::class_<ClReg>(m, "ClReg",
		R"pbdoc(A ClassicalRegister wraper.)pbdoc")

		.def(py::init<>(),
			R"pbdoc(default constructor  creates 0 size instance
					 with an empty name)pbdoc")
		.def(py::init<size_t, const string& >(),
			R"pbdoc(Construct an instance by specifying size and name
					 of a classical register.)pbdoc")
		.def(py::init<const ClassicalRegister&>(),
			R"pbdoc(Construct an instance of a wraper for the classical register.
					)pbdoc")

		.def("numClbits", [](ClReg& self) { return self.mNumClbits; },
			R"pbdoc(returns a size of the classical register)pbdoc")

		.def("name", [](ClReg& self) { return self.mName; },
			R"pbdoc(returns a name of the classical register)pbdoc")

		.def("ClassicalRegister", [](ClReg& self)
			{ return ClassicalRegister(self); },
			R"pbdoc(returns a ClassicalRegister instance with same size and name.
					)pbdoc")

		.def("__str__", [](ClReg& self) { return string(self); })
		.def("__repr__", [](ClReg& self) { return string(self); });

	py::class_<ClassicalBit>(m, "ClassicalBit",
		R"pbdoc(A Cl(assical)-Bit wraper.)pbdoc")

		.def(py::init<>(),
			R"pbdoc(default constructor creates an instance with 
					an default classical register and position set to 0)pbdoc")
		.def(py::init<const ClReg&, size_t >(),
			R"pbdoc(Construct an instance by specifying a classical register and
					the position of the classical bit within the register.
					)pbdoc")
		.def(py::init<const Clbit&>(),
			R"pbdoc(Construct an instance of a wraper for the classical bit.
					)pbdoc")

		.def("register", [](ClassicalBit& self) { return ClReg(self.mRegister); },
			R"pbdoc(returns a classical bit register as a QuReg instance.)pbdoc")

		.def("at", [](ClassicalBit& self) { return self.mAt; },
		R"pbdoc(returns a classical bit position within the register.)pbdoc")

		.def("Clbit", [](ClassicalBit& self) { return Clbit(self); },
			R"pbdoc(returns a Clbit instance with the same register and
					the same position.)pbdoc")

		.def("__str__", [](ClassicalBit& self) { return string(self); })
		.def("__repr__", [](ClassicalBit& self) { return string(self); });

	py::class_<Instruction, Instruction::Sp>(m, "Instruction",
		R"pbdoc( Qiskit instruction identifies qiskit gate and a list of its
         arguments as qiskit qu(antum)-bits or cl(asical)-bits.)pbdoc")

		.def(py::init<const string&>(), R"pbdoc( Construct the instruction with
				a name corresponding to a name of one of qiskit gates: measure, 
				swap, h, reset, x, cx, or ccx.)pbdoc")
		.def(py::init<const string&, const Qubits&>(), 
				R"pbdoc( Construct the instruction with a name corresponding to
				a name of one of qiskit gates: measure, swap, h, reset, x, cx,
				or ccx, with the list of instruction's qu(antum)-bits.)pbdoc")
		.def(py::init<const string&, const Qubits&, const Clbits>(),
			R"pbdoc( Construct the instruction with a name corresponding to
				a name of one of qiskit gates: measure, swap, h, reset, x, cx,
				or ccx, with the lists of instruction's qu(antum)-bits and 
				cl(assical)-bits.)pbdoc")

		.def("name", static_cast<const string & (Instruction::*)() const>(&Instruction::name),
			R"pbdoc(Returns an instruction name.)pbdoc")
		.def("name", static_cast<void (Instruction::*)(const string&)>(&Instruction::name),
			R"pbdoc(Sets an instruction name.)pbdoc")

		.def("qubits", static_cast<const Qubits & (Instruction::*)() const>(&Instruction::qubits),
			R"pbdoc(Returns a list of instruction's qu(antum)-bits.)pbdoc")
		.def("qubits", static_cast<void (Instruction::*)(const Qubits&)>(&Instruction::qubits),
			R"pbdoc(Sets a list of instruction's qu(antum)-bits.)pbdoc")
		.def("num_qubits", &Instruction::num_qubits, 
						"Returns the number of instruction's qu(antum)-bits.")

		.def("clbits", static_cast<const Clbits & (Instruction::*)() const>(&Instruction::clbits),
			R"pbdoc(Returns a list of instruction's cl(assical)-bits.)pbdoc")
		.def("clbits", static_cast<void (Instruction::*)(const Clbits&)>(&Instruction::clbits),
			R"pbdoc(Sets a list of instruction's cl(assical)-bits.)pbdoc")
		.def("num_clbits", &Instruction::num_clbits,
			"Returns the number of instruction's cl(assical)-bits.")

		.def("append", &Instruction::append,
			R"pbdoc(appends the qu(antum)-bits to the existing list, and
					optionally appends the cl(assical)-bits to the existing
					list.)pbdoc")
		.def("__str__", [](Instruction& self) { return string(self); })
		.def("__repr__", [](Instruction& self) { return string(self); });

	py::class_<D5circuit>(m, "D5circuit",
		R"pbdoc(dann5 circuit is a custom defined circuit.)pbdoc")

		.def(py::init<>(), R"pbdoc(Default constructor.)pbdoc")
		.def(py::init<const D5circuit&>(), R"pbdoc( Copy constructor.)pbdoc")

		.def("instructions", static_cast<const Instructions & (D5circuit::*)() const>(&Circuit::instructions),
			R"pbdoc(returns reference to list of instructions of this circuit.)pbdoc")

		.def("inputs", static_cast<const QuRegMap& (D5circuit::*)() const>(&D5circuit::inputs),
			R"pbdoc(Returns a list of the circuit's input quantum registers.)pbdoc")
		.def("inputs", static_cast<void (D5circuit::*)(const QuRegMap&)>(&D5circuit::inputs),
			R"pbdoc(Sets a list of the circuit's input quantum registers.)pbdoc")


		.def("outputs", static_cast<const QuRegMap& (D5circuit::*)() const>(&D5circuit::outputs),
			R"pbdoc(Returns a list of the circuit's output quantum registers.)pbdoc")
		.def("outputs", static_cast<void (D5circuit::*)(const QuRegMap&)>(&D5circuit::outputs),
			R"pbdoc(Sets a list of the circuit's output quantum registers.)pbdoc")

		.def("operands", static_cast<const QuOperandsMap& (D5circuit::*)() const>(&Circuit::operands),
			R"pbdoc(Returns a list of the circuit's operands.)pbdoc")
		.def("operands", static_cast<void (D5circuit::*)(const QuOperandsMap&)>(&Circuit::operands),
			R"pbdoc(Sets a list of the circuit's operands.)pbdoc")

		.def("initOperands", static_cast<const QuOperandsMap& (D5circuit::*)() const>(&D5circuit::initOperands),
			R"pbdoc(Returns a list of the circuit's operands requiring initialization.)pbdoc")
		.def("initOperands", static_cast<void (D5circuit::*)(const QuOperandsMap&)>(&D5circuit::initOperands),
			R"pbdoc(Sets a list of the circuit's operands requiring initialization.)pbdoc")
			
		.def("nodesNo", &D5circuit::nodesNo,
			R"pbdoc(Number of logical quantum nodes required for this circuit.)pbdoc")
		.def("reset", &D5circuit::reset,
			R"pbdoc(Resets a circuit into initial state.)pbdoc")
		.def("draw", &D5circuit::draw,
			R"pbdoc(Draws a circuit using ASCII characters.)pbdoc");

	/*--- CircuitCompiler.h definitions ---*/
	py::class_<CircuitCompiler, Qcompiler>(m, "CircuitCompiler",
		R"pbdoc( A specialization of quantum operation compiler that translates the
			operation definition into Qiskit cicuit list of instructions.)pbdoc")

		.def(py::init<>(),
			R"pbdoc( Default constructor sets the Circuit compiler into compile stage.)pbdoc")

		.def("compile", static_cast<void (CircuitCompiler::*)(const Qop&)>(&CircuitCompiler::compile),
			R"pbdoc( Compiles a quantum into its Qiskit circuit transalation.)pbdoc")

		.def("circuit", static_cast<const D5circuit& (CircuitCompiler::*)() const>(&CircuitCompiler::circuit),
			R"pbdoc(Returns a constant reference to a circuit representation of last compiled quantum operation.)pbdoc")
		.def("circuit", static_cast<D5circuit& (CircuitCompiler::*)()>(&CircuitCompiler::circuit),
			R"pbdoc(Returns a reference to a circuit representation of last compiled quantum operation.)pbdoc")
		
		.def("reset", &CircuitCompiler::reset, "Resets the compiler into its initial state")

		.def("stage", static_cast<CircuitCompiler::Stage (CircuitCompiler::*)() const>(&CircuitCompiler::stage),
			"Returns finalized status mode")
		.def("stage", static_cast<void (CircuitCompiler::*)(CircuitCompiler::Stage)>(&CircuitCompiler::stage),
			"Changes finalized status mode and resets the compiler object");

	/*--- QiskitSolver.h definitions ---*/
	py::class_<QiskitSolver, QiskitSolver::Sp, Qsolver, PyQiskitSolver>(m,
		"QiskitSolver", R"pbdoc( Qiskit solver is an adapter-superclass of dann5 quantum solver
							(Qsolver) and all implementations of Qiskit quantum simulators and
							backends. It evaluates solutions for a given Circuit or a given
							Qstatement.)pbdoc")

		.def(py::init<>(), R"pbdoc( Defual construtor.)pbdoc")
/*
		.def("lowest", static_cast<bool (QiskitSolver::*)() const>
			(&QiskitSolver::lowest),
			"returns a lowest energy flag")
		.def("lowest", static_cast<void (QiskitSolver::*)(bool)>
			(&QiskitSolver::lowest),
			"sets a lowest energy flag")
*/
		.def("solution", static_cast<Qevaluations(QiskitSolver::*)
			(const Qstatement&)>(&QiskitSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given quantum statement.)pbdoc")
		.def("solution", static_cast<Qevaluations(QiskitSolver::*)
			(const D5circuit&)>(&QiskitSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given dann5 circuit.)pbdoc")
		.def("solution", static_cast<const Qevaluations & (QiskitSolver::*)
			() const>(&QiskitSolver::solution),
			"Returns quantum evaluations")
		.def("solution", static_cast<void (QiskitSolver::*)(const Qevaluations&)>
			(&QiskitSolver::solution),
			"Sets quantum evaluations")

		.def("circuit", &QiskitSolver::circuit,
			R"pbdoc(Resets evaluated solutions and the coresponding circuit.)pbdoc")

		.def("reset", &QiskitSolver::reset,
			R"pbdoc(Resets evaluated solutions and the coresponding circuit.)pbdoc")
		.def("solve", &QiskitSolver::solve,
			R"pbdoc(Override to run solve algorith on a coresponding Qiskit quantum
					simulator or backend.)pbdoc");
}
