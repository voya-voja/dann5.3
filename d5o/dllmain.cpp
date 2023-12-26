// dllmain.cpp : Defines the entry point for the DLL application.

// pybind11 � Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <Qubo.h>

#include <D5QuboSolver.h>
#include <QuboAnalyzer.h>
#include <QuboCompiler.h>
#include <DwaveSolver.h>

#include <pybind11/stl.h>

#ifdef ABSTRACT_EXAMPLE
class Animal {
public:
	virtual ~Animal() { }
	virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
	std::string go(int n_times) override {
		std::string result;
		for (int i = 0; i < n_times; ++i)
			result += "woof! ";
		return result;
	}
};

class PyAnimal : public Animal {
public:
	/* Inherit the constructors */
	using Animal::Animal;

	/* Trampoline (need one for each virtual function) */
	std::string go(int n_times) override {
		PYBIND11_OVERRIDE_PURE(
			std::string, /* Return type */
			Animal,      /* Parent class */
			go,          /* Name of function in C++ (must match Python name) */
			n_times      /* Argument(s) */
		);
	}
};

std::string call_go(Animal* animal) {
	return animal->go(3);
}

/* Python Abstract example code
* 
class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

def animal():
    d = Dog()
    print(call_go(d))
    c = Cat()
    print(call_go(c))
*/
#endif // ABSTRACT_EXAMPLE

using namespace dann5;
using namespace dann5::ocean;


#define VERSION_INFO "3.0.0"

namespace py = pybind11;

