# -*- coding: utf-8 -*-
"""
Created on Wed Jul 31 13:41:38 2024

@author: pc
"""
from time import ctime

#%%

from dann5.d5 import Qfunction, Qblock, Qwhole, QwholeExpr

class Qmod(Qfunction):
    """
    A quantum mod function definition.
    """
    def __init__(self, name, noArguments = 0):
        """
        Construct quantum mod function instance with name as its identity and
        optinal number of arguments. The default number of arguments is 0 
        """
        Qfunction.__init__(self, name, noArguments)
    
    def clone(self):
        """
        Return a copy of this object
        """
        return Qfunction.__init__(self, self)
    
    def refresh(self):
        """
        """
        print ("refreshing:", self.name)
        return
    
    def expression(self, number, divisor):
        """
        """
        reminder = Qwhole(divisor.noqbs(), "reminder");
        #self.operands(reminder, [number, divisor])
        factor = Qwhole(divisor.noqbs(), "factor");
        modXpr = number.assign(reminder + factor * divisor)
        self = self << modXpr << (factor < divisor);
        return QwholeExpr(self)
    
mod = Qmod("test")
s = Qwhole(5, "s"); k = Qwhole(2, "k");
xpr = mod.expression(s, k)

#%%
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
s = Qwhole(2, "s"); t = Qwhole(2, "t"); 
k = Qwhole(1, "k"); n = Qwhole(1, "n");

s_gdc_exp = (s * k) == _1 + (t * n) 
t_gdc_exp = (t * n) == _1 + (s * k)

gcd = gcd << s_gdc_exp << t_gdc_exp << (t != s);

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
print(ctime())

from dann5.d5 import Qbinder
variables = Qbinder() << s << t << k << n 
variables.add(gcd.compute())
print(variables)
print(ctime())
#%%

for ki in range(0,2):
    for ni in range(0,2):
        for si in range(0,8):
            for ti in range(0,8):
                if((si * ki) == 1 + (ti * ni)):
                    print("DIVISOR is 1 ==> s:{} t:{} k:{} n:{}".format(si, ti, ki, ni))
#%%
from dann5.d5 import Qroutine, Qbin, Bits, Qwhole, Qbinder

mod = Qroutine("m = s % k"); # {
_0 = Qwhole("0_", 0); _7 = Qwhole("7_", 7);
s = Qwhole(5, "s"); m = Qwhole(2, "m"); 
a = Qwhole(2, "a"); k = Qwhole(2, "k");
mod = mod << s.assign(m + a * k) << (m < k) << (s == _7);
# } m = s % k
print(mod)
#%%
from dann5.d5o import QuboCompiler, QuboAnalyzer
compiler = QuboCompiler()
mod.compile(compiler)
analyzer = QuboAnalyzer(compiler.qubo())
print("{} nodes & {} branches".format(analyzer.nodesNo(), analyzer.branchesNo()))

#%%
from dann5.dwave import Solver
Solver.Active()
print(ctime())
solutions = mod.compute();

modBinder = Qbinder() << s << k << a << m
modBinder.add(solutions)
print(modBinder)
print(ctime())

#%%
from dann5.d5 import Qwhole
a = Qwhole(2, "a"); k = Qwhole(2, "k");
xpr = a + k
print(xpr)
print(xpr.root())
print(xpr.rootDef())