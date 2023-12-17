#pragma once

#include <pybind11/pybind11.h>

#include <Qdef.h>
#include <Qtype.h>
#include <Qexpr.h>
#include <Qassign.h>
#include <Qvalue.h>

namespace dann5 {
    // Quantum boolean is a Q cell type supporting instantiation of and operations
    // for variables which can have values 'T'-rue (value 84), 'F'-alse (value 71)
    // and 'S'-uperposition (value 83)
    class Qbool : public Qcell, public Qtype
    {
    public:
        // Q definition shared pointer
        typedef shared_ptr<Qbool> Sp;

        // Qbool default superposition value,
        // though any value except 0 and 1 will be considered superposition
        static const Qvalue cTrue	= 'T'; // value 84
        static const Qvalue cFalse	= 'F'; // value 71

        // default constants
        static const Qbool _T;
        static const Qbool _F;

        // Default constructor initializes Q bit with an empty id and supperposition
        // value
        Qbool() : Qcell(""), Qtype(), mValue(cSuperposition) {};

        // Initialize Q bool with a given id and supperposition value
        Qbool(const string& id) : Qtype(), Qcell(id), mValue(cSuperposition) {};

        // Initialize Q bool with a given id and value. If value is different from 'T' or 'F',
        // set Qbool into superposition state, i.e. value 'S'
        Qbool(const string& id, Qvalue value) : Qtype(), Qcell(id), mValue(value)
        {
            if(value == 1) mValue = cTrue;
            else if (value == 0) mValue = cFalse;
            else if (value != cTrue && value != cFalse) mValue = cSuperposition;
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

        // Override to set solution values from the evaluation set for this deffinition
        virtual void add(const Qevaluations& samples);

        // Returns a string representation of a solution value for a 'atEvltn'
        virtual string solution(size_t atEvltn) const;

        /*** Assignment ***/
        // assignment of an Q expression creates a Q bool assignment where this
        // Q bool is an assignee of equal expression between this to right Q bool
        Qassign<Qbool> operator=(const Qbool& right) const;

        // assignment of an Q expression creates a Q bool assignment where this
        // Q boolean is an assignee
        Qassign<Qbool> operator=(const Qexpr<Qbool>& right) const;

        /*** Compound Assignments ***/
        // 'and assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] & [right]
        Qassign<Qbool> operator&=(const Qbool& right) const;

        // 'and assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] & [right]
        Qassign<Qbool> operator&=(const Qexpr<Qbool>& right) const;

        // 'or assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] | [right]
        Qassign<Qbool> operator|=(const Qbool& right) const;

        // 'or assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] | [right]
        Qassign<Qbool> operator|=(const Qexpr<Qbool>& right) const;

        // 'xor assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] ^ [right]
        Qassign<Qbool> operator^=(const Qbool& right) const;

        // 'xor assignment' of an Q expression creates a following Q bool assignment
        // [this] = [this] ^ [right]
        Qassign<Qbool> operator^=(const Qexpr<Qbool>& right) const;

        /*** Logical ***/
        // instantiate Q expression with inversion logic, e.g. for Qbool with id 'x'
        // the expression is '!x' != 'x'
        Qexpr<Qbool> operator !() const;

        // instantiate Q expression with and logic, e.g. for Qbool ids 'x' and 'y'
        // the expression is 'x' & 'y'
        Qexpr<Qbool> operator &(const Qbool& right) const;
        Qexpr<Qbool> operator and(const Qbool& right) const { return (*this) & right; };

        // instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right]
        // object the expression is 'x' & [right]
        Qexpr<Qbool> operator &(const Qexpr<Qbool>& right) const;
        Qexpr<Qbool> operator and(const Qexpr<Qbool>& right) const { return (*this) & right; };

        // instantiate Q expression with or logic, e.g. for Qbool ids 'x' and 'y'
        // the expression is 'x' | 'y'
        Qexpr<Qbool> operator |(const Qbool& right) const;
        Qexpr<Qbool> operator or(const Qbool& right) const { return (*this) | right; };

        // instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right]
        // object the expression is 'x' | [right]
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
        Qexpr<Qbool> operator !=(const Qexpr<Qbool>& right) const;

        // same value Q expression with nXor logic, e.g. for Qbool ids 'x' and 'y'
        // the expression is 'x' || 'y'
        Qexpr<Qbool> alike(const Qbool& right) const;
        Qexpr<Qbool> operator*=(const Qbool& right) const { return alike(right); };
        Qexpr<Qbool> nxor(const Qbool& right) const { return alike(right); };

        // same value Q expression with nXor logic, e.g. for arguments
        // 'x' and [right] the expression is 'x' || [right root]
        Qexpr<Qbool> alike(const Qexpr<Qbool>& right) const;
        Qexpr<Qbool> operator*=(const Qexpr<Qbool>& right) const { return alike(right); };
        Qexpr<Qbool> nxor(const Qexpr<Qbool>& right) const { return alike(right); };

        // unlike Q expression with xor logic, e.g. for Qbool ids 'x' and 'y'
        // the expression is 'x' ^ 'y'
        Qexpr<Qbool> unlike(const Qbool& right) const;
        Qexpr<Qbool> operator^(const Qbool& right) const { return unlike(right); };

        // unlike Q expression with xor logic, e.g. for arguments
        // 'x' and [right] the expression is 'x' != [right root]
        Qexpr<Qbool> unlike(const Qexpr<Qbool>& right) const;
        Qexpr<Qbool> operator^(const Qexpr<Qbool>& right) const { return unlike(right); };

    protected:
    private:
        Qvalue mValue;
    };
};
