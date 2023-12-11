#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <Qop.h>
#include <QcellOps.h>
#include <QnaryOp.h>
#include <Qadd.h>
#include <Qmultiply.h>
#include <QderivedOps.h>
#include <Factory.h>
#include <Qtype.h>

using namespace std;

namespace dann5 {
    // A Quantum expression is a Q statement
    class Qexpression : public Qstatement
    {
    public:
        // Q expression shared pointer
        typedef shared_ptr<Qexpression> Sp;

        // default constructor creates an empty Q expression
        Qexpression() : mpRoot(nullptr) {};

        // Construct Q expression with its root operation definition
        Qexpression(const Qop::Sp& pRoot) : mpRoot(pRoot) {};

        // copy constructor
        Qexpression(const Qexpression& right)
            : Qstatement(right), mpRoot(right.mpRoot) {};

        // destructor
        ~Qexpression() {};

        // Override to return a shared_pointer to a cloned instance of this
        // Q expression
        virtual Qexpression::Sp clone(bool) const = 0;

        // Returns a Q statement shared_pointer to a cloned instance of
        // this Q expression
        virtual Qstatement::Sp clone() const
        {
            return static_pointer_cast<Qstatement>(clone(true));
        };

        // Returns a reference to the Q expression root Q operaton
        Qop& root() { return *mpRoot; };

        // Returns a root shared-pointer
        Qop::Sp root() const { return mpRoot; };

        // Sets a pointer to a new root Q operation of this expression
        void root(const Qop::Sp& pRoot) { mpRoot = pRoot; };

        // Returns a Q definition reference of the Q expression root
        // operation
        Qdef& rootDef() { return *dynamic_pointer_cast<Qdef>(mpRoot); };

        // Return a Q definition shared-pointera of the Q expression root
        // operation
        Qdef::Sp rootDef() const
                { return dynamic_pointer_cast<Qdef>(mpRoot); };

        // Returns the number of Q bits of this Q expression
        virtual size_t noqbs() const noexcept
                        { return mpRoot->noqbs(); };

        // Compiles this Q expression to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const;

        // Returns a string representation of this Q expression,
        // if not decomposed, returns a statement as initially specified
        // if decomposed, returns bit-level logic of the assignment
        // when applicable, if forBit is specified (differs from cAllBits),
        // returns decomposed logic only for the specified bit level
        string toString(bool decomposed = false,
                            size_t forBit = cAllBits) const;

        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of this Q expression
        virtual void add(const Qevaluations& samples);

        // For added sample set(s), returns a string representation of all
        // solutions of operands of this Q expression
        virtual string solutions() const;

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of this Q expression
        virtual string solution(size_t atEvltn) const
                    { return root()->solution(atEvltn);  };

        // Return computed evaluation set with all solutions of the expression
        virtual Qevaluations compute();

        // Reset the quantum expression into its initial state without
        // solutions by reseting its operations and operands
        virtual void reset();

        /*** Assignments ***/
        // Update Q assignment with a new Q expression
        Qexpression& operator=(const Qexpression& right) {
            mpRoot = right.mpRoot;
            mSolutions.clear();
            return(*this);
        };
        
        // Send a string representation of a Q expression into an output
        // stream
        friend std::ostream& operator << (std::ostream&,
                                            const Qexpression&);

    protected:
    private:
        Qop::Sp			mpRoot;		// A pointer to the root operation
        Qevaluations	mSolutions;	// A evaluation set
    };

    // Send a string representation of a Q expression into an output
    // stream
    std::ostream& operator << (std::ostream&,
        const Qexpression&);

    // A templated Q expression for Q types
    template <typename Q_Type = Qtype>
    class Qexpr : public Qexpression
    {
    public:
        // Q definition shared pointer
        typedef shared_ptr< Qexpr<Q_Type> > Sp;

        // default constructor creates an empty Q expression
        Qexpr() : Qexpression() {};

        // Initilized Q expression with its root definition
        Qexpr(const Qop::Sp& pRoot) : Qexpression(pRoot) {};

        // copy constructor
        Qexpr(const Qexpr<Q_Type>& right) : Qexpression(right) {};

        // destructor
        ~Qexpr() {};

        // Returns a shared_pointer to a cloned instance of this
        // Q expression
        virtual Qexpression::Sp clone(bool) const
        {
            return Qexpression::Sp(new Qexpr<Q_Type>(*this));
        };

        /*** Logical Operator ***/
        // instantiate Q expression with inversion/not logic, e.g. for
        // Qbool with id 'x' the logic is '!x' != 'x'
        Qexpr<Q_Type> operator!() const
        {
            Qdef::Sp pOut = as_const(*this).root()->output();
            Qdef::Sp pInverted = pOut->clone();
            pInverted->id("!" + pOut->id());
            Qdef::Sp pRoot = as_const(*this).rootDef()->clone();
            QcellOp::Sp pOp =
                Factory<string, QcellOp>::Instance().create(Qneq::cMark);
            pOp->operands(pInverted->clone(), {pRoot});
            Qexpr<Q_Type> expr(pOp);
            return(expr);
        }

