# -*- coding: utf-8 -*-
"""
Created on Mon Mar 27 11:28:10 2023

@author: pc
"""

from dann5.dwave import Solvers
from dann5.d5o2 import Qwhole, Qblock, Qanalyzer, Qbinder, SampleEng, ULint

# An abstraction of prime number quantum generator
class PNGenerator:
    # to initialize provide number of prime number qbits and optionally
    # if DWave hybrid computer should not be used exclusivery 
    def __init__(self, noQBs, nRuns = 1, justHybrid = False):
        self.noQBs = noQBs # number of Quantum Bits
        self.nRuns = nRuns
        self.qPrime = Qwhole(noQBs, "p") # A Quantum whole prime variable 
        self.nPNs = []  # a list of new Prime Numbers
        self.solvers = Solvers(1000) # A list of available quantum problem solvers
        self.useHybrid = justHybrid # True if only D-Wave hybrid solver should be used
        self.loadPrimes() # initialize a list of discovered prime numbers
        self.primeSamples = []
        
    def loadPrimes(self):
        filePrimes = open("PNs.txt", "r")
        self.primes = []    # a list of discovered prime numbers
        for primeStr in filePrimes:
            self.primes.append(ULint(primeStr))
        print("PRIME's:", end=" "); print(self.primes)
        filePrimes.close()
        
    def addPrimes(self):
        if(len(self.nPNs) > 0):
            fileCandidates = open("pnCandidates.txt", "a")
            [fileCandidates.write("\n{}".format(str(pn))) for pn in self.nPNs]
            fileCandidates.close()
            self.primes.extend(self.nPNs)
            self.nPNs.clear()
            self.primes.sort()
            filePrimes = open("PNs.txt", "w")
            [filePrimes.write("{}\n".format(str(pn))) for pn in self.primes]
            filePrimes.close()
    
    # override to define prime number quantum code
    #@abstractmethod
    def minusQcode(self):
        pass
    
    # override to define prime number quantum code
    #@abstractmethod
    def plusQcode(self):
        pass
    
    # generate prime numbers by running an algorithm on a quantum solver 
    def generate(self):
        for cRuns in range(0,self.nRuns):
            self.run(self.minusQcode())
            self.run(self.plusQcode())
            self.verify()
            self.addPrimes()

    # run quantum code representing an algorithm on a quantum solver and
    # add verified prime number candidates into the list and file with
    # previously discovered prime numbers        
    def run(self, qCode):
        print(qCode.toString())
        self.primeSamples = self.execute(qCode)
        write(self.primeSamples)
        self.qPrime.add(self.primeSamples)
        
    # execute quantum code on quantum solver and return samples
    def execute(self, qCode) -> SampleEng:
        qubo = qCode.qubo()
        anlyzeD2 = Qanalyzer(qubo);
        noNodes = anlyzeD2.nodesNo()
        print(" # of nodes: " + str(noNodes))
        print(" # of branches: " + str(anlyzeD2.branchesNo()) + "\n");
        if self.useHybrid or noNodes >= 4000:
            samples = self.solvers.solve('Hybrid', qubo, exact = False)
        elif noNodes < 32:
            samples = qCode.compute()
        elif noNodes < 250:
            samples = self.solvers.solve('Advantage2', qubo, exact = False)
        else: #noNodes < 4000
            samples = self.solvers.solve('Advantage', qubo, exact = False)
        return samples
    
    # Verified prime-samples will be saved in self.nPNs as ULint's
    def verify(self):
        candidates = self.qPrime.ulints()
        print("Candidate #'s:,", candidates)
        ulint0 = ULint(0, True); ulint1 = ULint(1, True)
        for candidate in candidates:
            if candidate > ulint1:  
                for prime in self.primes:
                    if candidate == prime: # The prime is in the list
                        break
                    elif candidate < prime: # The new prime to be saved
                        self.nPNs.append(candidate)
                        break
                    if candidate % prime == ulint0:    # The candidate is not prime
                        #print("NOT Prime number {} devisible by {}".format(candidate, prime))
                        break
        print("Prime #'s:,", self.nPNs)

    def factorTest(self, result) -> bool:
        r = Qwhole('r', result)
        m = Qwhole(self.qPrime.noqbs(), 'm')
        n = Qwhole(round(self.qPrime.noqbs()/2), 'n')
        asFactor = r._(m * n)
        factorSamples = self.execute(asFactor)
        factors = Qbinder() << r << m << n
        factors.add(factorSamples)
        if len(factorSamples) > 1:
            return False
        n.add(factorSamples)
        if n.results()[0] != 1:
            return False
        return True

