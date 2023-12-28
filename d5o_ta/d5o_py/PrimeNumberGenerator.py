# -*- coding: utf-8 -*-
"""
Created on Mon Mar 27 11:28:10 2023

@author: pc
"""

from dann5.dwave import QuboSolvers
from dann5.d5o2 import Qwhole, Qblock, Qanalyzer, Qbinder, SampleEng, ULint

from datetime import datetime
import time

class PNrec:
    def __init__(self, logId, pn, fctr, enrgy):
        self.logId = logId
        self.prime = pn
        self.factor = fctr
        self.energy = enrgy
        
    def __repr__(self):
        return "{}, {}, {}, {}".format(self.logId, self.prime, self.factor, self.energy)
    

class Log:
    def __init__(self):
        self.start = time.process_time()
        self.id = datetime.now().strftime("%Y%m%d%H%M%S")
        self.duration = 0
        self.noQbits = 0
        self.nodesNo = 0
        self.brnchNo = 0
        self.solver = ""
        self.solveTime = 0
        self.samplesNo = 0
        self.unqSmplsNo = 0
        self.primesNo = 0
        self.newPrmsNo = 0
        self.pnRecs = []
        self.pns = []

        
    def __repr__(self):
        line = "{}, {}, {}, ".format(self.id, self.duration, self.noQbits)
        line += "{}, {}, ".format(self.nodesNo, self.brnchNo)
        line += "{}, {}, {}, ".format(self.solver, self.solveTime, self.samplesNo)
        line += "{}, {}, {}".format(self.unqSmplsNo, self.primesNo, self.newPrmsNo)
        return line
        
    def add(self, pn, fctr, enrgy):
        if pn not in self.pns:
            self.pns.append(pn)
            self.pns.sort()
            self.pnRecs.append(PNrec(self.id, pn, fctr, enrgy))
    
    # finish log measurements and save the log records
    def finish(self):
        self.primesNo = len(self.pnRecs)
        self.duration = time.process_time() - self.start
        fileCandidates = open("pnRecs.csv", "a")
        [fileCandidates.write("{}\n".format(pnRec)) for pnRec in self.pnRecs]
        fileCandidates.close()
        fileCandidates = open("pnLog.csv", "a")
        fileCandidates.write("{}\n".format(self))
        fileCandidates.close()

