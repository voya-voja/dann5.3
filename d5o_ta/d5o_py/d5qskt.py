# -*- coding: utf-8 -*-
"""
Created on Mon Dec 27 16:59:22 2021

@author: Nebojsa.Vojinovic
"""

from qiskit import QuantumRegister, ClassicalRegister, QuantumCircuit
from qiskit.circuit import Qubit, Clbit

from qiskit import IBMQ, transpile

from qiskit.providers.aer import QasmSimulator
from qiskit_ibm_provider import IBMProvider

import qiskit
from qiskit_aer import AerSimulator

from dann5.d5 import Qbit
from dann5.d5q import CircuitCompiler
from dann5.qiskit import Solver

def getProvider():
    provider = None
    try:
        provider = IBMProvider()
    except:
        IBMProvider.save_account('3831550b1cb9f95399e73d27580df327545536ec32add72e4194247a9f7e02ade60a585057c1d030ca0cda5bc6f443d8f3dd010f4fbacbe8291295a24cfdd8fc')   
        provider = IBMProvider()
    return provider

def initialize():
    # initialize to 0
    #OPENQASM 2.0;
    #include "qelib1.inc";
    
    #// initialize to 0
    #qreg qi[2]; qreg aux[2]; qreg out[1]; creg c[1];
    qi = QuantumRegister(2, 'qi')
    aux = QuantumRegister(2, 'aux')
    out = QuantumRegister(1, 'out')

    cl = ClassicalRegister(5, "cl")
    qc = QuantumCircuit(qi, aux, out, cl)

    qc.measure(qi,cl[0:2])
    qc.measure(aux[0],cl[2])
    qc.measure(aux[1],cl[3])
    qc.measure(out,cl[4])
    return qc;
    
def superposition():
    qi = QuantumRegister(2, 'qi')
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, cl)
    # initialize to S
    #h qi[0]; h qi[1];
    qc.h(qi)
    qc.measure(qi, cl)
    return qc;
   
def reset():
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(1, "cl")
    qc = QuantumCircuit(out, cl)
    # set to 0
    # reset out[0];
    #barrier out;
    qc.reset(out[0])
    qc.barrier(out)
    # NOT 0 => initialize to 1
    # x out[0];
    #barrier out;
    qc.x(out[0])
    qc.barrier(out)
    qc.measure(out[0], cl[0])
    return qc;

def notOperator():
    #qreg qi[1];
    #h qi; 
    qi = QuantumRegister(1, 'qi')
    cl = ClassicalRegister(1, "cl")
    qc = QuantumCircuit(qi, cl)

    qc.h(qi[0])
    qc.barrier(qi)
    # NOT gate
    #x qi[0]; 
    #barrier qi;
    qc.x(qi[0]);
    qc.barrier(qi)
    qc.measure(qi[0], cl[0])
    return qc;


def eqOperator():   
    #qreg qi[1]; qreg out[1];
    #h qi; 
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)
    qc.h(qi)
    qc.barrier(qi, out)
    #cx qi[0], out[0];
    qc.cx(qi[0], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc

def neOperator():   
    #qreg in[1]; qreg out[1];
    #h in; 
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)
    qc.h(qi)
    qc.barrier(qi, out)
    #cx in[0], out[0];
    #x out[0];
    qc.cx(qi[0], out[0])
    qc.x(out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc

def ltOperator():
    #qreg in[1]; qreg out[1];
    #h in[0];
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)
    qc.h(qi)
    qc.barrier(qi, out)
    #swap in[0], out[0]; // swap is not commutative
    #reset out[0];
    #x out[0];
    #cx in[0], out[0];
    qc.swap(qi[0], out[0])     # swap is not commutative
    qc.reset(out[0])
    qc.x(out[0])
    qc.cx(qi[0], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc

def leOperator():
    #qreg in[1]; qreg out[1];
    #h in[0];
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)
    qc.h(qi)
    qc.barrier(qi, out)
    #swap in[0], out[0]; // swap is not commutative
    #cx in[0], out[0];
    qc.swap(qi[0], out[0])     # swap is not commutative
    qc.cx(qi[0], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc

def gtOperator():
    #qreg in[1]; qreg out[1];
    #h in[0];
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)
    qc.h(qi)
    qc.barrier(qi, out)
    #x out[0]           // has to be here
    #swap in[0], out[0];// swap is not commutative
    #cx in[0], out[0];
    #reset out[0];
    qc.x(out[0])        # has to ne here
    qc.swap(qi[0], out[0])     # swap is not commutative
    qc.cx(qi[0], out[0])
    qc.barrier(qi, out)
    qc.reset(out[0])
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc

