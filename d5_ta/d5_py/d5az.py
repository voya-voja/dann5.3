# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
from dann5.d5 import Qwhole, Qassignment
from dann5.d5o import QuboCompiler, QuboAnalyzer
from dann5.azure import QuantumRequest
from dann5.dwave import Solver

import time

def qwholeXlarge_test() -> Qassignment:
    print("\n\n==== qwholeXlarge_test() =====")
    p = Qwhole(2,"p")
    q = Qwhole(2, "q")
    #r = d5o.Qwhole(5, "r")
    M = Qwhole("M", 3)
    mM = M.assign(p * q) # * r)
    print(f"\n{mM.toString()}\n{mM.toString(True)}")
    compiler = QuboCompiler()
    mM.compile(compiler)
    analyze = QuboAnalyzer(compiler.qubo())
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("d5o simulation solutions: \n{}\n".format(mM.solutions()))
    
    return mM;
    
def main():
    Solver.Active()
    assignment = qwholeXlarge_test()
    assignment.reset()
    request = QuantumRequest(assignment)

    print('\nSubmitting simplified problem...')
    start = time.time()
    request.execute()
    time_elapsed_simplified = time.time() - start
    print(f'\nResult returned in {time_elapsed_simplified} seconds:')
    print(assignment.solutions())


if __name__ == "__main__":
    main()
    
