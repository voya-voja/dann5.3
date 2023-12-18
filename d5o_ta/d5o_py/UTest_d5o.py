# -*- coding: utf-8 -*-
"""
Created on Fri Feb 17 19:12:42 2023

@author: pc
"""

from UTestQwhole import UTestQwhole
from dann5.dwave import Solvers
#from dann5.dwave import DwaveSolvers
from dann5.d5o2 import Qwhole, Qbin, Qblock, Qanalyzer
from PrimeNumberGenerator import PNGen, PNGen6

import time

def rsa(primeP, primeQ, message):
    publicKeyN = primeP * primeQ
    baseFI = (primeP-1) * (primeQ-1)
    publicKeyE = 2
    for publicKeyE in range(2, baseFI):
        if baseFI % publicKeyE != 0:
            break
    #publicKeyE = 7
    secretD = int((baseFI + 1) / publicKeyE)
    encrypt = (message ** publicKeyE) % publicKeyN
    decrypt = (encrypt ** secretD) % publicKeyN
    print("N: {}, base-D: {}, e: {}, d: {}, encrypt: {}, decrypt: {}".format(publicKeyN, baseFI, publicKeyE, secretD, encrypt, decrypt))
    
    
    
    
def main():
    #rsa(11, 17, 99) # only works when public-key e is 7 for message in 2-186
    #rsa(3, 11, 23)   # works for mesage in 2-32 
    #rsa(13,19, 88)
    #prime(3,5)
    #solvers = Solvers(1000)

    #test = UTestQwhole(solvers)
    #test.runAll(0)
    
    #x = Qwhole(16, "x")
    #qbsNo = x.noqbs()    # ERROR
    
    start = time.process_time()
    png = PNGen(18, 20, debug = False)
    png.generate()
    finish = time.process_time()
    print("PN candidate generation takes", finish - start, "seconds")
"""    
    start = time.process_time()
    png = PNGen(4, False)
    png.generate()
    finish = time.process_time()
    print("PN candidate generation takes", finish - start, "seconds")
    start = time.process_time()
    png = PNGen(4, False, 3, 5, 1)
    png.generate()
    finish = time.process_time()
    print("PN candidate generation takes", finish - start, "seconds")

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