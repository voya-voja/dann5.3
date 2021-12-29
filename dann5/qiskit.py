# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
from qiskit import IBMQ

from qiskit.aqua import aqua_globals, QuantumInstance
from qiskit.aqua.algorithms import QAOA, NumPyMinimumEigensolver
from qiskit.optimization.algorithms import MinimumEigenOptimizer, RecursiveMinimumEigenOptimizer
from qiskit.optimization import QuadraticProgram

from dann5.d5o2 import Qanalyzer

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
