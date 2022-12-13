#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qcell.h>
#include <Qop.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quntum and is a binary operation
		class QcellOp : public Qcell, public Qop
		{
		public:
			// Qeq's shared pointer 
			typedef shared_ptr<QcellOp> Sp;

			// Instantiate a Quantum cell operation instance with identity and number of inputs
			QcellOp(const string& id, size_t size);

			// Copy constructor
			QcellOp(const QcellOp& right) : Qcell(right), Qop(right) {};

			// Destruct Quantum cell operation
			~QcellOp() {};

			// overrided inputs setter to sync output value with this operation 
			virtual void inputs(const Qdefs&);

			// overrides output setter to sync its value and value of this operation
			virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

			// Returns the largest number of Q bits of all its inputs
			virtual std::size_t noqbs() const noexcept { return Qop::noqbs(); };

			// set and get the value of this Q cell
			virtual void value(Qvalue v);	// throws logic_error exception
			virtual Qvalue value() const;
			virtual Qvalue value();

			// convert Quantum cell operation expration into a string as any other operation
			virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const {
				return Qop::toString(decomposed, forBit);
			};

			// Override to set solution values from the sample set for this deffinition
			virtual void add(const Qsolver::Samples& samples);

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Reset the quantum operation into its initial state without solutions by clearing 
			// all solution samples
			virtual void reset();

		protected:
			virtual Qvalue calculate(const Qvalues& values) const = 0;

		private:
		};

		// A Quntum null operation is an empty cell operation
		class QnullCellOp : public QcellOp
		{
		public:
			// Qeq's shared pointer 
			typedef shared_ptr<QnullCellOp> Sp;

			// Instantiate an Quantum null cell operation instance with its identity
			QnullCellOp() : QcellOp("!", 0) {};

			// Copy constructor
			QnullCellOp(const QnullCellOp& right) : QcellOp(right) {};

			// Destruct Qeq with shared pointers to its Qdef
			~QnullCellOp() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QnullCellOp(*this)); };

			// return an empty Qubo presentation of Q null cell operation
			virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const {
				return Qubo();
			};

		protected:
			virtual Qvalue calculate(const Qvalues& values) const { return cSuperposition; };

		private:
		};

		// A Quntum operator is an abstration of unnary operation
		class Qoperator : public QcellOp
		{
		public:
			// Qeq's shared pointer 
			typedef shared_ptr<Qoperator> Sp;

			// Instantiate an Quantum operator instance with its identity
			Qoperator(const string& id) : QcellOp(id, 1) {};

			// Copy constructor
			Qoperator(const Qoperator& right) : QcellOp(right) {};

			// Destruct Qeq with shared pointers to its Qdef
			~Qoperator() {};

			// overrides output getter to to return utput operand when operation is an output
			virtual Qdef::Sp output(size_t forBit = cAllBits) const;

			// convert Quantum operator expration into a string
			virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const;

		protected:

		private:
		};

		// A Quntum equal is an operator
		class Qeq : public Qoperator
		{
		public:
			// Qeq's shared pointer 
			typedef shared_ptr<Qeq> Sp;

			// Instantiate Qeq instance with '&' identity
			Qeq() : Qoperator(EqQT::cMark) {};

			// Copy constructor
			Qeq(const Qeq& right) : Qoperator(right){};

			// Destruct Qeq instance
			~Qeq() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qeq(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum not-equal is an operator
		class Qneq : public Qoperator
		{
		public:
			// Qneq's shared pointer 
			typedef shared_ptr<Qneq> Sp;

			// Instantiate Qneq instance with '&' identity
			Qneq() : Qoperator(NeqQT::cMark) {};

			// Copy constructor
			Qneq(const Qneq& right) : Qoperator(right) {};

			// Destruct Qneq with shared pointers to its Qdef
			~Qneq() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qneq(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum less-than is an operator
		class Qlt : public Qoperator
		{
		public:
			// Qlt's shared pointer 
			typedef shared_ptr<Qlt> Sp;

			// Instantiate Qlt instance with '&' identity
			Qlt() : Qoperator(LtQT::cMark) {};

			// Copy constructor
			Qlt(const Qlt& right) : Qoperator(right) {};

			// Destruct Qlt with shared pointers to its Qdef
			~Qlt() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qlt(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum less-equal is an operator
		class Qle : public Qoperator
		{
		public:
			// Qle's shared pointer 
			typedef shared_ptr<Qle> Sp;

			// Instantiate Qle instance with '&' identity
			Qle() : Qoperator(LeQT::cMark) {};

			// Copy constructor
			Qle(const Qle& right) : Qoperator(right) {};

			// Destruct Qle with shared pointers to its Qdef
			~Qle() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qle(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum greater-than is an operator
		class Qgt : public Qoperator
		{
		public:
			// Qgt's shared pointer 
			typedef shared_ptr<Qgt> Sp;

			// Instantiate Qgt instance with '&' identity
			Qgt() : Qoperator(GtQT::cMark) {};

			// Copy constructor
			Qgt(const Qgt& right) : Qoperator(right) {};

			// Destruct Qgt with shared pointers to its Qdef
			~Qgt() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qgt(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum greater-equal is an operator
		class Qge : public Qoperator
		{
		public:
			// Qge's shared pointer 
			typedef shared_ptr<Qge> Sp;

			// Instantiate Qge instance with '&' identity
			Qge() : Qoperator(GeQT::cMark) {};

			// Copy constructor
			Qge(const Qge& right) : Qoperator(right) {};

			// Destruct Qge with shared pointers to its Qdef
			~Qge() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qge(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		/****Quantum Binary operations *****/

		// A Quntum and is a binary operation
		class Qand : public QcellOp
		{
		public:
			// Qand's shared pointer 
			typedef shared_ptr<Qand> Sp;

			// Instantiate Qand instance with '&' identity
			Qand() : QcellOp(AndQT::cMark, 2) {};

			// Copy constructor
			Qand(const Qand& right) : QcellOp(right) {};

			// Destruct Qand with shared pointers to its two Qdefs
			~Qand() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qand(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};


		// A Q cell operation with 2 outputs is an abstration of a Q cell
		// operation that has an additional auxiliary output 
		class Qcell2OutOp : public QcellOp
		{
		public:
			// Qaddition's shared pointer 
			typedef shared_ptr<Qcell2OutOp> Sp;

			// An Q addition has identity and should have at least two argument
			Qcell2OutOp(const string& id, size_t size);

			// Copy constructor
			Qcell2OutOp(const Qcell2OutOp& right) : QcellOp(right) {};

			// Destruct Qeq with shared pointers to its Qdef
			~Qcell2OutOp() {};

			// overrides output setter to add 2nd output
			virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

			// Return Q addition result and carry outputs
			virtual Qdefs outputs() const;

		protected:
		private:
			Qcell::Sp mpAuxiliary;
		};

		// A Quntum nAnd is a binary operation
		class QnAnd : public Qcell2OutOp
		{
		public:
			// QnAnd's shared pointer 
			typedef shared_ptr<QnAnd> Sp;

			// Instantiate QnAnd instance with '&' identity
			QnAnd() : Qcell2OutOp(NandQT::cMark, 2) {};

			// Copy constructor
			QnAnd(const QnAnd& right) : Qcell2OutOp(right) {};

			// Destruct QnAnd with shared pointers to its two Qdefs
			~QnAnd() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QnAnd(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum or is a binary operation
		class Qor : public QcellOp
		{
		public:
			// Qor's shared pointer 
			typedef shared_ptr<Qor> Sp;

			// Instantiate Qor instance with '|' identity
			Qor() : QcellOp(OrQT::cMark, 2) {};

			// Copy constructor
			Qor(const Qor& right) : QcellOp(right) {};

			// Destruct Qor with shared pointers to its two Qdefs
			~Qor() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qor(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum nOr is a binary operation
		class QnOr : public Qcell2OutOp
		{
		public:
			// QnOr's shared pointer 
			typedef shared_ptr<QnOr> Sp;

			// Instantiate QnOr instance with '|' identity
			QnOr() : Qcell2OutOp(NorQT::cMark, 2) {};

			// Copy constructor
			QnOr(const QnOr& right) : Qcell2OutOp(right) {};

			// Destruct QnOr with shared pointers to its two Qdefs
			~QnOr() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QnOr(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Q addition is an abstration of a Q cell operation that has 
		// an additional carry Q cell output
		class Qaddition : public QcellOp
		{
		public:
			// Qaddition's shared pointer 
			typedef shared_ptr<Qaddition> Sp;

			// A Quntum carry is an operand defined by a Q addition
			class Carry : public QcellOp
			{
			public:
				// Q carry's shared pointer 
				typedef shared_ptr<Carry> Sp;

				// Initiate Carry instance with a pointer to a parent Q addition
				Carry(Qaddition*);

				// Copy constructor
				Carry(const Carry&);

				// Destruct a Q carry operand
				~Carry();

				// Override: Carry is an operation that should be treated as a definition
				virtual bool asDefinition() { return true; };

				// overrides output setter to set its output pointer
				virtual void output(const Qdef::Sp& pOut, size_t forBit = cAllBits);

				// Return a Qdef's shared pointer pointing to a copy of this object 
				virtual Qdef::Sp clone() const { return Qdef::Sp(new Carry(*this)); };

				// Return a string representation of Q carry operand
				virtual string toString(bool decomposed = false, size_t atBit = cAllBits) const;

				// return emty Qubo as carry is a special output of addition and not
				// solvable Q operation
				virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const { 
					return Qubo(); 
				};

				// Set a pointer to a new parent Q addition
				void addition(Qaddition*);

			protected:
				virtual Qvalue calculate(const Qvalues& values) const;

			private:
				Qaddition*	mpAddition;	// A parent Q addition
			};

			// An Q addition has identity and should have at least two argument
			Qaddition(const string& id, size_t size) : QcellOp(id, size), mpCarry(Carry::Sp(new Carry(this))) {};

			Qaddition(const string& id, size_t size, const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry);

			// Copy constructor
			Qaddition(const Qaddition& right) : QcellOp(right), mpCarry(right.mpCarry) 
			{
				mpCarry->addition(this);
			};

			// Destruct Qeq with shared pointers to its Qdef
			~Qaddition() {};

			// overrided inputs setter to sync carry output value with this operation 
			virtual void inputs(const Qdefs&);

			// overrides output setter to add carry output
			virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

			// Return Q addition result and carry outputs
			virtual Qdefs outputs() const;

			// get carry cell output
			Carry::Sp carry() const { return mpCarry; };

		protected:
			// get a reference to carry cell output
			Carry& carry() { return *mpCarry; };

		private:
			Carry::Sp	mpCarry;
		};

		// A Quntum xOr is a binary Q addition
		class Qxor : public Qaddition
		{
		public:
			// Qxor's shared pointer 
			typedef shared_ptr<Qxor> Sp;

			// Default constructor
			Qxor() : Qaddition(XorQT::cMark, 2) {};

			// Copy constructor
			Qxor(const Qxor& right) : Qaddition(right) {};

			// Destruct the Q xor instance with a shared pointer to its carry operand
			// and two inputs
			~Qxor() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qxor(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum half adder is a binary Q xor addition
		typedef Qxor Qadder05;


		// A Quntum xOr is a binary Q addition
		class Qnxor : public Qaddition
		{
		public:
			// Qnxor's shared pointer 
			typedef shared_ptr<Qxor> Sp;

			// Default constructor
			Qnxor() : Qaddition(NxorQT::cMark, 2) {};

			// Copy constructor
			Qnxor(const Qnxor& right) : Qaddition(right) {};

			// Destruct the Q xor instance with a shared pointer to its carry operand
			// and two inputs
			~Qnxor() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qnxor(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		/****Quantum Trinary operations *****/

		// A Quntum (full) adder is a binary Q addition
		class Qadder : public Qaddition
		{
		public:
			// QtriOperation's shared pointer 
			typedef shared_ptr<Qadder> Sp;

			// Default constructor
			Qadder() : Qaddition(AdderQT::cMark, 3) {};

			// Default constructor
			Qadder(const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry) : Qaddition(AdderQT::cMark, 3, pOut, pCarry) {};

			// Copy constructor
			Qadder(const Qadder& right) : Qaddition(right) {};

			// Destruct the Q adder instance with a shared pointer to its carry operand
			// and two inputs
			~Qadder() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qadder(*this)); };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

		private:
		};

		// A Quntum xOr that can be extended to act as Q adder
		class QxorAdder : public Qaddition
		{
		public:
			// Qxor's shared pointer 
			typedef shared_ptr<QxorAdder> Sp;

			// Default constructor
			QxorAdder() : Qaddition(XorQT::cMark, 2) {};

			// Copy constructor, Q xo-adder can have up to 3 inputs
			QxorAdder(const Qxor& right) : Qaddition(right) {};

			// Destruct the Q xor instance with a shared pointer to its carry operand
			// and two inputs
			~QxorAdder() {};

			// Sets Q xor-adder inputs. If any of the inputs is Q xor-adder acting as an Q xor, 
			// it will be upgraded into an adder by adding 3rd input.
			virtual Sp process(const Qdefs&);

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QxorAdder(*this)); };

			// Returns true if has 2 inputs and acts as an Q xor circuit. Returns false
			// if has 3 inputs and acts as a Q adder circuit.
			bool isXor() { return Qop::inputs().size() == 2; };

			// Returns true if has 3 inputs and acts as an Q xor circuit. Returns false
			// if has 2 inputs and acts as a Q xor circuit.
			bool isAdder() { return Qop::inputs().size() == 3; };

			// Returns true if this object has been extended from a xor to an adder.
			bool isExtended() { return mExtended; };

		protected:
			virtual Qvalue calculate(const Qvalues& values) const;

			void extended() { mExtended = true; };

		private:
			bool	mExtended = false;
		};
	};
};