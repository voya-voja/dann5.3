# -*- coding: utf-8 -*-
"""
Created on Sat Aug  7 17:48:12 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o2 as d5o
from dann5.dwave import Solvers as DwaveSolvers

from dimod import ExactSolver

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
    
    x = d5o.Qwhole("x", 33)
    print(x.toString(), x.toString(False, 0),
          x.toString(True), x.toString(True, 0))

def qbit_test(solvers):
    print("\n\n==== qbit_test() =====")
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
    qAssign.reset()
    samples = solvers.solve('Exact', qubo)
    qAssign.add(samples)
    print("Dwave simulation solutions: \n{}\n".format(qAssign.solutions()))

#    samples = solvers.solve('Advantage', qubo)
#    qAssign.solutions(samples)
#    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))


def qbool_test(solvers):
    print("\n\n==== qbool_test() =====")
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
    qAssign.reset()
    samples = solvers.solve('Exact', qubo)
    qAssign.add(samples)
    print("Dwave simulation solutions: \n{}\n".format(qAssign.solutions()))
#    samples = solvers.solve('Advantage', qubo)
#    qAssign.solutions(samples)
#    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))


def qbin_test(solvers):
    print("\n\n==== qbin_test() =====")
    bn0 = d5o.Qbin("bn0", d5o.Bits(0o03))
    bn1 = d5o.Qbin(3, "bn1")
    bn2 = d5o.Qbin(4, "bn2")
    bn3 = d5o.Qbin("bn3", d5o.Bits(0b110))
    bn4 = d5o.Qbin(2, "bn4")
    bnr = d5o.Qbin("bnr", d5o.Bits(0x5))
    qAssign = bnr.assign(((bn0 & bn1) | ((bn2 ^ bn3) == bn4)))
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
    qAssign.reset()
#    samples = solvers.solve('Exact', qubo)
#    qAssign.solutions(samples)
#    print("Dwave simulation solutions: \n{}\n".format(qAssign.solutions()))
    samples = solvers.solve('Advantage', qubo)
    qAssign.add(samples)
    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))


def qwholeAdd_test(solvers):
    print("\n\n==== qwholeAdd_test() =====")
    a = d5o.Qwhole(4,"a")
    b = d5o.Qwhole(2, "b")
    c = d5o.Qwhole(2, "c")
    d = d5o.Qwhole(1,"d")
    A = d5o.Qwhole("A", 21)
    _1 = d5o.Qwhole("1", 1)
    aA = A.assign(a + b + c + d + _1)
    print("\n {} \n\n {}\n".format(aA.toString(), 
                                   aA.toString(True)))
    qubo = aA.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        aA.qubo(False), qubo))
    analyze = d5o.Qanalyzer(aA.qubo(True, d5o.AllBits()))
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    aA.solve()
    print("d5o simulation solutions: \n{}\n".format(aA.solutions()))
    aA.reset()
    samples = solvers.solve('Exact', qubo)
    aA.add(samples)
    print("Dwave simulation solutions: \n{}\n".format(aA.solutions()))
#    samples = solvers.solve('Advantage', qubo)
#    qAssign.solutions(samples)
#    print("DWave Advantage solutions: \n{}\n".format(qAssign.solutions()))


def qwholeX_test(solvers):
    print("\n\n==== qwholeX_test() =====")
    p = d5o.Qwhole(3,"p")
    q = d5o.Qwhole(2, "q")
    r = d5o.Qwhole(2, "r")
    M = d5o.Qwhole("M", 18)
    mM = M.assign(p * q * r)
    print("\n {} \n\n {}\n".format(mM.toString(), 
                                   mM.toString(True)))
    qubo = mM.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        mM.qubo(False), qubo))
    analyze = d5o.Qanalyzer(mM.qubo(True, d5o.AllBits()))
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("d5o simulation solutions: \n{}\n".format(mM.solutions()))
    mM.reset()    
#    samples = solvers.solve('Exact', qubo)
#    mM.solutions(samples)
#    print("Dwave simulation solutions: \n{}\n".format(mM.solutions()))
    samples = solvers.solve('Advantage', qubo)
    mM.add(samples)
    print("DWave Advantage solutions: \n{}\n".format(mM.solutions()))


def qwholeXlarge_test(solvers):
    print("\n\n==== qwholeXlarge_test() =====")
    p = d5o.Qwhole(5,"p")
    q = d5o.Qwhole(5, "q")
    r = d5o.Qwhole(5, "r")
    M = d5o.Qwhole("M", 5580)
    mM = M.assign(p * q * r)
    print("\n {} \n".format(mM.toString()))
    qubo = mM.qubo()
    analyze = d5o.Qanalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    samples = solvers.solve('Hybrid', qubo)
    mM.add(samples)
    print("DWave Hybrid solutions: \n{}\n".format(mM.solutions()))
    mM.reset()    
    samples = solvers.solve('Advantage', qubo)
    mM.add(samples)
    print("DWave Advantage solutions: \n{}\n".format(mM.solutions()))

def qwholeLt_test(solvers):
    print("\n\n==== qwholeLt_test() =====")
    x = d5o.Qwhole(2,"x")
    y = d5o.Qwhole(2, "y")
    comp = x < y
    print("\n {} \n".format(comp.toString()))
    qubo = comp.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        comp.qubo(False), qubo))
    analyze = d5o.Qanalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    samples = solvers.solve('Exact', qubo)
    comp.add(samples)
    print("DWave simulator solutions: \n{}\n".format(comp.solutions()))
    comp.reset()
    comp.solve()
    print("d5o simulator solutions: \n{}\n".format(comp.solutions()))
    solver = ExactSolver()
    sampleset = solver.sample_qubo(qubo)
    print(sampleset)


def main():
    basic_types()
    solvers = DwaveSolvers(1000, 2)
#    qwholeLt_test(solvers)
#    qbit_test(solvers)
#    qbool_test(solvers)
#    qbin_test(solvers)
    qwholeAdd_test(solvers)
#    qwholeX_test(solvers)
#    qwholeXlarge_test(solvers)

if __name__ == "__main__":
    main()