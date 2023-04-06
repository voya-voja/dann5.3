# -*- coding: utf-8 -*-
"""
Created on Mon Mar 27 11:28:10 2023

@author: pc
"""

from dann5.dwave import Solvers
from dann5.d5o2 import Qwhole, Qblock, Qanalyzer, Qbinder, SampleEng

class PNGen:
    def __init__(self, noQBs):
        self.noQBs = noQBs
        self.prime = Qwhole(noQBs, "p")
        if(noQBs < 4):
            fNoQBs = int(noQBs/2) + 1
        else:
            fNoQBs = noQBs - 2
        self.factor = Qwhole(fNoQBs, "f")
        self._6 = Qwhole("6_", 6)
        self._1 = Qwhole("1_", 1)
        #self.min = Qwhole("m", 2**(noQBs-1))
        self.mPNs = []
        self.qVars = Qbinder() << self.prime << self.factor #<< self.min
        self.solvers = Solvers(1000, None)
        self.primes = [3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,181,233]

        
    def prime6(self):
        finished = False
        while not finished:
            xPrime6m1 = self.prime + self._1 == self._6 * self.factor
            #xPrimeMin = self.prime >= self.min
            blck = Qblock() << xPrime6m1 #<< xPrimeMin
            print(blck.toString())
            samples = self.execute(blck)
            finished = self.likePrime(samples)
        self.qVars.add(samples)
        print(self.qVars.solutions())
        
        finished = False
        while not finished:
            asPrime6p1 = self.prime._( self._6 * self.factor + self._1)
            #xPrimeMin = self.prime >= self.min
            blck = Qblock() << asPrime6p1 #<< xPrimeMin
            print(blck.toString())
            samples = self.execute(blck)
            finished = self.likePrime(samples)
        self.qVars.add(samples)
        print(self.qVars.solutions())
        
    def execute(self, qCode) -> SampleEng:
        qubo = qCode.qubo()
        anlyzeD2 = Qanalyzer(qubo);
        noNodes = anlyzeD2.nodesNo()
        print(" # of nodes: " + str(noNodes))
        print(" # of branches: " + str(anlyzeD2.branchesNo()) + "\n");
        if noNodes < 31:
            samples = qCode.compute()
        elif noNodes < 200:
            samples = self.solvers.solve('Advantage2', qubo)
        elif noNodes < 500:
            samples = self.solvers.solve('Advantage', qubo)
        else:
            samples = self.solvers.solve('Hybrid', qubo)
        return samples
    
    def likePrime(self, samples) -> bool:
        prm = Qwhole(self.prime)
        prm.add(samples)
        for result in prm.results():
            for prm in self.primes:
                if result % prm == 0:
                    return False
            #return self.factorTest(result)
        return True

    def factorTest(self, result) -> bool:
        r = Qwhole('r', result)
        m = Qwhole(self.prime.noqbs(), 'm')
        n = Qwhole(round(self.prime.noqbs()/2), 'n')
        asFactor = r._(m * n)
        factorSamples = self.execute(asFactor)
        factors = Qbinder() << r << m << n
        factors.add(factorSamples)
        print(factors.solutions())
        if len(factorSamples) > 1:
            return False
        n.add(factorSamples)
        if n.results()[0] != 1:
            return False
        return True