        /*** Bitwise Operator ***/
        // instantiate Q expression with inversion/not logic, e.g. for
        // Qbit with id 'x' the logic is '~x' != 'x'
        Qexpr<Q_Type> operator~() const
        {
            Qdef::Sp pOut = as_const(*this).root()->output();
            Qdef::Sp pInverted = pOut->clone();
            pInverted->id("~" + pOut->id());
            Qdef::Sp pRoot = as_const(*this).rootDef()->clone();
            Qop::Sp pOp = createOpFor(pOut, Qinvert::cMark);
            pOp->operands(pInverted->clone(), {pRoot});
            Qexpr<Q_Type> expr(pOp);
            return(expr);
        }

        /*** Bitwise + Logical  Operator ***/

        /*** Bitwise + Logical  Operations ***/
        // Update Q expression with and logic, e.g. for an argument with id
        // 'x' the expression root points to QandOp with [previous root] & 'x'
        Qexpr<Q_Type> operator&(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qand::cMark);
        };

        // Update Q expression with and logic, e.g. for an argument [right]
        // the expression root points to QandOp with
        // [previous root] & [right root]
        Qexpr<Q_Type> operator&(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qand::cMark);
        };

        // Update Q expression with and logic, e.g. for an argument with id
        // 'x' the expression root points to QandOp with
        // !([previous root] & 'x')
        Qexpr<Q_Type> nand(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qnand::cMark);
        };

        // Update Q expression with and logic, e.g. for an argument [right]
        // the expression root points to QandOp with
        // !([previous root] & [right root])
        Qexpr<Q_Type> nand(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qnand::cMark);
        };

        // Update Q expression with or logic, e.g. for an argument with id 'x'
        // the expression root points to QorOp with [previous root] | 'x'
        Qexpr<Q_Type> operator|(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qor::cMark);
        };

        // Update Q expression with or logic, e.g. for an argument [right] the
        // expression root points to QorOp with [previous root] | [right root]
        Qexpr<Q_Type> operator|(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qor::cMark);
        };

        // Update Q expression with or logic, e.g. for an argument with id 'x'
        // the expression root points to QnorOp with !([previous root] | 'x')
        Qexpr<Q_Type> nor(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qnor::cMark);
        };

        // Update Q expression with or logic, e.g. for an argument [right] the
        // expression root points to QnorOp with
        // !([previous root] | [right root])
        Qexpr<Q_Type> nor(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qnor::cMark);
        };

        // Update Q expression with xor logic, e.g. for an argument with id 'x'
        // the expression root points to QxorOp with [previous root] ^ 'x'
        Qexpr<Q_Type> unlike(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qxor::cMark);
        };
        Qexpr<Q_Type> operator^(const Q_Type& right) const
                                                    { return unlike(right); };

        // Update Q expression with xor logic, e.g. for an argument [right] the
        // expression root points to QxorOp with [previous root] ^ [right root]
        Qexpr<Q_Type> unlike(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qxor::cMark);
        };
        Qexpr<Q_Type> operator^(const Qexpr<Q_Type>& right) const
                                                    { return unlike(right); };

        // Update Q expression with nxor logic, e.g. for an argument with id 'x'
        // the expression root points to QxorOp with [previous root] *= 'x'
        Qexpr<Q_Type> alike(const Q_Type& right) const {
            return bitwiselogicOp(right.clone(), Qnxor::cMark);
        };
        Qexpr<Q_Type> operator*=(const Q_Type& right) const
                                                    { return alike(right); };
        Qexpr<Q_Type> nxor(const Q_Type& right) const { return alike(right); };

        // Update Q expression with nxor logic, e.g. for an argument with id 'x'
        // the expression root points to QxorOp with [previous root] *= 'x'
        Qexpr<Q_Type> alike(const Qexpr<Q_Type>& right) const {
            return bitwiselogicOp(right.rootDef()->clone(), Qnxor::cMark);
        };
        Qexpr<Q_Type> operator*=(const Qexpr<Q_Type>& right) const { return alike(right); };
        Qexpr<Q_Type> nxor(const Qexpr<Q_Type>& right) const { return alike(right); };

        /*** Comparison ***/
        // Update Q expression with comparison, e.g. for an argument with id 'x' the
        // expression root points to QeqOp with [previous root] == 'x'
        Qexpr<Q_Type> operator==(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qeq::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QeqOp with [previous root] == [right root]
        Qexpr<Q_Type> operator==(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qeq::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument with id 'x'
        // the expression root points to QneqOp with [previous root] != 'x'
        Qexpr<Q_Type> operator!=(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qneq::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QneqOp with [previous root] != [right root]
        Qexpr<Q_Type> operator!=(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qneq::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument with id 'x'
        // the expression root points to QltOp with [previous root] < 'x'
        Qexpr<Q_Type> operator<(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qlt::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QltOp with [previous root] < [right root]
        Qexpr<Q_Type> operator<(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qlt::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument with id 'x'
        // the expression root points to QleOp with [previous root] <= 'x'
        Qexpr<Q_Type> operator<=(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qle::cMark);
        };
       
        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QleOp with [previous root] <= [right root]
        Qexpr<Q_Type> operator<=(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qle::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument with id 'x'
        // the expression root points to QgtOp with [previous root] > 'x'
        Qexpr<Q_Type> operator>(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qgt::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QgtOp with [previous root] > [right root]
        Qexpr<Q_Type> operator>(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qgt::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument with id 'x'
        // the expression root points to QgeOp with [previous root] >= 'x'
        Qexpr<Q_Type> operator>=(const Q_Type& right) const {
            return comparisonOp(right.clone(), Qge::cMark);
        };

        // Update Q expression with comparison, e.g. for an argument [right] the
        // expression root points to QgeOp with [previous root] >= [right root]
        Qexpr<Q_Type> operator>=(const Qexpr<Q_Type>& right) const {
            return comparisonOp(right.rootDef()->clone(), Qge::cMark);
        };

        /*** Arithmetic ***/
        // Update Q expression with addition, e.g. for an argument with id 'x'
        // the expression root points to Qadd with [previous root] + 'x'
        Qexpr<Q_Type> operator+(const Q_Type& right) const {
            return arithmeticOp(right.clone(), Qadd::cMark);
        };

        // Update Q expression with addition, e.g. for an argument [right] the
        // expression root points to Qadd with [previous root] ^ [right root]
        Qexpr<Q_Type> operator+(const Qexpr<Q_Type>& right) const {
            return arithmeticOp(right.rootDef()->clone(), Qadd::cMark);
        };

        // Update Q expression with subtration, e.g. for an argument with id 'x'
        // the expression root points to to Qsubtract with [previous root] - 'x'
        Qexpr<Q_Type> operator-(const Q_Type& right) const {
            return arithmeticOp(right.clone(), Qsubtract::cMark);
        };

        // Update Q expression with subtration, e.g. for an argument [right] the
        // expression root points to to Qsubtract with
        // [previous root] - [right root]
        Qexpr<Q_Type> operator-(const Qexpr<Q_Type>& right) const {
            return arithmeticOp(right.rootDef()->clone(), Qsubtract::cMark);
        };

        // Update Q expression with multiplication, e.g. for an argument with id
        // 'x' the expression root points to Qmultiply with [previous root] * 'x'
        Qexpr<Q_Type> operator*(const Q_Type& right) const {
            return arithmeticOp(right.clone(), Qmultiply::cMark);
        };

        // Update Q expression with multiplication, e.g. for an argument [right]
        // the expression root points to Qmultiply with
        // [previous root] * [right root]
        Qexpr<Q_Type> operator*(const Qexpr<Q_Type>& right) const {
            return arithmeticOp(right.rootDef()->clone(), Qmultiply::cMark);
        };

        // Update Q expression with subtration, e.g. for an argument with id 'x'
        // the expression root points to to Qsubtract with [previous root] / 'x'
        Qexpr<Q_Type> operator/(const Q_Type& right) const {
            return arithmeticOp(right.clone(), Qdivide::cMark);
        };

        // Update Q expression with subtration, e.g. for an argument [right] the
        // expression root points to to Qsubtract with
        // [previous root] / [right root]
        Qexpr<Q_Type> operator/(const Qexpr<Q_Type>& right) const {
            return arithmeticOp(right.rootDef()->clone(), Qdivide::cMark);
        };
        
    protected:
    private:
        inline Qexpr<Q_Type> bitwiselogicOp(const Qdef::Sp& pRdef,
                                                    const string& mark) const {
            Qop::Sp pOp = createOpFor(pRdef, mark);
            Q_Type out(pOp->createOutId());
            pOp->operands(out.clone(),
                          {as_const(*this).rootDef()->clone(), pRdef});
            Qexpr<Q_Type> expr(pOp);
            return(expr);
        };

        inline Qexpr<Q_Type> comparisonOp(const Qdef::Sp& pRdef,
                                                    const string& mark) const {
            Qop::Sp pOp = createOpFor(pRdef, mark);
            pOp->operands(as_const(*this).rootDef()->clone(), {pRdef});
            Qexpr<Q_Type> expr(pOp);
            return(expr);
        };

        inline Qop::Sp createOpFor(const Qdef::Sp& pRdef,
                                                    const string& mark) const {
            Qop::Sp pOp = nullptr;
            Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(pRdef);
            if(pNary == nullptr)
                pOp = Factory<string, QcellOp>::Instance().create(mark);
            else
                pOp = Factory<string, QnaryOp>::Instance().create(mark);
            return pOp;
        };

        inline Qexpr<Q_Type> arithmeticOp(const Qdef::Sp& pRdef,
                                                    const string& mark) const {
            QnaryOp::Sp pOp = Factory<string, QnaryOp>::Instance().create(mark);
            Q_Type out(pOp->createOutId());
            pOp->operands(out.clone(),
                                {as_const(*this).rootDef()->clone(), pRdef});
            Qexpr<Q_Type> expr(pOp);
            return(expr);
        };
    };
};
