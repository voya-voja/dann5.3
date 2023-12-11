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
    Qwhole& out = *static_pointer_cast<Qwhole>(Qop::output());
    Qwhole& in = *static_pointer_cast<Qwhole>(Qop::inputs()[0]);
    Qwhole aux(in.noqbs(), createOutId());
    size_t size = aux.noqbs();
    if (size < 2)
        aux.resize(2);
    Qexpr<Qbit> qbXpr;
    qbXpr = aux[1] | aux[0];
    for (size_t at = 2; at < size; at++)
        qbXpr = aux[at] | qbXpr;
    Qbit _1("_1", 1);
    static_cast<Qfunction&>(*this) = out = aux + in, _1 = qbXpr;
}

void QnaryLt::refresh()
{
    Qfunction::refresh();
    Qwhole& out = *static_pointer_cast<Qwhole>(Qop::output());
    Qwhole& in = *static_pointer_cast<Qwhole>(Qop::inputs()[0]);
    Qwhole aux(in.noqbs(), createOutId());
    Qbit _0("_0", 0);
    static_cast<Qfunction&>(*this) = out = aux + in, _0 != out[out.noqbs() - 1];
}

void QnaryLe::refresh()
{
    Qfunction::refresh();
    Qwhole& out = *static_pointer_cast<Qwhole>(Qop::output());
    Qwhole& in = *static_pointer_cast<Qwhole>(Qop::inputs()[0]);
    Qwhole aux(out.noqbs(), createOutId());
    Qbit _0("_0", 0);
    static_cast<Qfunction&>(*this) = in = aux + out, _0 == in[in.noqbs() - 1];
}

void QnaryGt::refresh()
{
    Qfunction::refresh();
    Qwhole& out = *static_pointer_cast<Qwhole>(Qop::output());
    Qwhole& in = *static_pointer_cast<Qwhole>(Qop::inputs()[0]);
    Qwhole aux(out.noqbs(), createOutId());
    Qbit _0("_0", 0);
    static_cast<Qfunction&>(*this) = in = aux + out, _0 != in[in.noqbs() - 1];
}

void QnaryGe::refresh()
{
    Qfunction::refresh();
    Qwhole& out = *static_pointer_cast<Qwhole>(Qop::output());
    Qwhole& in = *static_pointer_cast<Qwhole>(Qop::inputs()[0]);
    Qwhole aux(in.noqbs(), createOutId());
    Qbit _0("_0", 0);
    static_cast<Qfunction&>(*this) = out = aux + in, _0 == out[out.noqbs() - 1];
}
