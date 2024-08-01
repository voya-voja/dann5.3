# -*- coding: utf-8 -*-
"""
Created on Wed Jul 31 13:41:38 2024

@author: pc
"""

from dann5.d5 import Qroutine, Qbin, Bits, Qwhole
gcd = Qroutine("gcd(s, t) = 1"); # {
_1 = Qwhole("1_", 1); _0 = Qwhole("0_", 0);
s = Qwhole(3, "s"); t = Qwhole(3, "t"); 
k = Qwhole(2, "k"); n = Qwhole(2, "n"); a = Qwhole(2, "a"); b = Qwhole(2, "b");

s_gdc_exp = (s * k) == _1 + (t * n)
t_gdc_exp = (t * a) == _1 + (s * b)

gcd = gcd << s_gdc_exp << t_gdc_exp << (s != t);

# } gcd(s, t) = 1
print(gcd)
#%%
from dann5.d5o import QuboCompiler, QuboAnalyzer
compiler = QuboCompiler()
gcd.compile(compiler)
analyzer = QuboAnalyzer(compiler.qubo())
print("{} nodes & {} branches".format(analyzer.nodesNo(), analyzer.branchesNo()))
#%%
from dann5.dwave import Solver
#Solver.Active()
Solver.Activate("Advantage2")

from dann5.d5 import Qbinder
variables = Qbinder() << s << t << k << n << a << b
variables.add(gcd.compute())
print(variables)
#%%
from dann5.d5 import Qroutine, Qbin, Bits, Qwhole
gcd = Qroutine("gcd(s, t) = 1"); # {
_1 = Qwhole("1_", 1); _0 = Qwhole("0_", 0);
s = Qwhole(3, "s"); t = Qwhole(3, "t"); 
k = Qwhole(2, "k"); n = Qwhole(2, "n"); a = Qwhole(2, "a"); b = Qwhole(2, "b");

s_gdc_exp = (s * k) == _1 + (t * n)

gcd = gcd << s_gdc_exp << (s != t);

# } gcd(s, t) = 1
print(gcd)
#%%
from dann5.d5o import QuboCompiler, QuboAnalyzer
compiler = QuboCompiler()
gcd.compile(compiler)
analyzer = QuboAnalyzer(compiler.qubo())
print("{} nodes & {} branches".format(analyzer.nodesNo(), analyzer.branchesNo()))
#%%
from dann5.dwave import Solver
#Solver.Active()
Solver.Activate("Advantage2")

from dann5.d5 import Qbinder
variables = Qbinder() << s << t << k << n 
variables.add(gcd.compute())
print(variables)
#%%
from dann5.d5 import Qroutine, Qbin, Bits, Qwhole
gcd = Qroutine("gcd(s, t) = 1"); # {
_1 = Qwhole("1_", 1); _0 = Qwhole("0_", 0);
s = Qwhole(3, "s"); t = Qwhole(3, "t"); 
k = Qwhole(2, "k"); n = Qwhole(2, "n"); a = Qwhole(2, "a"); b = Qwhole(2, "b");

t_gdc_exp = (t * b) == _1 + (s * a)

gcd = gcd << t_gdc_exp << (s != t);

# } gcd(s, t) = 1
print(gcd)
#%%
from dann5.d5o import QuboCompiler, QuboAnalyzer
compiler = QuboCompiler()
gcd.compile(compiler)
analyzer = QuboAnalyzer(compiler.qubo())
print("{} nodes & {} branches".format(analyzer.nodesNo(), analyzer.branchesNo()))
#%%
from dann5.dwave import Solver
#Solver.Active()
Solver.Activate("Advantage2")

from dann5.d5 import Qbinder
variables = Qbinder() << s << t << a << b
variables.add(gcd.compute())
print(variables)
#%%
from dann5.d5 import Qroutine, Qbin, Bits, Qwhole
gcd = Qroutine("gcd(s, t) = 1"); # {
_1 = Qwhole("1_", 1); _0 = Qwhole("0_", 0);
s = Qwhole(3, "s"); t = Qwhole(3, "t"); 
k = Qwhole(1, "k"); n = Qwhole(1, "n");

s_gdc_exp = (s * k) == _1 + (t * n)
t_gdc_exp = (t * k) == _1 + (s * n)

gcd = gcd << s_gdc_exp << (s != t);

# } gcd(s, t) = 1
print(gcd)
#%%
from dann5.d5o import QuboCompiler, QuboAnalyzer
compiler = QuboCompiler()
gcd.compile(compiler)
analyzer = QuboAnalyzer(compiler.qubo())
print("{} nodes & {} branches".format(analyzer.nodesNo(), analyzer.branchesNo()))
#%%
from dann5.dwave import Solver
#Solver.Active()
Solver.Activate("dann5")

from dann5.d5 import Qbinder
variables = Qbinder() << s << t << k << n 
variables.add(gcd.compute())
print(variables)
#%%

for ki in range(0,2):
    for ni in range(0,2):
        for si in range(0,8):
            for ti in range(0,8):
                if((si * ki) == 1 + (ti * ni)):
                    print("DIVISOR is 1 ==> s:{} t:{} k:{} n:{}".format(si, ti, ki, ni))