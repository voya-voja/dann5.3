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
        public:
            // Constructor sets the QUBO compiler mode. If finalized, compiles
            // an quantum operation into an executable qubo format with all
            // constant variables substituted by their values and recalcualtes
            // parameters of linear nodes. If not finalized, creates a full
            // qubo translation without substitutions.
            CircuitCompiler(bool finalized = true) : mFinalized(finalized) {};
            
            // Compiles an operation into its QUBO transalation. When the
            // compiler is in finalized mode, creates an executable qubo format
            // with all constant variables substituted by their values and
            // recalcualtes parameters of linear nodes. Otherwise, creates a
            // full qubo translation without substitutions.
            virtual void compile(const Qop&);
            
            // Returns a qubo representation of last compiled quantum operation
            const Circuit& qubo() const { return mCircuit; };
            
            // Resets the compiler into its initial state
            void reset() { mCircuit = Circuit(); };

            // Returns finalized status mode
            bool finalized() const { return mFinalized; };
            
            // Changes finalized status mode and resets the compiler object
            void finalized(bool fnlzd) {
                mFinalized = fnlzd;
                reset();
            };

        protected:
            // Returns QUBO object as a translation of parsed quantum operation
            void parse(const Qop&);
            
            // Returns QUBO object as a translation of parsed quantum cell
            // operation
            void parse(const QcellOp*);

            // Returns a QUBO object by translating the provided cell operation.
            // The provided operation should be a QcellOp, otherwise it throws
            // logical_error exception.
            void qubo(const QcellOp*);

            // Compiles quantum cell sub-operations to generate QUBO
            // Returns an IoPort entry for compiled sub-operations
            Circuit::IoPort compile(const QcellOp::Sp&);

            // Compiles this Qnary operation to generate QUBO
            void compile(const QnaryOp*);
            
        private:
            Circuit mCircuit;         // Compiled QUBO
            bool mFinalized;    // Finalized compiler mode
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
