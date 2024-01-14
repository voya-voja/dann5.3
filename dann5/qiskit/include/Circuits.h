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

#include <Factory.h>
#include <Qvalue.h>
#include <Qop.h>
#include <Qcell.h>

using namespace std;

namespace dann5 {
    namespace qiskit {
        // Defines a qiskit quantum-register with its size
        typedef pair<size_t, string> QuantumRegister;

        // QuantumRegister wraper structure 
        struct QuReg {
            size_t mNumQubits = 0;  // register size
            string mName;           // register name

            // default constructor creates 0 size instance with an empty name
            QuReg() : mNumQubits(0), mName("") {};

            // Construct an instance by specifying size and name of a quantum
            // register
            QuReg(size_t num_qubits, const string& name)
                : mNumQubits(num_qubits), mName(name) {};

            // Construct an instance of a wraper for the quantum register
            QuReg(const QuantumRegister& quReg)
                : mNumQubits(quReg.first), mName(quReg.second) {};

            // returns a QuantumRegister instance with same size and name
            operator QuantumRegister() {
                return QuantumRegister(mNumQubits, mName);
            };

            // a string representation of a quantum register
            operator string() {
                return "QuantumRegister(" + to_string(mNumQubits) 
                                                        + ", '" + mName + "')";
            };
        };

        // A list of qiskit quantum registers
        typedef vector<QuantumRegister> QuantumRegisters;

        // A map of qiskit quantum registers
        typedef map<string, QuantumRegister> QuRegMap;

        // Defines a qiskit quantum-bit coresponding element with its position
        // within a quantum register
        typedef pair<QuantumRegister, size_t> Qubit;

        // Qu(antum)-Bit wraper structure 
        struct QuantumBit {
            QuReg mRegister;    // a quantum register
            size_t mAt;         // a position within the register

            // default constructor creates an instance with an default quantum 
            // register and position set to 0
            QuantumBit() : mRegister(), mAt(0) {};

            // Construct an instance by specifying a quantum register and 
            // the position of the quantum bit within the register
            QuantumBit(const QuReg& reg, size_t at)
                : mRegister(reg), mAt(at) {};

            // Construct an instance of a wraper for the quantum bit
            QuantumBit(const Qubit& quBit)
                : mRegister(quBit.first), mAt(quBit.second) {};

            // returns a Qubit instance with the same register and the same
            // position
            operator Qubit() {
                return Qubit(mRegister, mAt);
            };

            // a string representation of a quantum bit
            operator string() {
                return "Qubit(" + string(mRegister) + ", " 
                                                    + to_string(mAt) + ")";
            };
        };

        // A list of qiskit qu(antum)-bits
        typedef vector<Qubit> Qubits;

        // Defines a qiskit cl(assical)-register coresponding element
        typedef pair<size_t, string> ClassicalRegister;

        // ClassicRegister wraper structure 
        struct ClReg {
            size_t mNumClbits = 0;  // register size
            string mName;           // register name

            // default constructor creates 0 size instance with an empty name
            ClReg() : mNumClbits(0), mName("") {};

            // Construct an instance by specifying size and name of a classical
            // register
            ClReg(size_t num_qubits, const string& name)
                : mNumClbits(num_qubits), mName(name) {};

            // Construct an instance of a wraper for the classical register
            ClReg(const ClassicalRegister& cntrlReg)
                : mNumClbits(cntrlReg.first), mName(cntrlReg.second) {};

            // returns a ClassicalRegister instance with same size and name
            operator ClassicalRegister() {
                return ClassicalRegister(mNumClbits, mName);
            };

            // a string representation of a classical register
            operator string() {
                return "ClassicalRegister(" + to_string(mNumClbits)
                                                        + ", '" + mName + "')";
            };
        };

        // A list of qiskit classical registers
        typedef vector<ClassicalRegister> ClassicalRegisters;

        // Defines a qiskit cl(assical)-bit coresponding element
        typedef pair<ClassicalRegister, size_t> Clbit;

        // Cl(assical)-Bit wraper structure 
        struct ClassicalBit {
            ClReg mRegister;    // a classical register
            size_t mAt;         // a position within the register

            // default constructor creates an instance with an default classical 
            // register and position set to 0
            ClassicalBit() : mRegister(), mAt(0) {};

            // Construct an instance by specifying a classical register and 
            // the position of the quantum bit within the register
            ClassicalBit(const ClReg& reg, size_t at)
                : mRegister(reg), mAt(at) {};

            // Construct an instance of a wraper for the classical bit
            ClassicalBit(const Clbit& clBit)
                : mRegister(clBit.first), mAt(clBit.second) {};

            // returns a Clbit instance with the same register and the same
            // position
            operator Clbit() {
                return Clbit(mRegister, mAt);
            };

            // a string representation of a classical bit
            operator string() {
                return "Clbit(" + string(mRegister) + ", " 
                                                    + to_string(mAt) + ")";
            };
        };

        // A list of qiskit cl(assical)-bits
        typedef vector<Clbit> Clbits;

