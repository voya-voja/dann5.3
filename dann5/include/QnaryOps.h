//
//  QnaryOps.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-16.
//

#ifndef QnaryOps_h
#define QnaryOps_h

#include <Factory.h>
#include <QnaryOp.h>
#include <QcellOps.h>
#include <Qfunc.h>

namespace dann5
{
    // A Quantum nary-operator is an abstration of an unnary operation
    class QnaryOperator : public QnaryOp
    {
    public:
        // Quantum nary-operator shared pointer
        typedef shared_ptr<QnaryOperator> Sp;

        // Instantiate an quantum nary-operator instance with its identity
        QnaryOperator(const string& id) : QnaryOp(id, 1) {};

        // Copy constructor
        QnaryOperator(const QnaryOperator& right) : QnaryOp(right) {};

        // Destruct quantum nary-operator
        ~QnaryOperator() {};

        // overrides output getter to return output operand when operation is an
        // output
    //    virtual Qdef::Sp output(size_t forBit = cAllBits) const;

        // Convert Quantum nary-operator expression into a string
        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const;
    protected:
        // Refreshes the Q-nary operator cells logic when inputs are added
        virtual void refreshOnInputs();
    private:
    };

    // A Quantum equal is an operator
    class QnaryEq : public QnaryOperator
    {
    public:
        // QnaryEq's shared pointer
        typedef shared_ptr<QnaryEq> Sp;
        
        // Quantum nary-equal operator identifiers
        static const string cMark;    // "=="
        static const string cName;    // "equal"

        // Instantiate quantum nary-equal instance with '==' identity
        QnaryEq() : QnaryOperator(cMark) {};

        // Copy constructor
        QnaryEq(const QnaryEq& right) : QnaryOperator(right){};

