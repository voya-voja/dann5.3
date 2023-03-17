#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qwhole.h>
#include <Qexpr.h>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {


		// A Q whole expression is specialization of quantum expression class
		class QwholeExpr : public Qexpr<Qwhole>
		{
		public:
			// QwholeComp's shared pointer 
			typedef shared_ptr<QwholeExpr> Sp;

			// default constructor creates an empty Q expression
			QwholeExpr() : Qexpr<Qwhole>() {};

			// An Q addition has identity and should have at least two argument
			QwholeExpr(const Qop::Sp& pOp) : Qexpr<Qwhole>(pOp) {};

			// Copy constructor
			QwholeExpr(const QwholeExpr& right) : Qexpr<Qwhole>(right) {};

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~QwholeExpr() {};

			// Returns a shared_pointer to a cloned instance of this
			// Q expression
			virtual Qexpression::Sp clone(bool) const
			{ 
				return Qexpression::Sp(new QwholeExpr(*this));
			};

			/*** Comparison ***/

			// instantiate Q expression with comparison, e.g. for [this] expression and 'y'
			// the expression is [ths] != 'y'
			QwholeExpr operator!=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for [this] and
			// [right] expressions, the expression is [this] != [right]
			QwholeExpr operator!=(const QwholeExpr& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' > 'y'
			QwholeExpr operator>(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole id 'x' and
			// expression [right] the expression is 'x' > [right]
			QwholeExpr operator>(const QwholeExpr& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			QwholeExpr operator>=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole id 'x' and
			// expression [right] the expression is 'x' >= [right]
			QwholeExpr operator>=(const QwholeExpr& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' < 'y'
			QwholeExpr operator<(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and
			// expression [right] the expression is 'x' < [right]
			QwholeExpr operator<(const QwholeExpr& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			QwholeExpr operator<=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and
			// expression [right] the expression is 'x' < [right]
			QwholeExpr operator<=(const QwholeExpr& right) const;
		protected:

		private:
		};
	};
};