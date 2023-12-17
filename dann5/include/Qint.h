#pragma once
#pragma once


#include <pybind11/pybind11.h>

#include <Qbin.h>
#include <Qexpr.h>

#include <bitset>

using namespace std;

namespace dann5 {
    // Quantum int is a Q type, subtype of Qbin supporting instantiation of
    // and operations for positive int number variables. As Qbianry, Q int
    // can be unknown when at least one of its Qbits is in supoerposition.
    class Qint : public Qbin
    {
    public:
        // Q definition shared pointer
        typedef shared_ptr<Qint> Sp;

        // default constants
        static const Qint _0;
        static const Qint _1;
        static const Qint _1_;

        // Qint is unknown, if any of its Qbits is in superposition state
        static const unsigned long long cUnknown = ULLONG_MAX;

        // Qint unknown string representation is "(U)"
        static const string cUnknownSign;

        // Default constructor initializes Q integer an empty binary
        Qint() :Qbin() {};

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
        virtual void resize(size_t size, Qvalue value = cSuperposition);

        // Return a Q int representation when forBit is cAllBits.
        // Otherwise, return a string representation of a Q cell at the specified
        // forBit
        virtual string toString(bool decomposed = false,
            size_t forBit = cAllBits) const;

        // Returns a string representation of a solution value of this quantum
        // integer for an evaluation at 'atEvltn'
        virtual string solution(size_t atEvltn) const;

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qint(*this)); };

        /*** Assignments ***/
        // assignment of a Q int creates a Q int assignment where this
        // Q int is an assignee
        Qassign<Qint> operator=(const Qint& right) const;

        // assignment of a Q expression creates a Q int assignment where this
        // Q int is an assignee
        Qassign<Qint> operator=(const Qexpr<Qint>& right) const;

        /*** Compound Assignments ***/
        // 'add assignment' of a Q int creates a following Q int assignment
        // [this] = [this] + [right]
        Qassign<Qint> operator+=(const Qint& right) const;

        // 'add assignment' of a Q expression creates a following Q int assignment
        // [this] = [this] + [right]
        Qassign<Qint> operator+=(const Qexpr<Qint>& right) const;

        // 'add assignment' of a Q int creates a following Q int assignment
        // [this] = [this] * [right]
        Qassign<Qint> operator*=(const Qint& right) const;

        // 'add assignment' of a Q expression creates a following Q int assignment
        // [this] = [this] * [right]
        Qassign<Qint> operator*=(const Qexpr<Qint>& right) const;

        // 'add assignment' of a Q int creates a following Q int assignment
        // [this] = [this] - [right]
        Qassign<Qint> operator-=(const Qint& right) const;

        // 'add assignment' of a Q expression creates a following Q int assignment
        // [this] = [this] - [right]
        Qassign<Qint> operator-=(const Qexpr<Qint>& right) const;


        // 'add assignment' of a Q int creates a following Q int assignment
        // [this] = [this] / [right]
        Qassign<Qint> operator/=(const Qint& right) const;

        // 'add assignment' of a Q expression creates a following Q int assignment
        // [this] = [this] / [right]
        Qassign<Qint> operator/=(const Qexpr<Qint>& right) const;

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
        // instantiate Q expression with add logic, e.g. for Qint ids 'x' and 'y'
        // the expression is 'x' + 'y'
        Qexpr<Qint> operator+(const Qint& right) const;

        // instantiate Q expression with add logic, e.g. for Qint id 'x' and [right]
        // expression the expression is 'x' + [right]
        Qexpr<Qint> operator+(const Qexpr<Qint>& right) const;

        // instantiate Q expression with multiply logic, e.g. for Qint ids 'x' and 'y'
        // the expression is 'x' * 'y'
        Qexpr<Qint> operator*(const Qint& right) const;

        // instantiate Q expression with multiply logic, e.g. for Qint id 'x' and [right]
        // expression the expression is 'x' * [right]
        Qexpr<Qint> operator*(const Qexpr<Qint>& right) const;

        // instantiate Q expression with subtract logic, e.g. for Qint ids 'x' and 'y'
        // the expression is 'x' - 'y'
        Qexpr<Qint> operator-(const Qint& right) const;

        // Subtraction with EXPRESSION needs to be tested
        // instantiate Q expression with subtract logic, e.g. for Qint id 'x' and [right]
        // expression the expression is 'x' - [right]
        Qexpr<Qint> operator-(const Qexpr<Qint>& right) const;

        // instantiate Q expression with divide logic, e.g. for Qint ids 'x' and 'y'
        // the expression is 'x' / 'y'
        Qexpr<Qint> operator/(const Qint& right) const;

        // Division with EXPRESSION needs to be tested
        // instantiate Q expression with divide logic, e.g. for Qint id 'x' and [right]
        // expression the expression is 'x' / [right]
        Qexpr<Qint> operator/(const Qexpr<Qint>& right) const;

    protected:
    private:
    };
};
