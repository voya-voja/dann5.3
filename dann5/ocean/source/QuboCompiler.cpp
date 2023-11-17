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
#include <QderivedOps.h>

using namespace dann5;
using namespace dann5::ocean;

void QuboCompiler::compile(const Qop& op)
{
    /*
            // Override to return a qubo representation of this Q statement,
            // if not finalized, return a full qubo without substitutions
            // if finalized, return a qubo with all constant variables
            // substituted by their values
            // when applicable, if forBit is specified (differs from cAllBits),
            // return qubo only  for the specified bit level
            virtual Qubo qubo(bool finalized = true,
                                size_t forBit = cAllBits) const = 0;

            // Override to return a Qubos vector containing qubo representation
            // of this Q statement fractionate into sub-qubos with no more than
            // specified number of nodes
            virtual Qubos qubos(size_t noNodes) const = 0;
     
     Qubo Qblock::qubo(bool finalized, size_t forBit) const
     {
         Qubo qubo;
         for (auto pStatement : mStatements)
         {
             qubo += pStatement->qubo(finalized, forBit);
         }
         return(qubo);
     }

     Qubos Qblock::qubos(size_t noNodes) const
     {
         Qubos qubos;
         for (auto pStatement : mStatements)
         {
             qubos += pStatement->qubos(noNodes);
         }
         return(qubos);
     }

     Qevaluations Qblock::compute()
     {
         if (noSolutions() != 0)
             reset();
         Qubo qubo;
         for (auto pStatement : mStatements)
         {
             qubo += pStatement->qubo();
         }
         D5QuboSolver solver(qubo);
         Qevaluations evltns = solver.solution();
         return evltns;
     }
     
     Qubo Qassignment::qubo(bool finalized, size_t forBit) const
     {
         if (mpExpr == nullptr)
             return Qubo();
         return(mpExpr->qubo(finalized, forBit));
     }

     Qubos Qassignment::qubos(size_t noNodes) const
     {
         if (mpExpr == nullptr)
             return Qubos();
         return(mpExpr->qubos(noNodes));
     }
     
     Qubo Qexpression::qubo(bool finalized, size_t forBit) const
     {
         if (forBit != cAllBits) return mpRoot->qubo(finalized, forBit);

         size_t size = mpRoot->noqbs();
         Qubo aQubo;
         for (size_t at = 0; at < size; at++)
         {
             aQubo += mpRoot->qubo(finalized, at);
         }
         return aQubo;
     }

     Qubos Qexpression::qubos(size_t noNodes) const
     {
         size_t size = mpRoot->noqbs();
         Qubos qubos;
         size_t nodeCnt = 0;
         Qubo subQubo;
         for (size_t at = 0; at < size; at++)
         {
             Qubo qubo = mpRoot->qubo(true, at);
             Qanalyzer anlzr(qubo);
             size_t ndsNo = anlzr.nodesNo();
             nodeCnt += ndsNo;
             if(nodeCnt > noNodes)
             {
                 qubos.push_back(subQubo);
                 subQubo.clear();
                 nodeCnt = ndsNo;
             }
             subQubo += qubo;
         }
         qubos.push_back(subQubo);
         return qubos;
     }
     */
    mQubo += parse(op);
}

Qubo QuboCompiler::parse(const Qop& op, size_t forBit) const
{
    const QnullCellOp* pNullOp = dynamic_cast<const QnullCellOp*>(&op);
    if(pNullOp != nullptr)
        return Qubo();
    const QderivedOp* pDerivedOp = dynamic_cast<const QderivedOp*>(&op);
    if(pDerivedOp != nullptr)
        return Qubo();
    const Qaddition::Carry* pCarry = dynamic_cast<const Qaddition::Carry*>(&op);
    if(pCarry != nullptr)
        return Qubo();
    const QnaryOp* pQnaryOp = dynamic_cast<const QnaryOp*>(&op);
    if(pQnaryOp != nullptr)
        return qubo(*pQnaryOp, forBit);
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

Qubo QuboCompiler::qubo(const QnaryOp& op, size_t forBit) const
{
    const Qcells& logic = op.cells();
    size_t size = op.noqbs();
    if (forBit != cAllBits)
    {
        if (forBit >= size)
            return Qubo();    // returns empty qubo
        Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[forBit]);
        if (pCellOp != nullptr)
            return parse(*pCellOp, forBit);
        else
            throw logic_error("Error@QnaryOp: The cell is not an operation");
    }
    Qubo aQubo;
    for (size_t atCell = 0; atCell < size; atCell++)
    {
        Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[atCell]);
        if (pCellOp != nullptr)
            aQubo += parse(*pCellOp, forBit);
        else
            throw logic_error("Error@QnaryOp: The cell is not an operation");
    }
    return aQubo;
}
