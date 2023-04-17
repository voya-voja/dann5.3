# -*- coding: utf-8 -*-
"""
Created on Fri Feb 17 19:12:42 2023

@author: pc
"""

from UTestQwhole import UTestQwhole
from dann5.dwave import Solvers
#from dann5.dwave import DwaveSolvers
from dann5.d5o2 import Qwhole, Qbin, Qblock, Qanalyzer
from PrimeNumberGenerator import PNGen

def main():
    solvers = Solvers(1000)
    """
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

    """ 
    test = UTestQwhole(solvers)
    test.runAll(0)
    """
    png = PNGen(16)
    png.prime6()

    solvers = Solvers(1000, None)
    for nQbits in range(2,22,2):
        x = Qwhole(nQbits, "x"); y = Qwhole(int(nQbits / 2), "y"); r = Qwhole("r", nQbits);
        xpr = r._( x * y);
        qubo = xpr.qubo()
        a = Qanalyzer(qubo);
        print("{}\n  Nodes: {} Branches: {}".format( xpr.toString(), a.nodesNo(), a.branchesNo()))
        if(solvers != None):
            samples = solvers.solve('Hybrid', qubo)
            xpr.add(samples)
            print("  {}".format(xpr.solutions()))
"""            
    solvers = Solvers(1000)
    x = Qwhole(3, "x"); y = Qwhole(3, "y"); r = Qwhole("r", 18);
    xpr = r._( x * y);
    samples = solvers.solve('Advantage2', xpr.qubo())
    xpr.add(samples)
    print("{}".format(xpr.solutions()))
    
    x = Qwhole(3, "x"); y = Qwhole(3, "y"); r = Qwhole("r", 24);
    xpr = r._( x * y);
    samples = solvers.solve('Advantage', xpr.qubo())
    xpr.add(samples)
    print("{}".format(xpr.solutions()))

    x = Qwhole(3, "x"); y = Qwhole(3, "y"); r = Qwhole("r", 6);
    xpr = r._( x * y);
    samples = solvers.solve('2000Q', xpr.qubo())
    xpr.add(samples)
    print("{}".format(xpr.solutions()))

    for i in range(5):
        x = Qwhole(16, "x"); y = Qwhole(8, "y"); r = Qwhole("r", 42173); 
        _1 = Qwhole("1_", 1)
        xpr = r._( x * y);
        yGt1 = y > _1
        blck = Qblock() << xpr << yGt1
        qubo = blck.qubo()
        a = Qanalyzer(qubo);
        print("{}\n  Nodes: {} Branches: {}".format( xpr.toString(), a.nodesNo(), a.branchesNo()))
        if(solvers != None):
            samples = solvers.solve('Hybrid', qubo)
            xpr.add(samples)
            print("  {}".format(xpr.solutions()))
    
    for i in range(5):
        x = Qwhole(16, "x"); y = Qwhole(8, "y"); r = Qwhole("r", 24509);
        xpr = r._( x * y);
        qubo = xpr.qubo()
        a = Qanalyzer(qubo);
        print("{}\n  Nodes: {} Branches: {}".format( xpr.toString(), a.nodesNo(), a.branchesNo()))
        if(solvers != None):
            samples = solvers.solve('Hybrid', qubo)
            xpr.add(samples)
            print("  {}".format(xpr.solutions()))
        
    for i in range(5):
        x = Qwhole(16, "x"); y = Qwhole(8, "y"); r = Qwhole("r", 24510);
        xpr = r._( x * y);
        yGt1 = y > _1
        blck = Qblock() << xpr << yGt1
        qubo = blck.qubo()
        a = Qanalyzer(qubo);
        print("{}\n  Nodes: {} Branches: {}".format( xpr.toString(), a.nodesNo(), a.branchesNo()))
        if(solvers != None):
            samples = solvers.solve('Hybrid', qubo)
            xpr.add(samples)
            print("  {}".format(xpr.solutions()))

    for i in range(5):
        x = Qwhole(16, "x"); y = Qwhole(8, "y"); r = Qwhole("r", 42170);
        xpr = r._( x * y);
        yGt1 = y > _1
        blck = Qblock() << xpr << yGt1
        qubo = blck.qubo()
        a = Qanalyzer(qubo);
        print("{}\n  Nodes: {} Branches: {}".format( xpr.toString(), a.nodesNo(), a.branchesNo()))
        if(solvers != None):
            samples = solvers.solve('Hybrid', qubo)
            xpr.add(samples)
            print("  {}".format(xpr.solutions()))
            
if __name__ == "__main__":
    main()