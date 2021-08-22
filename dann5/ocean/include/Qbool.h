#pragma once

#include <pybind11/pybind11.h>

#include <Qdef.h>
#include <Qtype.h>
#include <Qexpr.h>
#include <Qassign.h>

namespace dann5 {
	namespace ocean {

		class Qbool : public Qcell, public Qtype
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qbool> Sp;

			// Qbool default superposition value, 
			// though any value except 0 and 1 will be considered superposition
			static const Qvalue cTrue	= 'T'; // value 84
			static const Qvalue cFalse	= 'F'; // value 71

			// Default constructor initializes Q bit with an empty id and supperposition
			// value
			Qbool() : Qcell(""), Qtype(), mValue(cSuperposition) {};

			// Initialize Q bool with a given id and supperposition value
			Qbool(const string& id) : Qtype(), Qcell(id), mValue(cSuperposition) {};

			// Initialize Q bool with a given id and value. If value is different from 'T' or 'F', 
			// Qbool into superposition state, i.e. value 'S'
			Qbool(const string& id, Qvalue value) : Qtype(), Qcell(id), mValue(value)
			{
				if (value != cTrue && value != cFalse) mValue = cSuperposition;
			};

			// copy constructor
			Qbool(const Qbool& right) : Qtype(right), Qcell(right), mValue(right.mValue) {};

			// type conversion operator
			operator Qvalue() { return mValue; };
			operator const Qvalue() const { return mValue; };

			// set the new value of this Q boolean
			virtual void value(Qvalue v);

			// constant and non-constant get a value of this Q boolean
			virtual Qvalue value() const;
			virtual Qvalue value();

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbool(*this)); };

			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false,
									size_t forBit = cAllBits) const;

			// Override to set solution values from the sample set for this deffinition
			virtual void solutions(const Qsolver::Samples& samples);

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			/*** Assignment ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbool& operator=(const Qbool& right);

			// assignment of an Q expression creates a Q bool assignment where this 
			// Q boolean is an assignee
			Qassign<Qbool> operator=(const Qexpr<Qbool>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bool
			Qbool& operator&=(const Qbool& right);

			// 'and assignment' of an Q expression creates a following Q bool assignment
			// [this] = [this] & [right]
			Qassign<Qbool> operator&=(const Qexpr<Qbool>& right);

			// change value of this object as result of or operation between this 
			// and right Q bool
			Qbool& operator|=(const Qbool& right);

			// 'or assignment' of an Q expression creates a following Q bool assignment
			// [this] = [this] | [right]
			Qassign<Qbool> operator|=(const Qexpr<Qbool>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bool
			Qbool& operator^=(const Qbool& right);

			/*** Logical ***/
			// instantiate Q expression with inversion logic, e.g. for Qbool with id 'x'
			// the expression is '!x' != 'x'
			Qexpr<Qbool> operator !() const;

			// instantiate Q expression with and logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' && 'y'
			Qexpr<Qbool> operator &(const Qbool& right) const;
			Qexpr<Qbool> operator and(const Qbool& right) const { return (*this) & right; };

			// instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' && [right]
			Qexpr<Qbool> operator &(const Qexpr<Qbool>& right) const;
			Qexpr<Qbool> operator and(const Qexpr<Qbool>& right) const { return (*this) & right; };

			// instantiate Q expression with or logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' || 'y'
			Qexpr<Qbool> operator |(const Qbool& right) const;
			Qexpr<Qbool> operator or(const Qbool& right) const { return (*this) | right; };

			// instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' || [right]
			Qexpr<Qbool> operator |(const Qexpr<Qbool>& right) const;
			Qexpr<Qbool> operator or(const Qexpr<Qbool>& right) const { return (*this) | right; };

			/*** Comparison ***/

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbool> operator==(const Qbool& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' == [right root]			
			Qexpr<Qbool> operator==(const Qexpr<Qbool>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbool> operator!=(const Qbool& right) const;


			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' != [right root]			
			Qexpr<Qbool> operator!=(const Qexpr<Qbool>& right) const;

		protected:
		private:
			Qvalue mValue;
		};
	};
};