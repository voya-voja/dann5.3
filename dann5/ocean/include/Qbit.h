#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qcell.h>
#include <Qexpr.h>
#include <Qassign.h>

#include <utility>

using namespace std;

namespace dann5 {
	namespace ocean {

		class Qbool;

		// Quantum bit is a Q cell type supporting instantiation of and operations
		// forvariables which can have values 0, 1 and 'S'-uperposition, value 83
		class Qbit : public Qcell, public Qtype
		{
		public:
			// Q bit shared pointer 
			typedef shared_ptr<Qbit> Sp;

			// Default constructor initializes Q bit with an empty id and supperposition
			// value
			Qbit() : Qcell(""), Qtype(), mValue(cSuperposition) {};

			// Initialize Q bit with a given id and supperposition value
			Qbit(const string& id) : Qcell(id), Qtype(), mValue(cSuperposition) {};

			// Initialize Q bit with a given id and value. If value is different  from 0 or 1, 
			// Qbit into superposition state
			Qbit(const string& id, Qvalue value) : Qcell(id), Qtype(), mValue(value)
			{
				if (value > 1) mValue = cSuperposition;
			};

			// copy constructor
			Qbit(const Qbit& right) : Qcell(right), Qtype(right), mValue(right.mValue) {};

			// set the new value of this Q bit
			virtual void value(Qvalue v) { mValue = v; };

			// constant and non-constant get a value of this Q bit
			virtual Qvalue value() const { return mValue; };
			virtual Qvalue value() { return mValue; };

			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false,
									size_t forBit = cAllBits) const;

			// Return a Q def shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbit(*this)); };

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t noqbs() const noexcept { return 1; };

			// Override to set solution values from the sample set for this deffinition
			virtual void solutions(const Qsolver::Samples& samples);

			/*** Assignments ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbit& operator=(const Qbit& right);

			// assignment of an Q expression creates a Q bit assignment where this 
			// Q bit is an assignee
			Qassign<Qbit> operator=(const Qexpr<Qbit>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bit
			Qbit& operator&=(const Qbit& right);

			// 'and assignment' of an Q expression creates a following Q bit assignment
			// [this] = [this] & [right]
			Qassign<Qbit> operator&=(const Qexpr<Qbit>& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qbit& operator|=(const Qbit& right);

			// 'or assignment' of an Q expression creates a following Q bit assignment
			// [this] = [this] | [right]
			Qassign<Qbit> operator|=(const Qexpr<Qbit>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qbit& operator^=(const Qbit& right);

			// 'xor assignment' of an Q expression creates a following Q bit assignment
			// [this] = [this] ^ [right]
			Qassign<Qbit> operator^=(const Qexpr<Qbit>& right);

			/*** Bitwise ***/
			// instantiate Q boolean expression with inversion logic, e.g. for Qbit with 
			// id 'x' the expression is '~x' != 'x'
			Qexpr<Qbit> operator~() const;

			// instantiate Q expression with and logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' & 'y'
			Qexpr<Qbit> operator&(const Qbit& right) const;

			// instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' & [right]
			Qexpr<Qbit> operator&(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with nand logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is ~('x' & 'y')
			Qexpr<Qbit> nand(const Qbit& right) const;

			// instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right]
			// object the expression is ~('x' & [right])
			Qexpr<Qbit> nand(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' | 'y'
			Qexpr<Qbit> operator|(const Qbit& right) const;

			// instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' | [right]
			Qexpr<Qbit> operator|(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with nor logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is ~('x' | 'y')
			Qexpr<Qbit> nor(const Qbit& right) const;

			// instantiate Q expression with nor logic, e.g. for Qbit id 'x' and [right]
			// object the expression is ~('x' | [right])
			Qexpr<Qbit> nor(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' ^ 'y'
			Qexpr<Qbit> operator^(const Qbit& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' ^ [right]
			Qexpr<Qbit> operator^(const Qexpr<Qbit>& right) const;

			/*** Comparison ***/
			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbool> operator==(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' == [right root]			
			Qexpr<Qbool> operator==(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbool> operator!=(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' != [right root]			
			Qexpr<Qbool> operator!=(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' > 'y'
			Qexpr<Qbool> operator>(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' > [right root]			
			Qexpr<Qbool> operator>(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			Qexpr<Qbool> operator>=(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' >= [right root]			
			Qexpr<Qbool> operator>=(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' < 'y'
			Qexpr<Qbool> operator<(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' < [right root]			
			Qexpr<Qbool> operator<(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			Qexpr<Qbool> operator<=(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for arguments
			// 'x' and [right] the expression is 'x' <= [right root]			
			Qexpr<Qbool> operator<=(const Qexpr<Qbit>& right) const;

		protected:
		private:
			Qvalue mValue;
		};

		// Quantum vector of Q cell references
		typedef vector<Qbit::Sp> Qbits;
	};
};