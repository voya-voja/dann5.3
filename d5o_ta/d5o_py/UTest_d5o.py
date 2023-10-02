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

def prime(p0, p1):
    for k in range(p0):
        for f in range(1, 100, 2):
            p = f * p0 * p1 - 2 ** (k + 1)
            print(p)
            p = f * p0 * p1 + 2 ** (k + 1)
            print(p)

def rsa(primeP, primeQ, message):
    publikkeyN = primeP * primeQ
    baseD = (primeP-1) * (primeQ-1)
    publickKeyE = 2
    for publickKeyE in range(7, baseD):
        if baseD % publickKeyE != 0:
            break
    publickKeyE = 7
    secretD = int((baseD + 1) / publickKeyE)
    encrypt = (message ** publickKeyE) % publikkeyN
    decrypt = (encrypt ** secretD) % publikkeyN
    print("N: {}, base-D: {}, e: {}, d: {}, encrypt: {}, decrypt: {}".format(publikkeyN, baseD, publickKeyE, secretD, encrypt, decrypt))
    
    
    
    
def main():
    rsa(11, 17, 99) # only works when public-key e is 7 for message in 2-186
    rsa(3, 11, 23)   # works for mesage in 2-32 
    rsa(13,19, 88)
    #prime(3,5)
    #solvers = Solvers(1000)

    #test = UTestQwhole(solvers)
    #test.runAll(0)
"""    
    png = PNGen(4, False)
    png.prime6()

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