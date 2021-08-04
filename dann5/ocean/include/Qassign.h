#pragma once


#include <pybind11/pybind11.h>

#include <utility>
#include <Qdef.h>
#include <Qtype.h>
#include <Qexpr.h>
#include <Factory.h>
#include <Qsolver.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quantum assignment
		class Qassignment
		{
		public:
			// Q assignment shared pointer 
			typedef shared_ptr<Qassignment> Sp;

			// default constructor creates an Q assignment without assignee and 
			// Q expression
			Qassignment() : mpAssignee(nullptr), mpExpr(nullptr) {};

			// Initilized Q assignment with Q definition assignee
			Qassignment(const Qdef::Sp& pAssignee)
				: mpAssignee(pAssignee), mpExpr(nullptr) {};

			// Initilized Q assignment with Q definition assignee and coresponding 
			// Q expression
			Qassignment(const Qdef::Sp& pAssignee, const Qexpression::Sp& pExpr);

			// copy constructor
			Qassignment(const Qassignment& right) 
				: mpAssignee(right.mpAssignee), mpExpr(right.mpExpr) {};

			// destructor
			~Qassignment() {};

			virtual Qassignment::Sp clone() const = 0;

			// Assign a Q expression to Q defintion assignee. 
			// Use without inputs to remove (one or boath) Q assignment members
			void assign(const Qdef::Sp& pAssignee = nullptr, const Qexpression::Sp& pExpr = nullptr);

			// returns a Q definition pointer to the assignee
			const Qdef::Sp& assignee() const { return mpAssignee; };

			// set a Q definition pointer to a new assignee
			void assignee(const Qdef::Sp& pAssignee) { mpAssignee = pAssignee; };

			// returns a pointer to the Q expression
			const Qexpression::Sp& expression() const { return mpExpr; };

			// set a Q definition pointer to a new assignee
			void expression(const Qexpression::Sp& pExpr);

			// Returns the number of Q bits that the Q assignment holds
			virtual size_t noqbs() const noexcept { return mpAssignee->noqbs(); };

			// Returns a qubo representation of this Q assignment, 
			// if not finalized, returns a full qubo definition representation
			// if finalized, returns an expression that replaces symbols with values of
			// Q bits in deterministic states
			virtual Qubo qubo(bool finalized = true, size_t forBit = ULLONG_MAX) const;

			// Return a string representation of this Q expression
			// when decomposed is true, shows bit-level expressions per line
			// when decomposed and forBit specified, shows a decomposed expression only
			// for that bit level
			string toString(bool decomposed = false, size_t forBit = ULLONG_MAX) const;

			// Set a sample set with a node list defined by qubo() of this Q assignment
			// the combination of node values should be different for each sample
			virtual void solutions(const Qsolver::Samples& samples);

			// For existing samples, returns a string representation of all solutions of 
			// this Q assignment
			virtual string solutions() const;

			// Solve this Q assignment and return a string with all solutions
			virtual string solve();

			// Send a string representation of a Q expression into an output stream
			friend std::ostream& operator << (std::ostream&, const Qassignment&);

		protected:
			// return a pointer to assignee
			Qdef::Sp assignee() { return mpAssignee; };

			// return a pointer to expression
			Qexpression::Sp expression() { return mpExpr; };

			// bind the assignee to a quantum expression.
			// If Qnary, resize assignee if expresion has more Q cells.
			// If assignee has more Q cells than the expression, 
			virtual void bind();

		private:
			Qdef::Sp			mpAssignee;
			Qexpression::Sp		mpExpr;
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
				: Qassignment(Qdef::Sp(assignee.clone()), expr.clone()) {};

			// copy constructor
			Qassign(const Qassign<Q_Type>& right) : Qassignment(right) {};

			// destructor
			~Qassign() {};

			virtual Qassignment::Sp clone() const
			{
				return Qassignment::Sp(new Qassign<Q_Type>(*this));
			};

			/*** Assignments ***/
			// Update Q assignment with a new Q expression
			Qassign<Q_Type>& operator=(const Qexpr<Q_Type>& right) {
				expression(right.clone());
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