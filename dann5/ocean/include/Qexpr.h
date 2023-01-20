#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <QcellOps.h>
#include <QnaryOp.h>
#include <Factory.h>
#include <Qtype.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quantum expression is a Q statement
		class Qexpression : public Qstatement
		{
		public:
			// Q expression shared pointer 
			typedef shared_ptr<Qexpression> Sp;

			// default constructor creates an empty Q expression
			Qexpression() : mpRoot(nullptr) {};

			// Construct Q expression with its root operation definition 
			Qexpression(const Qop::Sp& pRoot) : mpRoot(pRoot) {};

			// copy constructor
			Qexpression(const Qexpression& right) 
				: Qstatement(right), mpRoot(right.mpRoot) {};

			// destructor
			~Qexpression() {};

			// Override to return a shared_pointer to a cloned instance of this
			// Q expression
			virtual Qexpression::Sp clone(bool) const = 0;

			// Returns a Q statement shared_pointer to a cloned instance of
			// this Q expression
			virtual Qstatement::Sp clone() const
			{
				return dynamic_pointer_cast<Qstatement>(clone(true));
			};

			// Returns a reference to the Q expression root Q operaton
			Qop& root() { return *mpRoot; };

			// Returns a root shared-pointer
			Qop::Sp root() const { return mpRoot; };

			// Sets a pointer to a new root Q operation of this expression
			void root(const Qop::Sp& pRoot) { mpRoot = pRoot; };

			// Returns a Q definition reference of the Q expression root 
			// operation
			Qdef& rootDef() { return *dynamic_pointer_cast<Qdef>(mpRoot); };

			// Return a Q definition shared-pointera of the Q expression root
			// operation
			Qdef::Sp rootDef() const 
					{ return dynamic_pointer_cast<Qdef>(mpRoot); };

			// Returns the number of Q bits of this Q expression 
			virtual size_t noqbs() const noexcept 
							{ return mpRoot->noqbs(); };

			// Returns a qubo representation of this Q expression, 
			// if not finalized, returns a full qubo without substitutions
			// if finalized, returns a qubo with all constant variables (with 
			// all Q bits in deterministic states) substituted by their values
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns qubo only  for the specified bit level
			virtual Qubo qubo(bool finalized = true, 
							size_t forBit = cAllBits) const;

			// Returns a string representation of this Q expression, 
			// if not decomposed, returns a statement as initially specified
			// if decomposed, returns bit-level logic of the assignment
			// when applicable, if forBit is specified (differs from cAllBits),
			// returns decomposed logic only for the specified bit level
			string toString(bool decomposed = false, 
								size_t forBit = cAllBits) const;

			// Adds a sample set containing nodes with solutions values, the
			// nodes should correspond to operands of this Q expression
			virtual void add(const Qsolver::Samples& samples);

			// For added sample set(s), returns a string representation of all
			// solutions of operands of this Q expression
			virtual string solutions() const;

			// For added sample set(s), returns a string represnting 'at'
			// solution of operands of this Q expression
			virtual string solution(size_t at) const 
						{ return root()->solution(at);  };

			// Return computed sample set with all solutions of the expression
			virtual Qsolver::Samples compute();

			// Reset the quantum expression into its initial state without 
			// solutions by reseting its operations and operands
			virtual void reset();

			// Send a string representation of a Q expression into an output
			// stream
			friend std::ostream& operator << (std::ostream&, 
												const Qexpression&);

		protected:
		private:
			Qop::Sp				mpRoot;		// A pointer to the root operation
			Qsolver::Samples	mSolutions;	// A sample set
		};

		// A templated Q expression for Q types
		template <typename Q_Type = Qtype>
		class Qexpr : public Qexpression
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr< Qexpr<Q_Type> > Sp;

			// default constructor creates an empty Q expression
			Qexpr() : Qexpression() {};

			// Initilized Q expression with its root definition 
			Qexpr(const Qop::Sp& pRoot) : Qexpression(pRoot) {};

			// copy constructor
			Qexpr(const Qexpr<Q_Type>& right) : Qexpression(right) {};

			// destructor
			~Qexpr() {};

			// Returns a shared_pointer to a cloned instance of this
			// Q expression
			virtual Qexpression::Sp clone(bool) const
			{
				return Qexpression::Sp(new Qexpr<Q_Type>(*this));
			};

			/*** Logical Operator ***/
			// return Q expression reference with inversion logic, e.g. for 
			// Qbool with id 'x' the logic is '!x' != 'x'
			Qexpr<Q_Type>& operator!()
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("!" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				root(pOp);
				return(*this);
			}

			// instantiate Q expression with inversion/not logic, e.g. for 
			// Qbool with id 'x' the logic is '!x' != 'x'
			Qexpr<Q_Type> operator!() const
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("!" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			}

			/*** Bitwise Operator ***/
			// return Q expression reference with inversion logic, e.g. for Q
			// bit with id 'x' the logic is '~x' != 'x'
			Qexpr<Q_Type>& operator~()
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("~" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				root(pOp);
				return(*this);
			}

			// instantiate Q expression with inversion/not logic, e.g. for 
			// Qbit with id 'x' the logic is '~x' != 'x'
			Qexpr<Q_Type> operator~() const
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("~" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			}

			/*** Bitwise + Logical  Operator ***/

			/*** Bitwise + Logical  Operations ***/
			// Update Q expression with and logic, e.g. for an argument with id
			// 'x' the expression root points to QandOp with [previous root] & 'x'
			Qexpr<Q_Type>& operator&(const Q_Type& right) {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument with id
			// 'x' the expression root points to QandOp with [previous root] & 'x'
			Qexpr<Q_Type> operator&(const Q_Type& right) const {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with and logic, e.g. for an argument [right]
			// the expression root points to QandOp with [previous root] & [right root]
			Qexpr<Q_Type>& operator&(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument [right]
			// the expression root points to QandOp with [previous root] & [right root]
			Qexpr<Q_Type> operator&(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with and logic, e.g. for an argument with id
			// 'x' the expression root points to QandOp with !([previous root] & 'x')
			Qexpr<Q_Type>& nand(const Q_Type& right) {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NandQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument [right]
			// the expression root points to QandOp with !([previous root] & [right root])
			Qexpr<Q_Type>& nand(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = 
					Factory<string, QcellOp>::Instance().create(NandQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument with id 'x' the 
			// expression root points to QorOp with [previous root] | 'x'
			Qexpr<Q_Type>& operator|(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument with id 'x' the 
			// expression root points to QorOp with [previous root] | 'x'
			Qexpr<Q_Type> operator|(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with or logic, e.g. for an argument [right] the
			// expression root points to QorOp with [previous root] | [right root]
			Qexpr<Q_Type>& operator|(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument [right] the
			// expression root points to QorOp with [previous root] | [right root]
			Qexpr<Q_Type> operator|(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(OrQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with or logic, e.g. for an argument with id 'x' the 
			// expression root points to QorOp with !([previous root] | 'x')
			Qexpr<Q_Type>& nor(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument [right] the
			// expression root points to QorOp with !([previous root] | [right root])
			Qexpr<Q_Type>& nor(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Unlike Q expression with xor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] ^ 'x'
			Qexpr<Q_Type>& unlike(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			Qexpr<Q_Type>& operator^(const Q_Type& right) { return unlike(right); };

			// alike Q expression with nxor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] *= 'x'
			Qexpr<Q_Type>& alike(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};
			Qexpr<Q_Type>& operator*=(const Q_Type& right) { return alike(right); };
			Qexpr<Q_Type>& nxor(const Q_Type& right) { return alike(right); };

			// Update Q expression with xor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] ^ 'x'
			Qexpr<Q_Type> unlike(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};
			Qexpr<Q_Type> operator^(const Q_Type& right) const { return unlike(right); };

			// Update Q expression with nxor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] *= 'x'
			Qexpr<Q_Type> alike(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(*this);
			};
			Qexpr<Q_Type> operator*=(const Q_Type& right) const { return alike(right); };
			Qexpr<Q_Type> nxor(const Q_Type& right) const { return alike(right); };

			// Update Q expression with xor logic, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] ^ [right root]
			Qexpr<Q_Type>& unlike(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};
			Qexpr<Q_Type>& operator^(const Qexpr<Q_Type>& right) { return unlike(right); };

			// Update Q expression with nxor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] *= 'x'
			Qexpr<Q_Type>& alike(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};
			Qexpr<Q_Type>& operator*=(const Qexpr<Q_Type>& right) { return alike(right); };
			Qexpr<Q_Type>& nxor(const Qexpr<Q_Type>& right) { return alike(right); };

			// Update Q expression with xor logic, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] ^ [right root]
			Qexpr<Q_Type> unlike(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};
			Qexpr<Q_Type> operator^(const Qexpr<Q_Type>& right) const { return unlike(right); };

			// Update Q expression with nxor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] *= 'x'
			Qexpr<Q_Type> alike(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NxorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(*this);
			};
			Qexpr<Q_Type> operator*=(const Qexpr<Q_Type>& right) const { return alike(right); };
			Qexpr<Q_Type> nxor(const Qexpr<Q_Type>& right) const { return alike(right); };

			/*** Comparison ***/
			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QeqOp with [previous root] == 'x'
			Qexpr<Q_Type> operator==(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QeqOp with [previous root] == [right root]
			Qexpr<Q_Type> operator==(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(EqQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QneqOp with [previous root] != 'x'
			Qexpr<Q_Type> operator!=(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QneqOp with [previous root] != [right root]
			Qexpr<Q_Type> operator!=(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QgtOp with [previous root] > 'x'
			Qexpr<Q_Type> operator>(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QgtOp with [previous root] > [right root]
			Qexpr<Q_Type> operator>(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GtQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QgeOp with [previous root] >= 'x'
			Qexpr<Q_Type> operator>=(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QgeOp with [previous root] >= [right root]
			Qexpr<Q_Type> operator>=(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(GeQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QltOp with [previous root] < 'x'
			Qexpr<Q_Type> operator<(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QltOp with [previous root] < [right root]
			Qexpr<Q_Type> operator<(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LtQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QleOp with [previous root] <= 'x'
			Qexpr<Q_Type> operator<=(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.clone());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QleOp with [previous root] <= [right root]
			Qexpr<Q_Type> operator<=(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(LeQT::cMark);
				pOp->inputs({ as_const(*this).rootDef() });
				pOp->output(right.rootDef());

				Qexpr<Q_Type> expr(pOp);
				return expr;
			};

			/*** Arithmetic ***/
			// Update Q expression with addition, e.g. for an argument with id 'x' the 
			// expression root points to Qadd with [previous root] + 'x'
			Qexpr<Q_Type>& operator+(const Q_Type& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with addition, e.g. for an argument with id 'x' the 
			// expression root points to Qadd with [previous root] + 'x'
			Qexpr<Q_Type> operator+(const Q_Type& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with addition, e.g. for an argument [right] the
			// expression root points to Qadd with [previous root] ^ [right root]
			Qexpr<Q_Type>& operator+(const Qexpr<Q_Type>& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with addition, e.g. for an argument [right] the
			// expression root points to Qadd with [previous root] ^ [right root]
			Qexpr<Q_Type> operator+(const Qexpr<Q_Type>& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with subtration, e.g. for an argument with id 'x' the 
			// expression root points to Qsubtract with [previous root] - 'x'
			Qexpr<Q_Type>& operator-(const Q_Type& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("-");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with subtration, e.g. for an argument with id 'x' the 
			// expression root points to to Qsubtract with [previous root] - 'x'
			Qexpr<Q_Type> operator-(const Q_Type& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("-");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

/*	UNCOMMENT to test expression subtraction
			// Update Q expression with subtration, e.g. for an argument [right] the
			// expression root points to to Qsubtract with [previous root] - [right root]
			Qexpr<Q_Type>& operator-(const Qexpr<Q_Type>& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("-");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with subtration, e.g. for an argument [right] the
			// expression root points to to Qsubtract with [previous root] - [right root]
			Qexpr<Q_Type> operator-(const Qexpr<Q_Type>& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("-");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};
*/
			// Update Q expression with multiplication, e.g. for an argument with id 'x' the 
			// expression root points to Qmultiply with [previous root] * 'x'
			Qexpr<Q_Type>& operator*(const Q_Type& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with multiplication, e.g. for an argument with id 'x' the 
			// expression root points to Qmultiply with [previous root] * 'x'
			Qexpr<Q_Type> operator*(const Q_Type& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with multiplication, e.g. for an argument [right] the
			// expression root points to Qmultiply with [previous root] * [right root]
			Qexpr<Q_Type>& operator*(const Qexpr<Q_Type>& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with multiplication, e.g. for an argument [right] the
			// expression root points to Qmultiply with [previous root] * [right root]
			Qexpr<Q_Type> operator*(const Qexpr<Q_Type>& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};
		};
	};
};