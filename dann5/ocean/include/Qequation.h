#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Qstatement.h>

#include <Qdef.h>

using namespace std;


namespace dann5 {
	namespace ocean {

//		class Qroutine;

		// Quantum equation is a coupling of result Q variable via Q expression with
		// Qvar arguments
		class Qequation : public Qstatement
		{
		public:
			// Qequation's shared pointer 
			typedef shared_ptr<Qequation> Sp;

			// Default constructor creates Q equation witout a result Q variable so the
			// result definition is an empty string
//			Qequation();

			// creates Q equation with a given Q variable as an expected result
			Qequation(const Qdef&);

			// instantiate a Q equation with result Q definition and corresonting Q expression
			// if Qnary result and expression, size of result has to be of same size as expression
			// if expression has less Q bits, remaining Q bits or result will result 0 bitwise logic, 
			// e.g. those Q bits are irelevant as they will not contribute to a derived Qubo model
			Qequation(const Qdef&, const Qexpression&);

			//copy constructor
			Qequation(const Qequation&);

			// desruct the Q equation with its members
			~Qequation() {};

			// Returns a constant reference to the result of this Q equation
			Qdef::Sp result() const { return mpResult; }
			Qdef& result() { return *mpResult; }

			// Returns a string representation of this Q equation, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, size_t level = ULLONG_MAX) const;

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const { return Qstatement::Sp(new Qequation(*this)); };

			// Assignment operator returns a reference of this Q equation with Q result,
			// expression and assigned Q expression
//			Qequation& operator=(const Qequation& right);
			Qequation& operator=(const Qexpression& right);

			// Insert string representation of a Q equation into an output stream
			friend std::ostream& operator << (std::ostream&, const Qequation&);

/*			// And operator returns a new Qequation object with after applying
			// and operation on this Q equation and right Q variable
			Qequation operator&(const Qvar& right) const;

			// And operator returns a new Qequation object resulting after applying
			// and operation on this and right Q equations
			Qequation operator&(const Qequation& right) const;

			// And operator returns a reference to this Qequation object after applying
			// and operation on right Q variable
			Qequation& operator&=(const Qvar& right);

			// And operator returns a reference to this Qequation object after applying
			// and operation on right Q equation
			Qequation& operator&=(const Qequation& right);

			// Or operator returns a new Qequation object with after applying
			// or operation on this Q equation and right Q variable
			Qequation operator|(const Qvar& right) const;

			// Or operator returns a new Qequation object resulting after applying
			// or operation on this and right Q equations
			Qequation operator|(const Qequation& right) const;

			// Or operator returns a reference to this Qequation object after applying
			// or operation on right Q variable
			Qequation& operator|=(const Qvar& right);

			// Or operator returns a reference to this Qequation object after applying
			// or operation on right Q equation
			Qequation& operator|=(const Qequation& right);

			// Nand operator returns a new Qequation object with after applying
			// nand operation on this Q equation and right Q variable
			Qequation nand(const Qvar& right) const;

			// Nand operator returns a new Qequation object resulting after applying
			// nand operation on this and right Q equations
			Qequation nand(const Qequation& right) const;

			// Nand operator returns a reference to this Qequation object after applying
			// nand operation on right Q variable
			Qequation& nandMutable(const Qvar& right);

			// Nand operator returns a reference to this Qequation object after applying
			// nand operation on right Q equation
			Qequation& nandMutable(const Qequation& right);

			// Nor operator returns a new Qequation object with after applying
			// nor operation on this Q equation and right Q variable
			Qequation nor(const Qvar& right) const;

			// Nor operator returns a new Qequation object resulting after applying
			// nor operation on this and right Q equations
			Qequation nor(const Qequation& right) const;

			// Nor operator returns a reference to this Qequation object after applying
			// nor operation on right Q variable
			Qequation& norMutable(const Qvar& right);

			// Nor operator returns a reference to this Qequation object after applying
			// nor operation on right Q equation
			Qequation& norMutable(const Qequation& right);

			// Xor operator returns a new Qequation object with after applying
			// xor operation on this Q equation and right Q variable
			Qequation operator^(const Qvar& right) const;

			// Xor operator returns a new Qequation object resulting after applying
			// xor operation on this and right Q equations
			Qequation operator^(const Qequation& right) const;

			// Xor operator returns a reference to this Qequation object after applying
			// xor operation on right Q variable
			Qequation& operator^=(const Qvar& right);

			// Xor operator returns a reference to this Qequation object after applying
			// xor operation on right Q equation
			Qequation& operator^=(const Qequation& right);

			// Addition operator returns a new Qequation object with added Q variable
			// to this Q equation
			Qequation operator+(const Qvar& right) const;

			// Addition operator returns a new Qequation object resulting from addition
			// of this and right Q equations
			Qequation operator+(const Qequation& right) const;

			// Addition operator returns a reference to this Qequation object with added
			// Q variable
			Qequation& operator+=(const Qvar& right);

			// Addition operator returns a reference to this Qequation object with added
			// right Q equation
			Qequation& operator+=(const Qequation& right);

			// Subtraction operator returns a Q routine instance with 2 Q equations with 
			// same result. One with the expression of this Q equation and the other
			// with multiplying expression of the result of this Q equation and the
			// right Q variable, e.g. for a Q variabble z, equation with result y and 
			// expression x, and routine 'sub':
			// y = x - z	=>	sub << S_y = x << S_y = y + z
			Qroutine operator-(const Qvar& right) const;

			// Multiplication operator returns a new Qequation object with multiplied Q
			// variable with this Q equation
			Qequation operator*(const Qvar& right) const;

			// Multiplication operator returns a new Qequation object resulting from
			// multiplication of this and right Q equations
			Qequation operator*(const Qequation& right) const;

			// Multiplication operator returns a reference to this Qequation object with
			// multiplied Q variable
			Qequation& operator*=(const Qvar& right);

			// Multiplication operator returns a reference to this Qequation object with
			// multiplied right Q equation
			Qequation& operator*=(const Qequation& right);

			// Division operator returns a Qroutine instance with 2 Q equations with same
			// result, one with the expression of this Q equation and the other with
			// multiplying expression of the result of this Q equation and the right Q 
			// variable, e.g. for a Q variabble z, equation with result y and expression
			// x, and routine 'div':
			// y = x / z	=>	div << D_y = x << D_y = y * z
			Qroutine operator/(const Qvar& right) const;
*/


		protected:
/*			class Reduct : public Qstatement::Reduct
			{
			public:
				// construct reduction object of a given Q equation
				Reduct(Qequation&);

				// prepare reduction information for a given Q equation 
				virtual void init();

			protected:
			private:
				Qequation&	mEquation;
			};

			virtual Reduct* createReduct() { return new Reduct(*this); };
*/
			virtual void bindExpression();

		private:
			Qdef::Sp			mpResult;		// result variable, e.g. defined as R = 2 with bits R0 = 0, R1 = 1, ...
//			bool				mNoResult;

//			friend class Reduct;
		};
	};
};