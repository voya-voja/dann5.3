#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <Qevaluation.h>

using namespace std;

namespace dann5 {
    // A Q binder is a collection quantum operands, used to be report
    // together on thier added solutions.
    class Qbinder
    {
    public:
        // Qbinder's shared pointer
        typedef shared_ptr<Qbinder> Sp;

        // default constructor
        Qbinder() {};

        // creates a Q binder with a given list of Q evaluations
        Qbinder(const Qevaluations& solutions)
            :mSolutions(solutions) {};

        // Copy constructor
        Qbinder(const Qbinder& right)
            :mSolutions(right.mSolutions), mOperands(right.mOperands) {};

        // Destruct the Q addition instance with a shared pointer to its
        // carry operand
        ~Qbinder() {};

        // Return a Qbinder's shared pointer pointing to a copy of this
        // object
        virtual Qbinder::Sp clone() const
            { return Qbinder::Sp(new Qbinder(*this)); };

        // Returns the mximal number of bit levels amongst all operands.
        // returns 0, if there are no operands.
        virtual size_t noqbs() const noexcept;

        // Returns a string with the list of containing operands toString
        // representation.
        // Returns an empty string, if there are no operands
        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const;

        // Adds a evaluation set of solutions to existing evaluation set and to
        // added operands
        virtual void add(const Qevaluations& samples);

        // For added sample set(s), returns a string represnting all
        // solutions of operands contained in the Q binder
        virtual string solutions() const;

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands contained in the Q binder
        virtual string solution(size_t atEvltn) const;

        // Resets the Q bundle by removing all added sample sets and
        // reseting all the operands
        virtual void reset();

        // An insertion operator (<<) to add a new operand into this
        // Q binder and returns teh reference to 'this' object
        Qbinder& operator<<(const Qdef& right);

        // An insertion operator (<<) to add a new operand into a copy of this
        // Q binder and returns teh reference to 'this' object
        Qbinder operator<<(const Qdef& right) const;

        // Comma Operator class allows use of comma operator to insert
        // operands into specified Q binder
        class CommaOp
        {
        public:
            // construct object with pointer to a Q buinder
            CommaOp(Qbinder* pBinder) : mpBinder(pBinder) {};

            // copy constructor
            CommaOp(const CommaOp& right) : mpBinder(right.mpBinder) {};

            // An insertion operator ',' to add a new operand definition
            // into specified Q binder
            CommaOp operator,(const Qdef& right)
            {
                *mpBinder << right;
                return *this;
            };

        protected:
        private:
            Qbinder* mpBinder;	// pointer to Q binder object
        };

        // An assignment of a operand Q definition to a Q binder, which can
        // be followed by list of other operand definitions separated by
        // comma operator
        CommaOp operator=(const Qdef& right);

        // returns a pointer to operand's definition 'at' specified index
        Qdef::Sp operator[](size_t at) { return mOperands[at]; };

        // Returns a pointer to operand's definition with specified 'id'
        // Returns nullptr, if there is no operand with the 'id'
        Qdef::Sp operator[](string id);

        // Inserts a string representation of a Q binder into an output
        // stream
        friend std::ostream& operator << (std::ostream&, const Qbinder&);

    protected:
    private:
        // Adds operand to the mOperands list and adds existing solutions
        // to the added operand
        void addOperand(const Qdef& right);

        Qevaluations	mSolutions;	// A sample set of solutions
        Qdefs			mOperands;	// A list of operands
    };

    // Inserts a string representation of a Q binder into an output
    // stream
    std::ostream& operator << (std::ostream&, const Qbinder&);

    // An std::vector containing a list of Q binders
    typedef vector<Qbinder::Sp> Qbinders;
};
