# -*- coding: utf-8 -*-
"""
Created on Fri Feb 17 19:12:42 2023

@author: pc
"""

from UTestQwhole import UTestQwhole
from dann5.dwave import Solvers as DwaveSolvers
#from dann5.dwave import DwaveSolvers
from dann5.d5o2 import Qwhole, Qbin, Qblock, Qanalyzer

def main():
    solvers = DwaveSolvers(1000, None)
    #solvers = None
    x = Qwhole(2, "x"); y = Qwhole(2, "y"); z = Qwhole(2, "z");
    _3 = Qwhole("_3", 3);
    qwExpr = x * y;
    z_3Expr = _3 * z;
    qxxExpr = qwExpr * z_3Expr;
    qubo = qxxExpr.qubo()
    print(str(qubo) );
    anlyzeD2 = Qanalyzer(qubo);
    print(" # of nodes: " + str(anlyzeD2.nodesNo()))
    print(" # of branches: " + str(anlyzeD2.branchesNo()));
    if(solvers != None):
        samples = solvers.solve('Hybrid', qubo)
        qxxExpr.add(samples)
        print(qxxExpr.toString() + "\n" + qxxExpr.solutions())

    qxxExpr.reset()
    R = Qwhole("R", 6)
    qxxAssign = R._(qxxExpr)
    aQubo = qxxAssign.qubo()
    print(str(qxxAssign) );
    anlyze = Qanalyzer(aQubo);
    print(" # of nodes: " + str(anlyze.nodesNo()))
    print(" # of branches: " + str(anlyze.branchesNo()));
    if(solvers != None):
        samples = solvers.solve('Hybrid', aQubo)
        qxxAssign.add(samples)
        print(qxxAssign.toString() + "\n" + qxxAssign.solutions())


    test = UTestQwhole(solvers)
    test.runAll(0)

if __name__ == "__main__":
    main()