# -*- coding: utf-8 -*-
"""
Created on Fri Sec 22 19:37:12 2023

@author: Nebojsa.Vojinovic
"""
import dann5.d5 as d5
from dann5.d5o import QuboCompiler, QuboAnalyzer, D5QuboSolver
from dann5.dwave import Solver, QuboSolvers, DwaveExactSolver, \
                                   DwaveHybridSolver, DwaveAdvantageSolver, \
                                   DwaveAdvantage2Solver

def basic_types():
    a0 = d5.Qbit("0a", 1)
    print(a0.toString(), a0.toString(False, 0),
          a0.toString(True), a0.toString(True, 0))
    
    b = d5.Qbool("boolean", d5.Qbool.true())
    print(b.toString(False, d5.AllBits()), b.toString(False, 0),
          b.toString(True, d5.AllBits()), b.toString(True, 0))
    
    a = d5.Qbin(3, "a")
    print(a.toString(), a.toString(False, 0),
          a.toString(True), a.toString(True, 0))
    
    x = d5.Qwhole("x", 33)
    print(x.toString(), x.toString(False, 0),
          x.toString(True), x.toString(True, 0))
    
    y = d5.Qint(4, "y", d5.Bits(0b10101))
    print(y.toString(), y.toString(False, 0),
          y.toString(True), y.toString(True, 0))

