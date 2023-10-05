# -*- coding: utf-8 -*-
"""
Created on Tue Aug 10 17:12:10 2021

@author: Nebojsa.Vojinovic
"""

from dimod import ExactSolver
from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler
from dann5.d5o2 import SampleEng, Qanalyzer, Qsolver

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
class Solvers:
    """
    Initialize Solvers by provinding num_reads to be executed on DWave qpu's 
    before the solutions are returned
    """
    def __init__(self, num_reads):
        self.mSolvers = {}
        self.mKwargs = {'num_reads': num_reads,
                        'answer_mode': 'histogram',
                        }

    """
    Connect to remote DWave qpu's or hybrid BQM solver or initialize DWave's 
    (local) exact simulator
    """
    def connect(self, solverType):
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
    

    """
    Returns solutions to a qubo function as a list of dann5.d502.SampleEng 
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
    def solve(self, solverType, qubo, exact = True):
        if solverType == 'dann5':
            solver = Qsolver(qubo)
            return solver.solution()
        try:
            solver = self.mSolvers[solverType]
        except KeyError:
            solver = self.connect(solverType)
        if solverType in ['Advantage2', 'Advantage', '2000Q']:
            analyzer = Qanalyzer(qubo)
            self.mKwargs['chain_strength'] = analyzer.chainStrength()
            sampleset = solver.sample_qubo(qubo, **self.mKwargs)
        else:
            sampleset = solver.sample_qubo(qubo)
        if exact:
            lowEnergy = sampleset.lowest().record['energy'][0]
            samples = [SampleEng(dict(sample), lowEnergy) for sample in sampleset.lowest().samples()]
        else:
            rowSamples = sampleset.samples()
            energies = sampleset.data_vectors['energy']
            #samples = [SampleEng(dict(rowSamples[i]), energies[i]) for i in range(len(rowSamples))]            
            samples = []
            for i in range(len(rowSamples)):
                sample = dict(rowSamples[i])
                energy = energies[i]
                samples.append(SampleEng(sample, energy))
        return samples
