#pragma once


#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <Qexpr.h>
#include <Qbit.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Q multiplication is a specific implementation of a Q operation
		class Qmultiply : public QnaryOp
		{
		public:
			// Qadd's shared pointer 
			typedef shared_ptr<Qmultiply> Sp;

			static const string cMark;
			static const string cName;

			// An Q addition has identity and should have at least two argument
			Qmultiply() : QnaryOp(cMark, 2) {};

			// Copy constructor
			Qmultiply(const Qmultiply& right) : QnaryOp(right) {};

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~Qmultiply() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qmultiply(*this)); };

		protected:
			// Refreshes the multiplication logic  
			virtual void refresh();

			// Return Q bit definition matrix that is result of matrix multiplication
			// of two Q bit vectors, this and right Q expression objects
			QcellMatrix x(const Qdefs& args) const;

			// Return x Q expression by adding elements of Q bit definition matrix
			// digonal for a given bitLevel. 
			void sumDiagonal(const QcellMatrix& matrix);

		private:
		};
	};
};