# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o2 as d5o
from dann5.azure import QuantumRequest

import time

def qwholeXlarge_test() -> d5o.Qassignment:
    print("\n\n==== qwholeXlarge_test() =====")
    p = d5o.Qwhole(2,"p")
    q = d5o.Qwhole(2, "q")
    #r = d5o.Qwhole(5, "r")
    M = d5o.Qwhole("M", 3)
    mM = M.assign(p + q) # * r)
    print(f"\n{mM.toString()}\n{mM.toString(True)}")
    qubo = mM.qubo()
    analyze = d5o.Qanalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("d5o simulation solutions: \n{}\n".format(mM.solutions()))
    
    return mM;
    
def main():
#    basic_types()
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
    
