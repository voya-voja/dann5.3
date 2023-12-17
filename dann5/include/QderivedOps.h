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
        // QeqOp's shared pointer
        typedef shared_ptr<QderivedOp> Sp;

        // An Q derived operation has identity and substitution operation
        //  to be used to perform quantum calculations
        QderivedOp(const string& id, const QnaryOp& substitueOp);

        // Copy constructor
        QderivedOp(const QderivedOp& right);

        // Destruct the Q subtractition instance with a shared pointer to its carry operand
        ~QderivedOp() {};

        // Return a Q add representation when decomposed.
        // Otherwise, return a string representation of a Q subbtract operation
        virtual string toString(bool decomposed = false,
            size_t forBit = cAllBits) const
        {
            if (decomposed) return mEq.toString(decomposed, forBit);
            return QnaryOp::toString(decomposed, forBit);
        };

        // Compiles this Qnary operation to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const
        {
            return mEq.compile(compiler);
        };

        const QnaryEq& equalOp() const { return mEq; };
        
        QnaryOp::Sp substituteOp() const
        {
            return mpSubstituteOp;
        };

    protected:
        // Refreshes QnaryOp cells with the subtractition logic when inputs are
        // added
        virtual void refreshOnInputs();
        
        virtual void refreshOnOutput();

    private:
        QnaryEq mEq;	// QnaryEq operand is used to form derived expression
        QnaryOp::Sp  mpSubstituteOp;// The substitute operation
    };

    // A Q subtract is a specific implementation of a Q derived operation
    class Qsubtract : public QderivedOp
    {
    public:
        // Qsubtract's shared pointer
        typedef shared_ptr<Qsubtract> Sp;

        static const string cMark;
        static const string cName;

        // An Q subtractition has identity cMark and uses Qadd as its
        // substitute
        Qsubtract();

        // Copy constructor
        Qsubtract(const Qsubtract& right) : QderivedOp(right) {};

        // Destruct the Q subtractition instance with a shared pointer to its carry operand
        ~Qsubtract() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qsubtract(*this)); };
    protected:
    private:
    };


    // A Q divide is a specific implementation of a Q derived operation
    class Qdivide : public QderivedOp
    {
    public:
        // Qsubtract's shared pointer
        typedef shared_ptr<Qdivide> Sp;

        static const string cMark;
        static const string cName;

        // An Q subtractition has identity cMark and uses Qadd as its
        // substitute
        Qdivide();

        // Copy constructor
        Qdivide(const Qdivide& right) : QderivedOp(right) {};

        // Destruct the Q subtractition instance with a shared pointer to its carry operand
        ~Qdivide() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qdivide(*this)); };
    protected:
    private:
    };

    /*
    class QsubtractQints : public Qsubtract
    {
    public:
        // Qsubtract's shared pointer
        typedef shared_ptr<QsubtractQints> Sp;

        // An Q subtractition has identity and should have at least two argument
        QsubtractQints();

        // Copy constructor
        QsubtractQints(const QsubtractQints& right);

        // Destruct the Q subtractition instance with a shared pointer to its carry operand
        ~QsubtractQints();
    };
*/
};
