#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qdef.h>
#include <Qexpr.h>
#include <Qubo.h>
#include <Qsolver.h>

using namespace std;
using namespace Eigen;


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

			// instantiate a Q statement with a Q expression and corresponding 
			// Q variables as arguments of the expression
			Qstatement(const Qexpression&);

			// copy constructor
			Qstatement(const Qstatement&);

			// desruct the Q statement with its members
			~Qstatement();

			// Returns the number of Q bits that the Q statement holds
			virtual size_t noqbs() const noexcept { return mpExpression->noqbs(); };

			// Returns a constant reference to the expression of this Q statement
			const Qexpression::Sp expression() const { return mpExpression; }

			// Returns a constant reference to the expression of this Q statement
			Qexpression& expression() { return *mpExpression; }

			// Returns a constant reference to the expression arguments of this Q statement
//			const Qdefs& arguments() const { return mArguments; }

			// Returns a constant reference to the expression arguments of this Q statement
//			Qdefs& arguments() { return mArguments; }

			// Returns a qubo representation of this Q statement, 
			// if not finalized, returns a full qubo definition representation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables, i.e. expression arguments
			virtual Qubo qubo(bool finalized = true, Index level = cAllBits) const;

			// Returns a string representation of this Q statement, 
			// if not decomposed, returns an statement line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, size_t level = cAllBits) const {
				return mpExpression->toString(decomposed, level);
			};

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const = 0;

/*			// Add a sample with a node list defined by qubo() of this Q equation
			virtual void add(Qsolver::Sample& sample);

			// Set a sample set with a node list defined by qubo() of this Q equation
			// the combination of node values should be different for each sample
			virtual void set(Qsolver::Samples& samples);

			// For existing samples, returns a string representation of all solutions of 
			// this Q routine
			virtual string solutions() const;
*/
		protected:
/*			class Reduct
			{
			public:
				static const Qkey cSkip;
				// construct reduction object of a given Q statement
				Reduct(Qstatement&);

				// destructor
				~Reduct();

				// prepare reduction information for a given Q statement 
				virtual void init();

				// Returns a reduced key derived from a given Qubo key by applying
				// reduction rules for this Q statement. 
				// If finalized is true, it will apply discretionary Qbit values for the
				// Q statement result and arguments.
				// Otherwise, it will just symplify keys using result Qbit definitions
				Qkey operator()(const Qkey& key, bool finalized) const;

			protected:
				typedef std::pair<string, Qcell::Sp> Reduction;
				typedef std::map<string, Reduction> Reductions;

				Reductions& reductions() { return mReductions; };

			private:
				Qstatement& mStatement;
				Reductions mReductions;
			};

			virtual Reduct* createReduct() { return new Reduct(*this); };

			Reduct& reduct() { return *mpReduct; };
			void reduct(Reduct* pReduct);
*/

			// Assignment of a new expression to this Q statement
			void expression(const Qexpression& expr) { mpExpression = Qexpression::Sp(expr.clone()); }

			virtual Qubo createRawQubo(bool finalized, Index level) const;
			virtual Qubo reduceRawQubo(Qubo& rawQubo, bool finalized) const;
			virtual Qubo initQubo(bool finalized) const { return Qubo(); };

		private:
			Qexpression::Sp		mpExpression;	// an expresion, e.g. 'a + b + c', where a, b & c are Qvar instaces
//			Qvars			mArguments;		// the list of expression variables, e.g. {a, b, c} for the Q statement above

//			Reduct*			mpReduct;		// an instace of object class that symplifies the expression of this Q statement
			Qsolver::Samples	mSolutions;
		};

		// An std::vector containing a list of Q statements
		typedef vector<Qstatement::Sp> Qstatements;
	};
};