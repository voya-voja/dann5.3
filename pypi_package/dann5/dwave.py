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

class PyQsolver(Qsolver):
    gActive = D5QuboSolver()
    
    def SetActive(solver):
        PyQsolver.gActive = solver
        Qsolver.Active(solver)
    
    def Active():
        active = Qsolver.Active()
        if(PyQsolver.gActive != active):
            Qsolver.Active(PyQsolver.gActive)
        return PyQsolver.gActive
   
class PyDwaveSolver(DwaveSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave exact solver, a DWave local quantum simulator
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
        #evaluations = [Qevaluation(dict(sample), lowEnergy) for sample in sampleset.lowest().samples()]
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
    def __init__(self, lowest = True):
        """
        Initializes an instance solver and connect to an Advantage sampler 
        """
        DwaveQuantumSolver.__init__(self, lowest)
        try:
            annealer = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
            self.sampler = EmbeddingComposite(annealer)
            print("CONNECTED to {}.".format(annealer.solver.id))
        except SolverNotFoundError:
            print("ERROR: Advantage is unavailable!")
  
class DwaveAdvantage2Solver(DwaveQuantumSolver):
    """
    A dann5 quantum solver that will calcualte quantum evaluations of a quantum
    statement using D-Wave Advantage 2 sampler with 'zephyr' topology
    """
    def __init__(self, lowest = True):
        """
        Initializes an instance solver and connect to an Advantage sampler 
        """
        DwaveQuantumSolver.__init__(self, lowest)
        try:
            annealer = DWaveSampler(solver={'topology__type': 'zephyr', 'qpu': True})
            self.sampler = EmbeddingComposite(annealer)
            print("CONNECTED to {}.".format(annealer.solver.id))
        except SolverNotFoundError:
            print("ERROR: Advantage 2 is unavailable!") 

class Solvers:
    """
    A facade abstracts access to different solvers of optimal functions expressed
    as qubo. It supports following types:
        - 'dann5' => a local quantum simulator
        - 'exact' => DWave local quantum simulator
        - 'Advantage2' => DWave qpu with zephyr topology
        - 'Advantage' => DWave qpu with pegasus topology
        - '2000Q' => DWave qpu with chimera topology
        - 'Hybrid' => DWave hybrid BQM solver
    """
    
    def __init__(self, num_reads):
        """
        Initialize Solvers by provinding num_reads to be executed on DWave qpu's 
        before the solutions are returned
        """
        self.mSolvers = {}
        self.mKwargs = {'num_reads': num_reads,
                        'answer_mode': 'histogram',
                        }

    def connect(self, solverType):
        """
        Connect to remote DWave qpu's or hybrid BQM solver or initialize DWave's 
        (local) exact simulator
        """
        try:
            sampler = None
            if solverType == 'Advantage2':
                sampler = DWaveSampler(solver={'topology__type': 'zephyr', 'qpu': True})
                solver = EmbeddingComposite(sampler)
            elif solverType == 'Advantage':
                sampler = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
                solver = EmbeddingComposite(sampler)
            elif solverType == '2000Q':
                sampler = DWaveSampler(solver={'topology__type': 'chimera', 'qpu': True})
                solver = EmbeddingComposite(sampler)
            elif solverType == 'exact':
                solver = ExactSolver()
            elif solverType == 'Hybrid':
                solver = LeapHybridSampler()
            else:
                print("ERROR: Solver type {} is not supported!".format(solverType))
            
            self.mSolvers[solverType] = solver
            if(sampler != None):
                print("CONNECTED to {}.".format(sampler.solver.id))
            else:
                print("CONNECTED to {}.".format(solver.solver.id))
                
        except SolverNotFoundError:
            print("ERROR: Solver type {} is unavailable.".format(solverType))
        return solver
    

    def solve(self, solverType, qubo, exact = True):
        """
        Returns solutions to a qubo function as a list of dann5.d502.Qevaluation 
        containing samples with lowest energy, e.g. optimal solutions for the given
        qubo.
        The qubo function will be executed using a requested solver based on 
        solverType parameter, which can have one of teh following values:
            - 'dann5' => a local quantum simulator
            - 'exact' => DWave local quantum simulator
            - 'Advantage2' => DWave qpu with zephyr topology
            - 'Advantage' => DWave qpu with pegasus topology
            - '2000Q' => DWave qpu with chimera topology
            - 'Hybrid' => DWave hybrid BQM solver
        """
        if solverType == 'dann5':
            solver = Qsolver(qubo)
            return solver.solution()
        try:
            solver = self.mSolvers[solverType]
        except KeyError:
            solver = self.connect(solverType)
        if solverType in ['Advantage2', 'Advantage', '2000Q']:
            analyzer = QuboAnalyzer(qubo)
            self.mKwargs['chain_strength'] = analyzer.chainStrength()
            sampleset = solver.sample_qubo(qubo, **self.mKwargs)
        else:
            sampleset = solver.sample_qubo(qubo)
        if exact:
            lowEnergy = sampleset.lowest().record['energy'][0]
            samples = [Qevaluation(dict(sample), lowEnergy) for sample in sampleset.lowest().samples()]
        else:
            rowSamples = sampleset.samples()
            energies = sampleset.data_vectors['energy']
            #samples = [Qevaluation(dict(rowSamples[i]), energies[i]) for i in range(len(rowSamples))]            
            samples = []
            for i in range(len(rowSamples)):
                sample = dict(rowSamples[i])
                energy = energies[i]
                samples.append(Qevaluation(sample, energy))
        return samples