def qbit_test():
    print("\n\n==== qbit_test() =====")
    a0 = d5.Qbit("0a", 1) 
    a1 = d5.Qbit("1a")
    a2 = d5.Qbit("2a", 5)
    a3 = d5.Qbit("3a", 0)
    a4 = d5.Qbit("4a")
    ar = d5.Qbit("ar",1)
    qAssign = ar._((a0 & a1) | ((a2 ^ a3) == a4))
    print("\n {} \n\n {}\n".format(qAssign.toString(), 
                                   qAssign.toString(True)))
    compiler = QuboCompiler()
    fllCmplr = QuboCompiler(False)
    qAssign.compile(compiler)
    qAssign.compile(fllCmplr)
    qubo = compiler.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        fllCmplr.qubo(), qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    print("Nodes: {}\n\nBranches: {}".format(
        analyze.nodes(), analyze.branches()))
    qAssign.solve()
    print("d5 simulation solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    solver = DwaveExactSolver()
    qAssign.add(solver.solution(qubo))
    print("DWave EXACT solver solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    Solver.SetActive(DwaveExactSolver())
    print("ACTIVE DWave EXACT solver solutions: \n{}\n".format(qAssign.solve()))
    


def qbool_test():
    print("\n\n==== qbool_test() =====")
    b0 = d5.Qbool("0b")
    b1 = d5.Qbool("1b", d5.Qbool.false())
    b2 = d5.Qbool("2b", 33)
    b3 = d5.Qbool("3b")
    b4 = d5.Qbool("4b")
    br = d5.Qbool("br", d5.Qbool.true())
    qAssign = br._(((b3 != b4) & (b2 == b0)) | b1)
    print("\n {} \n\n {}\n".format(qAssign.toString(), 
                                   qAssign.toString(True)))
    compiler = QuboCompiler()
    fllCmplr = QuboCompiler(False)
    qAssign.compile(compiler)
    qAssign.compile(fllCmplr)
    qubo = compiler.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        fllCmplr.qubo(), qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    qAssign.solve()
    print("Active DWave exact solver solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    solver = DwaveHybridSolver()
    qAssign.add(solver.solution(qubo))
    print("DWave HYBRID solver solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    Solver.SetActive(solver)
    print("Active DWave HYBRID solver solutions: \n{}\n".format(qAssign.solve()))


def qbin_test():
    print("\n\n==== qbin_test() =====")
    bn0 = d5.Qbin("bn0", d5.Bits(0o03))
    bn1 = d5.Qbin(3, "bn1")
    bn2 = d5.Qbin(4, "bn2")
    bn3 = d5.Qbin("bn3", d5.Bits(0b110))
    bn4 = d5.Qbin(2, "bn4")
    bnr = d5.Qbin("bnr", d5.Bits(0x5))
    qAssign = bnr._(((bn0 & bn1) | ((bn2 ^ bn3) == bn4)))
    print("\n {} \n\n {}\n".format(qAssign.toString(), 
                                   qAssign.toString(True)))
    compiler = QuboCompiler()
    fllCmplr = QuboCompiler(False)
    qAssign.compile(compiler)
    qAssign.compile(fllCmplr)
    qubo = compiler.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        fllCmplr.qubo(), qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    qAssign.solve()
    print("Active DWave hybrid solver solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    solver = DwaveAdvantageSolver()
    qAssign.add(solver.solution(qubo))
    print("DWave ADVANTAGE solver solutions: \n{}\n".format(qAssign.solutions()))
    qAssign.reset()
    Solver.SetActive(solver)
    print("Active DWave ADVANTAGE solver solutions: \n{}\n".format(qAssign.solve()))


def qwholeAdd_test():
    print("\n\n==== qwholeAdd_test() =====")
    a = d5.Qwhole(4,"a")
    b = d5.Qwhole(2, "b")
    c = d5.Qwhole(2, "c")
    d = d5.Qwhole(1,"d")
    A = d5.Qwhole("A", 21)
    _1 = d5.Qwhole("1", 1)
    aA = A._(a + b + c + d + _1)
    print("\n {} \n\n {}\n".format(aA.toString(), 
                                   aA.toString(True)))
    compiler = QuboCompiler()
    fllCmplr = QuboCompiler(False)
    aA.compile(compiler)
    aA.compile(fllCmplr)
    qubo = compiler.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        fllCmplr.qubo(), qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    aA.solve()
    print("Active DWave Advantage solver solutions: \n{}\n".format(aA.solutions()))
    aA.reset()
    solver = DwaveAdvantage2Solver()
    aA.add(solver.solution(qubo))
    print("DWave ADVANTAGE 2 solver solutions: \n{}\n".format(aA.solutions()))
    aA.reset()
    Solver.SetActive(solver)
    print("Active DWave ADVANTAGE 2 solver solutions: \n{}\n".format(aA.solve()))


def qwholeX_test():
    print("\n\n==== qwholeX_test() =====")
    p = d5.Qwhole(3,"p")
    q = d5.Qwhole(2, "q")
    r = d5.Qwhole(2, "r")
    M = d5.Qwhole("M", 18)
    mM = M._(p * q * r)
    print("\n {} \n\n {}\n".format(mM.toString(), 
                                   mM.toString(True)))
    compiler = QuboCompiler()
    fllCmplr = QuboCompiler(False)
    mM.compile(compiler)
    mM.compile(fllCmplr)
    qubo = compiler.qubo()
    print("\n--- Logic Qubo --- {} \n\n --- Reduced discrete values Qubo --- {}\n".format(
        fllCmplr.qubo(), qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("Active DWave Advantage 2 solver solutions: \n{}\n".format(mM.solutions()))
    mM.reset()
    solver = D5QuboSolver()
    mM.add(solver.solution(qubo))
    print("dann5 solver solutions: \n{}\n".format(mM.solutions()))


def qwholeXlarge_test():
    print("\n\n==== qwholeXlarge_test() =====")
    p = d5.Qwhole(5,"p")
    q = d5.Qwhole(5, "q")
    r = d5.Qwhole(5, "r")
    M = d5.Qwhole("M", 5580)
    mM = M._(p * q * r)
    print("\n {} \n".format(mM.toString()))
    compiler = QuboCompiler()
    mM.compile(compiler)
    qubo = compiler.qubo()
    print("\n--- Reduced discrete values Qubo --- {}\n".format(qubo))
    analyze = QuboAnalyzer(qubo)
    print("# of nodes: {}\t# of branches: {}".format(
        analyze.nodesNo(), analyze.branchesNo()))
    mM.solve()
    print("Active DWave Advantage 2 solver solutions: \n{}\n".format(mM.solutions()))
    mM.reset()
    solver = DwaveHybridSolver()
    mM.add(solver.solution(qubo))
    print("Dwave hybrid solver solutions: \n{}\n".format(mM.solutions()))

def qwholeLt_test():
    print("\n\n==== qwholeLt_test() =====")
    x = d5.Qwhole(2,"x")
    y = d5.Qwhole(2, "y")
    comp = x < y
    compArg = d5.Qbinder() << x << y
    print("\n {} \n".format(comp.toString()))
    compiler = QuboCompiler()
    comp.compile(compiler)
    qubo = compiler.qubo()
    print("\n--- Reduced discrete values Qubo --- {}\n".format(qubo))
    solver = D5QuboSolver()
    comp.add(solver.solution(qubo))
    compArg.add(solver.solution())
    print("# of nodes: {}\t# of branches: {}".format(
        solver.nodesNo(), solver.branchesNo()))
    print("LT solutions block: \n{}\nLT binder: \n{}\n".format( \
                                    comp.solutions(), compArg.solutions()))
    Solver.SetActive(solver)


def qwholeLe_test():
    print("\n\n==== qwholeLe_test() =====")
    x = d5.Qwhole(2,"x")
    y = d5.Qwhole(2, "y")
    comp = x <= y
    print("\n {} \n".format(comp.toString()))
    compiler = QuboCompiler()
    comp.compile(compiler)
    qubo = compiler.qubo()
    print("\n--- Reduced discrete values Qubo --- {}\n".format(qubo))
    comp.solve()
    solver = Solver.Active()
    print("# of nodes: {}\t# of branches: {}".format(
        solver.nodesNo(), solver.branchesNo()))
    print("LE solutions block: \n{}\n".format(comp.solutions()))

def qwholeGt_test():
    print("\n\n==== qwholeGt_test() =====")
    x = d5.Qwhole(2,"x")
    y = d5.Qwhole(2, "y")
    comp = x > y
    print("\n {} \n".format(comp.toString()))
    compiler = QuboCompiler()
    comp.compile(compiler)
    qubo = compiler.qubo()
    print("\n--- Reduced discrete values Qubo --- {}\n".format(qubo))
    comp.solve()
    solver = Solver.Active()
    print("# of nodes: {}\t# of branches: {}".format(
        solver.nodesNo(), solver.branchesNo()))
    print("GT solutions block: \n{}\n".format(comp.solutions()))

def qwholeGe_test():
    print("\n\n==== qwholeGe_test() =====")
    x = d5.Qwhole(2,"x")
    y = d5.Qwhole(2, "y")
    comp = x >= y
    print("\n {} \n".format(comp.toString()))
    compiler = QuboCompiler()
    comp.compile(compiler)
    qubo = compiler.qubo()
    print("\n--- Reduced discrete values Qubo --- {}\n".format(qubo))
    comp.solve()
    solver = Solver.Active()
    print("# of nodes: {}\t# of branches: {}".format(
        solver.nodesNo(), solver.branchesNo()))
    print("GE solutions block: \n{}\n".format(comp.solutions()))

    
def main():
    basic_types()
    
    Solver.Active()   # activates default D5QuboSolver
    #Solver.SetActive(QuboSolvers.solver("exact"))

    qbit_test()
    qbool_test()
    qbin_test()
    qwholeAdd_test()
    qwholeX_test()
    qwholeXlarge_test()
    qwholeLt_test()
    qwholeLe_test()
    qwholeGt_test()
    qwholeGe_test()


if __name__ == "__main__":
    main()