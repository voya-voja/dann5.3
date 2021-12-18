#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <QcellOps.h>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {


		// A Q add is a specific implementation of a Qnary operation
		class QwholeComp : public QnaryOp
		{
		public:
			// QwholeComp's shared pointer 
			typedef shared_ptr<QwholeComp> Sp;

			// An Q addition has identity and should have at least two argument
			QwholeComp(const QcellOp::Sp& pOp);

			// Copy constructor
			QwholeComp(const QwholeComp& right);

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~QwholeComp();

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QwholeComp(*this)); };

			// Returns the number of Q underlying Q cell comparison operation
			virtual size_t noqbs() const noexcept { return mpOp->noqbs(); };

			// override to refresh processing logic when an output is set
			virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

			// convert operation expration into a string
			virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const;

			// return Qubo presentation of this Q operation
			virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const;

		protected:
			// Refreshes QnaryOp cells with the addition logic
			virtual void refresh();

		private:
			QcellOp::Sp	mpOp;
		};
	};
};