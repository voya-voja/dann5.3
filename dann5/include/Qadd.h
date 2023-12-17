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

        static const string cMark;
        static const string cName;

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

        // Destruct the Q addition instance with a shared pointer to its carry operand
        ~QaddQints();
    };
};
