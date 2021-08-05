#pragma once


#include <pybind11/pybind11.h>

#include <Qbin.h>
#include <Qexpr.h>

#include <bitset>

using namespace std;

namespace dann5 {
	namespace ocean {

		// Quantum whole is a Q type, subtype of Qbin supporting instantiation of 
		// and operations for positive whole number variables. As Qbianry, Q whole
		// can be unknown when at least one of its Qbits is in supoerposition.
		class Qwhole : public Qbin
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qwhole> Sp;

			// Qwhole is unknown, if any of its Qbits is in superposition state
			static const unsigned long long cUnknown = ULLONG_MAX;

			// Qwhole unknown string representation is "(U)"
			static const string cUnknownSign;

			// Initialize Q whole with a given id and byte size 0 value
			Qwhole(const string& id) : Qbin(id) {};

			// Initialize Q whole with a given id and unknown value of specified size
			Qwhole(size_t size, const string& id) : Qbin(size, id) {};

			// Initialize Q whole with a given id and value as a Qbit vector
			Qwhole(const string& id, const Qbits& value) : Qbin(id, value) {};

			// Initialize Q whole with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qwhole(const string& id, const Bits& value, bool asis = false) : Qbin(id, value, asis) {};

			// copy constructor
			Qwhole(const Qwhole& right) : Qbin(right) {};

			// type conversion operator
			operator unsigned long long();
			operator const unsigned long long() const;

			// Return a Q whole representation when bitLevel is ULLONG_MAX. 
			// Otherwise, return a string representation of a Q cell at the specified 
			// bitLevel
			virtual string toString(bool decomposed = false,
							size_t bitLevel = ULLONG_MAX) const;

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qwhole(*this)); };

			/*** Assignments ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qwhole& operator=(const Qwhole& right);

			// assignment of an Q expression creates a Q whole assignment where this 
			// Q whole is an assignee
			Qassign<Qwhole> operator=(const Qexpr<Qwhole>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bit
			Qwhole& operator+=(const Qwhole& right);

			// 'add assignment' of an Q expression creates a following Q whole assignment
			// [this] = [this] + [right]
			Qassign<Qwhole> operator+=(const Qexpr<Qwhole>& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qwhole& operator*=(const Qwhole& right);

			// 'add assignment' of an Q expression creates a following Q whole assignment
			// [this] = [this] * [right]
			Qassign<Qwhole> operator*=(const Qexpr<Qwhole>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qwhole& operator-=(const Qwhole& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qwhole& operator/=(const Qwhole& right);

			/*** Comparison ***/
			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qwhole> operator==(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qwhole> operator!=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' > 'y'
			Qexpr<Qwhole> operator>(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			Qexpr<Qwhole> operator>=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' < 'y'
			Qexpr<Qwhole> operator<(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			Qexpr<Qwhole> operator<=(const Qwhole& right) const;

			/*** Arithmetic ***/

			// instantiate Q expression with and logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qwhole> operator+(const Qwhole& right) const;

			// instantiate Q expression with add logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' + [right]
			Qexpr<Qwhole> operator+(const Qexpr<Qwhole>& right) const;

			// instantiate Q expression with or logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qwhole> operator*(const Qwhole& right) const;

			// instantiate Q expression with multiply logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' * [right]
			Qexpr<Qwhole> operator*(const Qexpr<Qwhole>& right) const;

		protected:
		private:
		};
	};
};