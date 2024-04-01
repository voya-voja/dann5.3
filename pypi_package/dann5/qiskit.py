# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""

"""
from qiskit import IBMQ
from qiskit.utils import algorithm_globals, QuantumInstance
from qiskit.algorithms import QAOA #, NumPyMinimumEigensolver
from qiskit_optimization.algorithms import MinimumEigenOptimizer#, RecursiveMinimumEigenOptimizer
from qiskit_optimization import QuadraticProgram

import numpy as np

class QuantumRequest:
  provider = None
  
  def __init__(self, assignmnet, timeout=100):
    if IBMQ.active_account() == None:
        self.provider = IBMQ.load_account()
    else:
        self.provider = IBMQ.get_provider()
    self.assign = assignmnet
    self.tm = timeout
    self.problem = None
    self.result = None
    self.nodes = None
    self.branches = None
    

  def reset(self):
    self.problem = None
    self.result = None
    self.nodes = None
    self.branches = None
    self.assignment.reset()
    
    
  def execute(self):
    qubo = self.assign.qubo()
    self.qubo2problem(qubo)
    
    # Instantiate a solver to solve the problem.
    quantum_instance = QuantumInstance(self.provider.get_backend('ibmq_qasm_simulator'),
                                       seed_simulator=aqua_globals.random_seed,
                                       seed_transpiler=aqua_globals.random_seed)
    qaoa_mes = QAOA(quantum_instance=quantum_instance, initial_point=[0., 0.])
    qaoa = MinimumEigenOptimizer(qaoa_mes)   # using QAOA
    self.result = qaoa.solve(self.problem)
    #print(f'\nResult:\n{self.result}\n')
    self.solve()

    
  def qubo2problem(self, qubo) -> QuadraticProgram:
    analyzer = Qanalyzer(qubo)
    
    self.nodes = analyzer.nodes()
    energies = []
    
    self.problem = QuadraticProgram()
    for (node, energy) in self.nodes:
        self.problem.binary_var(node)
        energies.append(energy)
    
    self.branches = dict(analyzer.branches())
    self.problem.minimize(linear= np.array(energies), quadratic=self.branches)
    #print(self.problem.export_as_lp_string())
    return self.problem
    
        
  def solve(self):
    # Print a summary of the result
    d5osamples = []
    for sample in self.result.samples:
        if self.result.fval == sample.fval:
            d5osample = {}
            for qbit in range(len(sample.x)):
                value = int(sample.x[qbit])
                name = self.nodes[int(qbit)]
                d5osample[name[0]] = value
            d5osamples.append(d5osample)
    #print(f'The samples are: {d5osamples}')
    self.assign.add(d5osamples)
"""
import qiskit
from qiskit_aer import AerSimulator

from qiskit_ibm_provider import IBMProvider
from qiskit.providers.aer import QasmSimulator

from qiskit import QuantumRegister, ClassicalRegister, QuantumCircuit
from qiskit.circuit import Qubit, Clbit

from dann5.d5 import Qevaluation, Qsolver
from dann5.d5q import QiskitSolver, QuantumBit, ClassicalBit

class QiskitEvaluation(Qevaluation):
    """
    A dann5 qiskit evaluation converts Qiskit resluts into Qevaluation
    """
    def __init__(self, qregs, counts, energy = 0):
        """
        Initializes an instance of a Qiskit evaluation 
        """
        sample = dict()
        at = len(counts)
        for reg in qregs:
            at -= 1
            sample[reg[0]] = int(counts[at])
        Qevaluation.__init__(self, sample, energy)
        
