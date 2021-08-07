# -*- coding: utf-8 -*-
"""
Created on Sat Aug  7 17:48:12 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o as d5o

a0 = d5o.Qbit("0a", 1)
print(a0.toString(False, 0xffffffffffffffff))
print(a0.toString(False, 0))
print(a0.toString(True, 0xffffffffffffffff))
print(a0.toString(True, 0))

a = d5o.Qbin(3, "a")
print(a.toString(False, 0xffffffffffffffff))
print(a.toString(False, 0))
print(a.toString(True, 0xffffffffffffffff))
print(a.toString(True, 0))
