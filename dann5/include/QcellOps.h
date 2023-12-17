#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qcell.h>
#include <Qop.h>
#include <Qvalue.h>

using namespace std;

namespace dann5 {
    // A Quantum cell operation is a binary operation on a quantum cell level
    class QcellOp : public Qcell, public Qop
    {
    public:
        // Quantum cell operation shared pointer
        typedef shared_ptr<QcellOp> Sp;

        // Instantiate a Quantum cell operation instance with identity and
        // number of inputs
        QcellOp(const string& id, size_t size);

        // Copy constructor
        QcellOp(const QcellOp& right) : Qcell(right), Qop(right) {};

        // Destruct Quantum cell operation
        ~QcellOp() {};

        // overrided inputs setter to sync output value with this operation
        virtual void inputs(const Qdefs&);

        // overrides output setter to sync its value and value of this operation
        virtual void output(const Qdef::Sp& pOut, size_t forBit = cAllBits);

        // Returns 1 as cell operations have operands with 1 quantum bit
        virtual std::size_t noqbs() const noexcept { return 1; };

        // set and get the value of this Q cell
        virtual void value(Qvalue v);	// throws logic_error exception
        virtual Qvalue value() const;
        virtual Qvalue value() { return(as_const(*this).value()); };

        // Convert quantum cell operation expression into a string
        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const {
            return Qop::toString(decomposed, forBit);
        };

        // Sets solution values from the evaluation set for this cell operation
        virtual void add(const Qevaluations& evaluations) {
            Qop::add(evaluations);
        };

        // Returns a string representation of a solution value of evaluation at
        // 'atEvltn' for the cell operation
        virtual string solution(size_t atEvltn) const {
            return Qop::solution(atEvltn);
        };

        // Reset the quantum operation into its initial state without solutions
        // by clearing all evaluation samples
        virtual void reset() {
            Qop::reset();
        };

    protected:
        // Override to implement a calculation of the resulting value of this
        // cell operation
        virtual Qvalue calculate(const Qvalues& values) const = 0;
        
    private:
    };

    // An std::vector containing a list of shared pointers pointing to
    // Q-cell operations
    class QcellOps : public vector <QcellOp::Sp>
    {
    public:
        // default constructor
        QcellOps() : vector <QcellOp::Sp>() {};
        
        // copy cnstructor
        QcellOps(const QcellOps& right) : vector <QcellOp::Sp>(right) {};
        
        // conversion constructor of Qdefs to QcellOps
        QcellOps(const Qdefs& list){
            for(auto pDef: list)
                push_back(dynamic_pointer_cast<QcellOp>(pDef));
        };
    };

    // A Quantum null operation is an empty cell operation
    class QnullCellOp : public QcellOp
    {
    public:
        // Qeq's shared pointer
        typedef shared_ptr<QnullCellOp> Sp;

        // Instantiate an Quantum null cell operation instance with its identity
        QnullCellOp() : QcellOp("?null-op!", 0) {};

        // Copy constructor
        QnullCellOp(const QnullCellOp& right) : QcellOp(right) {};

        // Destruct Qeq with shared pointers to its Qdef
        ~QnullCellOp() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const {
            return Qdef::Sp(new QnullCellOp(*this));
        };

        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const {
            return ""; // present a null cell operation as an empty string
        };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const { return 0; };

    private:
    };

    // A Quantum operator is an abstration of an unnary operation
    class Qoperator : public QcellOp
    {
    public:
        // Quantum operator's shared pointer
        typedef shared_ptr<Qoperator> Sp;

        // Instantiate an Quantum operator instance with its identity
        Qoperator(const string& id) : QcellOp(id, 1) {};

        // Copy constructor
        Qoperator(const Qoperator& right) : QcellOp(right) {};

        // Destruct Quantum operator
        ~Qoperator() {};

        // overrides output getter to  return output operand when operation is
        // an output
        virtual Qdef::Sp output(size_t forBit = cAllBits) const;

        // convert Quantum operator expression into a string
        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const;
    protected:
    private:
    };

    // A Quantum equal is an operator
    class Qeq : public Qoperator
    {
    public:
        // Qeq's shared pointer
        typedef shared_ptr<Qeq> Sp;
        
        // Quantum equal operator identifiers
        static const string cMark;    // "=="
        static const string cName;    // "equal"

        // Instantiate Qeq instance with '==' identity
        Qeq() : Qoperator(cMark) {};

        // Copy constructor
        Qeq(const Qeq& right) : Qoperator(right){};