class PyQiskitSolver(QiskitSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using specific Qiskit bakend or local solver
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance of a Qiskit Aer simulator 
        """
        QiskitSolver.__init__(self, lowest)
        
    def solve(self):
        """
        Computes solutions samples using Qiskit Aer simulator 
        """
        qc = self.constructQC()
        result_ideal = qiskit.execute(qc, self.solver).result()
        self.evaluatuate(result_ideal)
        
    def constructQC(self):
        circuit = self.circuit()
        #initialize qc with circuit's quantum and clasical registers
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
        # factor qc operations based on circuit instructions
        for instrctn in circuit.instructions():
            if instrctn.name() == 'h':
                qbit = QuantumBit(instrctn.qubits()[0])
                qubit = qubits[qbit.register().name()]
                qc.h(qubit)
            elif instrctn.name() == 'reset':
                qbit = QuantumBit(instrctn.qubits()[0])
                qubit = qubits[qbit.register().name()]
                qc.reset(qubit)
            elif instrctn.name() == 'x':
                qbit = QuantumBit(instrctn.qubits()[0])
                qubit = qubits[qbit.register().name()]
                qc.x(qubit)
            elif instrctn.name() == 'cx':
                cQbit = QuantumBit(instrctn.qubits()[0])
                cQubit = qubits[cQbit.register().name()]
                xQbit = QuantumBit(instrctn.qubits()[1])
                xQubit = qubits[xQbit.register().name()]
                qc.cx(cQubit, xQubit)
            elif instrctn.name() == 'swap':
                cQbit = QuantumBit(instrctn.qubits()[0])
                cQubit = qubits[cQbit.register().name()]
                xQbit = QuantumBit(instrctn.qubits()[1])
                xQubit = qubits[xQbit.register().name()]
                qc.swap(cQubit, xQubit)
            elif instrctn.name() == 'ccx':
                c0Qbit = QuantumBit(instrctn.qubits()[0])
                c0Qubit = qubits[c0Qbit.register().name()]
                c1Qbit = QuantumBit(instrctn.qubits()[1])
                c1Qubit = qubits[c1Qbit.register().name()]
                xQbit = QuantumBit(instrctn.qubits()[2])
                xQubit = qubits[xQbit.register().name()]
                qc.ccx(c0Qubit, c1Qubit, xQubit)
            elif instrctn.name() == 'measure':
                qbit = QuantumBit(instrctn.qubits()[0])
                qubit = qubits[qbit.register().name()]
                cbit = ClassicalBit(instrctn.clbits()[0])
                clbit = Clbit(clreg, cbit.at() )
                qc.measure(qubit, clbit)
        return qc
        
    def evaluatuate(self, result_ideal):
        # Perform an ideal simulation
        counts_ideal = result_ideal.get_counts(0)
        qregs = result_ideal.results[0].header.qreg_sizes
        evaluations = []
        for counts in counts_ideal:
            evaluation = QiskitEvaluation(qregs, counts)
            evaluations.append(evaluation)
        self.solution(evaluations)   
        
class QiskitAerSolver(PyQiskitSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using Qiskit Aer solver, an IBM local quantum simulator
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance of a D-Wave exact solver 
        """
        QiskitSolver.__init__(self)
        self.solver = AerSimulator()


class QiskitSolvers:
    """
    A class facade abstracts an access to different qiskit solvers  
    of optimal functions expressed as circuits. It supports following types:
        - 'aer'       =>   A local Qiskit Aer Simulator
        #backend = AerSimulator()

        - 'qasm'      =>   A remote Qiskit QasmSimulator
        #backend = QasmSimulator()

        #backend = provider.get_backend('ibm_brisbane')
    
        #backend = provider.get_backend('ibm_perth')
    
        #backend = provider.get_backend('ibm_nairobi')
    
        #backend = provider.get_backend('ibmq_qasm_simulator')

    """
    mSolvers = {}
    """
    A list of initialized (and connected reote) solvers
    """

    def connect(solverType, lowest = True, num_reads = 1000):
        """
        Connect to remote DWave qpu's or hybrid BQM solver or initialize 
        DWave's exact or dann5 (local) simulator
        """
        if solverType == 'aer':
            solver = QiskitAerSolver(lowest)
        else:
            print("ERROR: Solver type {} is not supported!".format(solverType))
        #except SolverNotFoundError:
            #print("ERROR: Solver type {} is unavailable.".format(solverType))
        return solver
    

    def solver(solverType, lowest = True, num_reads = 1000):
        """
        Returns a solver for a given solver type.
        If solver is not initialized calls connect(...) method to create 
        a solver and adds the solver to the list of nitialized solvers.
        """
        try:
            solver = QiskitSolvers.mSolvers[solverType]
        except KeyError:
            solver = QiskitSolvers.connect(solverType, lowest, num_reads)
            QiskitSolvers.mSolvers[solverType] = solver
        return solver


class Solver(Qsolver):
    """
    A class facade sets and returns an active solver, which will be used to solve
    a quantum statement
    """
    gActive = QiskitSolvers.solver("aer")
    """
    A reference to an active quskit solver used to solve quantum statements
    """
    
    def SetActive(solver):
        """
        Sets an active qiskit solver used to solve quantum statements. It sets
        both as python's Solver.gActive and dann5.d5 library static reference
        """
        Solver.gActive = solver
        Qsolver.Active(solver)
    
    def Activate(solverType):
        """
        Sets an active qiskit solver used to solve quantum statements. It sets
        both as python's Solver.gActive and dann5.d5 library static reference
        It supports following Solver types:
            - 'aer'       =>   A local Qiskit Aer Simulator
            #backend = AerSimulator()

            - 'qasm'      =>   A remote Qiskit QasmSimulator
            #backend = QasmSimulator()

            #backend = provider.get_backend('ibm_brisbane')
        
            #backend = provider.get_backend('ibm_perth')
        
            #backend = provider.get_backend('ibm_nairobi')
        
            #backend = provider.get_backend('ibmq_qasm_simulator')
        """
        solver = QiskitSolvers.solver(solverType)
        Solver.gActive = solver
        Qsolver.Active(solver)
     
    def Active():
        """
        Returns an active qiskit solver used to solve quantum statements.
        When dann5.d5 library static reference is different than the python's
        Solver.gActive reference, dann5.d5 library is set to reference the same
        solver.
        """
        active = Qsolver.Active()
        if(Solver.gActive != active):
            Qsolver.Active(Solver.gActive)
        return Solver.gActive
   