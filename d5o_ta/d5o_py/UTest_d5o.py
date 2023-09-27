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
    #solvers = Solvers(1000)

    #test = UTestQwhole(solvers)
    #test.runAll(0)
    
    png = PNGen(24, False)
    png.prime6()
"""
    solvers = Solvers(1000)
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

        
if __name__ == "__main__":
    main()