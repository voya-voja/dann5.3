# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
# Take an array of container weights and return a Problem object that represents the cost function
from typing import List
from azure.quantum.optimization import Problem, ProblemType, Term

# Submit problem to Azure Quantum using the ParallelTempering solver:
from azure.quantum.optimization import ParallelTempering

from dann5.azureWS import workspace

class QuantumRequest:
  def __init__(self, assignmnet, timeout=100):
    self.assign = assignmnet
    self.tm = timeout
    self.problem = None
    self.sample = None
    self.nodes = None
    self.terms = None

  def reset(self):
    self.problem = None
    self.sample = None
    self.nodes = None
    self.terms = None
    self.assignment.reset()
    
  def execute(self):
    qubo = self.assign.qubo()
    self.qubo2problem(qubo)
    
    # Instantiate a solver to solve the problem.
    solver = ParallelTempering(workspace, timeout = self.tm)
    
    # Optimize the problem
    result = solver.optimize(self.problem)
#    print(f'\nResult:\n{result}\n')
    self.sample = result['configuration']
    self.solve()
    #    print_result_summary(result, nodes)
    
  def qubo2problem(self, qubo) -> Problem:
    self.terms: List[Term] = []
    self.nodes = []
    
    for (node0, node1), energy in qubo.items():
        if node0 == node1:
            self.nodes.append(node0)
#    print(self.nodes)
    
    # Expand the squared summation
    for i in range(len(self.nodes)):
        for j in range(len(self.nodes)):
            energy = qubo.get((self.nodes[i], self.nodes[j]))
#            print(i, ".", self.nodes[i], j, ".", self.nodes[j], "=", energy)
            if energy != None:
                self.terms.append(
                    Term(
                        c = energy,
                        indices = [i, j]
                    )
                )
#    print(self.terms)
    # Return an Ising-type problem
    self.problem = Problem(name="Qubo conversion", 
                           problem_type=ProblemType.pubo, 
                           terms=self.terms)
    
        
  def solve(self):
    # Print a summary of the result
    d5osample = {}
    for qbit in self.sample:
        value = self.sample[qbit]
        name = self.nodes[int(qbit)]
        d5osample[name] = value
    samples = []
    samples.append(d5osample)
 #   print(f'The samples are: {samples}')
    self.assign.add(samples)
