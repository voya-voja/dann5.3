//
//  Circuits.h
//  d5q
//
//  Created by Nebojsa Vojinovic on 2023-12-29.
//

#ifndef Circuits_h
#define Circuits_h
#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <map>
#include <vector>
#include <iostream>

#include <Qvalue.h>
#include <Factory.h>

using namespace std;

namespace dann5 {
    namespace qiskit {
        
        // An abstraction of a logical circuit predefined using Qiskit
        // components
        class Circuit
        {
        public:
            // a shared pointer to a specific logical circuit
            typedef shared_ptr<Circuit> Sp;

            // A circuit's input-output port
            typedef pair<string, size_t> Port;

            struct IoPort : public Port{
                IoPort(string name, size_t at = 0) : Port(name, at) {};

                IoPort(const Port& port) : Port(port) {};

                IoPort(const IoPort& right) : Port(right) {};

                const string& name() const { return first; };
                void name(const string& n) { first = n; };

                size_t at() const { return second; };
                void at(size_t position) { second = position; };
            };

            // A list of circuit's input-output ports
            // the list should be ordered as per a circuite specific logic 
            typedef vector<IoPort> IoPorts;

            // Defines a qiskit quantum-register coresponding element
            typedef pair<size_t, string> QuantumRegister;

            struct QuReg {
                size_t mNumQubits = 0;
                string mName;

                QuReg(size_t num_qubits, const string& name)
                    : mNumQubits(num_qubits), mName(name) {};

                QuReg(const QuantumRegister& quReg)
                    : mNumQubits(quReg.first), mName(quReg.second) {};

                operator QuantumRegister() {
                    return QuantumRegister(mNumQubits, mName);
                };
            };

            // Defines a qiskit quantum-bit coresponding element
            typedef pair<QuantumRegister, size_t> Qubit;

            struct QuantumBit {
                QuReg mRegister;
                size_t mAt;

                QuantumBit(const QuReg& reg, size_t at)
                    : mRegister(reg), mAt(at) {};

                QuantumBit(const Qubit& quBit)
                    : mRegister(quBit.first), mAt(quBit.second) {};

                operator Qubit() {
                    return Qubit(mRegister, mAt);
                };
            };

            // A list of qiskit quantum-bits
            typedef vector<Qubit> Qubits;

            // Defines a qiskit control-register coresponding element
            typedef pair<size_t, string> ClassicalRegister;

            struct ClReg {
                size_t mNumClbits = 0;
                string mName;

                ClReg(size_t num_qubits, const string& name)
                    : mNumClbits(num_qubits), mName(name) {};

                ClReg(const ClassicalRegister& cntrlReg)
                    : mNumClbits(cntrlReg.first), mName(cntrlReg.second) {};

                operator ClassicalRegister() {
                    return ClassicalRegister(mNumClbits, mName);
                };
            };

            // Defines a qiskit control-bit coresponding element
            typedef pair<ClassicalRegister, size_t> Clbit;

            struct ControllBit {
                ClReg mRegister;
                size_t mAt;

                ControllBit(const ClReg& reg, size_t at)
                    : mRegister(reg), mAt(at) {};

                ControllBit(const Clbit& clBit)
                    : mRegister(clBit.first), mAt(clBit.second) {};

                operator Clbit() {
                    return Clbit(mRegister, mAt);
                };
            };

            // A list of qiskit control-bits
            typedef vector<Clbit> Clbits;

            // Defines a circuit instruction
            struct Instruction {
                string mName;
                Qubits mQubits;
                Clbits mClbits;

                Instruction(const string& name, const Qubits& qubits = {},
                                                    const Clbits& clbits = {})
                    : mName(name), mQubits(qubits), mClbits(clbits) {};

                size_t num_qubits() const { return mQubits.size(); };
                size_t num_clbits() const { return mClbits.size(); }
            };

            // A list of qiskit circuit instructions
            typedef vector<Instruction> Instructions;

            // Expects a list of argument names to be provided in order specified by particual
            // implementation, which is controled by overriding format(...) method.
            // Returns a circuit object based on this circuit rules using provided argument list
            // if finalized, it will return a circuit with substitued deterministic ports based
            // on their value, i.e. for port value 0, its elements will be a QuantumRegister with a "reset" and "measure" gates, and for
            // port with value 1, it will be initialized as value 0 followed by "NOT" gate.
            // A Qbit in a superposition state, e.g. a/S/, will be initialized using "hadamard" gate
            Circuit::Sp circuit(const IoPorts& ports) const;

            // override to draw the circuit
            virtual string draw() const;

            // Sends a representation of Circuit into output stream
            friend std::ostream& operator << (std::ostream&, const Circuit&);

        protected:
            // construct default circuit object with instructions
            Circuit(const Instructions& instructions)
                : mInstructions(instructions) {};

            // construct default circuit object with instructions
            Circuit(const Circuit& right)
                : mInstructions(right.mInstructions) {};

            // returns reference to list of instructions of this circuit
            Instructions& instructions() { return mInstructions; };

        private:
            Instructions    mInstructions;	// Default circuite instructions
        };

        // Sends a representation of Circuit into output stream
        std::ostream& operator << (std::ostream&, const Circuit&);