def geOperator():
    #qreg in[1]; qreg out[1];
    #h in[0];
    qi = QuantumRegister(1, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(2, "cl")
    qc = QuantumCircuit(qi, out, cl)

    qc.h(qi)
    qc.barrier(qi, out)
    #x out[0]           // has to be here
    #swap in[0], out[0];// swap is not commutative
    #cx in[0], out[0];
    qc.x(out[0])        # has to ne here
    qc.swap(qi[0], out[0])     # swap is not commutative
    qc.cx(qi[0], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(out[0], cl[1])
    return qc    

def andOp():
    qi = QuantumRegister(2, 'qi')
    out = QuantumRegister(1, 'out')
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # AND op
    #ccx qi[0], qi[1], out[0]; 
    #barrier qi, out;
    qc.ccx(qi[0], qi[1], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(qi[1], cl[1])
    qc.measure(out[0], cl[2])
    return qc;
    
def nandOp():
    qi = QuantumRegister(2, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # NAND op
    #x out[0];
    #ccx qi[0], qi[1], out[0];
    #barrier qi, out;
    qc.x(out[0])
    qc.ccx(qi[0], qi[1], out[0])
    qc.barrier(qi, out)
    qc.measure(qi[0], cl[0])
    qc.measure(qi[1], cl[1])
    qc.measure(out[0], cl[2])
    return qc;
    
def xorOp():
    qi = QuantumRegister(2, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # XOR op
    #cx qi[0], out[0];
    #cx qi[1], out[0];
    #barrier qi, out;
    qc.cx(qi[0], out[0])
    qc.cx(qi[1], out[0])
    qc.barrier(qi, out)
    qc.measure(qi, cl[0:2])
    qc.measure(out[0], cl[2])
    return qc;
    
def nxorOp():
    qi = QuantumRegister(2, 'qi')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # XOR op
    #cx qi[0], out[0];
    #cx qi[1], out[0];
    #x out[0]
    #barrier qi, out;
    qc.cx(qi[0], out[0])
    qc.cx(qi[1], out[0])
    qc.x(out[0])
    qc.barrier(qi, out)
    qc.measure(qi, cl[0:2])
    qc.measure(out[0], cl[2])
    return qc;
    
def orOp():
    qi = QuantumRegister(2, 'qi')
    aux = QuantumRegister(2, 'aux')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, aux, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # OR op
    #ccx qi[0], qi[1], aux[0]; // AND
    #cx qi[0], aux[1];
    #cx qi[1], aux[1];      // XOR
    #cx aux[1], out[0];
    #cx aux[0], out[0];   // XOR
    #barrier qi, aux, out;
    qc.ccx(qi[0],qi[1], aux[0])
    qc.cx(qi[0], aux[1])
    qc.cx(qi[1], aux[1])
    qc.barrier(qi, aux)
    qc.cx(aux[0], out[0])
    qc.cx(aux[1], out[0])
    qc.barrier(qi, aux, out)
    qc.measure(qi, cl[0:2])
    qc.measure(out[0], cl[2])
    return qc;
    
def norOp():
    qi = QuantumRegister(2, 'qi')
    aux = QuantumRegister(2, 'aux')
    out = QuantumRegister(1, 'out')    
    cl = ClassicalRegister(3, "cl")
    qc = QuantumCircuit(qi, aux, out, cl)

    qc.h(qi[0:2])
    qc.barrier(qi, out)
    # OR op
    #ccx qi[0], qi[1], aux[0]; // AND
    #cx qi[0], aux[1];
    #cx qi[1], aux[1];      // XOR
    #cx aux[1], out[0];
    #cx aux[0], out[0];   // XOR
    #x out[0]               // NOT
    #barrier qi, aux, out;
    qc.ccx(qi[0],qi[1], aux[0])
    qc.cx(qi[0], aux[1])
    qc.cx(qi[1], aux[1])
    qc.barrier(qi, aux)
    qc.cx(aux[0], out[0])
    qc.cx(aux[1], out[0])
    qc.x(out[0])
    qc.barrier(qi, aux, out)
    qc.measure(qi, cl[0:2])
    qc.measure(out[0], cl[2])
    return qc;

def halfAdder():
    #qreg in[2]; 
    #qreg out[1]; qreg co[1];
    qi = QuantumRegister(2, 'qi')
    out = QuantumRegister(1, 'out')    
    co = QuantumRegister(1, 'co')
    cl = ClassicalRegister(4, "cl")
    qc = QuantumCircuit(qi, out, co, cl)
    
    #// half adder
    #h in;
    qc.h(qi)
    qc.barrier(qi, out, co)
    #// out = in0 XOR in1
    #cx in[0], out[0];
    #cx in[1], out[0];
    qc.cx(qi[0], out[0])
    qc.cx(qi[1], out[0])
    #// carry = in0 AND in1
    #ccx in[0], in[1], co[0];
    qc.ccx(qi[0], qi[1], co[0])
    qc.barrier(qi, out, co)
    qc.measure(qi, cl[0:2])
    qc.measure(out[0], cl[2])
    qc.measure(co[0], cl[3])
    return qc;

def adder():
    #qreg i[3];
    #qreg a[5];
    #qreg o[1]; qreg c[1];
    i = QuantumRegister(3, 'i')
    a = QuantumRegister(5, 'a')
    o = QuantumRegister(1, 'o')    
    c = QuantumRegister(1, 'c')
    cl = ClassicalRegister(5, "cl")
    qc = QuantumCircuit(i, a, o, c, cl)
    
    #// half adder: xorO = i0 + i1
    #h in; 
    qc.h(i)
    qc.barrier(i, a, o, c)
    #// half adder: a0 = i0 + i1
    #cx in[0], a[0];
    #cx in[1], a[0];
    qc.cx(i[0], a[0])
    qc.cx(i[1], a[0])
    #// a1 = i0 AND i1
    qc.ccx(i[0], i[1], a[1])
    qc.barrier(i, a, o, c)
    
    #// half adder: o = aO + i2
    qc.cx(a[0], o[0])
    qc.cx(i[2], o[0])
    #// a2 = aO AND i2
    #ccx a[0], i[2], a[2];
    #barrier i, a, o, c, cl;
    qc.ccx(a[0], i[2], a[2])
    qc.barrier(i, a, o, c)
    
    #// co = a1 OR a2
    # ccx a[1], a[2], a[3]; // AND
    # cx a[1], a[4];
    # cx a[2], a[4];      // XOR
    # cx a[3], c[0];
    # cx a[4], c[0];   // XOR
    qc.ccx(a[1],a[2], a[3])
    qc.cx(a[1], a[4])
    qc.cx(a[2], a[4])
    qc.cx(a[3], c[0])
    qc.cx(a[4], c[0])
    # barrier i, a, o, c, cl;
    qc.barrier(i, a, o, c)
    qc.measure(i, cl[0:3])
    qc.measure(o[0], cl[3])
    qc.measure(c[0], cl[4])
    return qc
     
def testCircuit(qc, provider):
    print(qc)
    # Construct an ideal simulator
    aersim = AerSimulator()
    
    # Perform an ideal simulation
    result_ideal = qiskit.execute(qc, aersim).result()
    counts_ideal = result_ideal.get_counts(0)
    print('Counts(ideal):', counts_ideal)
    """
    backend = provider.get_backend('ibm_brisbane')
    #backend = provider.get_backend('ibm_perth')
    #backend = provider.get_backend('ibm_nairobi')
    #backend = provider.get_backend('ibmq_qasm_simulator')
    # Use Aer's qasm_simulator
    #backend = QasmSimulator()

    compiled_circuit = transpile(qc, backend)
    # Execute the circuit on the qasm simulator
    job = backend.run(compiled_circuit, shots=1000)#, memory=True)  
    result = job.result() # memory=True
    print(result.get_counts())
    return result
    """
    
    
def logicalGates():
    provider = getProvider()
    """
    print("Initialize:"); testCircuit(initialize(), provider)
    print("Superposition:"); testCircuit(superposition(), provider);
    print("Reset:"); testCircuit(reset(), provider);
    print("NOT operator:"); testCircuit(notOperator(), provider);
    print("EQ operator:"); testCircuit(eqOperator(), provider)
    print("NE operator:"); testCircuit(neOperator(), provider)
    print("LT operator:"); testCircuit(ltOperator(), provider)
    print("LE operator:"); testCircuit(leOperator(), provider)
    print("GT operator:"); testCircuit(gtOperator(), provider)
    print("GE operator:"); testCircuit(geOperator(), provider)
    
    print("AND op:"); testCircuit(andOp(), provider);
    print("NAND op:"); testCircuit(nandOp(), provider);
    print("XOR op:"); testCircuit(xorOp(), provider);
    print("NXOR op:"); testCircuit(nxorOp(), provider);
    print("OR op:"); testCircuit(orOp(), provider);
    print("NOR op:"); testCircuit(norOp(), provider);
    
    print("Half Adder:"); testCircuit(halfAdder(), provider);
    """
    print("Adder:"); testCircuit(adder(), provider);
    
    #testCircuit(initialize())
    
    # constant value
    #if (c == 1) measure out[0] -> c[0];
    
    # up to 4 input bits AND gate
    #qreg qo[2];
    #h qo[0];
    #h qo[1];#c4x qo[1], qo[0], qi[0], qi[1], out[0];
    #barrier qi, qo, out;
    
def testQiskit():
    x = QuantumRegister(1, 'x')
    y = QuantumRegister(1, 'y')
    z = QuantumRegister(1, 'z')
    _and0 = QuantumRegister(1, '_&0')
    _xor0 = QuantumRegister(1, '_^0')
    _cXor0 = QuantumRegister(1, '#[_^0]')
    cl = ClassicalRegister(6, "cl")
    
    qc = QuantumCircuit(x, y, z, _and0, _xor0, _cXor0, cl)
    qc.h(x)
    qc.h(y)
    qc.h(z)
    #qc.h(_and0)
    #qc.h(_xor0)
    
    qc.ccx(x, y, _and0)
    qc.cx(_and0, _xor0)
    qc.cx(z, _xor0)
    
    qc.measure(x, cl[0])
    qc.measure(y, cl[1])
    qc.measure(z, cl[2])
    qc.measure(_and0, cl[3])
    qc.measure(_xor0, cl[4])
    qc.measure(_cXor0, cl[5])
    testCircuit(qc, None)
    
def testDann5():
    x = Qbit("x"); y = Qbit("y"); z = Qbit("z")
    xpr = (x & y) ^ z
    comp = CircuitCompiler()
    xpr.compile(comp)
    circuit = comp.circuit()
    # create qc
    #declare qc
    qc = QuantumCircuit()
    qubits = {}
    nclbs = 0
    regs = []
    for name, operand in circuit.operands().items():
        qreg = QuantumRegister(operand[0][0][0], name)
        regs.append(qreg)
        qubit = Qubit(qreg, 0)
        qubits[name] = qubit
        nclbs += 1
    clreg = ClassicalRegister(nclbs, "cl")
    regs.append(clreg)
    qc = QuantumCircuit(*regs)
    #print(qc.draw())
    #print(qubits)
    for instrctn in circuit.instructions():
        if instrctn.name() == 'h':
            qc.h(qubits[instrctn.qubits()[0][0][1]])
        elif instrctn.name() == 'cx':
            qc.cx(qubits[instrctn.qubits()[0][0][1]], \
                  qubits[instrctn.qubits()[1][0][1]])
        elif instrctn.name() == 'ccx':
            qc.ccx(qubits[instrctn.qubits()[0][0][1]], \
                  qubits[instrctn.qubits()[1][0][1]], \
                  qubits[instrctn.qubits()[2][0][1]])
        elif instrctn.name() == 'measure':
            clbit = Clbit(clreg, instrctn.clbits()[0][1] )
            qc.measure(qubits[instrctn.qubits()[0][0][1]], clbit)
    #print(qc.draw())
    testCircuit(qc, None)
    print(circuit.instructions())

def testSolver():
    x = Qbit("x"); y = Qbit("y"); z = Qbit("z")
    xpr = (x & y) ^ z
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    xpr = (x | y) ^ z
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    w = Qbit("w", 0)
    xpr = (x & y) ^ w
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    xpr = (x | y) ^ w
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    m = Qbit("m", 1)
    xpr = (x & y) ^ m
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    xpr = (x | y) ^ m
    print("\n {} \n\n {}\n".format(xpr, 
                                   xpr.toString(True)))    
    print("Active Qiskit Aer simulator solutions: \n{}\n".format(xpr.solve()))
    
def main():
        
    Solver.Active()   # activates default AerSimulator
    testSolver()
    
    #logicalGates()
    #testQiskit()
    #testDann5()


if __name__ == "__main__":
    main()
    

