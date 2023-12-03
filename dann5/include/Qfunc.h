//
//  Qfunc.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-12-01.
//

#ifndef Qfunc_h
#define Qfunc_h

#include <Qop.h>
#include <QcellOps.h>
#include <QnaryOp.h>
#include <Qblock.h>
#include <Qstatement.h>
#include <Qcompiler.h>
#include <Qbinder.h>

namespace dann5
{
    // A generic quantum function definition
    class Qatomicity : public QcellOp
    {
    public:
        // Qatomicity's shared pointer
        typedef shared_ptr<Qatomicity> Sp;

        // Instantiate Qatomicity instance with name as its identity
        // and number of arguments
        Qatomicity(const string& name, size_t noArgs) : QcellOp(name, noArgs) {};

        // Copy constructor
        Qatomicity(const Qatomicity& right) :
                    QcellOp(right), mOps(right.mOps), mBinder(right.mBinder) {};

        // Destruct quantum not-equal instance
        ~Qatomicity() {};

        // Add quantum cell operation into this Qatomicity function
        void add(QcellOp::Sp pOp) { mOps.push_back(pOp); };
        
        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qatomicity(*this)); };
        
        // Returns a string representation of Q atomicity function
        // if not decomposed, returns atomicity function name
        // if decomposed, returns the logic of the atomicity function
        virtual string toString(bool decomposed = false,
                                            size_t forBit = cAllBits) const;

        // Compiles this quantum operation to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const;
        
        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of statements assigned to this
        // quantum atomicity function
        virtual void add(const Qevaluations& evaluations){
            mBinder.add(evaluations);
            QcellOp::add(evaluations);
        };

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of this quantum atomicity function
        virtual string solution(size_t atEvltn) const{
            return mBinder.solution(atEvltn);
        };

        // Resets Q atomicity function into its initial state without added
        // solutions
        virtual void reset(){
            mBinder.reset();
            QcellOp::reset();
        };
        
        // Change the Qatomicity function's number of inputs
        void noInputs(size_t nIns) { Qop::noInputs(nIns); };

    protected:
        // valculate return value of quantum atomicity function
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
        QcellOps    mOps;   // a list of assigned quantum cell operations
        Qbinder     mBinder;    // contins function inputs and output
    };

    typedef vector<Qatomicity::Sp> Qatomicities;

    class Qfunction : public QnaryOp
    {
    public:
        // Qfunction's shared pointer
        typedef shared_ptr<Qfunction> Sp;

        // Construct quantum function instance with name as its identity and
        // optinal number of arguments. The default number of arguments is 0
        Qfunction(const string& name, size_t noArguments = 0)
                                                : QnaryOp(name, noArguments) {};

        // Construct quantum function instance with name as its identity with a
        // logic defined by the block and optionaly the number of arguments
        Qfunction(const string& name, const Qblock& block,
                                                        size_t noArguments = 0);

        // Construct quantum function instance with name as its identity with a
        // logic defined by the list of quantum statetments and optionaly the
        // number of arguments
        Qfunction(const string& name, const Qstatements& statements,
                                                        size_t noArguments = 0);

        // Copy constructor
        Qfunction(const Qfunction& right)
                                    : QnaryOp(right), mBinder(right.mBinder) {};

        // Destruct Q-nary operation instance
        ~Qfunction() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const {
            return Qdef::Sp(new Qfunction(*this));
        };

        // Resizes the Q array of oprations to contain a specified size of elements. If the size is
        // smaller than the current Q array size, the content is reduced to its
        // first size elements, removing those beyond (and destroying them). If the size
        // is greater than the current container size, the content is expanded by
        // inserting at the end as many Q null operations as needed.
        virtual void resize(size_t size, Qvalue value = 0);

        // Returs quantum function declaration as a string
        string declaration() const;
        
        // Returns a string representation of Q function
        // if not decomposed, returns function declaration
        // if decomposed, returns bit-level logic of the function as a list of
        // quantum atomicities
        // when forBit is different than cAllBits, returns decomposed logic only
        // for the specified qbit level
        virtual string toString(bool decomposed = false,
                                            size_t forBit = cAllBits) const {
            if(!decomposed)
                return(declaration());
            return(QnaryOp::toString(decomposed, forBit));
        };

        // Adds a evaluation set containing nodes with solutions values, the
        // nodes should correspond to operands of statements assigned to this
        // quantum function
        virtual void add(const Qevaluations& evaluations) {
            mBinder.add(evaluations);
            QnaryOp::add(evaluations);
        };

        // For added evaluation set(s), returns a string represnting 'atEvltn'
        // solution of operands of this quantum function
        virtual string solution(size_t atEvltn) const {
            return mBinder.solution(atEvltn);
        };

        // Resets Q function into its initial state without added solutions
        virtual void reset() {
            mBinder.reset();
            QnaryOp::reset();
        };

        // An insertion operator (<<) to add a new statement into logic of this
        // Q function and returns the reference to 'this' object
        Qfunction& operator<<(const Qstatement& right);

        // An insertion operator (<<) to add a new statement into a logic of a
        // copy of this Q function and returns the reference to 'this' object
        Qfunction operator<<(const Qstatement& right) const;

        // Comma Operator class allows use of comma operator to insert
        // operands into specified Q function
        class CommaOp
        {
        public:
            // construct object with pointer to a Q function
            CommaOp(Qfunction* pFunction) : mpFunction(pFunction) {};

            // copy constructor
            CommaOp(const CommaOp& right) : mpFunction(right.mpFunction) {};

            // An insertion operator ',' to add a new statement into
            // specified Q routine
            CommaOp operator,(const Qstatement& right)
            {
                *mpFunction << right;
                return *this;
            };

        protected:
        private:
            Qfunction* mpFunction;    // pointer to Q function
        };

        // An assignment of a Q statement to a Q function, which can
        // be followed by list of other Q statements separated by
        // comma operator
        CommaOp operator=(const Qstatement& statement);

        // Insert string representation of a Q function into an output stream
        friend std::ostream& operator << (std::ostream&, const Qfunction&);

    protected:
        // Adds assigned inputs into quantum function's binder
        virtual void refreshOnInputs();

        // Adds assigned output into quantum function's binder
        virtual void refreshOnOutput();

        // Compiles a quantum operation logic and creates a list of quantum
        // atomicity function shared-pointers. The size of list is defined by the
        // highest number-of-qbits of compiled operations, i.e. its noqbs().
        // each atomicity function contains quantum cell operation of compiled
        // operations at the coresponding qbit level
        class Compiler : public Qcompiler
        {
        public:
            // Construct compiler with the pointer to the function
            Compiler(Qfunction* pFunction) : mpFunction(pFunction) {};
            
            // A method to be executed to compile quantum logic of the speficified
            // operation.
            virtual void compile(const Qop& op);
            
        protected:
            // Adds a shared pointer to a quantum cell operation to a
            // quantum atomicity function at specified bit level.
            virtual void add(QcellOp::Sp pOp, size_t at);
            
        private:
            Qfunction* mpFunction;   // a pointer to the function
        };

    private:
        Qbinder mBinder;    // contins function inputs and output
    };

    // Insert string representation of a Q routine into an output stream
    std::ostream& operator << (std::ostream&, const Qfunction&);
};

#endif /* Qfunc_h */
