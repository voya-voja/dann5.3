//
//  Qmod.cpp
//  d5
//
//  Created by Nebojsa Vojinovic on 2024-08-10.
//

#include <Qmod.h>
#include <Qwhole.h>
#include <Qexpr.h>
#include <Qassign.h>

using namespace dann5;

string QwholeMod::toString(bool decomposed, size_t forBit) const
{
    if (decomposed) return Qfunction::toString(decomposed, forBit);
    Qdefs ins(Qop::inputs());
    string presentMod = ins[0]->toString() + " "
                        + id() + " " + ins[1]->toString();
    return presentMod;
}

void QwholeMod::refresh()
{
    Qfunction::refresh();
    Qwhole& out = *dynamic_pointer_cast<Qwhole>(Qop::output());
    Qdefs ins = Qop::inputs();
    // define aux variable
    Qwhole factor(ins[1]->noqbs(), createOutId());
    out.resize(ins[1]->noqbs());

    // define modulo expression considering the argumens can be quantum
    // whole or quantum expressions
    QnaryOp::Sp pInOp0  = dynamic_pointer_cast<QnaryOp>(ins[0]),
                pInOp1  = dynamic_pointer_cast<QnaryOp>(ins[1]);
    if(pInOp0 == nullptr && pInOp1 == nullptr)
    {
        Qwhole& number = *static_pointer_cast<Qwhole>(ins[0]);
        Qwhole& divisor = *static_pointer_cast<Qwhole>(ins[1]);
        Qassign<Qwhole> modAssignment = number = out + factor * divisor;
        /*DEBUG*/cout << modAssignment.toString(true) << endl; /*DEBUG_END*/
        Qexpr<Qwhole> compExpression = out < divisor;
        /*DEBUG*/cout << modAssignment.toString(true) << endl << compExpression.toString(true);/*DEBUG_END*/
        static_cast<Qfunction&>(*this) = modAssignment, compExpression;
    }
    else if(pInOp0 == nullptr)
    {
        Qwhole& number = *static_pointer_cast<Qwhole>(ins[0]);
        Qexpr<Qwhole> divisorXpr(pInOp1);
        Qassign<Qwhole> modAssignment = number = out + factor * divisorXpr;
        static_cast<Qfunction&>(*this) = modAssignment, out < divisorXpr;
    }
    else if(pInOp1 == nullptr)
    {
        Qexpr<Qwhole> numberXpr(pInOp0);
        Qwhole& divisor = *static_pointer_cast<Qwhole>(ins[1]);
        Qexpr<Qwhole> modXpr = numberXpr == out + factor * divisor;
        static_cast<Qfunction&>(*this) = modXpr, out < divisor;
    }
    else
    {
        Qexpr<Qwhole> numberXpr(pInOp0);
        Qexpr<Qwhole> divisorXpr(pInOp1);
        Qexpr<Qwhole> modXpr = numberXpr == out + factor * divisorXpr;
        static_cast<Qfunction&>(*this) = modXpr, out < divisorXpr;
    }
}
