#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <QcellOps.h>
#include <QnaryOp.h>
#include <Factory.h>
#include <Qtype.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quantum expression
		class Qexpression
		{
		public:
			// Q expression shared pointer 
			typedef shared_ptr<Qexpression> Sp;

			// default constructor creates an empty Q expression
			Qexpression() : mpRoot(nullptr) {};

			// Initilized Q expression with its root definition 
			Qexpression(const Qop::Sp& pRoot) : mpRoot(pRoot) {};

			// copy constructor
			Qexpression(const Qexpression& right) : mpRoot(right.mpRoot) {};

			// destructor
			~Qexpression() {};

			// returns a reference to the Q expression root Q operaton
			Qop& root() { return *mpRoot; };
			// return a root shared-pointer
			Qop::Sp root() const { return mpRoot; };

			// set a pointer to a new root Q definition of this expression
			void root(const Qop::Sp& pRoot) { mpRoot = pRoot; };

			// returns a reference to the Q expression root Q definition
			Qdef& rootDef() { return *dynamic_pointer_cast<Qdef>(mpRoot); };

			// return a root shared-pointer
			Qdef::Sp rootDef() const { return dynamic_pointer_cast<Qdef>(mpRoot); };

			// Returns the number of Q bits that the Q expression holds
			virtual size_t noqbs() const noexcept { return mpRoot->noqbs(); };

			virtual Qexpression::Sp clone() const = 0;

			// Returns a qubo representation of this Q expression, 
			// if not finalized, returns a full qubo definition representation
			// if finalized, returns an expression that replaces symbols with values of
			// Q bits in deterministic states
			virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const;

			// Return a string representation of this Q expression
			// when decomposed is true, shows bit-level expressions per line
			// when decomposed and forBit specified, shows a decomposed expression only
			// for that bit level
			string toString(bool decomposed = false, size_t forBit = cAllBits) const;

			// Set a sample set with a node list defined by qubo() of this Q expression
			// the combination of node values should be different for each sample
			virtual void add(const Qsolver::Samples& samples);

			// For existing samples, returns a string with all solutions of 
			// this Q expression
			virtual string solutions() const;

			// Solve this Q expression and return a string with all solutions
			virtual string solve() ;

			// Reset the quantum expression into its initial state without solutions by clearing 
			// all solution samples
			virtual void reset();

			// Send a string representation of a Q expression into an output stream
			friend std::ostream& operator << (std::ostream&, const Qexpression&);

		protected:


		private:
			Qop::Sp	mpRoot;
			Qsolver::Samples	mSolutions;
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

			virtual Qexpression::Sp clone() const
			{
				return Qexpression::Sp(new Qexpr<Q_Type>(*this));
			};

			/*** Bitwise + Logical ***/

			// instantiate Q expression with inversion logic, e.g. for Qbit with id 'x'
			// the logic is '~x' != 'x'
			Qexpr<Q_Type>& operator~()
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("~" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				root(pOp);
				return(*this);
			}

			// instantiate Q expression with inversion logic, e.g. for Qbit with id 'x'
			// the logic is '~x' != 'x'
			Qexpr<Q_Type> operator~() const
			{
				Qdef::Sp pOut = as_const(*this).root()->output();
				Qdef::Sp pInverted = pOut->clone();
				pInverted->id("~" + pOut->id());
				Qdef::Sp pRoot = as_const(*this).rootDef();
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NeqQT::cMark);
				pOp->inputs({ pRoot });
				pOp->output(pInverted->clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			}

			// instantiate Q expression with inversion logic, e.g. for Qbool with id 'x'
			// the expression is '!x' != 'x'
			Qexpr<Q_Type> operator !() const
			{
				Q_Type inverted("!" + rootDef()->toString());

				Qexpr<Q_Type> expr = inverted != (*this);
				return expr;
			}

			// Update Q expression with and logic, e.g. for an argument with id 'x' the
			// expression root points to QandOp with [previous root] & 'x'
			Qexpr<Q_Type>& operator&(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument with id 'x' the
			// expression root points to QandOp with [previous root] & 'x'
			Qexpr<Q_Type> operator&(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with and logic, e.g. for an argument [right] the
			// expression root points to QandOp with [previous root] & [right root]
			Qexpr<Q_Type>& operator&(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument [right] the
			// expression root points to QandOp with [previous root] & [right root]
			Qexpr<Q_Type> operator&(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with and logic, e.g. for an argument with id 'x' the
			// expression root points to QandOp with ~([previous root] & 'x')
			Qexpr<Q_Type>& nand(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument [right] the
			// expression root points to QandOp with ~([previous root] & [right root])
			Qexpr<Q_Type>& nand(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NandQT::cMark);
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
			// expression root points to QorOp with ~([previous root] | 'x')
			Qexpr<Q_Type>& nor(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument [right] the
			// expression root points to QorOp with ~([previous root] | [right root])
			Qexpr<Q_Type>& nor(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(NorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with xor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] ^ 'x'
			Qexpr<Q_Type>& operator^(const Q_Type& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with xor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] ^ 'x'
			Qexpr<Q_Type> operator^(const Q_Type& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with xor logic, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] ^ [right root]
			Qexpr<Q_Type>& operator^(const Qexpr<Q_Type>& right) {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with xor logic, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] ^ [right root]
			Qexpr<Q_Type> operator^(const Qexpr<Q_Type>& right) const {
				QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(XorQT::cMark);
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

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
			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to Qadder with [previous root] + 'x'
			Qexpr<Q_Type>& operator+(const Q_Type& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to Qadder with [previous root] + 'x'
			Qexpr<Q_Type> operator+(const Q_Type& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to Qadder with [previous root] ^ [right root]
			Qexpr<Q_Type>& operator+(const Qexpr<Q_Type>& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to Qadder with [previous root] ^ [right root]
			Qexpr<Q_Type> operator+(const Qexpr<Q_Type>& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("+");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to Qmultiply with [previous root] * 'x'
			Qexpr<Q_Type>& operator*(const Q_Type& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to Qmultiply with [previous root] * 'x'
			Qexpr<Q_Type> operator*(const Q_Type& right) const {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.clone() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				Qexpr<Q_Type> expr(pOp);
				return(expr);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to Qmultiply with [previous root] * [right root]
			Qexpr<Q_Type>& operator*(const Qexpr<Q_Type>& right) {
				QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create("*");
				pOp->inputs({ as_const(*this).rootDef(), right.rootDef() });
				Q_Type out(pOp->outId());
				pOp->output(out.clone());

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
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