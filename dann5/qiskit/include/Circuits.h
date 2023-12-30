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
            struct IoPort {
                string name;
                Qvalue value;

                IoPort() : name(""), value(cSuperposition) {};
                IoPort(string n, Qvalue v) : name(n), value(v) {};
            };

            // A list of circuit's input-output ports
            // the list should be ordered as per a circuite specific logic 
            typedef vector<IoPort> IoPorts;

            // Defines a qiskit quantum-register coresponding element
            struct QuantumRegister {
                size_t mNumQubits = 0;
                string mName;

                QuantumRegister(size_t num_qubits, const string& name)
                    : mNumQubits(num_qubits), mName(name) {};
            };

            // Defines a qiskit quantum-bit coresponding element
            struct Qubit {
                QuantumRegister mRegister;
                size_t mAt;

                Qubit(const QuantumRegister& reg, size_t at)
                    : mRegister(reg), mAt(at) {};
            };

            // A list of qiskit quantum-bits
            typedef vector<Qubit> Qubits;

            // Defines a qiskit control-register coresponding element
            struct ControlRegister {
                size_t mNumClbits = 0;
                string mName;

                ControlRegister(size_t num_qubits, const string& name)
                    : mNumClbits(num_qubits), mName(name) {};
            };

            // Defines a qiskit control-bit coresponding element
            struct Clbit {
                ControlRegister mRegister;
                size_t mAt;

                Clbit(const ControlRegister& reg, size_t at)
                    : mRegister(reg), mAt(at) {};
            };

            // A list of qiskit control-bits
            typedef vector<Clbit> Clbits;

            // Defines a circuit instruction
            struct Instruction {
                string mName;
                Qubits mQubits;
                Clbits mClbits;

                Instruction(const string& name, const Qubits& qubits,
                                                        const Clbits& clbits) 
                    : mName(name), mQubits(qubits), mClbits(clbits) {};
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
            Circuit circuit(const IoPorts& ports) const;

            ~Circuit();

            // Sends a representation of Circuit into output stream
            friend std::ostream& operator << (std::ostream&, const Circuit&);

        protected:
            // construct default circuit object
            Circuit() {};

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
        /*        Initialize:
         qi_0: -M-
                |
         cl: 5/=+=
                0  
        1.	CircuitInstruction(
            	operation=Instruction(
                	name='measure', 
                	num_qubits=1, 
                	num_clbits=1, 
                	params=[]
             	), 
            	qubits=(
                	Qubit(
                	    QuantumRegister( 2, 'qi' ), 
                	    0
                	),
                ), 
                clbits=(
                	Clbit(
                	    ClassicalRegister( 5, 'cl'), 
                	    0
                	),
            	)
        	)
*/
        class MeasureCircuit : public Circuit
        {
        private:
        protected:
        public:
            MeasureCircuit();
        };


        // Qiskit hadamard circuit is a specialization of a Circuit
        /*
        Superposition:
        qi_0: - H --M----


        1.	CircuitInstruction(
                operation=Instruction(
                    name='h',
                    num_qubits=1,
                    num_clbits=0,
                    params=[]
                ),
                qubits=(
                    Qubit(
                        QuantumRegister(2, 'qi'),
                        0
                    ),
                ),
                clbits=()
            )
        */
        class HadamardCircuit : public Circuit
        {
        private:
        protected:
        public:
            HadamardCircuit();
        };

        // Operator specialization of Circuit has one input and one output port
        class OperatorCircuit : public Circuit
        {
        private:
        protected:
            OperatorCircuit();
        public:
        };

        // Binary operation specialization of Circuit has two input and one
        // output port
        class BinaryOpCircuit : public Circuit
        {
        private:
        protected:
            BinaryOpCircuit();
        public:
        };

        // Binary operation specialization of Circuit has two input and two
        // output ports
        class BinaryOp2OutCircuit : public Circuit
        {
        private:
        protected:
            BinaryOp2OutCircuit();
        public:
        };

        // Qiskit's reset specialization of operator circuit
        /*
        Reset:
         out: -|0>----- X -----M-
                               |
        cl: 1/=================+=
                               0 == 1

        1.	CircuitInstruction(
                operation=Instruction(
                    name='reset', 
                    num_qubits=1, 
                    num_clbits=0, 
                    params=[]
                ), 
                qubits=(
                    Qubit(
                        QuantumRegister(1, 'out'), 
                        0
                     ),
                ), 
                clbits=()
            )
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        3.	CircuitInstruction(
                operation=Instruction(
                    name='x', 
                    num_qubits=1, 
                    num_clbits=0, 
                    params=[]
                ), 
                qubits=(
                    Qubit(
                        QuantumRegister(1, 'out'), 
                        0
                    ),
                ), 
                clbits=())
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(1, 'cl'), 0),))
*/
        class ResetCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            ResetCircuit();
        };

        // Invert operator specialization of Operator Circuit defined by
        // invert mark (sign) and name
        /*
        NOT operator:
          qi: - H ----- X -----M-
                               |
        cl: 1/=================+=
                               0 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        3.	CircuitInstruction(
                operation=Instruction(
                    name='x',
                    num_qubits=1,
                    num_clbits=0,
                    params=[]
                ),
                qubits=(
                    Qubit(
                        QuantumRegister(1, 'qi'),
                        0
                    ),
                ),
                clbits=())
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(1, 'cl'), 0),))
        */
        class InvertCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            InvertCircuit();
        };

        // Equal operator specialization of Operator Circuit defined by equal
        // mark and name
        /*
        EQ operator:
          qi: - H ------+------M----
                        |      |
         out: --------- X --------M-
                               |  | 
        cl: 2/=================+==+=
                               0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(
                operation=Instruction(
                        name='cx', 
                        num_qubits=2, 
                        num_clbits=0, 
                        params=[]
                ), 
                qubits=(
                    Qubit(
                        QuantumRegister(1, 'qi'), 
                        0
                    ), 
                    Qubit(
                        QuantumRegister(1, 'out'), 
                        0
                    )
                ), 
                clbits=()
            )
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
        */
        class EqCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            EqCircuit();
        };

        // Not equal operator specialization of Operator Circuit defined by
        // not-equal mark (sign) and name
        /*
        NE operator:
          qi: - H ------+-----------M----
                        |           |    
         out: --------- X -- X --------M-
                                    |  | 
        cl: 2/======================+==+=
                                    0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
*/
        class NeqCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            NeqCircuit();
        };

        // Less than operator specialization of Operator Circuit defined by
        // less-than mark (sign) and name
        /*
        LT operator:
          qi: - H -----X-------------+------M----
                       |             |      |    
         out: ---------X--|0>-- X -- X --------M-
                                            |  | 
        cl: 2/==============================+==+=
                                            0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(
                operation=Instruction(
                    name='swap', 
                    num_qubits=2, 
                    num_clbits=0, 
                    params=[]
                ), 
                qubits=(
                    Qubit(
                        QuantumRegister(1, 'qi'), 
                        0
                    ), 
                    Qubit(
                        QuantumRegister(1, 'out'), 
                        0
                    )
                ), 
                clbits=())
        4.	CircuitInstruction(operation=Instruction(name='reset', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
        */
        class LtCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            LtCircuit();
        };

        // Less eqal operator specialization of Operator Circuit defined by
        // less-eqal mark (sign) and name
        /*
        LE operator:
          qi: - H -----X---+------M----
                       |   |      |    
         out: ---------X-- X --------M-
                                  |  | 
        cl: 2/====================+==+=
                                  0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='swap', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
        */
        class LeCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            LeCircuit();
        };

        // Greater-than operator specialization of Operator Circuit defined by
        // reater-than mark (sign) and name
        /*
        GT operator:
          qi: - H ----------X---+-----------M----
                            |   |           |    
         out: --------- X --X-- X -----|0>-----M-
                                            |  | 
        cl: 2/==============================+==+=
                                            0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='swap', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='reset', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
        */
        class GtCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            GtCircuit();
        };

        // Greater-eqal operator specialization of Operator Circuit defined by
        // greater-eqal mark (sign) and name
        /*
        GE operator:
          qi: - H ----------X---+------M----
                            |   |      |    
         out: --------- X --X-- X --------M-
                                       |  | 
        cl: 2/=========================+==+=
                                       0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='swap', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))
        */
        class GeCircuit : public OperatorCircuit
        {
        private:
        protected:
        public:
            GeCircuit();
        };

        // And specialization of Binary Operation Circuit defined by
        // bitwise-and mark (sign) and name
        /*
        AND op:
        qi_0: - H ------+------M-------
                        |      |       
        qi_1: - H ------+---------M----
                        |      |  |    
         out: --------- X -----------M-
                               |  |  | 
        cl: 3/=================+==+==+=
                               0  1  2 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(
                operation=Instruction(
                    name='ccx', 
                    num_qubits=3, 
                    num_clbits=0, 
                    params=[]
                ), 
                qubits=(
                    Qubit(
                        QuantumRegister(2, 'qi'), 
                        0
                    ), 
                    Qubit(
                        QuantumRegister(2, 'qi'), 
                        1
                    ), 
                    Qubit(
                        QuantumRegister(1, 'out'), 
                        0
                    )
                ), 
                clbits=())
        5.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class AndCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            AndCircuit();
        };

        // Not and specialization of Binary Operation Circuit defined by
        // bitwise-inverted-and mark (sign) and name
        /*
        NAND op:
        qi_0: - H -----------+------M-------
                             |      |       
        qi_1: - H -----------+---------M----
                             |      |  |    
         out: --------- X -- X -----------M-
                                    |  |  | 
        cl: 3/======================+==+==+=
                                    0  1  2 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class NandCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            NandCircuit();
        };

        // Or specialization of Binary Operation Circuit defined by
        // bitwise-or mark (sign) and name
        /*
        OR op:
         qi_0: - H ------+----+------------------------M-------
                         |    |                        |  
         qi_1: - H ------+---------+----------------------M----
                         |    |    |                   |  |
        aux_0: --------- X ----------------+-------------------
                              |    |       |           |  |  
        aux_1: -------------- X -- X -----------+--------------
                                           |    |      |  |  
          out: --------------------------- X -- X -----------M-
                                                       |  |  |
         cl: 3/========================================+==+==+=
                                                       0  1  2 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=4, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        9.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        10.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=5, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        11.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        12.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        13.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class OrCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            OrCircuit();
        };

        // Not or specialization of Binary Operation Circuit defined by
        // bitwise-inverted-or mark (sign) and name
        /*
        NOR op:
         qi_0: - H ------+----+-----------------------------M-------
                         |    |                             |
         qi_1: - H ------+---------+---------------------------M----
                         |    |    |                        |  |
        aux_0: --------- X ----------------+------------------------
                              |    |       |                |  |
        aux_1: -------------- X -- X -----------+-------------------
                                           |    |           |  |
          out: --------------------------- X -- X -- X -----------M-
                                                            |  |  | 
         cl: 3/=============================================+==+==+=
                                                            0  1  2 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=4, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        9.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        10.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        11.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=5, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        12.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        13.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        14.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class NorCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            NorCircuit();
        };

        // Xor specialization of Binary Operation with 2 outputs Circuit
        // defined by bitwise-xor mark (sign) and name
        /*
        XOR op:
        qi_0: - H ------+-----------M-------
                        |           |       
        qi_1: - H -----------+---------M----
                        |    |      |  |    
         out: --------- X -- X -----------M-
                                    |  |  | 
        cl: 3/======================+==+==+=
                                    0  1  2 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class XorCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            XorCircuit();
        };

        // Not xor specialization of Binary Operation Circuit defined by
        // bitwise-inverted-xor mark (sign) and name
        /*
        NXOR op:
        qi_0: - H ------+----------------M-------
                        |                |
        qi_1: - H -----------+--------------M----
                        |    |           |  |
         out: --------- X -- X -- X -----------M-
                                         |  |  |
        cl: 3/===========================+==+==+=
                                         0  1  2

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        10.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))
        */
        class NxorCircuit : public BinaryOpCircuit
        {
        private:
        protected:
        public:
            NxorCircuit();
        };

        // Half adder is a specialization of Binary Operation with 2 outputs
        // Circuit defined by bitwise-xor mark (sign) and name
        /*
        Half Adder:
        qi_0: - H ------+---------+------M----------
                        |         |      |
        qi_1: - H -----------+----+---------M-------
                        |    |    |      |  |
         out: --------- X -- X ----------------M----
                                  |      |  |  |
          co: ------------------- X --------------M-
                                         |  |  |  | 
        cl: 4/===========================+==+==+==+=
                                         0  1  2  3 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=4, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=4, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(4, 'cl'), 0),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(4, 'cl'), 1),))
        10.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(4, 'cl'), 2),))
        11.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'co'), 0),), clbits=(Clbit(ClassicalRegister(4, 'cl'), 3),))
        */
        class XorCircuit : public BinaryOp2OutCircuit
        {
        private:
        protected:
        public:
            NxorCircuit();
        };



        // Adder is trinary operation specialization of Circuit with two outputs
        // defined arithmetic-add mark (sign) and name
        /*
        Adder:
          qi_0: - H ------+---------+-----------M-----------------------------------
                          |         |           |
          qi_1: - H -----------+----+--------------M--------------------------------
                          |    |    |           |  |
            ci: - H -----------------------------------+----+-----------M-----------
                          |    |    |           |  |   |    |           |
          xorO: --------- X -- X -----------+---------------+-----------------------
                                    |       |   |  |   |    |           |
        andO_0: ------------------- X ------------------------------+-------+-------
                                            |   |  |   |    |       |   |   |
        andO_1: ------------------------------------------- X ------+-----------------+----------------
                                            |   |  |   |            |   |   |         |
         aux_0: --------------------------------------------------- X -----------+---------------------
                                            |   |  |   |                |   |    |    |
         aux_1: ----------------------------------------------------------- X ------- X ---+-----------
                                            |   |  |   |                |        |         |
           out: --------------------------- X -------- X -----------------------------------------M----
                                                |  |                    |        |         |      |
            co: ---------------------------------------------------------------- X ------- X --------M-
                                                |  |                    |                         |  | 
          cl: 5/================================+==+====================+=========================+==+=
                                                0  1                    2                         3  4 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(1, 'xorO'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'xorO'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(2, 'andO'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=4, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'xorO'), 0), Qubit(QuantumRegister(2, 'andO'), 0)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'ci'), 0),), clbits=())
        9.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'ci'), 0),), clbits=())
        10.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'xorO'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        11.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'ci'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        12.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'xorO'), 0), Qubit(QuantumRegister(1, 'ci'), 0), Qubit(QuantumRegister(2, 'andO'), 1)), clbits=())
        13.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=7, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'ci'), 0), Qubit(QuantumRegister(1, 'xorO'), 0), Qubit(QuantumRegister(2, 'andO'), 0), Qubit(QuantumRegister(2, 'andO'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        14.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'andO'), 0), Qubit(QuantumRegister(2, 'andO'), 1), Qubit(QuantumRegister(2, 'aux'), 0)), clbits=())
        15.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'andO'), 0), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        16.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'andO'), 1), Qubit(QuantumRegister(2, 'aux'), 1)), clbits=())
        17.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        18.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        19.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=6, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'andO'), 0), Qubit(QuantumRegister(2, 'andO'), 1), Qubit(QuantumRegister(2, 'aux'), 0), Qubit(QuantumRegister(2, 'aux'), 1), Qubit(QuantumRegister(1, 'out'), 0), Qubit(QuantumRegister(1, 'co'), 0)), clbits=())
        20.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 0),))
        21.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 1),))
        22.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'ci'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 2),))
        23.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 3),))
        24.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'co'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 4),))
        */
        class AdderCircuit : public Circuit
        {
        private:
        protected:
            Labels format(const Labels&) const;
        public:
            AdderCircuit();
        };
    };
};

#endif /* Circuits_h */