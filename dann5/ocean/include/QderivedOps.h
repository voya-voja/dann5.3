#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <QcellOps.h>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {


		// A Q derived operation is an abstract base for operations like quantum
		// subtraction or division. The QeqOp specializes a Qnary operation
		class QderivedOp : public QcellOp
		{
		public:
			// QeqOp's shared pointer 
			typedef shared_ptr<QderivedOp> Sp;

			// An Q derived operation has identity and substitution operation
			//  to be used to perform quantum calculations
			QderivedOp(const string& id, const QnaryOp& op);

			// Copy constructor
			QderivedOp(const QderivedOp& right);

			// Destruct the Q subtractition instance with a shared pointer to its carry operand
			~QderivedOp();

			// Returns the number of Q bits that the Q binary holds
//			virtual size_t noqbs() const noexcept { return mEq.noqbs(); };

			// Overridden output setter to refresh mAdd object
			virtual void output(const Qdef::Sp& pOut, size_t forBit = cAllBits);

			// Return a Q add representation when decomposed. 
			// Otherwise, return a string representation of a Q subbtract operation
			virtual string toString(bool decomposed = false,
				size_t forBit = cAllBits) const
			{
				if (decomposed) return mEq.toString(decomposed, forBit);
				return QcellOp::toString(decomposed, forBit);
			};

			// return Qubo presentation of this Q operation
			virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const
			{
				return mEq.qubo(finalized, forBit);
			};

			const Qeq& equalOp() const { return mEq; };
			QnaryOp::Sp substituteOp() const 
			{ 
				return static_pointer_cast<QnaryOp>(as_const(mEq).Qop::inputs()[0]);
			};

		protected:
			// Refreshes QnaryOp cells with the subtractition logic
			virtual void refresh();

			virtual Qvalue calculate(const Qvalues& values) const;
		private:
			Qeq mEq;	// Qeq operand is used to form derived expression
		};

		// A Q subtract is a specific implementation of a Q derived operation
		class Qsubtract : public QderivedOp
		{
		public:
			// Qsubtract's shared pointer 
			typedef shared_ptr<Qsubtract> Sp;

			static const string cMark;
			static const string cName;

			// An Q subtractition has identity cMark and uses Qadd as its
			// substitute
			Qsubtract();

			// Copy constructor
			Qsubtract(const Qsubtract& right) : QderivedOp(right) {};

			// Destruct the Q subtractition instance with a shared pointer to its carry operand
			~Qsubtract() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qsubtract(*this)); };
		protected:
		private:
		};


		// A Q divide is a specific implementation of a Q derived operation
		class Qdivide : public QderivedOp
		{
		public:
			// Qsubtract's shared pointer 
			typedef shared_ptr<Qdivide> Sp;

			static const string cMark;
			static const string cName;

			// An Q subtractition has identity cMark and uses Qadd as its
			// substitute
			Qdivide();

			// Copy constructor
			Qdivide(const Qdivide& right) : QderivedOp(right) {};

			// Destruct the Q subtractition instance with a shared pointer to its carry operand
			~Qdivide() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qdivide(*this)); };
		protected:
		private:
		};

		/*
		class QsubtractQints : public Qsubtract
		{
		public:
			// Qsubtract's shared pointer 
			typedef shared_ptr<QsubtractQints> Sp;

			// An Q subtractition has identity and should have at least two argument
			QsubtractQints();

			// Copy constructor
			QsubtractQints(const QsubtractQints& right);

			// Destruct the Q subtractition instance with a shared pointer to its carry operand
			~QsubtractQints();
		};
*/
	};
};