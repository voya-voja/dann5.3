# -*- coding: utf-8 -*-
"""
Created on Mon Dec 27 16:59:22 2021

@author: Nebojsa.Vojinovic
"""

import dann5.d5o2 as d5o
from dann5.qiskit import QuantumRequest

import time

from qiskit import QuantumRegister, ClassicalRegister, QuantumCircuit

from qiskit import IBMQ, transpile

from qiskit.providers.aer import QasmSimulator


def getProvider():
    provider = None
    try:
        provider = IBMQ.load_account()
    except:
        IBMQ.save_account('3831550b1cb9f95399e73d27580df327545536ec32add72e4194247a9f7e02ade60a585057c1d030ca0cda5bc6f443d8f3dd010f4fbacbe8291295a24cfdd8fc')   
        provider = IBMQ.load_account()
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
    #qreg in[2]; qreg ci[1]; 
    #qreg xorO[1]; qreg andO[2]; qreg aux[2];
    #qreg out[1]; qreg co[1];
    qi = QuantumRegister(2, 'qi')
    ci = QuantumRegister(1, 'ci') 
    xorO = QuantumRegister(1, 'xorO') 
    andO = QuantumRegister(2, 'andO')
    aux = QuantumRegister(2, 'aux')
    out = QuantumRegister(1, 'out')    
    co = QuantumRegister(1, 'co')
    cl = ClassicalRegister(5, "cl")
    qc = QuantumCircuit(qi, ci, xorO, andO, aux, out, co, cl)
    
    #// half adder: xorO = in0 + in1
    #h in; 
    qc.h(qi)
    qc.barrier(qi)
    #// out = in0 XOR in1
    #cx in[0], xorO[0];
    #cx in[1], xorO[0];
    qc.cx(qi[0], xorO[0])
    qc.cx(qi[1], xorO[0])
    #// andO0 = in0 AND in1
    #ccx in[0], in[1], andO[0];
    #barrier in, ci, xorO, andO[0];
    qc.ccx(qi[0], qi[1], andO[0])
    qc.barrier(qi, xorO, andO[0])
    
    #// half adder: out = xorO + ci
    #h ci; 
    qc.h(ci)
    qc.barrier(ci)
    #// out = xorO XOR ci
    #cx xorO[0], out[0];
    #cx ci[0], out[0];
    qc.cx(xorO[0], out[0])
    qc.cx(ci[0], out[0])
    #// andO1 = xorO AND ci
    #ccx xorO[0], ci[0], andO[1];
    #barrier ci, xorO, andO, aux, out;
    qc.ccx(xorO[0], ci[0], andO[1])
    qc.barrier(ci, xorO, andO, aux, out)
    
    #// co = andO0 OR andO1
    # ccx andO[0], andO[1], aux[0]; // AND
    # cx andO[0], aux[1];
    # cx andO[1], aux[1];      // XOR
    # cx aux[1], co[0];
    # cx aux[0], co[0];   // XOR
    qc.ccx(andO[0],andO[1], aux[0])
    qc.cx(andO[0], aux[1])
    qc.cx(andO[1], aux[1])
    qc.cx(aux[0], co[0])
    qc.cx(aux[1], co[0])
    # barrier andO, aux, out, co;
    qc.barrier(andO, aux, out, co)
    qc.measure(qi, cl[0:2])
    qc.measure(ci[0], cl[2])
    qc.measure(out[0], cl[3])
    qc.measure(co[0], cl[4])
    return qc
     
def testCircuit(qc, provider):
    print(qc)
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
    
    
def logicalGates():
    provider = getProvider()
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
    print("Adder:"); testCircuit(adder(), provider);
    
    #testCircuit(initialize())
    
    # constant value
    #if (c == 1) measure out[0] -> c[0];
    
    # up to 4 input bits AND gate
    #qreg qo[2];
    #h qo[0];
    #h qo[1];#c4x qo[1], qo[0], qi[0], qi[1], out[0];
    #barrier qi, qo, out;
    


def qwholeXlarge_test() -> d5o.Qassignment:
    print("\n\n==== qwholeXlarge_test() =====")
    p = d5o.Qwhole(2,"p")
    q = d5o.Qwhole(2, "q")
    #r = d5o.Qwhole(5, "r")
    M = d5o.Qwhole("M", 3)
    mM = M._(p * q) # * r)
    print(f"\n{mM.toString()}\n{mM.toString(True)}")
    qubo = mM.qubo()
    analyze = d5o.Qanalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("d5o simulation solutions: \n{}\n".format(mM.solutions()))
    
    return mM;
    
def main():
    logicalGates()
    """
    assignment = qwholeXlarge_test()
    assignment.reset()
    request = QuantumRequest(assignment)

    print('\nSubmitting simplified problem...')
    start = time.time()
    request.execute()
    time_elapsed_simplified = time.time() - start
    print(f'\nResult returned in {time_elapsed_simplified} seconds:')
    print(assignment.solutions())
    """

if __name__ == "__main__":
    main()
    