        // Qiskit instruction identifies qiskit gate and a list of its
        // arguments as qiskit qu(antum)-bits or cl(asical)-bits
        class Instruction {
        public:
            // a shared pointer to a specific qiskit instruction
            typedef shared_ptr<Instruction> Sp;

            // Construct the instruction with a name corresponding to a name
            // of one of qiskit gates: measure, swap, h, reset, x, cx, or ccx.
            // lists of instruction's qu(antum)-bits and cl(assical)-bits are
            // optional
            Instruction(const string& name, const Qubits& qubits = {},
                const Clbits& clbits = {})
                : mName(name), mQubits(qubits), mClbits(clbits) {};

            // returns an instruction name
            const string& name() const { return mName; };

            // sets an instruction name
            void name(const string& nm) { mName = nm; };

            // returns a list of instruction's qu(antum)-bits
            const Qubits& qubits() const { return mQubits; };

            // sets a list of instruction's qu(antum)-bits
            void qubits(const Qubits& qbts) { mQubits = qbts; };

            // returns the number of instruction's qu(antum)-bits
            size_t num_qubits() const { return mQubits.size(); };

            // returns a list of instruction's cl(assical)-bits
            const Clbits& clbits() const { return mClbits; };

            // sets a list of instruction's cl(assical)-bits
            void clbits(const Clbits& clbts) { mClbits = clbts; };

            // appends the qu(antum)-bits to the existing list, and optionally
            // appends the cl(assical)-bits to the existing list
            void append(const Qubits& qbts, const Clbits& clbts = {});

            // returns the number of instruction's cl(assical)-bits
            size_t num_clbits() const { return mClbits.size(); };

            // returns a string representation of the instruction
            operator string() const;

            // Sends a representation of Instruction into an output stream
            friend ostream& operator<< (ostream&, const Instruction&);

        protected:
        private:
            string mName;   // qiskit gate
            Qubits mQubits; // list of qubit arguments
            Clbits mClbits; // list of clbit arguments
        };

        // Sends a representation of Instruction into an output stream
        ostream& operator<< (ostream& out, const Instruction& right);

        // Qiskit instructions is a list (vector) of a qiskit gates against 
        // qiskit qubits
        typedef vector<Instruction> Instructions;

        // Returns a reference of an updated left Instructions operand with 
        // appended elements of right Instructions object.
        Instructions& operator+=(Instructions& left, const Instructions& right);

        // Returns Instructions that are reslut of appending elements of the
        // right to the left Instructions operand 
        Instructions operator+(const Instructions& left, 
                                                    const Instructions& right);

        // Sends a representation of Instructions into an output stream
        ostream& operator << (ostream&, const Instructions&);
        
        // An abstraction of a logical circuit predefined using Qiskit
        // instructions and quantum
        class Circuit
        {
        public:
            // a shared pointer to a specific logical circuit
            typedef shared_ptr<Circuit> Sp;

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const = 0;

            // override to draw the circuit
            virtual string draw() const = 0;

            // Sends a representation of Circuit into output stream
            friend std::ostream& operator << (std::ostream&, const Circuit&);

            // returns reference to list of instructions of this circuit
            Instructions& instructions() { return mInstructions; };

            // returns reference to list of instructions of this circuit
            const Instructions& instructions() const { return mInstructions; };

        protected:
            // default construct with empty instructions
            Circuit() : mInstructions({}) {};

            // construct default circuit object with instructions
            Circuit(const Circuit& right)
                : mInstructions(right.mInstructions) {};

        private:
            Instructions    mInstructions;	// Default circuite instructions
        };

        // Sends a representation of Circuit into output stream
        std::ostream& operator << (std::ostream&, const Circuit&);

        // declaration of Qubo Table Factory
        typedef Factory<string, Circuit> CircuitFactory;

        // A list of qiskit circuit instructions
        typedef vector<Circuit::Sp> Circuits;

        // Returns a reference of an updated left Circuits operand with 
        // appended elements of right Circuits object.
        Circuits& operator+=(Circuits& left, const Circuits& right);

        // Returns Circuits that are reslut of addition of left and right
        // Circuits operands
        Circuits operator+(const Circuits& left, const Circuits& right);

        // Sends a representation of Qubo into output stream
        ostream& operator << (ostream&, const Circuits&);

        // Qiskit measurement circuit is a specialization of a Circuit
        class MeasureCircuit : public Circuit
        {
        public:
            // The qiskit measure gate (M)
//            static Instruction Measure(const IoPort& arg = IoPort("a"), const IoPort& cl = IoPort("cl"));
            static Instruction Measure(const Qubit& arg, const Clbit& cl);

            MeasureCircuit();

            virtual string draw() const;

        protected:
            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

        private:
        };

        // Qiskit hadamard circuit is a specialization of a Circuit
        class HadamardCircuit : public Circuit
        {
        public:
            // The qiskit hadamard gate (H)
//            static Instruction Hadamard(const IoPort& arg = IoPort("a"));
            static Instruction Hadamard(const Qubit& arg);

