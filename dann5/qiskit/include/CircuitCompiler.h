//
//  CircuitCompiler.h
//  d5q
//
//  Created by Nebojsa Vojinovic on 2023-12-30.
//

#ifndef CircuitCompiler_h
#define CircuitCompiler_h
#pragma once

#include <pybind11/pybind11.h>

#include <Qcompiler.h>

#include <Circuits.h>
#include <QnaryOp.h>
#include <QcellOps.h>

namespace dann5{
    namespace qiskit {
        // A specialization of quantum operation compiler that translates the
        // operation definition into QUBO
        class CircuitCompiler : public Qcompiler
        {
            enum Stage {
                cInitialize,
                cCompile,
                cMeasure
            };

        public:
            // Constructor sets the QUBO compiler mode. If finalized, compiles
            // an quantum operation into an executable qubo format with all
            // constant variables substituted by their values and recalcualtes
            // parameters of linear nodes. If not finalized, creates a full
            // qubo translation without substitutions.
            CircuitCompiler() : mStage(cCompile) {};
            
            // Compiles an operation into its QUBO transalation. When the
            // compiler is in finalized mode, creates an executable qubo format
            // with all constant variables substituted by their values and
            // recalcualtes parameters of linear nodes. Otherwise, creates a
            // full qubo translation without substitutions.
            virtual void compile(const Qop&);
            
            // Returns a qubo representation of last compiled quantum operation
            const Circuit& qubo() const { return mCircuit; };
            
            // Resets the compiler into its initial state
            void reset() {
                mCircuit.reset();
                mStage = cInitialize;
            };

            // Returns finalized status mode
            Stage stage() const { return mStage; };
            
            // Changes finalized status mode and resets the compiler object
            void stage(Stage stg) {
                mStage = stg;
                reset();
            };

            D5circuit& circuit() { return mCircuit; };

        protected:
            // Updates circuit instructions as a translation of parsed quantum
            // operation
            void parse(const Qop&);
            
            // Updates circuit instructions as a translation of parsed quantum
            // cell operation
            // The provided operation should be a QcellOp, otherwise it throws
            // logical_error exception.
            void parse(const QcellOp*);

            // Compiles quantum cell sub-operations to generate circuit
            // instructions
            // Returns ashared pointer to an output cell of compiled cell
            // operation
            Qcell::Sp compile(const QcellOp::Sp&);

            // Compiles a Qnary operation to generate circuit instructions
            void compile(const QnaryOp*);
            
        private:

            D5circuit mCircuit;           // Compiled Circuit
            size_t    mOpCount = 0;       // Counter of operations on the stack
            Stage     mStage = cCompile;  // Compiler's stage
        };
    
        // A specialization of quantum operation compiler that translates the
        // operation definition into an array of inerdependent QUBOs
        class CircuitsCompiler : public Qcompiler
        {
        public:
            // Constructor sets the maximal number of nodes that any QUBO in
            // the array can contain.
            CircuitsCompiler(size_t maxNoNodes) : mMaxNoNodes(maxNoNodes) {};
            
            // Compiles an operation into its QUBOs transalation where each
            // QUBO has less or equal number of nodes as specified by the
            // maximal number of nodes.
            virtual void compile(const Qop&);
            
            // Returns a QUBOs representation of last compiled quantum
            // operation
            const Circuits& qubos() const { return mCircuits; };
            
            // Resets the compiler into its initial state
            void reset() { mCircuits.clear(); };

            // Returns the maximal number of nodes per QUBO in the list
            size_t maxNoNodes() const { return mMaxNoNodes; };
            
            // Changes the maximal number of nodes per QUBO in the list and
            // rest the compiler object
            void maxNoNodes(size_t maxNoNodes) {
                mMaxNoNodes = maxNoNodes;
                reset();
            };
            
        protected:            
        private:
            Circuits mCircuits;       // Compiled array of QUBO objects
            size_t mMaxNoNodes; // The maximum of number of nodes in any QUBO
        };
    };
};
#endif /* CircuitCompiler_h */
