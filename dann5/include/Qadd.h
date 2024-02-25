//
//  Qadd.h
//  d5
//
//  Created by Nebojsa Vojinovic on 2022-11-12.
//

#ifndef Qadd_h
#define Qadd_h
#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <QcellOps.h>

using namespace std;

namespace dann5 {
    // A Q add is a specific implementation of a Qnary operation
    class Qadd : public QnaryOp
    {
    public:
        // Qadd's shared pointer 
        typedef shared_ptr<Qadd> Sp;

        // Quantum add operation identifiers
        static const string cMark() { return "+"; };
        static const string cName() { return "add"; };

        // An Q addition has identity and should have at least two argument
        Qadd(size_t noLastCarryBit = 1);

        // Copy constructor
        Qadd(const Qadd& right);

        // Destruct the Q addition instance with a shared pointer to its carry operand
        ~Qadd();

        // Return a Qdef's shared pointer pointing to a copy of this object 
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qadd(*this)); };


    protected:
        // An Q addition has identity and should have at least two argument
        size_t& noLastCarryBit() { return mNoLastCarryBit; }

        // Refreshes QnaryOp cells with the addition logic when inputs are added
        virtual void refreshOnInputs();

        // Assignes the inputs to this additon and returns nullptr
        // Override if assignment of operands should triger creation of different
        // Q addition object and return a shared pointer to a newlly created Q addition
        // object
        virtual Qcell::Sp opAt(const Qnary::Sp& pLeft, const Qnary::Sp& pRight,
                                size_t forBit, Qaddition::Carry::Sp& pCarry, 
                                bool assignOutput) const;

    private:
        size_t mNoLastCarryBit;	// number of dedicated last carry bit can be 1 or 0
    };

    class QaddQints : public Qadd
    {
    public:
        // Qadd's shared pointer 
        typedef shared_ptr<QaddQints> Sp;

        // An Q addition has identity and should have at least two argument
        QaddQints();

        // Copy constructor
        QaddQints(const QaddQints& right);

        // Destruct the Q addition instance with a shared pointer to its carry
        // operand
        ~QaddQints();

        // Returns a string representation of a solution value of this
        // Qnary-operation for an evaluation at 'atEvltn'
        virtual string solution(size_t atEvltn) const;

    protected:
        // Equalizes the size of addition input arguments and calls 
        // Qadd::refreshOnInputs()
        virtual void refreshOnInputs();

        // Override to refresh the Q-add quantum integers operation cells
        // according to the derived operation logic on output assignment
        /*virtual void refreshOnOutput();*/
    private:
    };
};

#endif /* Qadd_h */