# Prime numbers genereator specialization using 6 * n base algorithm
class PNGen6(PNGenerator):
    # to initialize provide number of prime number qbits and optionally
    # if DWave hybrid computer should not be used exclusivery 
    def __init__(self, noQBs, nRuns = 1, justHybrid = False):
        super().__init__(noQBs, nRuns, justHybrid)
        self._6 = Qwhole("6_", 6)
        self._1 = Qwhole("1_", 1)
        if(noQBs < 4):
            fNoQBs = int(noQBs/2) + 1
        else:
            fNoQBs = noQBs - 2
        #self.min = Qwhole("m", 2**(noQBs-1))
        self.factor = Qwhole(fNoQBs, "f") # A Quantum whole prime factor variable 
    
    # Quantum code for prime candidate = 6 * n - 1, where n is any whole number
    def minusQcode(self):
        xPrime6m1 = self.qPrime + self._1 == self._6 * self.factor
        #xPrimeMin = self.qPrime >= self.min
        blck = Qblock() << xPrime6m1 #<< xPrimeMin
        return blck
    
    # Quantum code for prime candidate = 6 * n + 1, where n is any whole number
    def plusQcode(self):
        asPrime6p1 = self.qPrime._( self._6 * self.factor + self._1)
        #xPrimeMin = self.qPrime >= self.min
        blck = Qblock() << asPrime6p1 #<< xPrimeMin
        return blck
    
    
# Generic prime numbers genereator specialization
class PNGen(PNGenerator):
    # to initialize provide number of prime number qbits and optionally
    # if DWave hybrid computer should not be used exclusivery 
    # prime0 and prime1 are used to initialize factor multiplier (base)
    # 2 ** offsetExponent is prime number offset from multiplied factor
    # when leaving default values it is the same as PNGen6
    def __init__(self, noQBs, nRuns = 1, justHybrid = False, prime0 = 2, prime1 = 3, offsetExponent = 0):
        super().__init__(noQBs, nRuns, justHybrid)
        b = prime0 * prime1
        self.base = Qwhole("{}_".format(b), b)
        o = 2 ** offsetExponent
        self.offset = Qwhole("{}_".format(o), o)
        if(noQBs < 4):
            fNoQBs = int(noQBs/2) + 1
        else:
            fNoQBs = noQBs - 2
        #self.min = Qwhole("m", 2**(noQBs-1))
        self.factor = Qwhole(fNoQBs, "f") # A Quantum whole prime factor variable 
        
    # Quantum code for prime candidate = base * n - offset, where n is any whole number
    def minusQcode(self):
        minusExpr = self.qPrime + self.offset == self.base * self.factor
        return minusExpr
        #xPrimeMin = self.qPrime >= self.min
        #blck = Qblock() << minusExpr #<< xPrimeMin
        #return blck
    
    # Quantum code for prime candidate = base * n + offset, where n is any whole number
    def plusQcode(self):
        plusAssignm = self.qPrime._( self.base * self.factor + self.offset)
        return plusAssignm
        #xPrimeMin = self.qPrime >= self.min
        #blck = Qblock() << plusAssignm #<< xPrimeMin
        #return blck
    
