#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qcell.h>
#include <Qassign.h>
#include <Qvalue.h>
#include <Qexpr.h>

#include <utility>

using namespace std;

namespace dann5 {
    // Quantum bit is a Q cell type supporting instantiation of and operations
    // forvariables which can have values 0, 1 and 'S'-uperposition, value 83
    class Qbit : public Qcell, public Qtype
    {
    public:
        // Q bit shared pointer
        typedef shared_ptr<Qbit> Sp;
        
        // default constants
        static const Qbit _0;
        static const Qbit _1;

        // Default constructor initializes Q bit with an empty id and supperposition
        // value
        Qbit() : Qcell(""), Qtype(), mValue(cSuperposition) {};

        // Initialize Q bit with a given id and supperposition value
        Qbit(const string& id) : Qcell(id), Qtype(), mValue(cSuperposition) {};

        // Initialize Q bit with a given id and value. If value is different  from 0 or 1,
        // Qbit into superposition state
        Qbit(const string& id, Qvalue value) : Qcell(id), Qtype(), mValue(value) {
            if (value > 1) mValue = cSuperposition;
        };

        // copy constructor
        Qbit(const Qbit& right) : Qcell(right), Qtype(right), mValue(right.mValue) {};

        // set the new value of this Q bit
        virtual void value(Qvalue v) {
            if (v > 1) mValue = cSuperposition;
            else mValue = v;
        };

        // constant and non-constant get a value of this Q bit
        virtual Qvalue value() const { return mValue; };
        virtual Qvalue value() { return mValue; };

        // Return a Q def shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbit(*this)); };

        /*** Assignments ***/
        // assignment of an Q expression creates a Q bit assignment where this
        // Q bit is an assignee of equal expression between this to right Q bit
        Qassign<Qbit> operator=(const Qbit& right) const;

        // assignment of an Q expression creates a Q bit assignment where this
        // Q bit is an assignee
        Qassign<Qbit> operator=(const Qexpr<Qbit>& right) const;

        /*** Compound Assignments ***/
        // 'and assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] & [right]
        Qassign<Qbit> operator&=(const Qbit& right) const;

        // 'and assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] & [right]
        Qassign<Qbit> operator&=(const Qexpr<Qbit>& right) const;

        // 'or assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] | [right]
        Qassign<Qbit> operator|=(const Qbit& right) const;

        // 'or assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] | [right]
        Qassign<Qbit> operator|=(const Qexpr<Qbit>& right) const;

        // 'xor assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] ^ [right]
        Qassign<Qbit> operator^=(const Qbit& right) const;

        // 'xor assignment' of an Q expression creates a following Q bit assignment
        // [this] = [this] ^ [right]
        Qassign<Qbit> operator^=(const Qexpr<Qbit>& right) const;

        /*** Bitwise ***/
        // instantiate Q boolean expression with inversion logic, e.g. for Qbit with
        // id 'x' the expression is '~x' != 'x'
        Qexpr<Qbit> operator~() const;

        // instantiate Q expression with and logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' & 'y'
        Qexpr<Qbit> operator&(const Qbit& right) const;
        Qexpr<Qbit> operator and(const Qbit& right) const { return (*this) & right; };

        // instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right]
        // object the expression is 'x' & [right]
        Qexpr<Qbit> operator&(const Qexpr<Qbit>& right) const;
        Qexpr<Qbit> operator and(const Qexpr<Qbit>& right) const { return (*this) & right; };

        // instantiate Q expression with nand logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is !('x' & 'y')
        Qexpr<Qbit> nand(const Qbit& right) const;

        // instantiate Q expression with nand logic, e.g. for Qbit id 'x' and [right]
        // object the expression is !('x' & [right])
        Qexpr<Qbit> nand(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with or logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' | 'y'
        Qexpr<Qbit> operator|(const Qbit& right) const;
        Qexpr<Qbit> operator or(const Qbit& right) const { return (*this) | right; };

        // instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right]
        // object the expression is 'x' | [right]
        Qexpr<Qbit> operator|(const Qexpr<Qbit>& right) const;
        Qexpr<Qbit> operator or(const Qexpr<Qbit>& right) const { return (*this) | right; };

        // instantiate Q expression with nor logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is !('x' | 'y')
        Qexpr<Qbit> nor(const Qbit& right) const;

        // instantiate Q expression with nor logic, e.g. for Qbit id 'x' and [right]
        // object the expression is !('x' | [right])
        Qexpr<Qbit> nor(const Qexpr<Qbit>& right) const;

        // unlike Q expression with xor logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' ^ 'y'
        Qexpr<Qbit> unlike(const Qbit& right) const;
        Qexpr<Qbit> operator^(const Qbit& right) const { return unlike(right); };

        // unlike Q expression with xor logic, e.g. for Qbit id 'x' and [right]
        // object the expression is 'x' ^ [right]
        Qexpr<Qbit> unlike(const Qexpr<Qbit>& right) const;
        Qexpr<Qbit> operator^(const Qexpr<Qbit>& right) const { return unlike(right); };

        // same value Q expression with nxor logic, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' ^ 'y'
        Qexpr<Qbit> alike(const Qbit& right) const;
        Qexpr<Qbit> operator*=(const Qbit& right) const { return alike(right); };
        Qexpr<Qbit> nxor(const Qbit& right) const { return alike(right); };

        // same value Q expression with nxor logic, e.g. for Qbit id 'x' and [right]
        // object the expression is 'x' ^ [right]
        Qexpr<Qbit> alike(const Qexpr<Qbit>& right) const;
        Qexpr<Qbit> operator*=(const Qexpr<Qbit>& right) const { return alike(right); };
        Qexpr<Qbit> nxor(const Qexpr<Qbit>& right) const { return alike(right); };

        /*** Comparison ***/
        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' == 'y'
        Qexpr<Qbit> operator==(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' == [right root]
        Qexpr<Qbit> operator==(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' != 'y'
        Qexpr<Qbit> operator!=(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' != [right root]
        Qexpr<Qbit> operator!=(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' > 'y'
        Qexpr<Qbit> operator>(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' > [right root]
        Qexpr<Qbit> operator>(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' >= 'y'
        Qexpr<Qbit> operator>=(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' >= [right root]
        Qexpr<Qbit> operator>=(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' < 'y'
        Qexpr<Qbit> operator<(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' < [right root]
        Qexpr<Qbit> operator<(const Qexpr<Qbit>& right) const;

        // instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
        // the expression is 'x' <= 'y'
        Qexpr<Qbit> operator<=(const Qbit& right) const;

        // instantiate Q expression with comparison, e.g. for arguments
        // 'x' and [right] the expression is 'x' <= [right root]
        Qexpr<Qbit> operator<=(const Qexpr<Qbit>& right) const;

    protected:
    private:
        Qvalue mValue;
    };

    // Quantum vector of Q cell references
    typedef vector<Qbit::Sp> Qbits;
};
