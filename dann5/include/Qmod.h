//
//  QnaryOps.h
//  d5
//
//  Created by Nebojsa Vojinovic on 2024-08-10.
//

#ifndef Qmod_h
#define Qmod_h

#include <Qfunc.h>

namespace dann5
{
    // A Quantum modulo (mod) is a binary operation implemented as a quantum
    // function mod = number % divisor, where mod is always a Qwhole variable,
    // and number and divisor can be Qwhole variable or QnaryOp (i.e. a root
    // operation of a linked Qexpr<Qwhole>
    class QwholeMod : public Qfunction 
    {
    public:
        // QnaryNeq's shared pointer
        typedef shared_ptr<QwholeMod> Sp;

        // Quantum modulo (mod) operator identifiers
        static const string cMark() { return "%";; };
        static const string cName() { return "mod"; };

        // Instantiate quantum modulo (mod) operator with '%' identity
        QwholeMod() : Qfunction(cMark(), 2) {};

        // Copy constructor
        QwholeMod(const QwholeMod& right) : Qfunction(right) {};

        // Destruct quantum modulo (mod)
        ~QwholeMod() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QwholeMod(*this)); };

        // Returns a string representation of the modulo function
        virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const;
        
    protected:
        // Refreshes the Quantum function cells according to modulo logic
        void refresh();
    private:
    };
};

#endif /* Qmod_h */
