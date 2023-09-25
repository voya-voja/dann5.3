# -*- coding: utf-8 -*-
"""
Created on Mon Aug 21 10:25:34 2023

@author: pc
"""

import dann5.d5o2 as d5o

x = d5o.Qbin(2, "x"); y = d5o.Qbin(2, "y"); z = d5o.Qbin(2, "z"); w = d5o.Qbin(2, "w"); 
n = d5o.Qbin(2, "n"); o = d5o.Qbin(2, "o"); p = d5o.Qbin(2, "p"); s = d5o.Qbin(2, "s"); 
_1 = d5o.Qbit("_1", 1); _3 = d5o.Qbin("_3", d5o.Bits(0b11))

expr1 = _3 == ( ~(x ^ y) & (y ^ z) & ~(z ^ w) )
print(expr1)
expr2 = _3 == ( ~(n ^ o) & (o ^ p) & ~(p ^ s) )
print(expr2)
expr3 = _1 == ( ( s[1] ^ y[1] ) | ( s[0] ^ y[0] ) )
print(expr3)
expr4 = _1 == ( ( s[1] ^ z[1] ) | ( s[0] ^ z[0] ) )
print(expr4)

eFnE = d5o.Qblock() << expr1 << expr2 << expr3 << expr4
print(eFnE)
qubo = eFnE.qubo()
print(qubo)

from dwave.system import DWaveSampler, EmbeddingComposite
sampler = DWaveSampler(solver={'topology__type': 'zephyr', 'qpu': True})
solver = EmbeddingComposite(sampler)
kwargs = {'num_reads': 1000 }

sampleset = solver.sample_qubo(qubo, **kwargs)
lowEnergy = sampleset.lowest().record['energy'][0]
samples = [d5o.SampleEng(dict(sample), lowEnergy) for sample in sampleset.lowest().samples()]
eFnE.add(samples)
print("  {}".format(eFnE.solutions()))
varBind = d5o.Qbinder() << x << y << z << w << n << o << p << s
varBind.add(samples)
print(varBind)