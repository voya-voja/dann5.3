//
//  QderivedOps.h
//  d5
//
//  Created by Nebojsa Vojinovic on 2022-11-12.
//

#ifndef QderivedOps_h
#define QderivedOps_h
#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <QcellOps.h>
#include <QnaryOps.h>
#include <Qcompiler.h>

using namespace std;

namespace dann5 {
    // A Q derived operation is an abstract base for operations like quantum
    // subtraction or division. The QeqOp specializes a Qnary operation
    class QderivedOp : public QnaryOp
    {
    public:
        // QderivedOp's shared pointer
        typedef shared_ptr<QderivedOp> Sp;

        // An Q derived operation has identity and substitution operation
        //  to be used to perform quantum calculations
        QderivedOp(const string& id, const QnaryOp& substitueOp);

        // Copy constructor
        QderivedOp(const QderivedOp& right);

        // Destruct the Q derived operation instance
        ~QderivedOp() {};

        // Returns the number of Q bits of the that the operation's right 
        // argument, e.g. for x - y, returns y.noqbs()
        virtual size_t noqbs() const noexcept;

        // Resizes the Q array of oprations to contain a specified size of elements. If the size is
        // smaller than the current Q array size, the content is reduced to its
        // first size elements, removing those beyond (and destroying them). If the size
        // is greater than the current container size, the content is expanded by
        // inserting at the end as many Q null operations as needed.
        virtual void resize(size_t size, Qvalue value = 0);

        // Return a quantum equal operand representation with substitute operation 
        // as one of the operands when decomposed.
        // Otherwise, return a string representation of a Q derived operation
        virtual string toString(bool decomposed = false,
            size_t forBit = cAllBits) const
        {
            if (decomposed) return mEq.toString(decomposed, forBit);
            return QnaryOp::toString(decomposed, forBit);
        };

        // Compiles this quantum derived operation to generate quantum solver
        // code by compiling quantum equation with substitute operation as an
        // argument
        virtual void compile(Qcompiler& compiler) const
        {
            return mEq.compile(compiler);
        };

        // returns a reference to quantum equal operator
        const QnaryEq& equalOp() const { return mEq; };
        
        // returns a sared pointer to substitute operation
        QnaryOp::Sp substituteOp() const {
            return mpSubstituteOp;
        };

    protected:
        // Refreshes QnaryOp cells with the substitute operation logic when
        //  inputs are added
        virtual void refreshOnInputs();
        
        // Refreshes QnaryOp cells, equal operator and substitute operation 
        // logic outputs are added
        virtual void refreshOnOutput();

    private:
        QnaryEq mEq;	// QnaryEq operator is used to form derived expression
        QnaryOp::Sp  mpSubstituteOp;// The substitute operation
    };

    // A quantum subtract is a specific implementation of a Q derived operation
    class Qsubtract : public QderivedOp
    {
    public:
        // Qsubtract's shared pointer
        typedef shared_ptr<Qsubtract> Sp;

        // Quantum subtract operation identifiers
        static const string cMark() { return "-"; };
        static const string cName() { return "sub"; };

        // An Q subtractition has identity cMark and uses Qadd as its
        // substitute operation
        Qsubtract();

        // Copy constructor
        Qsubtract(const Qsubtract& right) : QderivedOp(right) {};

        // Destruct the Q subtractition instance
        ~Qsubtract() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qsubtract(*this)); };
    protected:
    private:
    };

    // A quantum divide is a specific implementation of a Q derived operation
    class Qdivide : public QderivedOp
    {
    public:
        // Qdivide's shared pointer
        typedef shared_ptr<Qdivide> Sp;

        // Quantum divide operation identifiers
        static const string cMark() { return "/"; };
        static const string cName() { return "div"; };

        // An Q divide has identity cMark and uses Qmultiply as its
        // substitute operation
        Qdivide();

        // Copy constructor
        Qdivide(const Qdivide& right) : QderivedOp(right) {};

        // Destruct the Q divide instance
        ~Qdivide() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qdivide(*this)); };
    protected:
    private:
    };

    class QsubtractQints : public QderivedOp
    {
    public:
        // Qadd's shared pointer 
        typedef shared_ptr<QsubtractQints> Sp;

        // An Q addition has identity and should have at least two argument
        QsubtractQints();

        // Copy constructor
        QsubtractQints(const QsubtractQints& right);

        // Destruct the Q addition instance with a shared pointer to its carry
        // operand
        ~QsubtractQints();

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QsubtractQints(*this)); };

        // Adds a evaluation set containing nodes with solutions values,
        // the nodes should correspond to carryover this Q add operation
        virtual void add(const Qevaluations& samples);

        // Returns a string representation of a solution value of this
        // Qnary-operation for an evaluation at 'atEvltn'
        virtual string solution(size_t atEvltn) const;

    protected:
        // Equalizes the size of addition input arguments and calls 
        // Qadd::refreshOnInputs()
        virtual void refreshOnInputs();

        // Override to add a quantum integer carryover variable for Q-add
        // operation
        virtual void refreshOnOutput();

    private:
        Qdefs mCarryoverBits;
    };
};

#endif /* QderivedOps_h */