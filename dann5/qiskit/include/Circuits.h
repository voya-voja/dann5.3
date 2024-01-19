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
            operator QuantumRegister() const {
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
            operator Qubit() const {
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
            operator ClassicalRegister() const {
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
            operator Clbit() const {
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

        // Quantum operand is a Qubit with a value
        typedef pair<Qubit, Qvalue>      QuOperand;

        // A map of quantum operands with a name of quantum register as a key
        typedef map<string, QuOperand>   QuOperandsMap;

        // Returns a reference of an updated left QuOperandsMap operand with 
        // appended elements of right QuOperandsMap object.
        QuOperandsMap& operator+=(QuOperandsMap& left, 
                                                    const QuOperandsMap& right);

        // Returns QuOperandsMap that are reslut of appending elements of the
        // right to the left QuOperandsMap operand 
        QuOperandsMap operator+(const QuOperandsMap& left,
                                                    const QuOperandsMap& right);

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
            virtual Instructions instructions(const Qubits& arguments) {
                mInstructions = create(arguments);
                return mInstructions;
            };

            // Draws the circuit based on specifies operands and instructions
            // using ASCII characters
            virtual string draw() const;

            // returns reference to list of instructions of this circuit
            Instructions& instructions() { return mInstructions; };

            // returns reference to list of instructions of this circuit
            const Instructions& instructions() const { return mInstructions; };

            // Returns a constant reference to a list of the circuit's operands.
            const QuOperandsMap& operands() const { return mOperands; };

            // Returns a reference to a list of the circuit's operands.
            QuOperandsMap& operands() { return mOperands; };

            // Sets a list of the circuit's operands.
            void operands(const QuOperandsMap& oprnds) { mOperands = oprnds; };

            // Resets a circuit into initial state
            virtual void reset() {
                mOperands.clear();
                mInstructions.clear();
            };

            // Sends a representation of Circuit into output stream
            friend std::ostream& operator << (std::ostream&, const Circuit&);

        protected:
            // default construct with empty instructions
            Circuit() : mInstructions({}) {};

            // construct default circuit object with instructions
            Circuit(const Circuit& right)
                : mInstructions(right.mInstructions),
                  mOperands(right.mOperands) {};

            // Override to create and return instructions based on the circuit 
            // rules using provided argument list.
            virtual Instructions create(const Qubits& arguments) const = 0;

        private:
            Instructions    mInstructions;	// Default circuite instructions
            QuOperandsMap       mOperands;
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
            // Create and returns the qiskit measure gate (M) instruction with 
            // provided arg(ument) Qu(antum) bit and cl(assical) bit
            static Instruction Measure(const Qubit& arg, const Clbit& cl);

            // default constructor with default classical bit:
            // ClassicalBit(ClReg(mNumClbits: 1, mName: "cl"), mAt: 0)
            MeasureCircuit();

            // Constructs an instance with the specified clbit
            MeasureCircuit(const ClassicalBit& clbit);

            // draws a default mesure circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list and its classical-bit specified at
            // construction time. It expects a list of arguments with one
            // Qu(autum) bit.
            virtual Instructions create(const Qubits& arguments) const;

        private:
            ClassicalBit mClbit;    // default classical bit to be used
        };

        // Qiskit hadamard circuit is a specialization of a Circuit
        class HadamardCircuit : public Circuit
        {
        public:
            // Create and returns the qiskit hadamar gate (H) instruction with
            // provided arg(ument) Qu(antum) bit
            static Instruction Hadamard(const Qubit& arg);

            // default constructor 
            HadamardCircuit();

            // draws a default hadamard circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one 
            // Qu(autum) bit as an argument to be initilized using hadamard gate
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Qiskit's reset specialization of operator circuit
        class ResetCircuit : public Circuit
        {
        public:
            // Create and returns the qiskit reset gate instruction with 
            // provided arg(ument) Qu(antum) bit
            static Instruction Reset(const Qubit& arg);

            // default constructor 
            ResetCircuit();

            // draws a default reset circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one
            // Qu(autum) bit argument to be set to 0 using reset gate.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Invert operator specialization of Circuit defined by invert logic of
        // qiskit NOT gate
        class InvertCircuit : public Circuit
        {
        public:
            // Create and returns the qiskit NOT gate (X) instruction with 
            // provided arg(ument) Qu(antum) bit
            static Instruction Not(const Qubit& arg);

            // default constructor 
            InvertCircuit();

            // draws a default invert circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one
            // Qu(autum) bit argument to be changed using not gate.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Operator specialization of Circuit has one input and one output port
        class OperatorCircuit : public Circuit
        {
        public:
            // Create and returns the qiskit controlled-NOT gate (CX)
            // instruction with provided in and out Qu(antum) bit
            static Instruction ControlledNot(const Qubit& in, const Qubit& out);

            // Create and returns the qiskit swap gate instruction with provided
            // in and out Qu(antum) bit
            static Instruction Swap(const Qubit& in, const Qubit& out);

        protected:
            // default constructor 
            OperatorCircuit() : Circuit() {};

        private:
        };

        // Equal operator specialization of Operator Circuit defined by equal
        // logic of qiskit controlled-NOT gate
        class EqCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            EqCircuit();

            // draws a default equal operator qiskit circuit if operands
            // were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Not equal operator specialization of Operator Circuit defined by
        // qiskit controlled-not and not gates
        class NeqCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            NeqCircuit();

            // draws a default not-equal operator qiskit circuit if operands
            // were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Less than operator specialization of Operator Circuit defined by
        // qiskit swap, reset, not and controlled-not gates
        class LtCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            LtCircuit();

            // draws a default less-than operator qiskit circuit if operands
            // were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Less eqal operator specialization of Operator Circuit defined by
        // qiskit swap and controlled-not gates
        class LeCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            LeCircuit();

            // draws a default less-than-and-equal operator qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Greater-than operator specialization of Operator Circuit defined by
        // qiskit not, swap, controlled-not and reset gates
        class GtCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            GtCircuit();

            // draws a default great-than operator qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Greater-eqal operator specialization of Operator Circuit defined by
        // qiskit not, swap and controlled-not gates
        class GeCircuit : public OperatorCircuit
        {
        public:
            // default constructor 
            GeCircuit();

            // draws a default great-thanand-equal operator qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with one input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Binary operation specialization of Circuit has two input and one
        // output port
        class BinaryOpCircuit : public Circuit
        {
        public:
            // The qiskit Toffoli gate, also known as the double controlled-NOT
            // gate (CCX)
            static Instruction Toffoli(const Qubit& in0, const Qubit& in1,
                const Qubit& out);

        protected:
            // default constructor 
            BinaryOpCircuit() : Circuit() {};

        private:
        };

        // And specialization of Binary Operation Circuit defined by
        // qiskit Toffoli gate, also known as the double controlled-NOT gate
        class AndCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            AndCircuit();

            // draws a default and binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Not and specialization of Binary Operation Circuit defined by
        // qiskit not and toffoli gates
        class NandCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            NandCircuit();

            // draws a default not-and binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Or specialization of Binary Operation Circuit defined by
        // qiskit Toffoli and controlled-not gates
        class OrCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            OrCircuit();

            // draws a default or binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Not or specialization of Binary Operation Circuit defined by
        // qiskit Toffoli, controlled-not and not gates
        class NorCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            NorCircuit();

            // draws a default not-or binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Xor specialization of Binary Operation Circuit defined by
        // qiskit controlled-not gates
        class XorCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            XorCircuit();

            // draws a default xor binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Not xor specialization of Binary Operation Circuit defined by
        // qiskit controlled-not and not gates
        class NxorCircuit : public BinaryOpCircuit
        {
        public:
            // default constructor 
            NxorCircuit();

            // draws a default not-xor binary-operation qiskit circuit if
            // operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and one output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Binary operation specialization of BinaryOpCircuit has two input and
        // two output ports
        class BinaryOp2OutCircuit : public BinaryOpCircuit
        {
        public:
        protected:
            // default constructor 
            BinaryOp2OutCircuit() : BinaryOpCircuit() {};

        private:
        };

        // Half adder is a specialization of Binary Operation with 2 outputs,
        // o(ut) & c(arry), defined by qiskit controlled-not and Toffoli gates
        class HalfAdderCircuit : public BinaryOp2OutCircuit
        {
        public:
            // default constructor 
            HalfAdderCircuit();

            // draws a default half-adder binary-operation with two outpts
            // qiskit circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with two input
            // and two output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
        };

        // Adder is trinary operation specialization of Circuit with two outputs,
        // o(ut) & c(arry), defined by qiskit controlled-not and Toffoli gates
        class AdderCircuit : public Circuit
        {
        public:
            // default constructor 
            AdderCircuit();

            // draws a default full-adder trinary-operation with two outpts
            // qiskit circuit if operands were not specified
            virtual string draw() const;

        protected:
            // Creates and returns a instructions based on this circuit rules
            // using provided argument list. It expects a list with three input
            // and two output Qu(autum) bit arguments provided in that order.
            virtual Instructions create(const Qubits& arguments) const;

        private:
            static size_t gAuxCounter;

            static string AuxRegName();
        };

        // dann5 circuit is a custom defined circuit
        class D5circuit : public Circuit
        {
        public:
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

            // Resets a circuit into initial state
            virtual void reset() {
                Circuit::reset();
                mIns.clear();
                mOuts.clear();
                mInitOperands.clear();
            };

        protected:
            // Returns a copy of its existing instructions using provided
            // argument list. It expects a list of Qu(autum) bit arguments to be
            // provided in order specified by particual implementation.
            virtual Instructions create(const Qubits& arguments) const;

        private:
            QuRegMap            mIns,
                                mOuts;
            QuOperandsMap       mInitOperands;
        };
    };
};

#endif /* Circuits_h */