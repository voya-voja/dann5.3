//
//  QnaryOperation.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-16.
//

#ifndef QnaryOperation_h
#define QnaryOperation_h

#include <QnaryOp.h>
#include <QcellOps.h>

namespace dann5
{
    // A templated Q-nary operation for implementation of Qoperators
    template <typename Q_OP = QcellOp>
    class QnaryOperation : public QnaryOp
    {
    public:
        // Q-nary operation's shared pointer
        typedef shared_ptr<QnaryOperation<Q_OP> > Sp;
        
        // Quantum equal operator identifiers
        static const string cMark;    // Q_OP::cMark
        static const string cName;    // Q_OP::cName

        // Construct Q-nary operation instance as instantionation of Q_OP identity
        QnaryOperation() : QnaryOp(cMark, Q_OP().noInputs()) {};

        // Copy constructor
        QnaryOperation(const QnaryOperation<Q_OP>& right) : QnaryOp(right){};

        // Destruct Q-nary operation instance
        ~QnaryOperation() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new QnaryOperation<Q_OP>(*this)); };
        
        // convert Quantum operator expration into a string
        virtual string toString(bool decomposed, size_t forBit) const
        {
            if(noInputs() > 1) return QnaryOp::toString(decomposed, forBit);
            
            string str("?"), rest(""), rStr("");
            if (!decomposed) rStr += "(";
            Qdef::Sp pIn = Qop::inputs()[0];
            if (pIn != nullptr)
            {
                str = pIn->toString(decomposed, forBit);
                if (decomposed)
                {
                    // if operand is a sub-operation
                    Qop::Sp pOp = dynamic_pointer_cast<Qop>(pIn);
                    if (pOp != nullptr)
                    {
                        rest += str;
                        str = pOp->output(forBit)->toString(decomposed, forBit); // extract sub-operation output
                    }
                }
            }
            rStr += str + " " + identifier() + " ";
            Qdef::Sp pOut = Qop::output();
            if (pOut != nullptr)
            {
                str = pOut->toString(decomposed, forBit);
                if (decomposed)
                {
                    // if operand is a sub-operation
                    Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOut);
                    if (pOp != nullptr)
                    {
                        if (rest != "") rest += "; ";
                        rest += str;
                        str = pOp->output(forBit)->toString(decomposed, forBit); // extract sub-operation output
                    }
                }
                rStr += str;
            }
            if (!decomposed)
                rStr += ")";
            else if (rest != "")
                rStr += "; " + rest;
            return rStr;
        }

    protected:
        // Override to refresh the Q-nary operation cells according to the derived
        // operation logic
        virtual void refresh()
        {
            size_t size = Qop::noqbs();
            Qcells& opCells = cells();
            opCells.resize(size);
            const Qdefs& args = Qop::inputs();
            Qnary::Sp pLeft = dynamic_pointer_cast<Qnary>(args[0]), pRight = nullptr;
            size_t noArgs = args.size();
            if(noArgs > 1)
                pRight = dynamic_pointer_cast<Qnary>(args[1]);
            for (size_t atBit = 0; atBit < size; atBit++)
            {
                QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Q_OP::cMark);
                Qcell::Sp pLcell(as_const(*pLeft)[atBit]);
                if(noArgs == 1)
                {
                    pOp->inputs({pLcell});
                }
                else
                {
                    Qcell::Sp pRcell(as_const(*pRight)[atBit]);
                    pOp->inputs({pLcell, pRcell});
                }
                opCells[atBit] = pOp;
            }
        };
    private:
    };
};

#endif /* QnaryOperation_h */
