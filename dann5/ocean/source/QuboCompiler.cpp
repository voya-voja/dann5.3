//
//  QuboCompiler.cpp
//  d5o_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-12.
//

#include <QuboCompiler.h>

#include <Qdef.h>
#include <Qop.h>
#include <Qcell.h>
#include <Qnary.h>
#include <QcellOps.h>
#include <QnaryOp.h>
#include <QuboAnalyzer.h>

using namespace dann5;
using namespace dann5::ocean;

/**** Qubo Compiler ****/
void QuboCompiler::compile(const Qop& op)
{
    mQubo += parse(op);
}

Qubo QuboCompiler::parse(const Qop& op, size_t forBit) const
{
    const QnullCellOp* pNullOp = dynamic_cast<const QnullCellOp*>(&op);
    if(pNullOp != nullptr)
        return Qubo();
    const Qaddition::Carry* pCarry = dynamic_cast<const Qaddition::Carry*>(&op);
    if(pCarry != nullptr)
        return Qubo();
    return qubo(op, forBit);
}

Qubo QuboCompiler::qubo(const Qop& op, size_t forBit) const
{
    Qdefs ins = op.inputs();
    size_t iSize = ins.size();
    Qubo aQubo;
    QuboTable::IoPorts ports;
    // process input ports
    for (size_t at = 0; at < iSize; at++)
    {
        // use names of argument and this operands as unique string describing inputs
        Qdef::Sp pIn = ins[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pIn);
        if (pOprnd == nullptr)
        {
            Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(pIn);
            pOprnd = as_const(*pNary)[forBit];
        }
        Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOprnd);
        if (pOp != nullptr)
        {
            // add sub-qubo from argument operand
            aQubo += parse(*pOp, forBit);
            Qcell::Sp pOut = static_pointer_cast<Qcell>(pOp->output(forBit));
            ports.push_back(QuboTable::IoPort(pOut->id(), pOut->value()));
        }
        else
        {
            ports.push_back(QuboTable::IoPort(pOprnd->id(), pOprnd->value()));
        }
    }
    // process output ports
    Qdefs outs = op.outputs();
    size_t oSize = outs.size();
    for (size_t at = 0; at < oSize; at++)
    {
        Qdef::Sp pOut = outs[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pOut);
        if (pOprnd == nullptr)
        {
            Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(pOut);
            pOprnd = as_const(*pNary)[forBit];
        }
        Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOprnd);
        if (pOp != nullptr)
        {
            // add sub-qubo from argument operand
            aQubo += parse(*pOp, forBit);
            Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(pOp->output(forBit));
            if(pOut != nullptr)
                ports.push_back(QuboTable::IoPort(pOut->id(), pOut->value()));
        }
        else
        {
            ports.push_back(QuboTable::IoPort(pOprnd->id(), pOprnd->value()));
        }
    }
    if ((iSize + oSize) > ports.size()) return aQubo;
    // create QuboTable rule object for this operand
    QuboTable::Sp pQubo = QTfactory::Instance().create(op.identifier());
    Qubo qubo = pQubo->qubo(ports, mFinalized);
    qubo += aQubo;
    return(qubo);
}

/**** Qubos Compiler ****/
void QubosCompiler::compile(const Qop& op)
{
    const QnaryOp& naryOp = dynamic_cast<const QnaryOp&>(op);
    size_t size = op.noqbs();
    size_t nodeCnt = 0;
    Qubo subQubo = Qubo();
    for (size_t at = 0; at < size; at++)
    {
        QuboCompiler quboCompiler;
        quboCompiler.compile(*dynamic_pointer_cast<QcellOp>(naryOp[at]));
        Qubo qubo = quboCompiler.qubo();
        QuboAnalyzer anlzr(qubo);
        size_t ndsNo = anlzr.nodesNo();
        nodeCnt += ndsNo;
        if(nodeCnt > mMaxNoNodes)
        {
            mQubos.push_back(subQubo);
            subQubo.clear();
            nodeCnt = ndsNo;
        }
        subQubo += qubo;
    }
    mQubos.push_back(subQubo);
}
