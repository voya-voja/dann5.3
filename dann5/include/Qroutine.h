#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <Qblock.h>
#include <Qbinder.h>

#include <string>

using namespace std;


namespace dann5 {
    // Quantum routine is a Q statement with a sequence of logical
    // quantum statements organized as Q block
    class Qroutine : public Qstatement
    {
    public:
        // Qroutine's shared pointer
        typedef shared_ptr<Qroutine> Sp;

        // default constructor
        Qroutine() :Qstatement() {};

        // creates Q routine with a name
        Qroutine(const string& name) :Qstatement(), mName(name) {};

        // creates a named Q routine with a given arguments in a quantum binder
        Qroutine(const string& name, const Qbinder& arguments)
            :Qstatement(), mName(name), mArguments(arguments) {};

        // creates a named Q routine with a given arguments in a quantum binder
        Qroutine(const string& name, const Qbinder& arguments,
                                                            const Qblock& block)
            :Qstatement(), mName(name), mArguments(arguments), mLogic(block) {};

        // creates a named Q routine with a given logic as a quantum block
        Qroutine(const string& name, const Qblock& block)
            :Qstatement(), mName(name), mLogic(block) {};

        //copy constructor
        Qroutine(const Qroutine& right)
            :Qstatement(right), mName(right.mName), mLogic(right.mLogic),
                mArguments(right.mArguments) {};

        // desruct the Q equation with its members
        ~Qroutine() {};

        // Return a Q statement shared pointer to a copy of this Q routine
        virtual Qstatement::Sp clone() const { return Sp(new Qroutine(*this)); };

        // Returns the maximal number of qbit levels amongst containg
        // statements
        virtual size_t noqbs() const noexcept { return(mLogic.noqbs()); };

        // Returs routine declaration as a string
        string declaration() const;

        // Returns a string representation of Q routine statements
        // if not decomposed, returns routine declaration followed by routine
        // statements as specified
        // if decomposed, returns bit-level logic of the statements
        // when applicable,
        // when forBit is specified (differs from cAllBits),
        // returns decomposed logic only for the specified bit level
        virtual string toString(bool decomposed = false,
                                    size_t forBit = cAllBits) const;

        // Solves this Q routine and return a quantum evaluation
        // with all solutions
        virtual Qevaluations compute() { return mLogic.compute(); };

        // Compiles this quantum routine to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const {
            mLogic.compile(compiler);
        };

        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of statements within this
        // quantum routine
        virtual void add(const Qevaluations& evaluations);

        // Override to provide a string representation of all solutions of
        // this Q statement. Return empty string, when solutions samples
        // are not assigned by add() or solve method
        virtual string solutions() const { return mArguments.solutions(); };

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of this quantum routine
        virtual string solution(size_t atEvltn) const {
            return mArguments.solution(atEvltn);
        };

        // Resets Q block statements into their initial state without
        // added solutions
        virtual void reset();
        
        // Assigns quantum routine's list of arguments
        virtual void arguments(const Qdefs& args);
        
        // Assigns quantum routine's list of arguments
        virtual void argument(const Qdef& arg) { mArguments << arg; };

        // An insertion operator (<<) to add a new argument into this
        // Q routine and returns the reference to 'this' object
        Qroutine& operator<<(const Qdef& arg) ;

        // An insertion operator (<<) to add a new argument into a copy of this
        // Q routine and returns the copy
        Qroutine operator<<(const Qdef& arg) const;
        
        // An insertion operator (<<) to add a new statement into this
        // Q routine and returns the reference to 'this' object
        Qroutine& operator<<(const Qstatement& right);

        // An insertion operator (<<) to add a new statement into a copy of this
        // Q routine and returns the copy
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
        const Qblock& block() const { return mLogic; };
        Qblock& block() { return mLogic; };

    private:
        string  mName;
        Qblock	mLogic;
        Qbinder mArguments;
    };

    // Insert string representation of a Q routine into an output stream
    std::ostream& operator << (std::ostream&, const Qroutine&);
};
