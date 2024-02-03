# -*- coding: utf-8 -*-
"""
Created on Thu Jan  6 15:12:23 2022

@author: Nebojsa.Vojinovic
"""

from dann5.d5 import Qbit, Superposition, Qanalyzer

class QbitTest:
    def __init__(self):
        q_bit = Qbit('qBit')
        print("*** default ***\n\tAll qbits:\t{}\n\t0 q-bit:\t{}\n".format(
                                    q_bit.toString(), 
                                    q_bit.toString(False, 0)),
              "*** decomposed ***\n\tAll qubits:\t{}\n\t0 q-bit:\t{}\n".format( 
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
        print("\nAssignment 'r = qBit' => '{}'\n\tQUBO: {}".format(aR.toString(), aR.qubo()),
              "\n\tSOLUTION: \n{}\n".format(aR.solve()))
              
        xEq = self.x == self.y
        print("Equal Expression '{}'\n\tQUBO: {}".format(xEq.toString(), xEq.qubo()),
              "\n\tSOLUTION: \n{}\n".format(xEq.solve()))
        xAl = self.x.alike(self.y)
        print("Alike Expression '{}'\n\tQUBO: {}".format(xAl.toString(), xAl.qubo()),
              "\n\tSOLUTION: \n{}\n".format(xAl.solve()))
        axEq = self.r.assign(self.x == self.y)
        print("Equal Assignment 'r = x == y' => '{}'\n\tQUBO: {}".format(axEq.toString(), axEq.qubo()),
              "\n\tSOLUTION: \n{}\n".format(axEq.solve()))
        axAl = self.r.assign(self.x.alike(self.y))
        print("Alike Assignment '{}'\n\tQUBO: {}".format(axAl.toString(), axAl.qubo()),
              "\n\tSOLUTION: \n{}\n".format(axAl.solve()))
        axAl = self._1_.assign(self.x.alike(self.y))
        print("Alike Assignment to _1_ => '{}'\n\tQUBO: {}".format(axAl.toString(), axAl.qubo()),
              "\n\tSOLUTION: \n{}\n".format(axAl.solve()))


    def aNxNeUl(self):
        xNe = self.x != self.y
        print("Not Equal Expression '{}'\n\tQUBO: {}".format(xNe.toString(), xNe.qubo()),
              "\n\tSOLUTION: \n{}\n".format(xNe.solve()))
        xUl = self.x ^ self.y
        print("Unlike Expression '{}'\n\tQUBO: {}".format(xUl.toString(), xUl.qubo()),
              "\n\tSOLUTION: \n{}\n".format(xUl.solve()))
        axUl = self._1_.assign(self.x ^ self.y)
        print("Unlike Assignment to _1_ => '{}'\n\tQUBO: {}".format(axUl.toString(), axUl.qubo()),
              "\n\tSOLUTION: \n{}\n".format(axUl.solve()))

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
        qbitAssign = self._1_.assign((self.z != (self.b & self.x)) | ((self.y ^ self._0_) == self.z))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
        qbitAssign = self._1_.assign((self.z != (self.b & self.x)) | (self.z) == (self.y ^ self._0_))
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
        qbitAssign = self._1_.assign(((self.b & self.x).unlike(self.z)) | ((self.y ^ self._0_).alike(self.z)))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
        qbitAssign = self._1_.assign((self.z.unlike(self.b & self.x)) | ((self.y ^ self._0_).alike(self.z)))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
        qbitAssign = self._1_.assign((self.z.unlike(self.b & self.x)) | (self.z.alike(self.y ^ self._0_)))
        print("\nLOGIC:\n{}\n{}".format(qbitAssign.toString(),qbitAssign.toString(True)))
        print("*** SOLUTION ***\n{}".format(qbitAssign.solve()))
