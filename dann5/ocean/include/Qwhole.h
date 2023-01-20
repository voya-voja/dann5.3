#pragma once


#include <pybind11/pybind11.h>

#include <Qbin.h>
#include <Qexpr.h>

#include <bitset>

using namespace std;

namespace dann5 {
	namespace ocean {

		class Qint;

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

			// Default constructor initializes Q whole an empty binary
			Qwhole() :Qbin() {};

			// Initialize Q whole with a given id and byte size 0 value
			Qwhole(const string& id) : Qbin(id) {};

			// Initialize Q whole with a given id and unknown value of specified size
			Qwhole(size_t size, const string& id) : Qbin(size, id) {};

			// Initialize Q whole with a given id and value as a Qbit vector
			Qwhole(const string& id, const Qbits& value) : Qbin(id, value) {};

			// Initialize Q whole with a given id and value as a Qbit vector
			Qwhole(const string& id, unsigned long long value) : Qbin(id, value) {};

			// Initialize Q whole with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qwhole(const string& id, const Bits& value, bool asis = false) : Qbin(id, value, asis) {};

			// copy constructor
			Qwhole(const Qwhole& right) : Qbin(right) {};

			// type conversion operator
			operator unsigned long long();
			operator const unsigned long long() const;

			// Resizes the Q whole to size elements. If deterministic and size
			// is greater than the current size, the content is expanded by 
			// inserting at the end as many elements with value 0 as needed to reach
			// the new size.
			virtual void resize(size_t size, Qvalue value = 0);

			// Return a Q whole representation when forBit is cAllBits. 
			// Otherwise, return a string representation of a Q cell at the specified 
			// forBit
			virtual string toString(bool decomposed = false,
							size_t forBit = cAllBits) const;

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qwhole(*this)); };

			/*** Assignments ***/
			// assignment of a Q whole creates a Q whole assignment where this 
			// Q whole is an assignee
			Qassign<Qwhole> operator=(const Qwhole& right);

			// assignment of a Q expression creates a Q whole assignment where this 
			// Q whole is an assignee
			Qassign<Qwhole> operator=(const Qexpr<Qwhole>& right);

			/*** Compound Assignments ***/
			// 'add assignment' of a Q whole creates a following Q whole assignment
			// [this] = [this] + [right]
			Qassign<Qwhole> operator+=(const Qwhole& right);

			// 'add assignment' of a Q expression creates a following Q whole assignment
			// [this] = [this] + [right]
			Qassign<Qwhole> operator+=(const Qexpr<Qwhole>& right);

			// 'add assignment' of a Q whole creates a following Q whole assignment
			// [this] = [this] * [right]
			Qassign<Qwhole> operator*=(const Qwhole& right);

			// 'add assignment' of a Q expression creates a following Q whole assignment
			// [this] = [this] * [right]
			Qassign<Qwhole> operator*=(const Qexpr<Qwhole>& right);

			// 'add assignment' of a Q whole creates a following Q whole assignment
			// [this] = [this] - [right]
			Qassign<Qwhole> operator-=(const Qwhole& right);

			// 'add assignment' of a Q expression creates a following Q whole assignment
			// [this] = [this] - [right]
			Qassign<Qwhole> operator-=(const Qexpr<Qwhole>& right);


			// 'add assignment' of a Q whole creates a following Q whole assignment
			// [this] = [this] / [right]
			Qassign<Qwhole> operator/=(const Qwhole& right);

			// 'add assignment' of a Q expression creates a following Q whole assignment
			// [this] = [this] / [right]
			Qassign<Qwhole> operator/=(const Qexpr<Qwhole>& right);

			/*** Comparison ***/
			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qwhole> operator==(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole id 'x' and 'y'
			// expression [right] the expression is 'x' == [right]
			Qexpr<Qwhole> operator==(const Qexpr<Qwhole>& right);

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qwhole> operator!=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole id 'x' and 'y'
			// expression [right] the expression is 'x' != [right]
			Qexpr<Qwhole> operator!=(const Qexpr<Qwhole>& right);

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

			// instantiate Q expression with add logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qwhole> operator+(const Qwhole& right) const;

			// instantiate Q expression with add logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' + [right]
			Qexpr<Qwhole> operator+(const Qexpr<Qwhole>& right) const;

			// instantiate Q expression with multiply logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qwhole> operator*(const Qwhole& right) const;

			// instantiate Q expression with multiply logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' * [right]
			Qexpr<Qwhole> operator*(const Qexpr<Qwhole>& right) const;

			// instantiate Q expression with subtract logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' - 'y'
			Qexpr<Qwhole> operator-(const Qwhole& right) const;

			// Subtraction with EXPRESSION needs to be tested
			// instantiate Q expression with subtract logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' - [right]
			Qexpr<Qwhole> operator-(const Qexpr<Qwhole>& right) const;

			// instantiate Q expression with divide logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' / 'y'
			Qexpr<Qwhole> operator/(const Qwhole& right) const;

			// Division with EXPRESSION needs to be tested
			// instantiate Q expression with divide logic, e.g. for Qwhole id 'x' and [right]
			// object the expression is 'x' / [right]
			Qexpr<Qwhole> operator/(const Qexpr<Qwhole>& right) const;

		protected:
		private:
		};
	};
};