        // Destruct Qeq instance
        ~QnaryEq() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryEq(*this)); };

    protected:
    private:
    };

    // A Quantum nary-not-equal is an unary operator implemented as a quantum
    // function
    class QnaryNeq : public Qfunction
    {
    public:
        // QnaryNeq's shared pointer
        typedef shared_ptr<QnaryNeq> Sp;

        // Quantum nary-not-equal operator identifiers
        static const string cMark;    // "!="
        static const string cName;    // "notequal"

        // Instantiate quantum nary-not-equal operator with '!=' identity
        QnaryNeq() : Qfunction(cMark, 1) {};

        // Copy constructor
        QnaryNeq(const QnaryNeq& right) : Qfunction(right) {};

        // Destruct quantum nary-not-equal
        ~QnaryNeq() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryNeq(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to not-equal logic
        void refresh();
    private:
    };

    // A Quantum nary-invert is an unary operator
    class QnaryInvert : public QnaryOperator
    {
    public:
        // QnaryInvert's shared pointer
        typedef shared_ptr<QnaryInvert> Sp;

        // Quantum nary-invert operator identifiers
        static const string cMark;    // "~"
        static const string cName;    // "invert"

        // Instantiate quantum nary-invert operator with '~' identity
        QnaryInvert() : QnaryOperator(cMark) {};

        // Copy constructor
        QnaryInvert(const QnaryInvert& right) : QnaryOperator(right) {};

        // Destruct quantum nary-invert
        ~QnaryInvert() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryInvert(*this)); };
        
        // Returns not-equal mark as this operation identifier
        virtual string identifier() const noexcept {
            return Qneq::cMark;
        };

    protected:
    private:
    };

    // A Qunatum nary-less-than is an unary operator implemented as a quantum
    // function
    class QnaryLt : public Qfunction
    {
    public:
        // QnaryLt's shared pointer
        typedef shared_ptr<QnaryLt> Sp;

        // Quantum nary-not-equal operator identifiers
        static const string cMark;    // "<"
        static const string cName;    // "lessthan"

        // Instantiate quantum nary-not-equal operator with '<' identity
        QnaryLt() : Qfunction(cMark, 1) {};

        // Copy constructor
        QnaryLt(const QnaryLt& right) : Qfunction(right) {};

        // Destruct quantum nary-not-equal
        ~QnaryLt() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryLt(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to less-than logic
        void refresh();
    private:
    };

    // A Qunatum nary-less-than-or-equal is an unary operator implemented as
    // a quantum function
    class QnaryLe : public Qfunction
    {
    public:
        // QnaryLt's shared pointer
        typedef shared_ptr<QnaryLe> Sp;

        // Quantum nary-not-equal operator identifiers
        static const string cMark;    // "<="
        static const string cName;    // "lessequal"

        // Instantiate quantum nary-not-equal operator with '<=' identity
        QnaryLe() : Qfunction(cMark, 1) {};

        // Copy constructor
        QnaryLe(const QnaryLe& right) : Qfunction(right) {};

        // Destruct quantum nary-not-equal
        ~QnaryLe() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryLe(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to less than or equal
        // logic
        void refresh();
    private:
    };

    // A quantum nary-greater-than is an unary operator implemented as a quantum
    // function
    class QnaryGt : public Qfunction
    {
    public:
        // QnaryGt's shared pointer
        typedef shared_ptr<QnaryGt> Sp;

        // Quantum nary-greater-than operator identifiers
        static const string cMark;    // ">"
        static const string cName;    // "greaterthan"
        
        // Instantiate QnaryGt instance with '>' identity
        QnaryGt() : Qfunction(cMark, 1) {};

        // Copy constructor
        QnaryGt(const QnaryGt& right) : Qfunction(right) {};

        // Destruct QnaryGt with shared pointers to its Qdef
        ~QnaryGt() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryGt(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to greater-than logic
        void refresh();
    private:
    };

    // A Quantum nary-greater-than-or-equal  is an unary operator implemented as
    //  a quantum function
    class QnaryGe : public Qfunction
    {
    public:
        // QnaryGe's shared pointer
        typedef shared_ptr<QnaryGe> Sp;

        // Quantum nary-greater than or equal operator identifiers
        static const string cMark;    // ">="
        static const string cName;    // "greaterequal"
        
        // Instantiate QnaryGe instance with '>=' identity
        QnaryGe() : Qfunction(cMark, 1) {};

        // Copy constructor
        QnaryGe(const QnaryGe& right) : Qfunction(right) {};

        // Destruct QnaryGe with shared pointers to its Qdef
        ~QnaryGe() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryGe(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to greater-than or e
        // qual logic
        void refresh();
    private:
    };

    // A Quantum nary-binary-operation template implementation using quantum cell
    // operations
    template <typename Q_OP = QcellOp>
    class QnaryBiOp : public QnaryOp
    {
    public:
        // Q-nary operation's shared pointer
        typedef shared_ptr<QnaryBiOp<Q_OP> > Sp;
        
        // Quantum equal operator identifiers
        static const string cMark;    // Q_OP::cMark
        static const string cName;    // Q_OP::cName

        // Construct Q-nary operation instance as instantionation of Q_OP
        // identity
        QnaryBiOp() : QnaryOp(cMark, Q_OP().noInputs()) {};

        // Copy constructor
        QnaryBiOp(const QnaryBiOp<Q_OP>& right) : QnaryOp(right){};

        // Destruct Q-nary operation instance
        ~QnaryBiOp() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const {
            return Qdef::Sp(new QnaryBiOp<Q_OP>(*this));
        };
        
    protected:
        // Override to refresh the Q-nary operation cells using quantum cell
        // operations when inputs are added
        virtual void refreshOnInputs()
        {
            const Qdefs& args = Qop::inputs();
            if(args.size() != 2)
                throw logic_error("ERROR @QnaryBiOp: number of arguments is not 2!");
            Qnary::Sp pLeft = dynamic_pointer_cast<Qnary>(args[0]), pRight = nullptr;
            pRight = dynamic_pointer_cast<Qnary>(args[1]);
            size_t size = Qop::noqbs();
            Qcells& opCells = cells();
            opCells.resize(size);
            for (size_t atBit = 0; atBit < size; atBit++)
            {
                QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Q_OP::cMark);
                Qcell::Sp pLcell(as_const(*pLeft)[atBit]);
                Qcell::Sp pRcell(as_const(*pRight)[atBit]);
                pOp->inputs({pLcell, pRcell});
                opCells[atBit] = pOp;
            }
        };
    private:
    };

    // A Quantum subtract is a binary operation implemented as
    // a quantum function
    class Qminus : public Qfunction
    {
    public:
        // QnaryGe's shared pointer
        typedef shared_ptr<Qminus> Sp;

        // Quantum nary-greater than or equal operator identifiers
//        static const string cMark;    // "-"
//        static const string cName;    // "minus"
        
        // Instantiate Qminus instance with '-' identity
        Qminus() : Qfunction("-", 2) {};

        // Copy constructor
        Qminus(const Qminus& right) : Qfunction(right) {};

        // Destruct Qminus with shared pointers to its Qdef
        ~Qminus() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qminus(*this)); };

    protected:
        // Refreshes the Q-nary operator cells according to greater-than or e
        // qual logic
        void refresh();
    private:
    };


};

#endif /* QnaryOps_h */