        // declaration of Qubo Table Factory
        typedef Factory<string, Circuit> CircuitFactory;

        // A list of qiskit circuit instructions
        typedef vector<Circuit::Sp> Circuits;

        // Qiskit measurement circuit is a specialization of a Circuit
        class MeasureCircuit : public Circuit
        {
        public:
            // The qiskit measure gate (M)
            static Instruction Measure(const IoPort& arg = IoPort("a"), const IoPort& cl = IoPort("cl"));

            MeasureCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Qiskit hadamard circuit is a specialization of a Circuit
        class HadamardCircuit : public Circuit
        {
        public:
            // The qiskit hadamard gate (H)
            static Instruction Hadamard(const IoPort& arg = IoPort("a"));

            HadamardCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Qiskit's reset specialization of operator circuit
        class ResetCircuit : public Circuit
        {
        public:
            // The qiskit reset gate
            static Instruction Reset(const IoPort& arg = IoPort("a") );

            ResetCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Invert operator specialization of Circuit defined by invert logic of
        // qiskit NOT gate
        class InvertCircuit : public Circuit
        {
        public:
            // The qiskit NOT gate (X)
            static Instruction Not(const IoPort& arg = IoPort("a"));

            InvertCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Operator specialization of Circuit has one input and one output port
        class OperatorCircuit : public Circuit
        {
        public:
            // The qiskit controlled-NOT gate (CX)
            static Instruction ControlledNot(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );

            // The qiskit swap gate
            static Instruction Swap(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );

        protected:
            OperatorCircuit(const Instructions& insructions)
                : Circuit(insructions) {};
        private:
        };

        // Binary operation specialization of Circuit has two input and one
        // output port
        class BinaryOpCircuit : public Circuit
        {
        public:
            // The qiskit Toffoli gate, also known as the double controlled-NOT gate (CCX)
            static Instruction Toffoli(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );

            static Instruction Toffoli(const IoPort& in0, const IoPort& in1, const IoPort& out);

        protected:
            BinaryOpCircuit(const Instructions& insructions)
                : Circuit(insructions) {};
        private:
        };

        // Binary operation specialization of Circuit has two input and two
        // output ports
        class BinaryOp2OutCircuit : public Circuit
        {
        public:
        protected:
            BinaryOp2OutCircuit(const Instructions& insructions)
                : Circuit(insructions) {};
        private:
        };

        // Equal operator specialization of Operator Circuit defined by equal
        // logic of qiskit controlled-NOT gate
        class EqCircuit : public OperatorCircuit
        {
        public:
            EqCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Not equal operator specialization of Operator Circuit defined by
        // qiskit controlled-not and not gates
        class NeqCircuit : public OperatorCircuit
        {
        public:
            NeqCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Less than operator specialization of Operator Circuit defined by
        // qiskit swap, reset, not and controlled-not gates
        class LtCircuit : public OperatorCircuit
        {
        public:
            LtCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Less eqal operator specialization of Operator Circuit defined by
        // qiskit swap and controlled-not gates
        class LeCircuit : public OperatorCircuit
        {
        public:
            LeCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Greater-than operator specialization of Operator Circuit defined by
        // qiskit not, swap, controlled-not and reset gates
        class GtCircuit : public OperatorCircuit
        {
        public:
            GtCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Greater-eqal operator specialization of Operator Circuit defined by
        // qiskit not, swap and controlled-not gates
        class GeCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            GeCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // And specialization of Binary Operation Circuit defined by
        // qiskit Toffoli gate, also known as the double controlled-NOT gate
        class AndCircuit : public BinaryOpCircuit
        {
        public:
            AndCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Not and specialization of Binary Operation Circuit defined by
        // qiskit not and toffoli gates
        class NandCircuit : public BinaryOpCircuit
        {
        public:
            NandCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Or specialization of Binary Operation Circuit defined by
        // qiskit Toffoli and controlled-not gates
        class OrCircuit : public BinaryOpCircuit
        {
        public:
            OrCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Not or specialization of Binary Operation Circuit defined by
        // qiskit Toffoli, controlled-not and not gates
        class NorCircuit : public BinaryOpCircuit
        {
        public:
            NorCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Xor specialization of Binary Operation Circuit defined by
        // qiskit controlled-not gates
        class XorCircuit : public BinaryOpCircuit
        {
        public:
            XorCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Not xor specialization of Binary Operation Circuit defined by
        // qiskit controlled-not and not gates
        class NxorCircuit : public BinaryOpCircuit
        {
        public:
            NxorCircuit();

            virtual string draw() const;

        protected:
        private:
        };

        // Half adder is a specialization of Binary Operation with 2 outputs,
        // o(ut) & c(arry), defined by qiskit controlled-not and Toffoli gates
        class HalfAdder : public BinaryOp2OutCircuit
        {
        public:
            HalfAdder();

            virtual string draw() const;

        protected:
        private:
        };

        // Adder is trinary operation specialization of Circuit with two outputs,
        // o(ut) & c(arry), defined by qiskit controlled-not and Toffoli gates
        class AdderCircuit : public Circuit
        {
        public:
            AdderCircuit();

            virtual string draw() const;

        protected:
        private:
        };
    };
};

#endif /* Circuits_h */