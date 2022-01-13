# -*- coding: utf-8 -*-
"""
Created on Thu Jan  6 16:29:37 2022

@author: Nebojsa.Vojinovic
"""

from qbit_test import QbitTest
from dann5.d5o2 import Qbit

def main():
    x = Qbit("x")
    y = Qbit("y")
    _1_ = Qbit("_1_", 1)
    _0_ = Qbit("_0_", 0)
    xpr = x.nxor(y)
    print('x !& y:\n{}'.format(xpr.solve()))
    print('_1_ & (x !& y):\n{}'.format((_1_.nand(x.nand(y))).solve()))
    print('x !| y:\n{}'.format((x.nor(y)).solve()))
    print('_0_ !| (x & y):\n{}'.format((_0_.nor(x.nor(y))).solve()))
    test = QbitTest()
    test.aNxEqAl()
    test.aNxNeUl()
    test.eqNne()
    test.alNul()

if __name__ == "__main__":
    main()