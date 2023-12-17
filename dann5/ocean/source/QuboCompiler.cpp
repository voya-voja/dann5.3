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
#include <QnaryOp.h>
#include <QuboAnalyzer.h>

using namespace dann5;
using namespace dann5::ocean;

/**** Qubo Compiler ****/
void QuboCompiler::compile(const Qop& op)
{
    parse(op);
}

void QuboCompiler::parse(const Qop& op)
{
    const QcellOp* pCellOp = dynamic_cast<const QcellOp*>(&op);
    if(pCellOp != nullptr)
        parse(pCellOp);
    else
    {
        const QnaryOp* pNaryOp = dynamic_cast<const QnaryOp*>(&op);
        if(pNaryOp != nullptr)
            compile(pNaryOp);
        else
            throw logic_error(string("ERROR @QuboCompiler: '")
                  + typeid(op).name() + "' unknown operation to be compiled!");
    }
}

void QuboCompiler::parse(const QcellOp* pCellOp)
{
    const QnullCellOp* pNullOp = dynamic_cast<
                                            const QnullCellOp*>(pCellOp);
    if(pNullOp != nullptr) return;
    const Qaddition::Carry* pCarry = dynamic_cast<
                                        const Qaddition::Carry*>(pCellOp);
    if(pCarry != nullptr) return;
    
    qubo(pCellOp);
}

void QuboCompiler::qubo(const QcellOp* pCellOp)
{
    const Qdefs& ins = pCellOp->Qop::inputs();
    size_t iSize = ins.size();
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
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pOprnd);
        if (pOp != nullptr)
            ports.push_back(compile(pOp));
        else
            ports.push_back(QuboTable::IoPort(pOprnd->id(), pOprnd->value()));
    }
    // process output ports
    const Qdefs& outs = pCellOp->outputs();
    size_t oSize = outs.size();
    for (size_t at = 0; at < oSize; at++)
    {
        Qdef::Sp pOut = outs[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pOut);
        if (pOprnd == nullptr)
            throw logic_error("ERROR @QuboCompiler: Output '" + pOut->id()
                                                        + "' is not a Qcell.");
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pOprnd);
        if (pOp != nullptr)
                ports.push_back(compile(pOp));
        else
            ports.push_back(QuboTable::IoPort(pOprnd->id(), pOprnd->value()));
    }
    if ((iSize + oSize) > ports.size())
        return;
    // create QuboTable rule object for this operand
    QuboTable::Sp pQubo = QTfactory::Instance().create(pCellOp->identifier());
    mQubo += pQubo->qubo(ports, mFinalized);
}

QuboTable::IoPort QuboCompiler::compile(const QcellOp::Sp& pCellOp)
{
    QcellOp::Sp pOp = pCellOp;
    // compile a sub-operation(s) that is (are) an input argument
    pOp->compile(*this);
    Qcell::Sp pOut = nullptr;
    do { // parse through all compiled operations until finding the
         // operation output that is not another operation
        pOut = dynamic_pointer_cast<Qcell>(pOp->Qop::output());
        pOp = dynamic_pointer_cast<QcellOp>(pOut);
    } while(pOp != nullptr);
    if(pOut == nullptr)
        throw logic_error("ERROR @QuboCompiler: output argument is null poiner!");
    QuboTable::IoPort port(pOut->id(), pOut->value());
    return port;
}


void QuboCompiler::compile(const QnaryOp* pOp)
{
    Qubo qubo;
    const Qcells& logic = pOp->cells();
    size_t size = pOp->noqbs();
    for (size_t atCell = 0; atCell < size; atCell++)
    {
        QcellOp::Sp pCellOp = dynamic_pointer_cast<QcellOp>(logic[atCell]);
        if (pCellOp != nullptr)
            pCellOp->compile(*this);
        else
            throw logic_error("Error@QnaryOp: The cell is not an operation");
    }
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
