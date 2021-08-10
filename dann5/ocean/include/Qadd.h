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
		class Qadd : public QnaryOp
		{
		public:
			// Qadd's shared pointer 
			typedef shared_ptr<Qadd> Sp;

			// An Q addition has identity and should have at least two argument
			Qadd();

			// Copy constructor
			Qadd(const Qadd& right);

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~Qadd();

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qadd(*this)); };


		protected:
			// Refreshes QnaryOp cells with the addition logic
			virtual void refresh();

			// Assignes the inputs to this additon and returns nullptr
			// Override if assignment of operands should triger creation of different
			// Q addition object and return a shared pointer to a newlly created Q addition
			// object
			virtual Qcell::Sp opAt(const Qnary::Sp& pLeft, const Qnary::Sp& pRight,
									size_t forBit, Qaddition::Carry::Sp& pCarry, 
									bool assignOutput) const;

		private:
		};
	};
};