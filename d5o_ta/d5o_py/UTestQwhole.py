# -*- coding: utf-8 -*-
"""
Created on Fri Feb 17 18:56:16 2023

@author: Nebojsa.Vojinovic
"""

import dann5.d5o2 as d5o
from dann5.d5o2 import Qwhole, Qbin, Qblock, Qanalyzer
import filecmp

class UTestQwhole:
  def __init__(self, slvrs):
    #self.baseline = open("UTestQwhole_baseline.txt")
    self.solvers = slvrs
    self.testfile = open("UTestQwhole.txt", "w")
    print("UTestQwhole file opened.")


  def runAll(self, out):
    print(" --- Initializations ----");
    self.testfile.write("\n--- Initializations ----\n");
    self.initialization(out)
    print(" --- Bitwise Expressions ----");
    self.testfile.write("\n--- Bitwise Expressions ----\n");
    self.bitwise(out)
    print(" --- Arithmetic Expressions ----");
    self.testfile.write("\n--- Arithmetic Expressions ----\n");
    self.arithmetic(out);
    print(" --- Comparison Expressions ----");
    self.testfile.write("\n--- Comparison Expressions ----\n");
    self.comparison(out);
    print(" --- Assignments ----");
    self.testfile.write("\n--- Assignments ----\n");
    self.assignment(out);
    print(" --- Factorial ----");
    self.testfile.write("\n--- Factorial ----\n");
    self.factorial(out);
    print(" --- Prime numbers ----");
    self.testfile.write("\n--- Prime numbers ----\n");
    self.prime(out);
    print(" --- Prime numbers (6 * Qwhole +/- 1) algorithm ----");
    self.testfile.write("\n--- Prime numbers (6 * Qwhole +/- 1) algorithm ----\n");
    self.prime6(out);
    #clode test files
    self.testfile.close()
    print("UTestQwhole file closed.")
    # deep comparison
    result = filecmp.cmp("UTestQwhole_baseline.txt", "UTestQwhole.txt", shallow=False)
    print("test file is same as baseline :", result)


  def initialization(self, out):
    # Default constructor initializes Q whole with an empty id and supperposition value
    # Initialize Q whole with a given id and supperposition value
    dcQwhole = Qwhole(); idQwhole = Qwhole("id")
    self.testfile.write(dcQwhole.toString() + "\n")
    self.testfile.write(idQwhole.toString() + "\n")
    size3 = Qwhole(3, "size3");
    self.testfile.write(size3.toString() + "\n")
    self.testfile.write(size3.toString(False, 0) + "\n")
    self.testfile.write(size3.toString(True) + "\n")
    self.testfile.write(size3.toString(True, 0) + "\n")
    
    # Initialize Q whole with a given id and value.
    # If value is different from 0 or 1, Qwhole into superposition state
    oct10 = Qwhole("octal10", 0o10); dec5= Qwhole("dec5", 5); bin6 = Qwhole("bin6", 0b110);
    hexA = Qwhole("hexA", 0xA);
    self.testfile.write(oct10.toString() + "\n")
    self.testfile.write(oct10.toString(True) + "\n")
    self.testfile.write(oct10.toString(True, 3) + "\n");
    self.testfile.write(dec5.toString() + "\n")
    self.testfile.write(dec5.toString(True) + "\n")
    self.testfile.write(dec5.toString(True, 3) + "\n");
    self.testfile.write(bin6.toString() + "\n")
    self.testfile.write(bin6.toString(True) + "\n")
    self.testfile.write(bin6.toString(True, 3) + "\n");    
    self.testfile.write(hexA.toString() + "\n")
    self.testfile.write(hexA.toString(True) + "\n")
    self.testfile.write(hexA.toString(True, 3) + "\n");    
    qBits = Qwhole("bin6qBits", Qbin(bin6).toQbits());
    self.testfile.write(qBits.toString() + "\n")
    
    cQwhole = Qwhole(hexA);
    self.testfile.write(cQwhole.id() + "\n")
    self.testfile.write(Qbin(cQwhole).toBits().to_string() + "\n")


  def bitwise(self, out):
    x = Qwhole(3, "x"); y = Qwhole("y", 5); 
    _0 = Qwhole("_0", 0); _1 = Qwhole("_1", 1);
    xI = ~x;
    self.testfile.write(xI.toString() + "\n")
    self.testfile.write(xI.toString(True) + "\n")
    #for q in xI.qubo(): self.testfile.write(q.__str__())
    self.testfile.write(xI.qubo().__str__() + "\n")
    self.testfile.write(xI.solve() + "\n");
    qbExpr = x & y;
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    qbExpr = x.nand(y);
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    x.resize(4);
    self.testfile.write(x.toString(True) + "\n");
    qbExpr = x | y;
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    qbExpr = x.nor(y);
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    x[2] = 0;
    self.testfile.write(x.toString(True) + "\n");
    qbExpr = x ^ y;
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    qbExpr = x.nxor(y);
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
    x.resize(2); y.resize(2);
    self.testfile.write(x.toString(True) + "\n");
    self.testfile.write(y.toString(True) + "\n");
    qbExpr = (_1 & (x.nand(y))) ^ (Qbin(_0).alike(y | x));
    self.testfile.write(qbExpr.toString() + "\n")
    self.testfile.write(qbExpr.toString(True) + "\n")
    self.testfile.write(qbExpr.qubo(False).__str__() + "\n")
    self.testfile.write(qbExpr.qubo().__str__() + "\n")
    self.testfile.write(qbExpr.solve() + "\n");
               
  def arithmetic(self, out):
    x = Qwhole(2, "x"); y = Qwhole("y", 5); z = Qwhole(1, "z");
    _3 = Qwhole("_3", 3);
    qwExpr = x + y;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr = y + x;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwwExpr = x + y + z + _3;
    self.testfile.write(qwwExpr.toString() + "\n")
    self.testfile.write(qwwExpr.toString(True) + "\n")
    self.testfile.write(str(qwwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwwExpr.qubo()) + "\n");
    self.testfile.write(qwwExpr.solve() + "\n");
    qwExpr.reset();
    qxwExpr = qwExpr + z + _3;
    self.testfile.write(qxwExpr.toString() + "\n")
    self.testfile.write(qxwExpr.toString(True) + "\n")
    self.testfile.write(str(qxwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxwExpr.qubo()) + "\n");
    self.testfile.write(qxwExpr.solve() + "\n");
    qwExpr = x + y;
    qxwExpr = z + qwExpr + _3;
    self.testfile.write(qxwExpr.toString() + "\n")
    self.testfile.write(qxwExpr.toString(True) + "\n")
    self.testfile.write(str(qxwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxwExpr.qubo()) + "\n");
    self.testfile.write(qxwExpr.solve() + "\n");
    z_3Expr = z + _3;
    qwxExpr = x + y + z_3Expr;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");
    qwExpr = x + y;
    z_3Expr = z + _3;
    qxxExpr = qwExpr + z_3Expr;
    self.testfile.write(qxxExpr.toString() + "\n")
    self.testfile.write(qxxExpr.toString(True) + "\n")
    self.testfile.write(str(qxxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxxExpr.qubo()) + "\n");
    self.testfile.write(qxxExpr.solve() + "\n");
    qwExpr = x + y;
    z_3Expr = z + _3;
    qxxExpr = z_3Expr + qwExpr;
    self.testfile.write(qxxExpr.toString() + "\n")
    self.testfile.write(qxxExpr.toString(True) + "\n")
    self.testfile.write(str(qxxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxxExpr.qubo()) + "\n");
    self.testfile.write(qxxExpr.solve() + "\n");
    qwExpr = y - x;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    #Issue #6
    """qwExpr.reset();
    qwxExpr = x - qwExpr;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");   """ 
    qwExpr = qwExpr - x;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    #Issue #6
    """qwExpr.reset();
    qxxExpr = z_3Expr - qwExpr;
    self.testfile.write(qxxExpr.toString() + "\n")
    self.testfile.write(qxxExpr.toString(True) + "\n")
    self.testfile.write(str(qxxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxxExpr.qubo()) + "\n");
    self.testfile.write(qxxExpr.solve() + "\n");   """ 
    qwExpr = x * y;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr.reset();
    qwxExpr = z * qwExpr;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");
    qwExpr.reset();
    qwxExpr = qwExpr * z;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");
    qwExpr = x * y;
    z_3Expr = _3 * z;
    qxxExpr = qwExpr * z_3Expr;
    self.testfile.write(qxxExpr.toString() + "\n")
    self.testfile.write(qxxExpr.toString(True) + "\n")
    self.testfile.write(str(qxxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qxxExpr.qubo()) + "\n");
    #self.testfile.write(qxxExpr.solve() + "\n"); 
    qubo = qxxExpr.qubo()
    self.testfile.write(str(qubo) + "\n");
    anlyzeD2 = Qanalyzer(qubo);
    self.testfile.write(" # of nodes: " + str(anlyzeD2.nodesNo()))
    self.testfile.write(" # of branches: " + str(anlyzeD2.branchesNo()) + "\n");
    if(self.solvers != None):
        samples = self.solvers.solve('Advantage', qubo)
        qxxExpr.add(samples)
        self.testfile.write(qxxExpr.solutions() + "\n")
        print(qxxExpr.toString() + "\n" + qxxExpr.solutions())
    qwExpr.reset();
    qwExpr = y / x;
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr.reset();
    qwxExpr.reset();
    qwxExpr = qwExpr / z;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");
    qwExpr.reset();
    qwxExpr.reset();
    qwExpr = x + z;
    qwxExpr = y / qwExpr;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwxExpr.qubo()) + "\n");
    self.testfile.write(qwxExpr.solve() + "\n");
    qwExpr.reset();
    qwxExpr.reset();
    z_3Expr.reset();
    qwxExpr = z_3Expr / qwExpr;
    self.testfile.write(qwxExpr.toString() + "\n")
    self.testfile.write(qwxExpr.toString(True) + "\n")
    self.testfile.write(str(qwxExpr.qubo(False)) + "\n")
    qubo = qwxExpr.qubo()
    self.testfile.write(str(qubo) + "\n");
    anlyzeD2 = Qanalyzer(qubo);
    self.testfile.write(" # of nodes: " + str(anlyzeD2.nodesNo()))
    self.testfile.write(" # of branches: " + str(anlyzeD2.branchesNo()) + "\n");
    if(self.solvers != None):
        samples = self.solvers.solve('Advantage', qubo)
        qwxExpr.add(samples)
        self.testfile.write(qwxExpr.solutions() + "\n")
        print(qwxExpr.toString() + "\n" + qwxExpr.solutions())
    r = Qwhole("r", 6); k = Qwhole(2, "k");
    blck = Qblock();
    blck._a(r - x)
    blck._a(x / k);
    self.testfile.write(blck.toString() + "\n")
    self.testfile.write(blck.toString(True) + "\n")
    self.testfile.write(str(blck.qubo(False)) + "\n")
    self.testfile.write(str(blck.qubo()) + "\n");
    self.testfile.write(blck.solve() + "\n");
    
  def comparison(self, out):
    x = Qwhole(2, "x"); y = Qwhole("y", 5); z = Qwhole(3, "z");
    xEq = (x == y);
    self.testfile.write(xEq.toString() + "\n")
    self.testfile.write(xEq.toString(True) + "\n") 
    self.testfile.write(str(xEq.qubo()) + "\n") 
    self.testfile.write(xEq.solve() + "\n");
    xNe = (x != z);
    self.testfile.write(xNe.toString() + "\n")
    self.testfile.write(xNe.toString(True) + "\n") 
    self.testfile.write(str(xNe.qubo()) + "\n") 
    self.testfile.write(xNe.solve() + "\n");
    xXnYeZ0 = x != y == z;
    #ERROR ->    out << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl << "Qubo: " << xXnYeZ0.qubo() << endl << xXnYeZ0.solve() << endl;
    self.testfile.write(xXnYeZ0.toString() + "\n")
    self.testfile.write(xXnYeZ0.toString(True) + "\n") 
    self.testfile.write(str(xXnYeZ0.qubo()) + "\n") 
    self.testfile.write(xXnYeZ0.solve() + "\n");
    print(xXnYeZ0.toString() + "\n")
    print(xXnYeZ0.toString(True) + "\n") 
    print(str(xXnYeZ0.qubo()) + "\n") 
    print(xXnYeZ0.solve() + "\n");
    xXnYeZ1 = (x != y) == z;
    #EXEPTION ->    out << xXnYeZ1.toString() << " decomposed: " << xXnYeZ1.toString(true) << endl << "Qubo: " << xXnYeZ1.qubo() << endl << xXnYeZ1.solve() << endl;
    self.testfile.write(xXnYeZ1.toString() + "\n")
    #self.testfile.write(xXnYeZ1.toString(True) + "\n") 
    #self.testfile.write(str(xXnYeZ1.qubo()) + "\n") 
    #self.testfile.write(xXnYeZ1.solve() + "\n");
    print(xXnYeZ1.toString() + "\n")
    #print(xXnYeZ1.toString(True) + "\n") 
    #print(str(xXnYeZ1.qubo()) + "\n") 
    #print(xXnYeZ1.solve() + "\n");
    xXnYeZ2 = x != (y == z);    
    self.testfile.write(xXnYeZ2.toString() + "\n")
    self.testfile.write(xXnYeZ2.toString(True) + "\n") 
    self.testfile.write(str(xXnYeZ2.qubo()) + "\n") 
    self.testfile.write(xXnYeZ2.solve() + "\n");
    x1 = Qwhole(2, "x"); y1 = Qwhole(2, "y"); z1 = Qwhole(2, "z"); w1 = Qwhole(2, "w");
    comp = x1 + y1 == z1 * w1;
    self.testfile.write(comp.toString() + "\n")
    self.testfile.write(comp.toString(True) + "\n") 
    self.testfile.write(str(comp.qubo(False)) + "\n") 
    self.testfile.write(str(comp.qubo()) + "\n") 
    analyzeM = Qanalyzer(comp.qubo());
    self.testfile.write(" # of nodes: " + str(analyzeM.nodesNo()))
    self.testfile.write(" # of branches: " + str(analyzeM.branchesNo()) + "\n");
    self.testfile.write(comp.solve() + "\n");
    x2 = Qwhole(2, "x"); y2 = Qwhole(2, "y"); z2 = Qwhole(2, "z"); w2 = Qwhole(2, "w");
    comp2 = x2 + y2 != z2 + w2;
    self.testfile.write(comp2.toString() + "\n")
    self.testfile.write(comp2.toString(True) + "\n") 
    self.testfile.write(str(comp2.qubo(False)) + "\n") 
    self.testfile.write(str(comp2.qubo()) + "\n") 
    analyzeM = Qanalyzer(comp2.qubo());
    self.testfile.write(" # of nodes: " + str(analyzeM.nodesNo()))
    self.testfile.write(" # of branches: " + str(analyzeM.branchesNo()) + "\n");
    #"ERROR: [QwholeExpr] != [right] issue needs to be fixed!"
    self.testfile.write(comp2.solve() + "\n");
    x.resize(3)
    qwExpr = x > y;
    self.testfile.write("x > y:\n")
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr = y < x;
    self.testfile.write("x < y:\n")
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr = x >= y;
    self.testfile.write("x >= y:\n")
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");
    qwExpr = y <= x;
    self.testfile.write("x <= y:\n")
    self.testfile.write(qwExpr.toString() + "\n")
    self.testfile.write(qwExpr.toString(True) + "\n")
    self.testfile.write(str(qwExpr.qubo(False)) + "\n")
    self.testfile.write(str(qwExpr.qubo()) + "\n");
    self.testfile.write(qwExpr.solve() + "\n");

  def assignment(self, out):
    dcQwhole = d5o.Qwhole(); idQwhole = d5o.Qwhole("id")

  def factorial(self, out):
    dcQwhole = d5o.Qwhole(); idQwhole = d5o.Qwhole("id")

  def prime(self, out):
    dcQwhole = d5o.Qwhole(); idQwhole = d5o.Qwhole("id")

  def prime6(self, out):
    dcQwhole = d5o.Qwhole(); idQwhole = d5o.Qwhole("id")
