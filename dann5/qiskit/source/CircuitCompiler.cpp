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

/**** Circuit Compiler ****/
void CircuitCompiler::compile(const Qop& op)
{
    mOpCount++;
    if (mStage == cCompile)
    {
        mCircuit.declare(op);
        parse(op);
        mStage = cMeasure;
    }
    mOpCount--;
    if (mOpCount == 0 && mStage == cMeasure)
    {
        mCircuit.Circuit::instructions() = mCircuit.initialize() + mCircuit.Circuit::instructions();
        mCircuit.measure();
        mStage = cCompile;
    }
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
    
    const Qdefs& ins = pCellOp->Qop::inputs();
    size_t iSize = ins.size();
    Qubits arguments;
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
            pOprnd = compile(pOp);
        arguments.push_back(mCircuit.input(pOprnd));
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
            pOprnd = compile(pOp);
        arguments.push_back(mCircuit.output(pOprnd));
    }
    if ((iSize + oSize) > arguments.size())
        return;
    // create Circuit rule object for this operand
    Circuit::Sp pCircuit = CircuitFactory::Instance().create(pCellOp->identifier());
    mCircuit.Circuit::instructions() += pCircuit->instructions(arguments);

}

Qcell::Sp CircuitCompiler::compile(const QcellOp::Sp& pCellOp)
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
    return pOut;
}


void CircuitCompiler::compile(const QnaryOp* pOp)
{
    Circuits qubo;
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

/**** Circuits Compiler ****/
void CircuitsCompiler::compile(const Qop& op)
{
    const QnaryOp& naryOp = dynamic_cast<const QnaryOp&>(op);
    size_t size = op.noqbs();
    size_t nodeCnt = 0;
    Circuit::Sp pCircuit;
    for (size_t at = 0; at < size; at++)
    {
        CircuitCompiler compiler;
        compiler.compile(*dynamic_pointer_cast<QcellOp>(naryOp[at]));
        D5circuit& circuit = compiler.circuit();
        size_t ndsNo = circuit.nodesNo();
        nodeCnt += ndsNo;
        pCircuit = Circuit::Sp(new D5circuit(circuit));
        if (nodeCnt > mMaxNoNodes)
        {
            mCircuits.push_back(pCircuit);
            nodeCnt = ndsNo;
        }
             
    }
    mCircuits.push_back(pCircuit);
}
