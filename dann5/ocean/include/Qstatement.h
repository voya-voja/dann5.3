#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <set>

#include <Qdef.h>
#include <Qubo.h>
#include <Qsolver.h>

using namespace std;


namespace dann5 {
	namespace ocean {

		// Quantum statement is a coupling of a Q expression with Q variables as 
		// arguments
		class Qstatement
		{
		public:
			// Qstatement's shared pointer 
			typedef shared_ptr<Qstatement> Sp;

			// Default constructor creates Q statement witout an expression or 
			// corresponding arguments
			// result definition is an empty string
			Qstatement();

			// copy constructor
			Qstatement(const Qstatement&);

			// desruct the Q statement with its members
			~Qstatement();

			// Override to return a number of bit levels in this Q statement
			virtual size_t noqbs() const = 0;

			// Override to return a qubo representation of this Q statement, 
			// if not finalized, return a full qubo without substitutions
			// if finalized, return a qubo with all constant variables 
			// substituted by their values
			// when applicable, if forBit is specified (differs from cAllBits),
			// return qubo only  for the specified bit level
			virtual Qubo qubo(bool finalized = true, 
								size_t forBit = cAllBits) const = 0;

			// Override to return a string representation of this Q statement, 
			// if not decomposed, return a statement as initially specified
			// if decomposed, return bit-level logic of the statement
			// when applicable, if forBit is specified (differs from cAllBits),
			// return decomposed logic only for the specified bit level
			virtual string toString(bool decomposed = false, 
									size_t forBit = cAllBits) const = 0;

			// Override to return a shared_pointer of a cloned instance of this
			// Q statement
			virtual Qstatement::Sp clone() const = 0;

			// Override to solve this Q statement and return a Qsolver::Samples
			// with all solutions
			virtual Qsolver::Samples compute() = 0;

			// Updates the number of added samples to this Qs statement.
			// The sample set nodes have to correspond to the Q statement 
			// operands.
			// Override to apply the list of solutions samples values to 
			// Q statement's operands,
			virtual void add(const Qsolver::Samples& samples);

			// Returns a string representation of all solutions calculated by
			// compute method
			virtual string solve();

			// Override to provide a string representation of all solutions of 
			// this Q statement. Return empty string, when solutions samples 
			// are not assigned by add() or solve method
			virtual string solutions() const = 0;

			// Override to provide a string representation of an 'at' solution
			// out of all solutions for this Q statement. Return empty string, 
			// when solutions samples are not assigned by add() or solve method
			virtual string solution(size_t at) const = 0;

			// Resets the number of addedd solution samples.
			// Override to reset the operands of this Qstatement
			virtual void reset();

		protected:
			// Return the number of added solutions to this Q statement.
			const size_t& noSolutions() const { return mNoSolutions; };

		private:
			size_t					mNoSolutions = 0;
		};

		// An std::vector containing a list of Q statements
		typedef vector<Qstatement::Sp> Qstatements;
	};
};