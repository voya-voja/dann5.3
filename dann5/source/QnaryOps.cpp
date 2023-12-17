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
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_1 = qbXpr;
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_1 = qbXpr;
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);;
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
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 != out[out.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 != out[out.noqbs() - 1];
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);;
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 != outOpOut;
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 != outOpOut;
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
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 == in[in.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 == inOpOut;
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);;
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 == in[in.noqbs() - 1];
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pInCell = as_const(*pInOp)[pInOp->noqbs() - 1];
        QcellOp::Sp pInCellOp = dynamic_pointer_cast<QcellOp>(pInCell);
        Qbit& inOpOut = *dynamic_pointer_cast<Qbit>(pInCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 == inOpOut;
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
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 != in[in.noqbs() - 1];
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
        Qexpr<Qwhole> out(pOutOp);;
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        static_cast<Qfunction&>(*this) = in = aux + out, Qbit::_0 != in[in.noqbs() - 1];
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
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 == out[out.noqbs() - 1];
    }
    else if(pOutOp == nullptr)
    {
        Qwhole& out = *static_pointer_cast<Qwhole>(pOut);
        Qexpr<Qwhole> in(pInOp);
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 == out[out.noqbs() - 1];
    }
    else if(pInOp == nullptr)
    {
        Qexpr<Qwhole> out(pOutOp);;
        Qwhole& in = *static_pointer_cast<Qwhole>(pIn);
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 == outOpOut;
    }
    else
    {
        Qexpr<Qwhole> out(pOutOp);
        Qexpr<Qwhole> in(pInOp);
        Qcell::Sp pOutCell = as_const(*pOutOp)[pOutOp->noqbs() - 1];
        QcellOp::Sp pOutCellOp = dynamic_pointer_cast<QcellOp>(pOutCell);
        Qbit& outOpOut = *dynamic_pointer_cast<Qbit>(pOutCellOp->Qop::output());
        static_cast<Qfunction&>(*this) = out = aux + in, Qbit::_0 == outOpOut;
    }
}

void Qminus::refresh()
{
    Qfunction::refresh();
    //Qexpr<Qwhole> xy_x(y - x);
    Qnaries ins(Qop::inputs());
    Qdef::Sp pOut = Qop::output();
/*    size_t iSize = ins.size();
    if (iSize != 2) return;
    // ... is same as ins[0] = out + ins[1]
    size_t outSize = ins[1]->noqbs();
    if (pOut->noqbs() == 0)
    {
        Qnary::Sp pNaryOut = static_pointer_cast<Qnary>(pOut);
        pNaryOut->resize(outSize);
    }
    QnaryOp::Sp mpSubstituteOp = dynamic_pointer_cast<QnaryOp>(Qadd().clone());
    Qwhole subOut(mpSubstituteOp->createOutId());
    mpSubstituteOp->operands(subOut.clone(), {pOut, ins[1]});
//    mEq.operands(ins[0], {mpSubstituteOp});
//    resize(mEq.noqbs());
 */
    QnaryOp::Sp pLeftOp = dynamic_pointer_cast<QnaryOp>(ins[0]),
                pRightOp = dynamic_pointer_cast<QnaryOp>(ins[1]);
    if(pLeftOp == nullptr && pRightOp == nullptr)
    {
        Qwhole& left = *dynamic_pointer_cast<Qwhole>(ins[0]);
        Qwhole& right = *dynamic_pointer_cast<Qwhole>(ins[1]);
        Qwhole& out = *dynamic_pointer_cast<Qwhole>(pOut);
        Qwhole aux(left.noqbs(), Qadd().createOutId());
        static_cast<Qfunction&>(*this) = left == aux, aux = out + right;
    }
    else if(pLeftOp != nullptr && pRightOp != nullptr)
    {
        Qexpr<Qwhole> left(pLeftOp);
        Qexpr<Qwhole> right(pRightOp);
        Qwhole& out = *dynamic_pointer_cast<Qwhole>(pOut);
        Qwhole aux(left.noqbs(), Qadd().createOutId());
        static_cast<Qfunction&>(*this) = left == aux, aux = out + right;
    }
    else if(pLeftOp != nullptr && pRightOp == nullptr)
    {
        Qexpr<Qwhole> left(pLeftOp);
        Qwhole& right = *dynamic_pointer_cast<Qwhole>(ins[1]);
        Qwhole& out = *dynamic_pointer_cast<Qwhole>(pOut);
        Qwhole aux(left.noqbs(), Qadd().createOutId());
        static_cast<Qfunction&>(*this) = left == aux, aux = out + right;
    }
    else if(pLeftOp == nullptr && pRightOp != nullptr)
    {
        Qwhole& left = *dynamic_pointer_cast<Qwhole>(ins[0]);
        Qexpr<Qwhole> right(pRightOp);
        Qwhole& out = *dynamic_pointer_cast<Qwhole>(pOut);
        Qwhole aux(left.noqbs(), Qadd().createOutId());
        static_cast<Qfunction&>(*this) = left == aux, aux = out + right;
    }
    else
        throw logic_error(string("ERROR @Qminus::refresh(): Input arguments are")
                                + " not Qwhole or QnaryOp");
}
