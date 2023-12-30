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
    namespace ocean {
        /*
        Initialize:
         qi_0: -M-------------
         qi_1: ----M----------
        aux_0: -------M-------
        aux_1: ----------M----
          out: -------------M-
         cl: 5/=+==+==+==+==+=
                0  1  2  3  4 
        1.	CircuitInstruction(
        -	operation=Instruction(
        -	name='measure', 
        -	num_qubits=1, 
        -	num_clbits=1, 
        -	params=[]
        -	), 
        -	qubits=(
        -	Qubit(
        -	QuantumRegister(
        -	2, 
        -	'qi'
        -	), 
        -	0
        -	),
        -	), 
        -	clbits=(
        -	Clbit(
        -	ClassicalRegister(
        -	5, 
        -	'cl'
        -	), 
        -	0
        -	),
        -	)
        -	)
        2.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 1),))
        3.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 2),))
        4.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'aux'), 1),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 3),))
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(5, 'cl'), 4),))

        Superposition:
        qi_0: - H --M----
                    |    
        qi_1: - H -----M-
                    |  |  
        cl: 2/======+==|=
                    0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        4.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))

        Reset:
         out: -|0>----- X -----M-
                               |
        cl: 1/=================+=
                               0 

        1.	CircuitInstruction(operation=Instruction(name='reset', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(1, 'cl'), 0),))

        NOT operator:
          qi: - H ----- X -----M-
                               |
        cl: 1/=================+=
                               0 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(1, 'cl'), 0),))

        EQ operator:
          qi: - H ------+------M----
                        |      |
         out: --------- X --------M-
                               |  | 
        cl: 2/=================+==+=
                               0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))

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

        LT operator:
          qi: - H -----X-------------+------M----
                       |             |      |    
         out: ---------X--|0>-- X -- X --------M-
                                            |  | 
        cl: 2/==============================+==+=
                                            0  1 

        1.	CircuitInstruction(operation=Instruction(name='h', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=())
        2.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        3.	CircuitInstruction(operation=Instruction(name='swap', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        4.	CircuitInstruction(operation=Instruction(name='reset', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='x', num_qubits=1, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='cx', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        7.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=2, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 0),))
        9.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(2, 'cl'), 1),))

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
        4.	CircuitInstruction(operation=Instruction(name='ccx', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        5.	CircuitInstruction(operation=Instruction(name='barrier', num_qubits=3, num_clbits=0, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0), Qubit(QuantumRegister(2, 'qi'), 1), Qubit(QuantumRegister(1, 'out'), 0)), clbits=())
        6.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 0),))
        7.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(2, 'qi'), 1),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 1),))
        8.	CircuitInstruction(operation=Instruction(name='measure', num_qubits=1, num_clbits=1, params=[]), qubits=(Qubit(QuantumRegister(1, 'out'), 0),), clbits=(Clbit(ClassicalRegister(3, 'cl'), 2),))

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
    };
};

#endif /* Circuits_h */