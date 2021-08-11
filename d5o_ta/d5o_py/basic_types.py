# -*- coding: utf-8 -*-
"""
Created on Sat Aug  7 17:48:12 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o as d5o
from DwaveSolvers import DwaveSolvers

def basic_types():
    a0 = d5o.Qbit("0a", 1)
    print(a0.toString(), a0.toString(False, 0),
          a0.toString(True), a0.toString(True, 0))
    
    b = d5o.Qbool("boolean", d5o.Qbool.true())
    print(b.toString(False, d5o.AllBits()), b.toString(False, 0),
          b.toString(True, d5o.AllBits()), b.toString(True, 0))
    
    a = d5o.Qbin(3, "a")
    print(a.toString(), a.toString(False, 0),
          a.toString(True), a.toString(True, 0))
    
    x = d5o.Qwhole("x", d5o.Bits(33))
    print(x.toString(), x.toString(False, 0),
          x.toString(True), x.toString(True, 0))

def qbit_test(solvers):
    a0 = d5o.Qbit("0a", 1) 
    a1 = d5o.Qbit("1a")
    a2 = d5o.Qbit("2a", 5)
    ar = d5o.Qbit("ar",1)
    qAssign = ar.assign(a0 & a1 | a2)
    print("\n {} \n\n {}\n".format(qAssign.toString(), 
                                   qAssign.toString(True)))
    qubo = qAssign.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        qAssign.qubo(False), qubo))
    analyze = d5o.Qanalyzer(qAssign.qubo(True, d5o.AllBits()))
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    qAssign.solve()
    print("d5o simulation solutions: \n{}\n".format(qAssign.solutions()))
    
    samples = solvers.solve('Exact', qubo)
    qAssign.solutions(samples)
    print("Dwave simulation solutions: \n{}\n".format(qAssign.solutions()))

#    samples = solvers.solve('Advantage', qubo)
#    qAssign.solutions(samples)
#    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))


def qbool_test(solvers):
    a0 = d5o.Qbit("0a", 1)
    a1 = d5o.Qbit("1a")
    b0 = d5o.Qbool("0b", d5o.Qbool.true())
    b1 = d5o.Qbool("1b", d5o.Qbool.false())
    b2 = d5o.Qbool("2b", 33)
    br = d5o.Qbool("br");
    qAssign = br.assign((a0 != a1) & b2 == b0 | b1)
    print("\n {} \n\n {}\n".format(qAssign.toString(), 
                                   qAssign.toString(True)))
    qubo = qAssign.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        qAssign.qubo(False), qubo))
    analyze = d5o.Qanalyzer(qAssign.qubo(True, d5o.AllBits()))
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    qAssign.solve()
    print("d5o simulation solutions: \n{}\n".format(qAssign.solutions()))
    
    samples = solvers.solve('Exact', qubo)
    qAssign.solutions(samples)
    print("Dwave simulation solutions: \n{}\n".format(qAssign.solutions()))

#    samples = solvers.solve('Advantage', qubo)
#    qAssign.solutions(samples)
#    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))

    
def main():
    basic_types()
    solvers = DwaveSolvers(5000, 5)
    qbit_test(solvers)
    qbool_test(solvers)

if __name__ == "__main__":
    main()