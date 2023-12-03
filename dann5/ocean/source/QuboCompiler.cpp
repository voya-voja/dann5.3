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

Qubo QuboCompiler::parse(const Qop& op) const
{
    const QnullCellOp* pNullOp = dynamic_cast<const QnullCellOp*>(&op);
    if(pNullOp != nullptr)
        return Qubo();
    const Qaddition::Carry* pCarry = dynamic_cast<const Qaddition::Carry*>(&op);
    if(pCarry != nullptr)
        return Qubo();
    const QnaryOp* pNaryOp = dynamic_cast<const QnaryOp*>(&op);
    if(pNaryOp != nullptr)
        return compile(pNaryOp);
    return qubo(op);
}

Qubo QuboCompiler::qubo(const Qop& op) const
{
    Qdefs ins = op.inputs();
    size_t iSize = ins.size();
    Qubo aQubo;
    QuboTable::IoPorts ports;
    // process input ports
    for (size_t at = 0; at < iSize; at++)
    {
        // use names of argument and this operands as
        // unique string describing inputs
        Qdef::Sp pIn = ins[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pIn);
        if (pOprnd == nullptr)
            throw logic_error("ERROR @QuboCompiler: Input '" + pIn->id()
                                                        + "' is not a Qcell.");
        Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOprnd);
        if (pOp != nullptr)
        {
            // add sub-qubo from argument operand
            QuboCompiler compiler;
            pOp->compile(compiler);
            aQubo += compiler.qubo();
            Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(pOp->output());
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
            throw logic_error("ERROR @QuboCompiler: Output '" + pOut->id()
                                                        + "' is not a Qcell.");
        Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOprnd);
        if (pOp != nullptr)
        {
            // add sub-qubo from argument operand
            QuboCompiler compiler;
            pOp->compile(compiler);
            aQubo += compiler.qubo();
            Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(pOp->output());
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

Qubo QuboCompiler::compile(const QnaryOp* pOp) const
{
    Qubo qubo;
    const Qcells& logic = pOp->cells();
    size_t size = pOp->noqbs();
    QuboCompiler compiler;
    for (size_t atCell = 0; atCell < size; atCell++)
    {
        Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[atCell]);
        if (pCellOp != nullptr)
        {
            pCellOp->compile(compiler);
            qubo += compiler.qubo();
            compiler.reset();
        }
        else
            throw logic_error("Error@QnaryOp: The cell is not an operation");
    }
    return qubo;
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
