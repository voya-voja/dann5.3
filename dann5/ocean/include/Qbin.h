#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qnary.h>
#include <Qbit.h>
#include <Qexpr.h>

namespace dann5 {
	namespace ocean {
		// Quantum binary is a Q type supporting instantiation of and operations for 
		// variables which can have one or more of Qbits. For example Qbin with 8
		// Qbits would correspond to a Quantum byte
		class Qbin : public Qtype, public Qnary
		{
		public:
			// Q binary shared pointer 
			typedef shared_ptr<Qbin> Sp;

			// Default constructor initializes Q bin with an empty id and byte size 0 value
			Qbin() :Qtype(), Qnary("") {};

			// Initialize Q binary with a given id and byte size 0 value
			Qbin(const string& id) :Qtype(), Qnary(id) {};

			// Initialize Q binary with a given id and unknown value of specified size
			Qbin(size_t size, const string& id);

			// Initialize Q binary with a given id and value as a Qbit vector
			Qbin(const string& id, const Qbits& value);

			// Initialize Q binary with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qbin(const string& id, const Bits& value, bool asis = false);

			// copy constructor
			Qbin(const Qbin& right) : Qtype(right), Qnary(right) {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbin(*this)); };


			/*** Assignments ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbin& operator=(const Qbin& right);

			// assignment of an Q expression creates a Q bin assignment where this 
			// Q bin is an assignee
			Qassign<Qbin> operator=(const Qexpr<Qbin>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bin
			Qbin& operator&=(const Qbin& right);

			// 'and assignment' of an Q expression creates a following Q bin assignment
			// [this] = [this] & [right]
			Qassign<Qbin> operator&=(const Qexpr<Qbin>& right);

			// change value of this object as result of or operation between this 
			// and right Q bin
			Qbin& operator|=(const Qbin& right);

			// 'or assignment' of an Q expression creates a following Q bin assignment
			// [this] = [this] | [right]
			Qassign<Qbin> operator|=(const Qexpr<Qbin>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bin
			Qbin& operator^=(const Qbin& right);

			// 'xor assignment' of an Q expression creates a following Q bin assignment
			// [this] = [this] ^ [right]
			Qassign<Qbin> operator^=(const Qexpr<Qbin>& right);

			/*** Bitwise ***/
			// instantiate Q expression with inversion logic, e.g. for Qbin with id 'x'
			// the logic is '~x' != 'x'
			Qexpr<Qbin> operator~() const;

			// instantiate Q expression with and logic, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' & 'y'
			Qexpr<Qbin> operator&(const Qbin& right) const;

			// instantiate Q expression with and logic, e.g. for Qbin id 'x' and [right]
			// object the expression is 'x' & [right]
			Qexpr<Qbin> operator&(const Qexpr<Qbin>& right) const;

			// instantiate Q expression with nand logic, e.g. for Qbin ids 'x' and 'y'
			// the expression is ~('x' & 'y')
			Qexpr<Qbin> nand(const Qbin& right) const;

			// instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right]
			// object the expression is ~('x' & [right])
			Qexpr<Qbin> nand(const Qexpr<Qbin>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' | 'y'
			Qexpr<Qbin> operator|(const Qbin& right) const;

			// instantiate Q expression with or logic, e.g. for Qbin id 'x' and [right]
			// object the expression is 'x' | [right]
			Qexpr<Qbin> operator|(const Qexpr<Qbin>& right) const;

			// instantiate Q expression with nor logic, e.g. for Qbin ids 'x' and 'y'
			// the expression is ~('x' | 'y')
			Qexpr<Qbin> nor(const Qbin& right) const;

			// instantiate Q expression with nor logic, e.g. for Qbin id 'x' and [right]
			// object the expression is ~('x' | [right])
			Qexpr<Qbin> nor(const Qexpr<Qbin>& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' ^ 'y'
			Qexpr<Qbin> operator^(const Qbin& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbin id 'x' and [right]
			// object the expression is 'x' ^ [right]
			Qexpr<Qbin> operator^(const Qexpr<Qbin>& right) const;

			/*** Comparison ***/
			// instantiate Q expression with comparison, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbin> operator==(const Qbin& right) const;

			// instantiate Q expression with comparison, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbin> operator==(const Qexpr<Qbin>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbin> operator!=(const Qbin& right) const;

			// instantiate Q expression with comparison, e.g. for Qbin ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbin> operator!=(const Qexpr<Qbin>& right) const;

		protected:
		private:
		};
	};
};