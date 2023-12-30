//
//  CircuitCompiler.cpp
//  d5s
//
//  Created by Nebojsa Vojinovic on 2023-12-30.
//

#include <CircuitCompiler.h>

#include <Qdef.h>
#include <Qop.h>
#include <Qcell.h>
#include <Qnary.h>
#include <QnaryOp.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** Qubo Compiler ****/
void CircuitCompiler::compile(const Qop& op)
{
    parse(op);
}

void CircuitCompiler::parse(const Qop& op)
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
            throw logic_error(string("ERROR @CircuitCompiler: '")
                  + typeid(op).name() + "' unknown operation to be compiled!");
    }
}

void CircuitCompiler::parse(const QcellOp* pCellOp)
{
    const QnullCellOp* pNullOp = dynamic_cast<
                                            const QnullCellOp*>(pCellOp);
    if(pNullOp != nullptr) return;
    const Qaddition::Carry* pCarry = dynamic_cast<
                                        const Qaddition::Carry*>(pCellOp);
    if(pCarry != nullptr) return;
    
    qubo(pCellOp);
}

void CircuitCompiler::qubo(const QcellOp* pCellOp)
{
    const Qdefs& ins = pCellOp->Qop::inputs();
    size_t iSize = ins.size();
    Circuit::IoPorts ports;
    // process input ports
    for (size_t at = 0; at < iSize; at++)
    {
        // use names of argument and this operands as
        // unique string describing inputs
        Qdef::Sp pIn = ins[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pIn);
        if (pOprnd == nullptr)
            throw logic_error("ERROR @CircuitCompiler: Input '" + pIn->id()
                                                        + "' is not a Qcell.");
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pOprnd);
        if (pOp != nullptr)
            ports.push_back(compile(pOp));
        else
            ports.push_back(Circuit::IoPort(pOprnd->id(), pOprnd->value()));
    }
    // process output ports
    const Qdefs& outs = pCellOp->outputs();
    size_t oSize = outs.size();
    for (size_t at = 0; at < oSize; at++)
    {
        Qdef::Sp pOut = outs[at];
        Qcell::Sp pOprnd = dynamic_pointer_cast<Qcell>(pOut);
        if (pOprnd == nullptr)
            throw logic_error("ERROR @CircuitCompiler: Output '" + pOut->id()
                                                        + "' is not a Qcell.");
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pOprnd);
        if (pOp != nullptr)
                ports.push_back(compile(pOp));
        else
            ports.push_back(Circuit::IoPort(pOprnd->id(), pOprnd->value()));
    }
    if ((iSize + oSize) > ports.size())
        return;
    // create Circuit rule object for this operand
    Circuit::Sp pQubo = QTfactory::Instance().create(pCellOp->identifier());
    mQubo += pQubo->qubo(ports, mFinalized);
}

Circuit::IoPort CircuitCompiler::compile(const QcellOp::Sp& pCellOp)
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
        throw logic_error("ERROR @CircuitCompiler: output argument is null poiner!");
    Circuit::IoPort port(pOut->id(), pOut->value());
    return port;
}


void CircuitCompiler::compile(const QnaryOp* pOp)
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
        CircuitCompiler quboCompiler;
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
