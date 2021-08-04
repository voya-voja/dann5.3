#pragma once
#pragma once


#include <pybind11/pybind11.h>

#include <Qbin.h>
#include <Qexpr.h>

#include <bitset>

using namespace std;

namespace dann5 {
	namespace ocean {

		// Quantum int is a Q type, subtype of Qbin supporting instantiation of 
		// and operations for positive int number variables. As Qbianry, Q int
		// can be unknown when at least one of its Qbits is in supoerposition.
		class Qint : public Qbin
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qint> Sp;

			// Qint is unknown, if any of its Qbits is in superposition state
			static const unsigned long long cUnknown = ULLONG_MAX;

			// Qint unknown string representation is "(U)"
			static const string cUnknownSign;

			// Initialize Q int with a given id and byte size 0 value
			Qint(const string& id) : Qbin(id) {};

			// Initialize Q int with a given id and unknown value of specified size
			Qint(size_t size, const string& id) : Qbin(size + 1, id) {};

			// Initialize Q int with a given id and value as a Qbit vector
			Qint(const string& id, const Qbits& value) : Qbin(id, value) {};

			// Initialize Q int with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qint(const string& id, const Bits& value, bool asis = false);

			// Initialize Q int of given size, with a given id and a deterministic
			// value as a bitset
			Qint(size_t size, const string& id, const Bits& value);

			// copy constructor
			Qint(const Qint& right) : Qbin(right) {};

			// type conversion operator
			operator long long();
			operator const long long() const;

			// Resizes the Q int to size elements. If negative and size
			// is greater than the current size, the content is expanded by 
			// inserting at the end as many elements with value 1 as needed to reach
			// the new size.
			virtual void resize(size_t size, Qvalue value = 0);

			// Return a Q int representation when bitLevel is ULLONG_MAX. 
			// Otherwise, return a string representation of a Q cell at the specified 
			// bitLevel
			virtual string toString(bool decomposed = false,
				size_t bitLevel = ULLONG_MAX) const;

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qint(*this)); };

			/*** Assignments ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qint& operator=(const Qint& right);

			// assignment of an Q expression creates a Q int assignment where this 
			// Q int is an assignee
			Qassign<Qint> operator=(const Qexpr<Qint>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bit
			Qint& operator+=(const Qint& right);

			// 'add assignment' of an Q expression creates a following Q int assignment
			// [this] = [this] + [right]
			Qassign<Qint> operator+=(const Qexpr<Qint>& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qint& operator*=(const Qint& right);

			// 'add assignment' of an Q expression creates a following Q int assignment
			// [this] = [this] * [right]
			Qassign<Qint> operator*=(const Qexpr<Qint>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qint& operator-=(const Qint& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qint& operator/=(const Qint& right);

			/*** Comparison ***/
			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qint> operator==(const Qint& right) const;

			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qint> operator!=(const Qint& right) const;

			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' > 'y'
			Qexpr<Qint> operator>(const Qint& right) const;

			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			Qexpr<Qint> operator>=(const Qint& right) const;

			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' < 'y'
			Qexpr<Qint> operator<(const Qint& right) const;

			// instantiate Q expression with comparison, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			Qexpr<Qint> operator<=(const Qint& right) const;

			/*** Arithmetic ***/

			// instantiate Q expression with and logic, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qint> operator+(const Qint& right) const;

			// instantiate Q expression with and logic, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qint> operator+(const Qexpr<Qint>& right) const;

			// instantiate Q expression with or logic, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qint> operator*(const Qint& right) const;

			// instantiate Q expression with or logic, e.g. for Qint ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qint> operator*(const Qexpr<Qint>& right) const;

		protected:
		private:
		};
	};
};