# -*- coding: utf-8 -*-
"""
Created on Thu Dec 16 14:31:46 2021

@author: Nebojsa.Vojinovic
"""
from qiskit import BasicAer
from qiskit.aqua import aqua_globals, QuantumInstance
from qiskit.aqua.algorithms import QAOA, NumPyMinimumEigensolver
from qiskit.optimization.algorithms import MinimumEigenOptimizer, RecursiveMinimumEigenOptimizer
from qiskit.optimization import QuadraticProgram

# create a QUBO
qubo = QuadraticProgram()
qubo.binary_var('x')
qubo.binary_var('y')
qubo.binary_var('z')
qubo.minimize(linear=[1,-2,3], quadratic={('x', 'y'): 1, ('x', 'z'): -1, ('y', 'z'): 2})
print(qubo.export_as_lp_string())

op, offset = qubo.to_ising()
print('offset: {}'.format(offset))
print('operator:')
print(op)

qp=QuadraticProgram()
qp.from_ising(op, offset, linear=True)
print(qp.export_as_lp_string())

aqua_globals.random_seed = 10598
quantum_instance = QuantumInstance(BasicAer.get_backend('statevector_simulator'),
                                   seed_simulator=aqua_globals.random_seed,
                                   seed_transpiler=aqua_globals.random_seed)
qaoa_mes = QAOA(quantum_instance=quantum_instance, initial_point=[0., 0.])
exact_mes = NumPyMinimumEigensolver()

qaoa = MinimumEigenOptimizer(qaoa_mes)   # using QAOA
exact = MinimumEigenOptimizer(exact_mes)  # using the exact classical numpy minimum eigen solver

exact_result = exact.solve(qubo)
print(exact_result)

qaoa_result = qaoa.solve(qubo)
print(qaoa_result)

from qiskit import IBMQ
provider = IBMQ.load_account()

quantum_instance = QuantumInstance(provider.get_backend('ibmq_qasm_simulator'),
                                   seed_simulator=aqua_globals.random_seed,
                                   seed_transpiler=aqua_globals.random_seed)
qaoa_mes = QAOA(quantum_instance=quantum_instance, initial_point=[0., 0.])
exact_mes = NumPyMinimumEigensolver()

qaoa = MinimumEigenOptimizer(qaoa_mes)   # using QAOA
exact = MinimumEigenOptimizer(exact_mes)  # using the exact classical numpy minimum eigen solver

exact_result = exact.solve(qubo)
print(exact_result)

qaoa_result = qaoa.solve(qubo)
print(qaoa_result)