#ifdef _DEBUG
PYBIND11_MODULE(d5o_d, m) {
#else
PYBIND11_MODULE(d5o, m) {
#endif

#ifdef ABSTRACT_EXAMPLE
	py::class_<Animal, PyAnimal /* <--- trampoline*/>(m, "Animal")
		.def(py::init<>())
		.def("go", &Animal::go);

	py::class_<Dog, Animal>(m, "Dog")
		.def(py::init<>());

	m.def("call_go", &call_go);
#endif // ABSTRACT_EXAMPLE

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

/*--- Qubo.h definitions ---*/
	m.def("Qubo", []() { return Qubo(); }, 
		R"pbdoc( Qubo is a dictionary (map) of a solution graph for a binary objective
				 function where a node pair is a key (Qkey) mapped into its bias, which
				 corespondes to Quantum energy values for that node
				 Qubo corresponds to DWave's dimod.BinaryQuadraticModel class, represented
				 as an upper-diagonal matrix Q, where diagonal terms are the linear
				 coefficients (key pairs with the same Quantum node name) and the nonzero
				 off-diagonal terms the quadratic coefficients(graph branches, with
				 different mode names.)pbdoc");

/*--- QuboCompiler.h definitions ---*/
	py::class_<QuboCompiler, Qcompiler>(m, "QuboCompiler",
		R"pbdoc( Constructor sets the QUBO compiler mode. If finalized, compiles
				 an quantum operation into an executable qubo format with all
				 constant variables substituted by their values and recalcualtes
				 parameters of linear nodes. If not finalized, creates a full
				 qubo translation without substitutions.)pbdoc")

		.def(py::init<>())
		.def(py::init<bool>())

		.def("compile", static_cast<void (QuboCompiler::*)(const Qop&)>(&QuboCompiler::compile),
			R"pbdoc( Compiles an operation into its QUBO transalation. When the
					 compiler is in finalized mode, creates an executable qubo 
					 format with all constant variables substituted by their 
					 values and recalcualtes parameters of linear nodes. 
					 Otherwise, creates a full qubo translation without
					 substitutions.)pbdoc")
		.def("qubo", static_cast<const Qubo & (QuboCompiler::*)() const>(&QuboCompiler::qubo),
			R"pbdoc(Returns a qubo representation of last compiled quantum operation.)pbdoc")
		.def("reset", &QuboCompiler::reset, "Resets the compiler into its initial state")

		.def("finalized", static_cast<bool (QuboCompiler::*)() const>(&QuboCompiler::finalized),
								 "Returns finalized status mode")
		.def("finalized", static_cast<void (QuboCompiler::*)(bool)>(&QuboCompiler::finalized),
			"Changes finalized status mode and resets the compiler object");

/*--- QuboAnalyzer.h definitions ---*/
	py::class_<QuboAnalyzer, QuboAnalyzer::Sp>(m, "QuboAnalyzer",
		R"pbdoc( Qubo analyzer of a given qubo.)pbdoc")

		.def(py::init<const Qubo&>())

		.def("nodes", &QuboAnalyzer::nodes, "returns a list of linear nodes.")
		.def("branches", &QuboAnalyzer::branches, "returns a list of branches, i.e. binary lements")
		.def("branchesNo", &QuboAnalyzer::branchesNo, "returns a number of branches, i.e. binary QUBO elements")
		.def("nodesNo", &QuboAnalyzer::nodesNo, "returns a number of linear nodes in a given QUBO instance")
		.def("qubo", static_cast<const Qubo & (QuboAnalyzer::*)() const>(&QuboAnalyzer::qubo), "returns the given QUBO instance")
		.def("qubo", static_cast<void (QuboAnalyzer::*)(const Qubo&)>(&QuboAnalyzer::qubo), "set a new qubo")
		.def("chainStrength", &QuboAnalyzer::chainStrength, "returns suggested chain streinght for a given QUBO instance")
		.def("reset", &QuboAnalyzer::reset, "reset qubo, and nodes and branches analyses");

/*--- D5QuboSolver.h definitions ---*/
	py::class_<D5QuboSolver, D5QuboSolver::Sp, Qsolver, QuboAnalyzer>(m, 
		"D5QuboSolver",
			R"pbdoc( Qubo solver ia a quantum annealing simulator
			and a specialization of a Quantum analyzer. It evaluates solutions
			for a given QUBO.)pbdoc")

		.def(py::init<>(), R"pbdoc( Defual construtor
			will process just evaluations with the lowest energy.)pbdoc")
		.def(py::init<bool>(), R"pbdoc( A construtor with an optinal flag
			to process just evaluations with the lowest energy. When the
			argument is 'false' the solver will process all the evaluation
			regardless of their evaluated energy)pbdoc")

		.def("solution", static_cast<Qevaluations (D5QuboSolver::*)
								(const Qstatement&)>(&D5QuboSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given quantum statement,
					Depending on the solver initialization, the returned quantum
					evaluations can be just those with lowes assessed energy, or
					the full set of all evaluations.)pbdoc")
		.def("solution", static_cast<Qevaluations(D5QuboSolver::*)(const Qubo&)>
													(&D5QuboSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given QUBO.
					Depending on the solver initialization, the returned quantum
					evaluations can be just those with lowes assessed energy, or
					the full set of all evaluations.s)pbdoc")
		.def("solution", static_cast<const Qevaluations & (D5QuboSolver::*)
											() const>(&D5QuboSolver::solution), 
			"Returns quantum evaluations")

		.def("minEnergy", &D5QuboSolver::minEnergy, 
			"returns minimal evaluated energy")
		.def("reset", &D5QuboSolver::reset, 
			R"pbdoc( reset solutions, assessed minimum energy, qubo, and nodes
					 and branches of analyses)pbdoc");

/*--- DwaveSolver.h definitions ---*/
	py::class_<DwaveSolver, DwaveSolver::Sp, Qsolver, QuboAnalyzer, PyDwaveSolver>(m,
		"DwaveSolver", R"pbdoc( D-Wave solver is an adapter-superclass
			of dann5 quantum solver (Qsolver) and all implementations of D-Wave
			quantum annealing simulator and samplers. It evaluates solutions for
			a given QUBO or given Qstatement..)pbdoc")

		.def(py::init<>(), R"pbdoc( Defual construtor
			will process just evaluations with the lowest energy.)pbdoc")
		.def(py::init<bool>(), R"pbdoc( A construtor with an optinal flag
			to process just evaluations with the lowest energy. When the
			argument is 'false' the solver will process all the evaluation
			regardless of their evaluated energy)pbdoc")

		.def("lowest", static_cast<bool (DwaveSolver::*)() const>
			(&DwaveSolver::lowest),
			"returns a lowest energy flag")
		.def("lowest", static_cast<void (DwaveSolver::*)(bool)>
			(&DwaveSolver::lowest),
			"sets a lowest energy flag")

		.def("solution", static_cast<Qevaluations(DwaveSolver::*)
								(const Qstatement&)>(&DwaveSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given quantum statement.
					Depending on the solver initialization, the returned quantum
					evaluations can be just those with lowes assessed energy, or
					the full set of all evaluations.s)pbdoc")
		.def("solution", static_cast<Qevaluations(DwaveSolver::*)
			(const Qubo&)>(&DwaveSolver::solution),
			R"pbdoc(Returns quantum evaluations for a given QUBO.
					Depending on the solver initialization, the returned quantum
					evaluations can be just those with lowes assessed energy, or
					the full set of all evaluations.s)pbdoc")
		.def("solution", static_cast<const Qevaluations& (DwaveSolver::*)
											() const>(&DwaveSolver::solution), 
			"Returns quantum evaluations")
		.def("solution", static_cast<void (DwaveSolver::*)(const Qevaluations&)>
													(&DwaveSolver::solution),
			"Sets quantum evaluations")

		.def("minEnergy", static_cast<double (DwaveSolver::*)() const>
													(&DwaveSolver::minEnergy), 
			"returns a minimal evaluated energy")
		.def("minEnergy", static_cast<void (DwaveSolver::*)(double)>
													(&DwaveSolver::minEnergy), 
			"sets a minimal evaluated energy")

		.def("reset", &DwaveSolver::reset, 
			R"pbdoc( reset solutions, assessed minimum energy, qubo,
					 and nodes and branches of analyses)pbdoc")
		.def("solve", &DwaveSolver::solve, 
			R"pbdoc( Override to run solve algorith 
					 on a coresponding D-Wave quantum annealing simulator or sampler
					 Throws logic_error: 'implementation reired')pbdoc");
}
