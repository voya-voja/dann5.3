# -*- coding: utf-8 -*-
"""
Created on Thu Jan  6 15:12:23 2022

@author: Nebojsa.Vojinovic
"""

from dann5.d5o2 import Qbit, Superposition, Qanalyzer

class QbitTest:
    def __init__(self):
        q_bit = Qbit('qBit')
        print("*** default ***\n\tAll: {}\n\t0 q-bit :{}\n".format(
                                    q_bit.toString(), 
                                    q_bit.toString(False, 0)),
              "*** decomposed ***\n\tAll: {}\n\t0 q-bit: {}\n".format( 
                                          q_bit.toString(True), 
                                          q_bit.toString(True, 0)))
        self.qBit = q_bit;
        self.b = Qbit('b', 1)
        self._0_ = Qbit("_0_", 0)
        self._1_ = Qbit("_1_", 1)
        self.x = Qbit('x', Superposition())
        self.y = Qbit("y", 5)
        self.z = Qbit("z")
        self.r = Qbit("r")
    
    def aNxEqAl(self):
        aR = self.r.assign(self.qBit)
        xEq = self.x == self.y
        xAl = self.x.alike(self.y)
        axEq = self.r.assign(self.x == self.y)
        axAl = self.r = self.x.alike(self.y)
        print("\nQUBO:\nAssignment\n{}\nEqual Expression\n{}".format(aR.qubo(),xEq.qubo()),
              "\nEqual Assignment\n{}\nAlike Expression\n".format(axEq.qubo()),
              "{}\nAlike Assignment\n{}\n".format(xAl.qubo(),axAl.qubo()))
        print("\nSOLUTION:\nAssignment\n{}\nEqual Expression\n".format(aR.solve()), 
              "{}\nEqual Assignment\n{}\n".format(xEq.solve(),axEq.solve()),
              "Alike Expression\n{}\nAlike Assignment\n{}\n".format(xAl.solve(),axAl.solve()))


    def aNxNeUl(self):
        xNe = self.x != self.y
        xUl = self.x ^ self.y
        axUl = self._1_.assign(self.x ^ self.y)
        print("\nQUBO:\nNot Equal Expression\n{}\nUnlike Expression\n".format(xNe.qubo()), 
              "{}\nUnlike Assignment\n{}\n".format(xUl.qubo(),axUl.qubo()))
        print("\nSOLUTION:\nNot equal Expression\n{}\nUnlike Expression\n".format(xNe.solve()), 
              "{}\nUnlike Assignment\n{}\n".format(xUl.solve(),axUl.solve()))

    def eqNne(self):
        qbitAssign = self._1_.assign(((self.b & self.x) != self.z) | (self.z == (self.y ^ self._0_)))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** Generic Qubo ***\n{}\n".format(qbitAssign.qubo(False))); 
        print("*** Finalized Qubo ***\n{}\n".format(qbitAssign.qubo()))
        analyze = Qanalyzer(qbitAssign.qubo())
        print("# of nodes: {}, # of branches: {}\n".format(analyze.nodesNo(),analyze.branchesNo()))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
        qbitAssign = self._1_.assign(((self.b & self.x) != self.z) | ((self.y ^ self._0_) == self.z))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))

    def alNul(self):       
        qbitAssign = self._1_.assign(((self.b & self.x).unlike(self.z)) | (self.z.alike(self.y ^ self._0_)))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** Generic Qubo ***\n{}\n".format(qbitAssign.qubo(False))); 
        print("*** Finalized Qubo ***\n{}\n".format(qbitAssign.qubo()))
        analyze = Qanalyzer(qbitAssign.qubo())
        print("# of nodes: {}, # of branches: {}\n".format(analyze.nodesNo(),analyze.branchesNo()))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
