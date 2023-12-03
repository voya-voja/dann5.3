#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>
#include <Qblock.h>
#include <Qbinder.h>

using namespace std;


namespace dann5 {
    // Quantum routine is a Q definition with a sequence of logical
    // quantum statements organized as Q block
    class Qroutine : public QnaryOp
    {
    public:
        // Qroutine's shared pointer
        typedef shared_ptr<Qroutine> Sp;

        // default constructor
        Qroutine(size_t noArguments = 0) :QnaryOp("", noArguments){};

        // creates Q routine with a name
        Qroutine(const string& name, size_t noArguments = 0) :QnaryOp(name, noArguments) {};

        // creates a named Q routine with a given Q block
        Qroutine(const string& name, const Qblock& block, size_t noArguments = 0)
            :QnaryOp(name, noArguments), mBlock(block) {};

        // creates a named Q routine with a given list of Q statements
        Qroutine(const string& name, const Qstatements& statements, size_t noArguments = 0)
            :QnaryOp(name, noArguments), mBlock(statements) {};

        //copy constructor
        Qroutine(const Qroutine& right)
            :QnaryOp(right), mBlock(right.mBlock) {};

        // desruct the Q equation with its members
        ~Qroutine() {};

        // Return a Q definition shared pointer to a copy of this Q routine
        virtual Qdef::Sp clone() const { return Sp(new Qroutine(*this)); };

        // Returns the maximal number of qbit levels amongst containg
        // statements
        virtual size_t noqbs() const noexcept { return(mBlock.noqbs()); };

        // Compiles this quantum operation to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const;

        // Returs routine declaration as a string
        string declaration() const;
        
        // Returns a string representation of Q routines statements
        // if not decomposed, returns statements as initially specified
        // if decomposed, returns bit-level logic of the statements
        // when applicable, if forBit is specified (differs from cAllBits),
        // returns decomposed logic only for the specified bit level
        virtual string toString(bool decomposed = false,
                                    size_t forBit = cAllBits) const;

        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of statements within this
        // quantum routine
        virtual void add(const Qevaluations& evaluations);

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of this quantum routine
        virtual string solution(size_t atEvltn) const;

        // Returns computed evaluation set with all solutions for the Q block
        // logic
        virtual Qevaluations compute() { return mBlock.compute(); };

        // Resets Q block statements into their initial state without
        // added solutions
        virtual void reset();

        // An insertion operator (<<) to add a new statement into this
        // Q routine and returns the reference to 'this' object
        Qroutine& operator<<(const Qstatement& right);

        // An insertion operator (<<) to add a new statement into a copy of this
        // Q routine and returns the reference to 'this' object
        Qroutine operator<<(const Qstatement& right) const;

        // Comma Operator class allows use of comma operator to insert
        // operands into specified Q routine
        class CommaOp
        {
        public:
            // construct object with pointer to a Q routine
            CommaOp(Qroutine* pRoutine) : mpRoutine(pRoutine) {};

            // copy constructor
            CommaOp(const CommaOp& right) : mpRoutine(right.mpRoutine) {};

            // An insertion operator ',' to add a new statement into
            // specified Q routine
            CommaOp operator,(const Qstatement& right)
            {
                *mpRoutine << right;
                return *this;
            };

        protected:
            
        private:
            Qroutine* mpRoutine;	// pointer to Q routine object
        };

        // An assignment of a Q statement to a Q routine, which can
        // be followed by list of other Q statements separated by
        // comma operator
        CommaOp operator=(const Qstatement& right);

        // Insert string representation of a Q routine into an output stream
        friend std::ostream& operator << (std::ostream&, const Qroutine&);

    protected:
        // Returns reference to the quantum block
        const Qblock& block() const { return mBlock; };
        Qblock& block() { return mBlock; };

        // Override to refresh the Q-nary operation cells according to the derived
        // operation logic when inputs are added
        virtual void refreshOnInputs();

        virtual void refreshOnOutput();
        
        virtual void updateLogic();

    private:
        Qblock	mBlock;
        Qbinder mBinder;
    };

    // Insert string representation of a Q routine into an output stream
    std::ostream& operator << (std::ostream&, const Qroutine&);
};
