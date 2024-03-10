//
//  QnaryOps.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-21.
//

#include <QnaryOps.h>
#include <Qbit.h>
#include <Qwhole.h>
#include <Qexpr.h>
#include <Qassign.h>

using namespace dann5;

string QnaryOperator::toString(bool decomposed, size_t forBit) const
{
    string str("?"), rest(""), rStr("");
    if (!decomposed) rStr += "(";
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
    }
    rStr += str + " " + identifier() + " ";
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
        rStr += str;
    }
    if (!decomposed)
        rStr += ")";
    else if (rest != "")
        rStr += "; " + rest;
    return rStr;
}

void QnaryOperator::refreshOnInputs()
{
    Qnaries ins(Qop::inputs());
    Qnary::Sp pLeft = ins[0];

    size_t size = Qop::noqbs();
    Qcells& opCells = cells();
    opCells.clear();
    for (size_t atBit = 0; atBit < size; atBit++)
    {
        QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(id());
        Qcell::Sp pLcell(as_const(*pLeft)[atBit]);
        pOp->inputs({pLcell});
        opCells.push_back(pOp);
    }
}

void QnaryNeq::refresh()
{
    Qfunction::refresh();
    // define aux variable
    Qdef::Sp pOut = Qop::output();
    Qdef::Sp pIn = Qop::inputs()[0];
    Qwhole aux(pIn->noqbs(), createOutId());
    size_t size = aux.noqbs();
    if (size < 2)
        aux.resize(2);
    // set not-equal condition
    Qexpr<Qbit> qbXpr;
    qbXpr = aux[1] | aux[0];
    for (size_t at = 2; at < size; at++)
        qbXpr = aux[at] | qbXpr;
    // define not-equal expression considering the argumens can be quantum
    // whole or quantum operations
    QnaryOp::Sp pOutOp = dynamic_pointer_cast<QnaryOp>(pOut),
    pInOp  = dynamic_pointer_cast<QnaryOp>(pIn);
    if(pOutOp == nullptr && pInOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qfunction* pThis = static_cast<Qfunction*>(this);
        static_cast<Qfunction&>(*this) = 
            out = aux + in, 
            Qbit::_1 = qbXpr;
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_1 = qbXpr;
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_1 = qbXpr;
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_1 = qbXpr;
    }
}

void QnaryLt::refresh()
{
    Qfunction::refresh();
    // define aux variable
    Qdef::Sp pOut = Qop::output();
    Qdef::Sp pIn = Qop::inputs()[0];
    Qwhole aux(pIn->noqbs(), createOutId());
    // define less-than expression considering the argumens can be quantum
    // whole or quantum operations
    QnaryOp::Sp pOutOp = dynamic_pointer_cast<QnaryOp>(pOut),
    pInOp  = dynamic_pointer_cast<QnaryOp>(pIn);
    if(pOutOp == nullptr && pInOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = out = aux + in;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 != out[addition.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        Qassign<Qwhole> addition = out = aux + in;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 != out[addition.noqbs() - 1];
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qexpr<Qwhole> compare = aux + in;
        const QnaryOp& compOp = dynamic_cast<const QnaryOp&>(compare.root());
        Qcell::Sp pOutCell = compOp[compOp.noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out == compare, Qbit::_0 != outOpOut;
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qexpr<Qwhole> compare = aux + in;
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out == compare, Qbit::_0 != outOpOut;
    }
}

void QnaryLe::refresh()
{
    Qfunction::refresh();
    // define aux variable
    Qdef::Sp pOut = Qop::output();
    Qdef::Sp pIn = Qop::inputs()[0];
    Qwhole aux(pOut->noqbs(), createOutId());
    // define less-than-or-equal expression considering the argumens can be
    // quantum whole or quantum operations
    QnaryOp::Sp pOutOp = dynamic_pointer_cast<QnaryOp>(pOut),
    pInOp  = dynamic_pointer_cast<QnaryOp>(pIn);
    if(pOutOp == nullptr && pInOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = in = aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == in[addition.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        Qexpr<Qwhole> addition = in == aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == inOpOut;
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = in = aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == in[addition.noqbs() - 1];
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        Qexpr<Qwhole> addition = in == aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == inOpOut;
    }
}

void QnaryGt::refresh()
{
    Qfunction::refresh();
    // define aux variable
    Qdef::Sp pOut = Qop::output();
    Qdef::Sp pIn = Qop::inputs()[0];
    Qwhole aux(pOut->noqbs(), createOutId());
    // define greater-than expression considering the argumens can be
    // quantum whole or quantum operations
    QnaryOp::Sp pOutOp = dynamic_pointer_cast<QnaryOp>(pOut),
    pInOp  = dynamic_pointer_cast<QnaryOp>(pIn);
    if(pOutOp == nullptr && pInOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = in = aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 != in[addition.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 != inOpOut;
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = in = aux + out;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 != in[addition.noqbs() - 1];
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 != inOpOut;
    }
}

void QnaryGe::refresh()
{
    Qfunction::refresh();
    // define aux variable
    Qdef::Sp pOut = Qop::output();
    Qdef::Sp pIn = Qop::inputs()[0];
    Qwhole aux(pIn->noqbs(), createOutId());
    // define greater-than-or-equal expression considering the argumens can be
    // quantum whole or quantum operations
    QnaryOp::Sp pOutOp = dynamic_pointer_cast<QnaryOp>(pOut),
    pInOp  = dynamic_pointer_cast<QnaryOp>(pIn);
    if(pOutOp == nullptr && pInOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qassign<Qwhole> addition = out = aux + in;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == out[addition.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        Qassign<Qwhole> addition = out = aux + in;
        static_cast<Qfunction&>(*this) = addition, Qbit::_0 == out[addition.noqbs() - 1];
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qexpr<Qwhole> compare = aux + in;
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out == compare, Qbit::_0 == outOpOut;
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qexpr<Qwhole> compare = aux + in;
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out == compare, Qbit::_0 == outOpOut;
    }
}