            HadamardCircuit();

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

            virtual string draw() const;

        protected:
        private:
        };

        // Qiskit's reset specialization of operator circuit
        class ResetCircuit : public Circuit
        {
        public:
            // The qiskit reset gate
//            static Instruction Reset(const IoPort& arg = IoPort("a") );
            static Instruction Reset(const Qubit& arg);

            ResetCircuit();

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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
//            static Instruction Not(const IoPort& arg = IoPort("a"));
            static Instruction Not(const Qubit& arg);

            InvertCircuit();

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

            virtual string draw() const;

        protected:
        private:
        };

        // Operator specialization of Circuit has one input and one output port
        class OperatorCircuit : public Circuit
        {
        public:
            // The qiskit controlled-NOT gate (CX)
//            static Instruction ControlledNot(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );
            static Instruction ControlledNot(const Qubit& in, const Qubit& out);

            // The qiskit swap gate
//            static Instruction Swap(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );
            static Instruction Swap(const Qubit& in, const Qubit& out);

        protected:
            OperatorCircuit() : Circuit() {};

        private:
        };

        // Binary operation specialization of Circuit has two input and one
        // output port
        class BinaryOpCircuit : public Circuit
        {
        public:
            // The qiskit Toffoli gate, also known as the double controlled-NOT gate (CCX)
//            static Instruction Toffoli(const IoPort& in = IoPort("i"), const IoPort& out = IoPort("o") );

//            static Instruction Toffoli(const IoPort& in0, const IoPort& in1, const IoPort& out);
            static Instruction Toffoli(const Qubit& in0, const Qubit& in1, const Qubit& out);

        protected:
            BinaryOpCircuit() : Circuit() {};

        private:
        };

        // Binary operation specialization of Circuit has two input and two
        // output ports
        class BinaryOp2OutCircuit : public Circuit
        {
        public:
        protected:
            BinaryOp2OutCircuit() : Circuit() {};

        private:
        };

        // Equal operator specialization of Operator Circuit defined by equal
        // logic of qiskit controlled-NOT gate
        class EqCircuit : public OperatorCircuit
        {
        public:
            EqCircuit();

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

            virtual string draw() const;

        protected:
        private:
        };

        // Half adder is a specialization of Binary Operation with 2 outputs,
        // o(ut) & c(arry), defined by qiskit controlled-not and Toffoli gates
        class HalfAdderCircuit : public BinaryOp2OutCircuit
        {
        public:
            HalfAdderCircuit();

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

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

            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

            virtual string draw() const;

        protected:
        private:
        };

        // dann5 circuit is a custom defined circuit
        class D5circuit : public Circuit
        {
        public:

            typedef pair<Qubit, Qvalue>      QuOperand;
            typedef map<string, QuOperand>   QuOperandsMap;

            // default constructor
            D5circuit();
            // copy constructor
            D5circuit(const D5circuit&);

            // Declare circuit's quantum registers
            void declare(const Qop&);

            // Return initialization instructions for qubits in the init list
            Instructions initialize() const;

            // Adds an quantum-cell operand into the list of input quantum 
            // registers, all qubit operands and init qubit operands
            QuantumBit input(const Qcell::Sp&);
            // Adds an quantum-cell operand into the list of input quantum 
            // registers, all qubit operands
            QuantumBit output(const Qcell::Sp&);

            // Returns a list of the circuit's input quantum registers
            const QuRegMap& inputs() const { return mIns; };
            // Sets a list of the circuit's input quantum registers
            void inputs(const QuRegMap& ins) { mIns = ins; };

            // Returns a list of the circuit's output quantum registers
            const QuRegMap& outputs() const { return mOuts; };
            // Sets a list of the circuit's output quantum registers
            void outputs(const QuRegMap& outs) { mOuts = outs; };

            // Returns a list of the circuit's operands.
            const QuOperandsMap& operands() const { return mOperands; };
            // Sets a list of the circuit's operands.
            void operands(const QuOperandsMap& oprnds) { mOperands = oprnds; };

            // Returns a list of the circuit's operands requiring initialization
            const QuOperandsMap& initOperands() const { return mInitOperands; };
            // Sets a list of the circuit's operands requiring initialization
            void initOperands(const QuOperandsMap& oprnds) 
                                                    { mInitOperands = oprnds; };

            // Appends measure instructions for all operands to the circuit's
            // list of instructions
            void measure();

            // number of logical quantum nodes required for this circuit
            size_t nodesNo() const;

            // Draws a circuit using ASCII characters
            virtual string draw() const;

            // Resets a circuit into initial state
            virtual void reset() {
                mIns.clear();
                mOuts.clear();
                mOperands.clear();
                Circuit::instructions().clear();
            };

        protected:
            // Returns a instructions based on this circuit rules using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions instructions(const Qubits& arguments) const;

        private:
            QuRegMap            mIns,
                                mOuts;
            QuOperandsMap       mOperands, 
                                mInitOperands;
        };
    };
};

#endif /* Circuits_h */