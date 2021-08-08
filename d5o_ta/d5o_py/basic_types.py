# -*- coding: utf-8 -*-
"""
Created on Sat Aug  7 17:48:12 2021

@author: Nebojsa.Vojinovic
"""
import dann5.d5o as d5o

a0 = d5o.Qbit("0a", 1)
print(a0.toString(False, 0xffffffffffffffff),
      a0.toString(False, 0),
      a0.toString(True, 0xffffffffffffffff),
      a0.toString(True, 0))

b = d5o.Qbool("boolean", d5o.Qbool.true())
print(b.toString(False, 0xffffffffffffffff), 
      b.toString(False, 0),
      b.toString(True, 0xffffffffffffffff),
      b.toString(True, 0))

a = d5o.Qbin(3, "a")
print(a.toString(False, 0xffffffffffffffff), 
      a.toString(False, 0),
      a.toString(True, 0xffffffffffffffff),
      a.toString(True, 0))

x = d5o.Qwhole("x", d5o.Bits(33))
print(x.toString(False, 0xffffffffffffffff), 
      x.toString(False, 0),
      x.toString(True, 0xffffffffffffffff),
      x.toString(True, 0))
