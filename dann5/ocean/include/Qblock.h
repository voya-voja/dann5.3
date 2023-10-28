#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {


		// A Q block is a lexical structure of quantum code, which is grouped
		// together. Qblock consist of one or more quantum statements. Qblock 
		// is a quantum statement, too, permitting block-structured quantum 
		// programming through the creation of blocks nested within other blocks
		class Qblock : public Qstatement
		{
		public:
			// Qblock's shared pointer 
			typedef shared_ptr<Qblock> Sp;

			// default constructor of an instance without Q statements
			Qblock() {};

			// creates a named Q block with a given list of Q statements
			Qblock(const Qstatements& statements) 
				:Qstatement(), mStatements(statements) {};

			// Copy constructor
			Qblock(const Qblock& right) 
				:Qstatement(right), mStatements(right.mStatements) {};

			// destructor
			~Qblock() {};

			// Return a Qstatement's shared pointer pointing to a copy of this
			// object 
			virtual Qstatement::Sp clone() const 
				{ return Qblock::Sp(new Qblock(*this)); };

			// Returns the maximal number of qbit levels amongst containg 
			// statements
			virtual size_t noqbs() const noexcept;

			// Returns a Qubo presentation of statements within this Q block 
			// if not finalized, returns a full qubo without substitutions
			// if finalized, return a qubo with all constant variables (with 
			// all Q bits in deterministic states) substituted by their values
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns qubo only  for the specified bit level
			virtual Qubo qubo(bool finalized = true, 
								size_t forBit = cAllBits) const;
            
            // Returns a Qubos vector containing qubo representation
            // of this Q block fractionate into sub-qubos with no more than
            // specified number of nodes
            virtual Qubos qubos(size_t noNodes) const;

			// Returns a string representation of Q block statements
			// if not decomposed, returns statements as initially specified
			// if decomposed, returns bit-level logic of the statements
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns decomposed logic only for the specified bit level
			virtual string toString(bool decomposed = false,
				size_t forBit = cAllBits) const;

			// Adds a evaluation set containing nodes with solutions values, the
			// nodes should correspond to operands of statements within this 
			// Q block
			virtual void add(const Qevaluations& evaluations);

			// For added sample set(s), returns a string representation of all
			// solutions of operands of statements within this Q block
			virtual string solutions() const;

			// For added sample set(s), returns a string represnting 'at'
			// solution of operands of statements within this Q block
			virtual string solution(size_t at) const;

			// Returns computed evaluation set with all solutions for the Q block
			// logic
			virtual Qevaluations compute();

			// Resets Q block statements into their initial state without 
			// added solutions
			virtual void reset();

			// An insertion operator (<<) to add a new statement into this 
			// Q block and returns the reference to 'this' object
			Qblock& operator<<(const Qstatement& right);

			// An insertion operator (<<) to add a new statement into a copy of this 
			// Q block and returns the reference to 'this' object
			Qblock operator<<(const Qstatement& right) const;

			// Comma Operator class allows use of comma operator to insert 
			// operands into specified Q block
			class CommaOp
			{
			public:
				// construct object with pointer to a Q block
				CommaOp(Qblock* pBlock) : mpBlock(pBlock) {};

				// copy constructor
				CommaOp(const CommaOp& right) : mpBlock(right.mpBlock) {};

				// An insertion operator ',' to add a new statement into 
				// specified Q block
				CommaOp operator,(const Qstatement& right)
				{ 
					*mpBlock << right; 
					return *this;
				};

			protected:
			private:
				Qblock* mpBlock;	// pointer to Q block object
			};

			// An assignment of a Q statement to a Q block, which can
			// be followed by list of other Q statements separated by
			// comma operator
			CommaOp operator=(const Qstatement& right);

			// Insert string representation of a Q block into an output stream
			friend std::ostream& operator << (std::ostream&, const Qblock&);

		protected:
		private:
			Qstatements mStatements;	// a list of Q block statements
		};

		// Insert string representation of a Q block into an output stream
		std::ostream& operator << (std::ostream&, const Qblock&);

		// An std::vector containing a list of Q blocks
		typedef vector<Qblock::Sp> Qblocks;
	};
};
