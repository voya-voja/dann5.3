# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o2 as d5o

# This allows you to connect to the Workspace you've previously deployed in Azure.
# Be sure to fill in the settings below which can be retrieved by running 'az quantum workspace show' in the terminal.
from azure.quantum import Workspace

# Take an array of container weights and return a Problem object that represents the cost function
from typing import List
from azure.quantum.optimization import Problem, ProblemType, Term

# Submit problem to Azure Quantum using the ParallelTempering solver:
from azure.quantum.optimization import ParallelTempering

import time


# Copy the settings for your workspace below
workspace = Workspace (
  subscription_id = "5e9f578c-16d1-4e61-987f-db672c9bf0cb",
  resource_group = "d5o2",
  name = "d5o2",
  location = "eastus"
)

def qubo2problem(qubo) -> Problem:
    terms: List[Term] = []
    nodes = []
    
    for (node0, node1), energy in qubo.items():
        if node0 == node1:
            nodes.append(node0)
    print(nodes)
    
    # Expand the squared summation
    for i in range(len(nodes)):
        for j in range(len(nodes)):
            energy = qubo.get((nodes[i], nodes[j]))
            print(i, ".", nodes[i], j, ".", nodes[j], "=", energy)
            if energy != None:
                terms.append(
                    Term(
                        c = energy,
                        indices = [i, j]
                    )
                )
    print(terms)
    # Return an Ising-type problem
    return Problem(name="Qubo conversion", problem_type=ProblemType.pubo, terms=terms)

def qwholeXlarge_test() -> dict:
    print("\n\n==== qwholeXlarge_test() =====")
    p = d5o.Qwhole(2,"p")
    q = d5o.Qwhole(2, "q")
    #r = d5o.Qwhole(5, "r")
    M = d5o.Qwhole("M", 3)
    mM = M.assign(p + q) # * r)
    print("\n {} \n".format(mM.toString()))
    qubo = mM.qubo()
    analyze = d5o.Qanalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("d5o simulation solutions: \n{}\n".format(mM.solutions()))
    
    return qubo;


# Print out a summary of the results:
def print_result_summary(result, nodes):
    # Print a summary of the result
    ship_a_weight = 0
    ship_b_weight = 0
    '''
    for container in result['configuration']:
            
       container_assignment = result['configuration'][container]
        container_weight = container_weights[int(container)]
        ship = ''
        if container_assignment == 1:
            ship = 'A'
            ship_a_weight += container_weight
        else:
            ship = 'B'
            ship_b_weight += container_weight

        print(f'Container {container} with weight {container_weight} was placed on Ship {ship}')

    print(f'\nTotal weights: \n\tShip A: {ship_a_weight} tonnes \n\tShip B: {ship_b_weight} tonnes\n')
    '''  
    
def main():
#    basic_types()
    qubo = qwholeXlarge_test()
    print(qubo)
    problem = qubo2problem(qubo)
    
    # Instantiate a solver to solve the problem.
    solver = ParallelTempering(workspace, timeout=100)

    # Optimize the problem
    print('\nSubmitting simplified problem...')
    start = time.time()
    result = solver.optimize(problem)
    time_elapsed_simplified = time.time() - start
    print(f'\nResult in {time_elapsed_simplified} seconds:\n{result}\n')
#    print_result_summary(result, nodes)


if __name__ == "__main__":
    main()