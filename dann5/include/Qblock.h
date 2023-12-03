#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qstatement.h>
#include <Qevaluation.h>
#include <QcellOps.h>

using namespace std;

namespace dann5 {
    // A Q block is a lexical structure of quantum code, which is grouped
    // together. Qblock consist of one or more quantum statements. Qblock
    // is a quantum statement, too, permitting block-structured quantum
    // programming through the creation of blocks nested within other blocks
    class Qblock : public Qstatement
    {
    public:
        // Qblock's shared pointer
        typedef shared_ptr<Qblock> Sp;

        // default constructor of an instance without Q statements
        Qblock() {};

        // creates a named Q block with a given list of Q statements
        Qblock(const Qstatements& statements)
            :Qstatement(), mStatements(statements) {};

        // Copy constructor
        Qblock(const Qblock& right)
            :Qstatement(right), mStatements(right.mStatements) {};

        // destructor
        ~Qblock() {};

        // Return a Qstatement's shared pointer pointing to a copy of this
        // object
        virtual Qstatement::Sp clone() const
            { return Qblock::Sp(new Qblock(*this)); };

        // Returns the maximal number of qbit levels amongst containg
        // statements
        virtual size_t noqbs() const noexcept;

        // Compiles this Q block to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const;

        // Returns a string representation of Q block statements
        // if not decomposed, returns statements as initially specified
        // if decomposed, returns bit-level logic of the statements
        // when applicable, if forBit is specified (differs from cAllBits),
        // returns decomposed logic only for the specified bit level
        virtual string toString(bool decomposed = false,
            size_t forBit = cAllBits) const;

        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of statements within this
        // Q block
        virtual void add(const Qevaluations& evaluations);

        // For added sample set(s), returns a string representation of all
        // solutions of operands of statements within this Q block
        virtual string solutions() const;

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of statements within this Q block
        virtual string solution(size_t atEvltn) const;

        // Returns computed evaluation set with all solutions for the Q block
        // logic
        virtual Qevaluations compute();

        // Resets Q block statements into their initial state without
        // added solutions
        virtual void reset();

        // An insertion operator (<<) to add a new statement into this
        // Q block and returns the reference to 'this' object
        Qblock& operator<<(const Qstatement& right);

        // An insertion operator (<<) to add a new statement into a copy of this
        // Q block and returns the reference to 'this' object
        Qblock operator<<(const Qstatement& right) const;

        // Comma Operator class allows use of comma operator to insert
        // operands into specified Q block
        class CommaOp
        {
        public:
            // construct object with pointer to a Q block
            CommaOp(Qblock* pBlock) : mpBlock(pBlock) {};

            // copy constructor
            CommaOp(const CommaOp& right) : mpBlock(right.mpBlock) {};

            // An insertion operator ',' to add a new statement into
            // specified Q block
            CommaOp operator,(const Qstatement& right)
            {
                *mpBlock << right;
                return *this;
            };

        protected:
        private:
            Qblock* mpBlock;	// pointer to Q block object
        };

        // An assignment of a Q statement to a Q block, which can
        // be followed by list of other Q statements separated by
        // comma operator
        CommaOp operator=(const Qstatement& right);
        
        // Quantum block cell operation contains a list of quantum cell
        // operations
        class Op : public QcellOp
        {
        public:
            // Quantum block operation shared pointer
            typedef shared_ptr<Op> Sp;

            // Instantiate an quantum block operation instance with its identity
            Op() : QcellOp("{}", 0), mOps() {};

            // Copy constructor
            Op(const Op& right) : QcellOp(right), mOps(right.mOps) {};

            // Destruct quantum block operation
            ~Op() {};

            // Return a Qdef's shared pointer pointing to a copy of this object
            Qdef::Sp clone() const { return Qdef::Sp(new Op(*this)); };

            // Convert quantum block operation into a string
            string toString(bool decomposed = false,
                                    size_t forBit = cAllBits) const;

            // Compiles quantum block operation to generate quantum solver code
            void compile(Qcompiler& compiler) const {
                for(auto pOp : mOps)
                    pOp->compile(compiler);
            };

            // Sets solution values from the evaluation set for this block
            // operation
            void add(const Qevaluations& evaluations) {
                for(auto pOp : mOps)
                    pOp->add(evaluations);
            };

            // Returns a string representation of a solution value of evaluation
            // at 'atEvltn' for the cell operation
            string solution(size_t atEvltn) const;

            // Reset the quantum operation into its initial state without solutions
            // by clearing all evaluation samples
            void reset() {
                for(auto pOp : mOps)
                    pOp->reset();
            };

        protected:
            virtual Qvalue calculate(const Qvalues& values) const { return 0; };

        private:
            QcellOps    mOps;   // containing cell operations
        };
        
        Op at(size_t forBit) const;
        Op at(size_t forBit);
        Op operator[](size_t forBit) const {
            return as_const(*this).at(forBit);
        };
        Op operator[](size_t forBit){ return at(forBit); };
        
        // Insert string representation of a Q block into an output stream
        friend std::ostream& operator << (std::ostream&, const Qblock&);

    protected:
    private:
        Qstatements mStatements;	// a list of Q block statements
    };

    // Insert string representation of a Q block into an output stream
    std::ostream& operator << (std::ostream&, const Qblock&);

    // An std::vector containing a list of Q blocks
    typedef vector<Qblock::Sp> Qblocks;
};
