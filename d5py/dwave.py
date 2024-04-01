# -*- coding: utf-8 -*-
"""
Created on Fri Dec 23 10:20:10 2023

@author: Nebojsa.Vojinovic
"""

import dimod
from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler
from dann5.d5 import Qevaluation, Qsolver
from dann5.d5o import QuboAnalyzer, DwaveSolver, D5QuboSolver

class PyDwaveSolver(DwaveSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using a specific D-Wave quantum remote sampler or local simulator
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance of a D-Wave exact solver 
        """
        DwaveSolver.__init__(self, lowest)
        
    def solve(self):
        """
        Computes solutions samples using Dwave exact solver 
        """
        sampleset = self.sampler.sample_qubo(self.qubo())
        self.evaluatuate(sampleset)
        
    def evaluatuate(self, sampleset):
        if(self.lowest()):
            samples = sampleset.lowest().samples()
            energies = sampleset.lowest().data_vectors['energy']
        else:
            samples = sampleset.samples()
            energies = sampleset.data_vectors['energy']
        #evaluations = [Qevaluation(dict(sample), lowEnergy) 
        #                           for sample in sampleset.lowest().samples()]
        evaluations = []
        for i in range(len(samples)):
            sample = dict(samples[i])
            energy = energies[i]
            evaluations.append(Qevaluation(sample, energy))
        self.solution(evaluations)   
        
class DwaveExactSolver(PyDwaveSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave exact solver, a DWave local quantum simulator
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance of a D-Wave exact solver 
        """
        DwaveSolver.__init__(self, lowest)
        self.sampler = dimod.ExactSolver()
  
class DwaveHybridSolver(PyDwaveSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave hybrid sampler (LeapHybridSampler)
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance of an hybrid sampler 
        """
        DwaveSolver.__init__(self, lowest)
        self.sampler = LeapHybridSampler()
        print("CONNECTED to {}.".format(self.sampler.solver.id))

class DwaveQuantumSolver(PyDwaveSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave quantum samplers, including hybrid sampler
    """
    def __init__(self, lowest = True, num_reads = 1000):
        """
        Initializes an instance of a D-Wave quantum solver 
        """
        DwaveSolver.__init__(self, lowest)
        self.mKwargs = {'num_reads': num_reads,
                        'answer_mode': 'histogram',
                        }
                
    def solve(self):
        """
        Computes solutions samples using Dwave quantum sampler 
        """
        self.mKwargs['chain_strength'] = self.chainStrength()
        sampleset = self.sampler.sample_qubo(self.qubo(), **self.mKwargs)
        self.evaluatuate(sampleset)

class DwaveAdvantageSolver(DwaveQuantumSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave Advantage sampler with 'pegasus' topology
    """
    def __init__(self, lowest = True, num_reads = 1000):
        """
        Initializes an instance solver and connect to an Advantage sampler 
        """
        DwaveQuantumSolver.__init__(self, lowest, num_reads)
        try:
            annealer = DWaveSampler(solver={'topology__type': 'pegasus', 
                                                                'qpu': True})
            self.sampler = EmbeddingComposite(annealer)
            print("CONNECTED to {}.".format(annealer.solver.id))
        except SolverNotFoundError:
            print("ERROR: Advantage is unavailable!")
  
class DwaveAdvantage2Solver(DwaveQuantumSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave Advantage 2 sampler with 'zephyr' topology
    """
    def __init__(self, lowest = True, num_reads = 1000):
        """
        Initializes an instance solver and connect to an Advantage sampler 
        """
        DwaveQuantumSolver.__init__(self, lowest, num_reads)
        try:
            annealer = DWaveSampler(solver={'topology__type': 'zephyr',
                                                                'qpu': True})
            self.sampler = EmbeddingComposite(annealer)
            print("CONNECTED to {}.".format(annealer.solver.id))
        except SolverNotFoundError:
            print("ERROR: Advantage 2 is unavailable!")


class QuboSolvers(Qsolver):
    """
    A class facade abstracts an access to different qubo solvers  
    of optimal functions expressed as qubo. It supports following types:
        - 'dann5'      =>   A local dann5 quantum annealing simulator
        - 'exact'      =>   A local DWave quantum annealing simulator
        - 'Advantage2' =>   DWave qpu with zephyr topology
        - 'Advantage'  =>   DWave qpu with pegasus topology
        - 'Hybrid'     =>   DWave hybrid BQM solver
    """
    mSolvers = {}
    """
    A list of initialized (and connected reote) solvers
    """

    def connect(solverType, lowest = True, num_reads = 1000):
        """
        Connect to remote DWave qpu's or hybrid BQM solver or initialize 
        DWave's exact or dann5 (local) simulator
        """
        try:
            if solverType == 'Advantage2':
                solver = DwaveAdvantage2Solver(lowest, num_reads)
            elif solverType == 'Advantage':
                solver = DwaveAdvantageSolver(lowest, num_reads)
            elif solverType == 'exact':
                solver = DwaveExactSolver(lowest)
            elif solverType == 'Hybrid':
                solver = DwaveHybridSolver(lowest)
            elif solverType == 'dann5':
                solver = D5QuboSolver(lowest)
            else:
                print("ERROR: Solver type {} is not supported!".format(
                                                                solverType))
        except SolverNotFoundError:
            print("ERROR: Solver type {} is unavailable.".format(solverType))
        return solver
    

    def solver(solverType, lowest = True, num_reads = 1000):
        """
        Returns a solver for a given solver type.
        If solver is not initialized calls connect(...) method to create 
        a solver and adds the solver to the list of nitialized solvers.
        """
        try:
            solver = QuboSolvers.mSolvers[solverType]
        except KeyError:
            solver = QuboSolvers.connect(solverType, lowest, num_reads)
            QuboSolvers.mSolvers[solverType] = solver
        return solver
 
    
class Solver(Qsolver):
    """
    A class facade sets and returns an active solver, which will be used to solve
    a quantum statement
    """   
    gActive = QuboSolvers.solver("dann5")
    """
    A reference to an active qubo solver used to solve quantum statements
    """
    
    def SetActive(solver):
        """
        Sets an active qubo solver used to solve quantum statements. It sets
        both as python's Solver.gActive and dann5.d5 library static reference
        """
        Solver.gActive = solver
        Qsolver.Active(solver)
    
    def Activate(solverType):
        """
        Sets an active qubo solver used to solve quantum statements. It sets
        both as python's Solver.gActive and dann5.d5 library static reference
        It supports following Solver types:
            - 'dann5'      =>   A local dann5 quantum annealing simulator
            - 'exact'      =>   A local DWave quantum annealing simulator
            - 'Advantage2' =>   DWave qpu with zephyr topology
            - 'Advantage'  =>   DWave qpu with pegasus topology
            - 'Hybrid'     =>   DWave hybrid BQM solver
        """
        solver = QuboSolvers.solver(solverType)
        Solver.gActive = solver
        Qsolver.Active(solver)
        
    def Active():
        """
        Returns an active qubo solver used to solve quantum statements.
        When dann5.d5 library static reference is different than the python's
        Solver.gActive reference, dann5.d5 library is set to reference the same
        solver.
        """
        active = Qsolver.Active()
        if(Solver.gActive != active):
            Qsolver.Active(Solver.gActive)
        return Solver.gActive
   