# An abstraction of prime number quantum generator
class PNGenerator:
    # to initialize provide number of prime number qbits and optionally
    # if DWave hybrid computer should not be used exclusivery 
    def __init__(self, noQBs, nRuns = 1, justHybrid = False, debug = False):
        self.debug = debug
        self.noQBs = noQBs # number of Quantum Bits
        self.nRuns = nRuns
        self.qPrime = Qwhole(noQBs, "p") # A Quantum whole prime variable 
        self.qPrime1 = Qwhole(noQBs + 1, "p") # A Quantum whole prime variable 
        if(noQBs < 4):
            fNoQBs = int(noQBs/2) + 1
        else:
            fNoQBs = noQBs - 2
        self.qFactor = Qwhole(fNoQBs, "f") # A Quantum whole prime factor variable 
        self.nPNs = []  # a list of new Prime Numbers
        self.loadPrimes() # initialize a list of discovered prime numbers
        self.primeSamples = []
        self.useHybrid = justHybrid # True if only D-Wave hybrid solver should be used
        self.log = None
        
    def loadPrimes(self):
        filePrimes = open("PNs.txt", "r")
        self.primes = []    # a list of discovered prime numbers
        for primeStr in filePrimes:
            self.primes.append(ULint(primeStr))
        if self.debug:
            print("PRIME's:", end=" "); print(self.primes)
        filePrimes.close()
        
    def addPrimes(self):
        if(len(self.nPNs) > 0):
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

    # run quantum code representing an algorithm on a quantum solver and
    # add verified prime number candidates into the list and file with
    # previously discovered prime numbers        
    def run(self, qCode):
        if self.debug:
            print(qCode.toString())
        self.log = Log() # start log measurements
        self.log.noQbits = self.noQBs
        self.primeSamples = self.execute(qCode)
        self.qPrime.add(self.primeSamples)
        self.qPrime1.add(self.primeSamples)
        self.qFactor.add(self.primeSamples)
        self.verify()
        self.addPrimes()
        self.log.finish() # finish log measurements & save it
        self.qPrime.reset()
        self.qPrime1.reset()
        self.qFactor.reset()
        
    # execute quantum code on quantum solver and return samples
    def execute(self, qCode) -> SampleEng:
        qubo = qCode.qubo()
        anlyzer = Qanalyzer(qubo)
        noNodes = anlyzer.nodesNo(); noBrnchs = anlyzer.branchesNo()
        self.log.nodesNo = noNodes; self.log.brnchNo = noBrnchs
        if self.debug:
            print(" # of nodes: " + str(noNodes))
            print(" # of branches: " + str(noBrnchs) + "\n");
        if self.useHybrid or noNodes >= 4000:
            solverType = 'Hybrid';
        elif noNodes < 32:
            solverType = 'dann5';
        elif noNodes < 250:
            solverType = 'Advantage2';
        else: #noNodes < 4000
            solverType = 'Advantage';
        start = time.process_time()
        solver = QuboSolvers.solver(solverType, lowest=False)
        samples = solver.solution(qubo)
        self.log.solver = solverType; self.log.solveTime += time.process_time() - start
        self.log.samplesNo += len(samples); self.log.unqSmplsNo += len(set(samples))
        return samples
    
    # Verified prime-samples will be saved in self.nPNs as ULint's
    def verify(self):
        qF = self.qFactor.ulints()
        for qP in [self.qPrime, self.qPrime1]:
            candidates = qP.ulints()
            if self.debug:
                print("Candidate #'s:,", candidates)
            ulint0 = ULint(0); ulint1 = ULint(1)
            at = 0
            for candidate in candidates:
                if candidate > ulint1 and not(candidate in self.nPNs):
                    for prime in self.primes:
                        if candidate == prime: # The prime is in the list
                            self.log.add(candidate, qF[at], self.primeSamples[at].energy())
                            break
                        elif candidate < prime: # The new prime to be saved
                            self.nPNs.append(candidate)
                            self.log.add(candidate, qF[at], self.primeSamples[at].energy())
                            break
                        if candidate % prime == ulint0:    # The candidate is not prime
                            if self.debug:
                                print("NOT Prime number {} devisible by {}".format(candidate, prime))
                            break
                at += 1
        self.log.newPrmsNo += len(self.nPNs)
        if self.debug:
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
    def __init__(self, noQBs, nRuns = 1, justHybrid = False, debug = False):
        super().__init__(noQBs, nRuns, justHybrid, debug)
        self._6 = Qwhole("6_", 6)
        self._1 = Qwhole("1_", 1)
        #self.min = Qwhole("m", 2**(noQBs-1))
    
    # Quantum code for prime candidate = 6 * n - 1, where n is any whole number
    def minusQcode(self):
        xPrime6m1 = self.qPrime + self._1 == self._6 * self.qFactor
        #xPrimeMin = self.qPrime >= self.min
        blck = Qblock() << xPrime6m1 #<< xPrimeMin
        return blck
    
    # Quantum code for prime candidate = 6 * n + 1, where n is any whole number
    def plusQcode(self):
        asPrime6p1 = self.qPrime._( self._6 * self.qFactor + self._1)
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
    def __init__(self, noQBs, nRuns = 1, justHybrid = False, prime0 = 2, \
                 prime1 = 3, offst2onExpnnt = 0, debug = False):
        super().__init__(noQBs, nRuns, justHybrid, debug)
        b = prime0 * prime1
        self.base = Qwhole("{}_".format(b), b)
        o = 2 ** offst2onExpnnt
        self.offset = Qwhole("{}_".format(o), o)
        #self.min = Qwhole("m", 2**(noQBs-1))
        
    # Quantum code for prime candidate = base * n - offset, where n is any whole number
    def minusQcode(self):
        minusExpr = self.qPrime + self.offset == self.base * self.qFactor
        return minusExpr
        #xPrimeMin = self.qPrime >= self.min
        #blck = Qblock() << minusExpr #<< xPrimeMin
        #return blck
    
    # Quantum code for prime candidate = base * n + offset, where n is any whole number
    def plusQcode(self):
        plusAssignm = self.qPrime._( self.base * self.qFactor + self.offset)
        return plusAssignm
        #xPrimeMin = self.qPrime >= self.min
        #blck = Qblock() << plusAssignm #<< xPrimeMin
        #return blck
    
