# -*- coding: utf-8 -*-
"""
Created on Tue Aug 10 17:12:10 2021

@author: Nebojsa.Vojinovic
"""

from dimod import ExactSolver
from dwave.system import DWaveSampler, EmbeddingComposite
from dwave.cloud.exceptions import SolverNotFoundError
from dwave.system.samplers import LeapHybridSampler
from dann5.d5o2 import SampleEng


class Solvers:
  def __init__(self, num_reads, chain_strength):
    # Return num_reads solutions (responses are in the D-Wave's graph of indexed qubits)
    # strength 5 for R 16, 15, 6, 12 works for both, pegasus and chimera
    self.mKwargs = {'num_reads': num_reads,
                    'answer_mode': 'histogram',
                    }
    if chain_strength != None:
        self.mKwargs['chain_strength'] = chain_strength

    try:
        qpu_advantage2 = DWaveSampler(solver={'topology__type': 'zephyr', 'qpu': True})
        qpu_advantage = DWaveSampler(solver={'topology__type': 'pegasus', 'qpu': True})
        qpu_2000q = DWaveSampler(solver={'topology__type': 'chimera', 'qpu': True})
        exact = ExactSolver()
        hybrid = LeapHybridSampler()
        
        self.mSolvers = {'Advantage2': EmbeddingComposite(qpu_advantage2), 
                         'Advantage': EmbeddingComposite(qpu_advantage), 
                         'DW-2000Q': EmbeddingComposite(qpu_2000q), 
                         'Exact': exact,
                         'Hybrid': hybrid
                         }
    
        print(">>>> Connected to Advantage2 {}, Advantage {} and 2000Q {}.".format(qpu_advantage2.solver.id, qpu_advantage.solver.id, qpu_2000q.solver.id))
    except SolverNotFoundError:
        print(">>>> Currently a pair of solvers are unavailable for sections comparing QPU technologies. Try those examples later.")
    

  def solve(self, solverType, qubo):
    solver = self.mSolvers[solverType]
    if solverType == 'Advantage':
        sampleset = solver.sample_qubo(qubo, **self.mKwargs)
    else:
        sampleset = solver.sample_qubo(qubo)
    lowEnergy = sampleset.lowest().record['energy'][0]
    samples = [SampleEng(dict(sample), lowEnergy) for sample in sampleset.lowest().samples()]
    return samples