        // Destruct Qeq instance
        ~Qeq() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qeq(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum not-equal is an operator
    class Qneq : public Qoperator
    {
    public:
        // Qneq's shared pointer
        typedef shared_ptr<Qneq> Sp;

        // Quantum not equal operator identifiers
        static const string cMark;    // "!="
        static const string cName;    // "notequal"

        // Instantiate Qneq instance with '!=' identity
        Qneq() : Qoperator(cMark) {};

        // Copy constructor
        Qneq(const Qneq& right) : Qoperator(right) {};

        // Destruct quantum not-equal instance
        ~Qneq() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qneq(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum invert is an operator
    class Qinvert : public Qoperator
    {
    public:
        // Qinvert's shared pointer
        typedef shared_ptr<Qinvert> Sp;

        // Quantum invert operator identifiers
        static const string cMark;    // "~"
        static const string cName;    // "invert"

        // Instantiate Qinvert instance with '~' identity
        Qinvert() : Qoperator(cMark) {};

        // Copy constructor
        Qinvert(const Qinvert& right) : Qoperator(right) {};

        // Destruct quantum not-equal instance
        ~Qinvert() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qinvert(*this)); };
        
        // Returns not-equal mark as this operation identifier
        virtual string identifier() const noexcept {
            return Qneq::cMark;
        };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum less-than is an operator
    class Qlt : public Qoperator
    {
    public:
        // Qlt's shared pointer
        typedef shared_ptr<Qlt> Sp;

        // Quantum less than operator identifiers
        static const string cMark;    // "<"
        static const string cName;    // "lessthan"
        
        // Instantiate Qlt instance with '&' identity
        Qlt() : Qoperator(cMark) {};

        // Copy constructor
        Qlt(const Qlt& right) : Qoperator(right) {};

        // Destruct Qlt with shared pointers to its Qdef
        ~Qlt() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qlt(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum less-than-or-equal is an operator
    class Qle : public Qoperator
    {
    public:
        // Qle's shared pointer
        typedef shared_ptr<Qle> Sp;

        // Quantum less than or equal operator identifiers
        static const string cMark;    // "<="
        static const string cName;    // "lessequal"
        
        // Instantiate Qle instance with '&' identity
        Qle() : Qoperator(cMark) {};

        // Copy constructor
        Qle(const Qle& right) : Qoperator(right) {};

        // Destruct Qle with shared pointers to its Qdef
        ~Qle() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qle(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum greater-than is an operator
    class Qgt : public Qoperator
    {
    public:
        // Qgt's shared pointer
        typedef shared_ptr<Qgt> Sp;

        // Quantum greater than operator identifiers
        static const string cMark;    // ">"
        static const string cName;    // "greaterthan"
        
        // Instantiate Qgt instance with '&' identity
        Qgt() : Qoperator(cMark) {};

        // Copy constructor
        Qgt(const Qgt& right) : Qoperator(right) {};

        // Destruct Qgt with shared pointers to its Qdef
        ~Qgt() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qgt(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum greater-than-or-equal is an operator
    class Qge : public Qoperator
    {
    public:
        // Qge's shared pointer
        typedef shared_ptr<Qge> Sp;

        // Quantum greater than or equal operator identifiers
        static const string cMark;    // ">="
        static const string cName;    // "greaterequal"
        
        // Instantiate Qge instance with '&' identity
        Qge() : Qoperator(cMark) {};

        // Copy constructor
        Qge(const Qge& right) : Qoperator(right) {};

        // Destruct Qge with shared pointers to its Qdef
        ~Qge() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qge(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    /****Quantum Binary operations *****/

    // A Quantum and is a binary operation
    class Qand : public QcellOp
    {
    public:
        // Qand's shared pointer
        typedef shared_ptr<Qand> Sp;

        // Quantum and binary operation identifiers
        static const string cMark;    // "&"
        static const string cName;    // "and"
        
        // Instantiate Qand instance with '&' identity
        Qand() : QcellOp(cMark, 2) {};

        // Copy constructor
        Qand(const Qand& right) : QcellOp(right) {};

        // Destruct Qand with shared pointers to its two Qdefs
        ~Qand() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qand(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };


    // A Q cell operation with 2 outputs is an abstration of a Q cell
    // operation that has an additional auxiliary output
    class Qcell2outOp : public QcellOp
    {
    public:
        // Qaddition's shared pointer
        typedef shared_ptr<Qcell2outOp> Sp;

        // An Q addition has identity and should have at least two argument
        Qcell2outOp(const string& id, size_t size);

        // Copy constructor
        Qcell2outOp(const Qcell2outOp& right)
                : QcellOp(right), mpAuxiliary(right.mpAuxiliary) {};

        // Destruct Qeq with shared pointers to its Qdef
        ~Qcell2outOp() {};
        
        // overrides operands setter to initialize 2nd output
        virtual void operands(const Qdef::Sp& pOut, const Qdefs& ins);
        
        // overrides output setter to initialize 2nd output
        virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

        // Return Q addition result and carry outputs
        virtual Qdefs outputs() const;

    protected:
        // initializes 2nd output
        inline void initialize(const Qdef::Sp& pOut);
        
    private:
        Qdef::Sp mpAuxiliary;
    };

    // A Quantum nand is a binary operation with 2 outputs
    class Qnand : public Qcell2outOp
    {
    public:
        // Qnand's shared pointer
        typedef shared_ptr<Qnand> Sp;

        // Quantum nand binary operation identifiers
        static const string cMark;    // "~&"
        static const string cName;    // "nand"
        
        // Instantiate Qnand instance with '&' identity
        Qnand() : Qcell2outOp(cMark, 2) {};

        // Copy constructor
        Qnand(const Qnand& right) : Qcell2outOp(right) {};

        // Destruct QnAnd with shared pointers to its two Qdefs
        ~Qnand() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qnand(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum or is a binary operation
    class Qor : public QcellOp
    {
    public:
        // Qor's shared pointer
        typedef shared_ptr<Qor> Sp;

        // Quantum or binary operation identifiers
        static const string cMark;    // "|"
        static const string cName;    // "or"
        
        // Instantiate Qor instance with '|' identity
        Qor() : QcellOp(cMark, 2) {};

        // Copy constructor
        Qor(const Qor& right) : QcellOp(right) {};

        // Destruct Qor with shared pointers to its two Qdefs
        ~Qor() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qor(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum nor is a binary operation
    class Qnor : public Qcell2outOp
    {
    public:
        // Qnor's shared pointer
        typedef shared_ptr<Qnor> Sp;

        // Quantum nor binary operation identifiers
        static const string cMark;    // "~|"
        static const string cName;    // "nor"
        
        // Instantiate Qnor instance with '|' identity
        Qnor() : Qcell2outOp(cMark, 2) {};

        // Copy constructor
        Qnor(const Qnor& right) : Qcell2outOp(right) {};

        // Destruct QnOr with shared pointers to its two Qdefs
        ~Qnor() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qnor(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Q addition is an abstration of a Q cell operation that has
    // an additional carry Q cell output
    class Qaddition : public QcellOp
    {
    public:
        // Qaddition's shared pointer
        typedef shared_ptr<Qaddition> Sp;

        // A Quantum carry is an operand defined by a Q addition
        class Carry : public QcellOp
        {
        public:
            // Q carry's shared pointer
            typedef shared_ptr<Carry> Sp;

            // Quantum carry operation identifiers
            static const string cMark;    // "#"
            static const string cName;    // "carry"
            
            // Return converted given string to Q carry definition, e.g. add-tag
            // 'a0 ^ b0' is converted to '#[a0 ^ b0]'
            static string Symbol(const string& addTag);
            
            // Initiate Carry instance with a pointer to a parent Q addition
            Carry(Qaddition*);

            // Copy constructor
            Carry(const Carry&);

            // Destruct a Q carry operand
            ~Carry();

            // Override: Carry is an operation that should be treated as a definition
            virtual bool asDefinition() { return true; };

            // overrides output setter to set its output pointer
            virtual void output(const Qdef::Sp& pOut, size_t forBit = cAllBits);

            // Returns a Qdef's shared pointer pointing to a copy of this object
            // same Carry object is set to mpAddition
            virtual Qdef::Sp clone() const;

            // Return a string representation of Q carry operand
            virtual string toString(bool decomposed = false, size_t atBit = cAllBits) const;

            // Set a pointer to a new parent Q addition
            void addition(Qaddition*);

        protected:
            virtual Qvalue calculate(const Qvalues& values) const;

        private:
            Qaddition*	mpAddition;	// A parent Q addition
        };

        // An Q addition has identity and should have at least two argument
        Qaddition(const string& id, size_t size) : QcellOp(id, size), mpCarry(Carry::Sp(new Carry(this))) {};

        Qaddition(const string& id, size_t size, const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry);

        // Copy constructor
        Qaddition(const Qaddition& right) : QcellOp(right), mpCarry(right.mpCarry)
        {
            mpCarry->addition(this);
        };

        // Destruct Qeq with shared pointers to its Qdef
        ~Qaddition() {};
        
        // overrides operands to sync carry output value with this operation
        // inputs and to initialize carry output
        virtual void operands(const Qdef::Sp& pOut, const Qdefs& ins);

        // overrided inputs setter to sync carry output value with this operation
        virtual void inputs(const Qdefs&);

        // overrides output setter to initialize carry output
        virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

        // Return Q addition result and carry outputs
        virtual Qdefs outputs() const;

        // get carry cell output
        Carry::Sp carry() const { return mpCarry; };
        
        // set carry cell output
        void set(const Carry::Sp& pCarry) { mpCarry = pCarry; };

    protected:
        // get a reference to carry cell output
        Carry& carry() { return *mpCarry; };

        // initializes carry output
        inline void initialize();

    private:
        Carry::Sp	mpCarry;
    };

    // A Quantum xOr is a binary Q addition
    class Qxor : public Qaddition
    {
    public:
        // Qxor's shared pointer
        typedef shared_ptr<Qxor> Sp;

        // Quantum xOr binary addition identifiers
        static const string cMark;    // "^"
        static const string cName;    // "xor"
        
        // Default constructor
        Qxor() : Qaddition(cMark, 2) {};

        // Copy constructor
        Qxor(const Qxor& right) : Qaddition(right) {};

        // Destruct the Q xOr instance with a shared pointer to its carry operand
        // and two inputs
        ~Qxor() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qxor(*this)); };

    protected:
        // Pass-through constructor
        Qxor(const string& id) : Qaddition(id, 2) {};

        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum xOr is a binary Q addition
    class Qnxor : public Qaddition
    {
    public:
        // Qnxor's shared pointer
        typedef shared_ptr<Qxor> Sp;

        // Quantum nXor binary addition identifiers
        static const string cMark;    // "*="
        static const string cName;    // "nxor"
        
        // Default constructor
        Qnxor() : Qaddition(cMark, 2) {};

        // Copy constructor
        Qnxor(const Qnxor& right) : Qaddition(right) {};

        // Destruct the Q nXor instance with a shared pointer to its carry operand
        // and two inputs
        ~Qnxor() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qnxor(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum half-adder is a binary Q xor addition
    class QhalfAdder : public Qxor
    {
    public:
        // Qnxor's shared pointer
        typedef shared_ptr<QhalfAdder> Sp;

        // Quantum binary half-adder addition identifiers
        static const string cMark;    // ""h+"
        static const string cName;    // "halfadder"
        
        // Default constructor
        QhalfAdder() : Qxor(cMark) {};

        // Copy constructor
        QhalfAdder(const QhalfAdder& right) : Qxor(right) {};

        // Destruct the Q half-adder instance with a shared pointer to its carry operand
        // and two inputs
        ~QhalfAdder() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QhalfAdder(*this)); };

    protected:
    private:
    };

    /****Quantum Trinary operations *****/

    // A Quantum (full) adder is a trinary Q addition
    class Qadder : public Qaddition
    {
    public:
        // QtriOperation's shared pointer
        typedef shared_ptr<Qadder> Sp;

        // Quantum trinary adder identifiers
        static const string cMark;    // "+"
        static const string cName;    // "adder"
        
        // Default constructor
        Qadder() : Qaddition(cMark, 3) {};

        // Default constructor
        Qadder(const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry) : Qaddition(cMark, 3, pOut, pCarry) {};

        // Copy constructor
        Qadder(const Qadder& right) : Qaddition(right) {};

        // Destruct the Q adder instance with a shared pointer to its carry operand
        // and two inputs
        ~Qadder() {};

        // Returns the largest number of Q bits of all its inputs
        virtual std::size_t noqbs() const noexcept { return 1; };

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qadder(*this)); };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

    private:
    };

    // A Quantum xOr that can be extended to act as Q adder
    class QxorAdder : public Qaddition
    {
    public:
        // Qxor's shared pointer
        typedef shared_ptr<QxorAdder> Sp;

        // Default constructor for XOr
        QxorAdder() : Qaddition(Qxor::cMark, 2) {};

        // Constructor for Adder regardless of asAdder value
        QxorAdder(bool asAdder) : Qaddition(Qadder::cMark, 3) {};

        // Copy constructor, Q xo-adder can have up to 3 inputs
        QxorAdder(const Qxor& right) : Qaddition(right) {};

        // Destruct the Q xor instance with a shared pointer to its carry operand
        // and two inputs
        ~QxorAdder() {};

        // Returns the largest number of Q bits of all its inputs
        virtual std::size_t noqbs() const noexcept { return 1; };

        // Sets Q xor-adder inputs. If any of the inputs is Q xor-adder acting as an Q xor,
        // it will be upgraded into an adder by adding 3rd input.
        virtual Sp process(const Qdefs&);

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QxorAdder(*this)); };

        // Returns true if has 2 inputs and acts as an Q xor circuit. Returns false
        // if has 3 inputs and acts as a Q adder circuit.
        bool isXor() { return Qop::inputs().size() == 2; };

        // Returns true if has 3 inputs and acts as an Q xor circuit. Returns false
        // if has 2 inputs and acts as a Q xor circuit.
        bool isAdder() { return Qop::inputs().size() == 3; };

        // Returns true if this object has been extended from a xor to an adder.
        bool isExtended() { return mExtended; };

    protected:
        virtual Qvalue calculate(const Qvalues& values) const;

        void extended() { mExtended = true; };

    private:
        bool	mExtended = false;
    };
};
