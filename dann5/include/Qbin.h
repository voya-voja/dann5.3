#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qnary.h>
#include <Qbit.h>
#include <Qexpr.h>

namespace dann5 {
    // Quantum binary is a Q type supporting instantiation of and operations for
    // variables which can have one or more of Qbits. For example Qbin with 8
    // Qbits would correspond to a Quantum byte
    class Qbin : public Qtype, public Qnary
    {
    public:
        // Q binary shared pointer
        typedef shared_ptr<Qbin> Sp;

        // default constants
        static const Qbin _0;
        static const Qbin _1;

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

        // type conversion operator to a std::vertor of Qcells
        operator Qbits();
        operator const Qbits() const;

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbin(*this)); };

        Qbit& operator[](size_t pos);

        /*** Assignments ***/
        // assignment of a Q binary creates a Q bin assignment where this
        // Q bin is an assignee
        Qassign<Qbin> operator=(const Qbin& right) const;

        // assignment of a Q expression creates a Q bin assignment where this
        // Q bin is an assignee
        Qassign<Qbin> operator=(const Qexpr<Qbin>& right) const;

        /*** Compound Assignments ***/
        // 'and assignment' of a Q binary creates a following Q bin assignment
        // [this] = [this] & [right]
        Qassign<Qbin> operator&=(const Qbin& right) const;

        // 'and assignment' of a Q expression creates a following Q bin assignment
        // [this] = [this] & [right]
        Qassign<Qbin> operator&=(const Qexpr<Qbin>& right) const;

        // 'or assignment' of a Q binary creates a following Q bin assignment
        // [this] = [this] | [right]
        Qassign<Qbin> operator|=(const Qbin& right) const;

        // 'or assignment' of a Q expression creates a following Q bin assignment
        // [this] = [this] | [right]
        Qassign<Qbin> operator|=(const Qexpr<Qbin>& right) const;

        // 'xor assignment' of a Q binary creates a following Q bin assignment
        // [this] = [this] ^ [right]
        Qassign<Qbin> operator^=(const Qbin& right) const;

        // 'xor assignment' of a Q expression creates a following Q bin assignment
        // [this] = [this] ^ [right]
        Qassign<Qbin> operator^=(const Qexpr<Qbin>& right) const;

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
        // the expression is !('x' & 'y')
        Qexpr<Qbin> nand(const Qbin& right) const;

        // instantiate Q expression with nand logic, e.g. for Qbin id 'x' and [right]
        // object the expression is !('x' & [right])
        Qexpr<Qbin> nand(const Qexpr<Qbin>& right) const;

        // instantiate Q expression with or logic, e.g. for Qbin ids 'x' and 'y'
        // the expression is 'x' | 'y'
        Qexpr<Qbin> operator|(const Qbin& right) const;

        // instantiate Q expression with or logic, e.g. for Qbin id 'x' and [right]
        // object the expression is 'x' | [right]
        Qexpr<Qbin> operator|(const Qexpr<Qbin>& right) const;

        // instantiate Q expression with nor logic, e.g. for Qbin ids 'x' and 'y'
        // the expression is !('x' | 'y')
        Qexpr<Qbin> nor(const Qbin& right) const;

        // instantiate Q expression with nor logic, e.g. for Qbin id 'x' and [right]
        // object the expression is !('x' | [right])
        Qexpr<Qbin> nor(const Qexpr<Qbin>& right) const;

        // instantiate Q expression with xor logic, e.g. for Qbin ids 'x' and 'y'
        // the expression is 'x' ^ 'y'
        Qexpr<Qbin> unlike(const Qbin& right) const;
        Qexpr<Qbin> operator^(const Qbin& right) const { return unlike(right); };;

        // instantiate Q expression with xor logic, e.g. for Qbin id 'x' and [right]
        // object the expression is 'x' ^ [right]
        Qexpr<Qbin> unlike(const Qexpr<Qbin>& right) const;
        Qexpr<Qbin> operator^(const Qexpr<Qbin>& right) const { return unlike(right); };;

        // same value Q expression with nxor logic, e.g. for Qbin ids 'x' and 'y'
        // the expression is 'x' ^ 'y'
        Qexpr<Qbin> alike(const Qbin& right) const;
        Qexpr<Qbin> operator*=(const Qbin& right) const { return alike(right); };
        Qexpr<Qbin> nxor(const Qbin& right) const { return alike(right); };

        // same value Q expression with nxor logic, e.g. for Qbin id 'x' and [right]
        // object the expression is 'x' ^ [right]
        Qexpr<Qbin> alike(const Qexpr<Qbin>& right) const;
        Qexpr<Qbin> operator*=(const Qexpr<Qbin>& right) const { return alike(right); };
        Qexpr<Qbin> nxor(const Qexpr<Qbin>& right) const { return alike(right); };

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
