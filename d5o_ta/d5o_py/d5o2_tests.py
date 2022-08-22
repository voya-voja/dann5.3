# -*- coding: utf-8 -*-
"""
Created on Thu Jan  6 16:29:37 2022

@author: Nebojsa.Vojinovic
"""

from qbit_test import QbitTest
from dann5.d5o2 import Qbit

def main():
    print("----- INITIALIZATION: q_bit = Qbit('qBit') ----")
    test = QbitTest()
    print("\n----- EQUAL & ALIKE EXPRESSIONS & ASSIGNMENTS ----")
    test.aNxEqAl()
    print("\n----- NOT EQUAL & UNLIKE EXPRESSIONS & ASSIGNMENTS ----")
    test.aNxNeUl()
    print("\n----- EQUAL & NOT EQUAL EXPRESSIONs COMMUTATIVITY ----")
    test.eqNne()

    print("\n----- ALIKE & UNLIKE EXPRESSIONs COMMUTATIVITY ----")
    test.alNul()

if __name__ == "__main__":
    main()