# -*- coding: utf-8 -*-
"""
Created on Sat Aug  7 17:48:12 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o_d as d5o
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
    a3 = d5o.Qbit("3a", 0)
    a4 = d5o.Qbit("4a")
    ar = d5o.Qbit("ar",1)
    qAssign = ar.assign((a0 & a1) | ((a2 ^ a3) == a4))
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
    b0 = d5o.Qbool("0b")
    b1 = d5o.Qbool("1b", d5o.Qbool.false())
    b2 = d5o.Qbool("2b", 33)
    b3 = d5o.Qbool("3b")
    b4 = d5o.Qbool("4b")
    br = d5o.Qbool("br", d5o.Qbool.true())
    qAssign = br.assign(((b3 != b4) & (b2 == b0)) | b1)
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


def qbin_test(solvers):
    bn0 = d5o.Qbin(3, "bn0")
    bn1 = d5o.Qbin(3, "bn1")
    bn2 = d5o.Qbin("bn2", d5o.Bits(6))
    bn3 = d5o.Qbin(3, "bn3")
    bn4 = d5o.Qbin(3, "bn4")
    bnr = d5o.Qbin("bnr", d5o.Bits(0b011))
    qExpr = bn3 & bn4
    qAssign = bnr.assign(((bn3 != bn4) & (bn2 == bn0)) | bn1)
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
    qbin_test(solvers)

if __name__ == "__main__":
    main()