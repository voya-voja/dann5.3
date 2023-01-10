#pragma once


#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <Qdef.h>
#include <Qtype.h>
#include <Qexpr.h>
#include <Factory.h>
#include <Qsolver.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quantum assignment
		class Qassignment : public Qstatement
		{
		public:
			// Q assignment shared pointer 
			typedef shared_ptr<Qassignment> Sp;

			// default constructor creates an Q assignment without assignee and
			// expression
			Qassignment() : mpAssignee(nullptr), mpExpr(nullptr) {};

			// Constructs a Q assignment with an assignee Q definition
			Qassignment(const Qdef::Sp& pAssignee)
				: mpAssignee(pAssignee), mpExpr(nullptr) {};

			// Constructs a Q assignment with an assignee Q definition and 
			// coresponding Q expression
			Qassignment(const Qdef::Sp& pAssignee, 
						const Qexpression::Sp& pExpr);

			// copy constructor
			Qassignment(const Qassignment& right) 
				: mpAssignee(right.mpAssignee), mpExpr(right.mpExpr) {};

			// destructor
			~Qassignment() {};

			// Override to return a Q assignment shared pointer to a clone of 
			// this Q assignmnet
			virtual Qassignment::Sp clone(bool) const = 0;

			// Returns a Q statement shared pointer to a clone of this Q 
			// assignmnet
			virtual Qstatement::Sp clone() const
			{
				return dynamic_pointer_cast<Qstatement>(clone(true));
			};

			// Assign a Q expression to Q defintion assignee. 
			// Use without inputs to remove (one or boath) Q assignment members
			void assign(const Qdef::Sp& pAssignee = nullptr, 
						const Qexpression::Sp& pExpr = nullptr);

			// Returns a Q definition pointer to the assignee
			const Qdef::Sp& assignee() const { return mpAssignee; };

			// return a pointer to assignee
			Qdef::Sp assignee() { return mpAssignee; };

			// Sets a Q definition pointer to a new assignee
			void assignee(const Qdef::Sp& pAssignee) { mpAssignee = pAssignee; };

			// Returns a pointer to the Q expression
			const Qexpression::Sp& expression() const { return mpExpr; };

			// return a pointer to expression
			Qexpression::Sp expression() { return mpExpr; };

			// Sets assignment's new Q expression
			void expression(const Qexpression::Sp& pExpr);

			// Returns the number of Q bits that the Q assignment holds
			virtual size_t noqbs() const noexcept { return mpAssignee->noqbs(); };

			// Returns a qubo representation of this Q assignment, 
			// if not finalized, returns a full qubo without substitutions
			// if finalized, returns a qubo with all constant variables (with 
			// all Q bits in deterministic states) substituted by their values
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns qubo only  for the specified bit level
			virtual Qubo qubo(bool finalized = true, 
								size_t forBit = cAllBits) const;

			// Returns a string representation of this Q assignment, 
			// if not decomposed, returns a statement as initially specified
			// if decomposed, returns bit-level logic of the assignment
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns decomposed logic only for the specified bit level
			string toString(bool decomposed = false, 
								size_t forBit = cAllBits) const;

			// Adds a sample set containing nodes with solutions values,
			// the nodes should correspond to operands of this Q assignment
			virtual void add(const Qsolver::Samples& samples);

			// For added sample set(s), returns a string representation of all
			// solutions of operands of this Q assignment
			virtual string solutions() const;

			// For added sample set(s), returns a string represnting 'at'
			// solution of operands of this Q assignment
			virtual string solution(size_t at) const 
							{ return mpExpr->solution(at); };

			// Returns computed sample set with all solutions of the assignment
			virtual Qsolver::Samples compute();

			// Resets the assignment into its initial state without solutions
			// for the operands of this Q assignment
			virtual void reset();

			// Send a string representation of a Q expression into an output stream
			friend std::ostream& operator << (std::ostream&, const Qassignment&);

		protected:
			// bind the assignee to a quantum expression.
			// If Qnary, resize assignee if expresion has more Q cells.
			// If assignee has more Q cells than the expression, 
			virtual void bind();

		private:
			Qdef::Sp			mpAssignee;	// A pointer to assignee's defintion
			Qexpression::Sp		mpExpr;		// A pointer to the expression
		};

		// A templated Q assign for all Q types
		template <typename Q_Type = Qtype>
		class Qassign : public Qassignment
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr< Qexpr<Q_Type> > Sp;

			// default constructor creates an empty Q expression
			Qassign() : Qassignment() {};

			// Initilized Q assignment with Q definition assignee
			Qassign(const Q_Type& assignee)
				: Qassignment(Qdef::Sp(assignee.clone())) {};

			// Initilized Q expression with its root definition 
			Qassign(const Q_Type& assignee, const Qexpr<Q_Type>& expr)
				: Qassignment(Qdef::Sp(assignee.clone()), expr.clone(true)) {};

			// copy constructor
			Qassign(const Qassign<Q_Type>& right) : Qassignment(right) {};

			// destructor
			~Qassign() {};

			// Returns a Q assignment shared pointer to a clone of 
			// this Q assignmnet
			virtual Qassignment::Sp clone(bool) const
			{
				return Qassignment::Sp(new Qassign<Q_Type>(*this));
			};

			// Qstatement cast operator
			operator Qstatement() { return *this; };

			/*** Assignments ***/
			// Update Q assignment with a new Q expression
			Qassign<Q_Type>& operator=(const Qexpr<Q_Type>& right) {
				expression(right.clone(true));
				return(*this);
			};

			/*** Compound Assignments ***/
			// Replace the Q assignment expression with 'and assign' logic, e.g. for an 
			// assignee 'A' amd argument 'x' the assignment becomes 'A' = 'A' & 'x'
			Qassign<Q_Type>& operator&=(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ assignee(), right.clone() });
				expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'and assign' logic, e.g. for an
			// argument [expr] and assignee 'A' becomes 'A' = 'A' & [expr]
			Qassign<Q_Type>& operator&=(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ assignee(), right.rootDef() });
				expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'or assign' logic, e.g. for an 
			// assignee 'A' amd argument 'x' the assignment becomes 'A' = 'A' | 'x'
			Qassign<Q_Type>& operator|=(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ assignee(), right.clone() });
				expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'or assign' logic, e.g. for an
			// argument [expr] and assignee 'A' becomes 'A' = 'A' | [expr]
			Qassign<Q_Type>& operator|=(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ assignee(), right.rootDef() });
				expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'xor assign' logic, e.g. for an 
			// assignee 'A' amd argument 'x' the assignment becomes 'A' = 'A' ^ 'x'
			Qassign<Q_Type>& operator^=(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ assignee(), right.clone() });
				expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'xor assign' logic, e.g. for an
			// argument [expr] and assignee 'A' becomes 'A' = 'A' ^ [expr]
			Qassign<Q_Type>& operator^=(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ assignee(), right.rootDef() });
				expression()->root(pOp);
				return(*this);
			};
		
			// Replace the Q assignment expression with 'add assign' logic, e.g. for an 
			// assignee 'A' amd argument 'x' the assignment becomes 'A' = 'A' + 'x'
			Qassign<Q_Type>& operator+=(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create("add");
				pOp->inputs({ Qassign<Q_Type>::assignee(), right.clone() });
				Qassign<Q_Type>::expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'add assign' logic, e.g. for an
			// argument [expr] and assignee 'A' becomes 'A' = 'A' + [expr]
			Qassign<Q_Type>& operator+=(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create("add");
				pOp->inputs({ Qassign<Q_Type>::assignee(), right.rootDef() });
				Qassign<Q_Type>::expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'multiply assign' logic, e.g. 
			// for an assignee 'A' amd argument 'x' becomes 'A' = 'A' * 'x'
			Qassign<Q_Type>& operator*=(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create("multiply");
				pOp->inputs({ Qassign<Q_Type>::assignee(), right.clone() });
				Qassign<Q_Type>::expression()->root(pOp);
				return(*this);
			};

			// Replace the Q assignment expression with 'multiply assign' logic, e.g. 
			// for anargument [expr] and assignee 'A' becomes 'A' = 'A' * [expr]
			Qassign<Q_Type>& operator*=(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create("multiply");
				pOp->inputs({ Qassign<Q_Type>::assignee(), right.rootDef() });
				Qassign<Q_Type>::expression()->root(pOp);
				return(*this);
			};
		};
	};
};