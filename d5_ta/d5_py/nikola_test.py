# -*- coding: utf-8 -*-
from dann5.d5 import Qwhole
from dann5.d5 import Qbool
from dann5.d5 import Qbit
from dann5.d5 import Qbin
from dann5.d5 import Bits
from dann5.dwave import Solver

def qBoolTest():
    print("===== Q bool test start =======\n")
    
    nik = Qbool("nik")
    shawnie = Qbool("shwanie")
    
    expression = nik & shawnie;

    print(nik)
    print(shawnie)
    print(expression.toString())
    
    print(expression.toString(True))
    print(expression.solve())
    
    print("============\n")
    

    result = Qbool("result",True)
    
    # needs to be reset since we solved the expression already
    expression.reset()
    

    assignment = result._(expression)
    
    print(assignment.toString())
    print(assignment.solve())
    
    print()
    print("===== Q bool test end =======\n")

def qBitTest():
    print("===== Q bit test start =======\n")
    
    jim = Qbit("jim")
    pam = Qbit("pam")
    
    expression = jim & pam;
   
    print(jim.toString())
    print(pam.toString())
    print(expression.toString())
    
    print(expression.toString(True))
    print(expression.solve())
    
    print("============\n")
    
    
    result = Qbit("result", 1)
    expression.reset()

    assignment = result.assign(expression)
    
    print(assignment.toString())
    print(assignment.solve())
    
    print()
    print("===== Q bit test end =======\n")

def qBinTest():
    print("===== Q bin test start =======\n")
    
    dan = Qbin(2, "dan")
    flora = Qbin(2, "flora")
    
    expression = dan & flora;

    print(dan.toString())
    print(flora.toString())
    print(expression.toString())
    
    print(expression.toString(True))
    print(expression.solve())
    
    print("============\n")
    
    result = Qbin("result", Bits(3))
    
    assignment = result.assign(expression)
    
    print(assignment.toString())
    print(assignment.solve())
    
    print()
    print("===== Q bin test end =======\n")
    
def qBinTest2():
    print("===== Q bin test 2 start =======\n")
    
    phil = Qbin(2, "phil")
    claire = Qbin(2, "claire")
    
    expression = phil & claire;

    print(phil.toString())
    print(claire.toString())
    print(expression.toString())
    
    print(expression.toString(True))
    print(expression.solve())
    
    print("============\n")
    
    result = Qbin(3, "result")
    
    assignment = result.assign(expression)
    
    print(assignment.toString())
    print(assignment.solve())
    
    print()
    print("===== Q bin test 2 end =======\n")

def qWholeTest():
    print("===== Q whole test start =======\n")
    
    kurt = Qwhole(3, "kurt")
    john = Qwhole(3, "john")
    
    # cannot use & in this version as the QbinExpr will not allow qWhole values
    expression = kurt & john;

    print(kurt.toString())
    print(john.toString())
    print(expression.toString())
    
    print(expression.toString(True))
    print(expression.solve())
    
    print("============\n")
    
    _10 = Qbin("10", Bits(10))
    expression.reset()
    assignment = _10.assign(expression)

    print(assignment.toString())

    print(assignment.solve())
    
    print()
    print("===== Q whole test end =======\n")    

if __name__ == "__main__":
    Solver.Active()   # activates default D5QuboSolver
    qBoolTest()
    qBitTest()
    qBinTest()
    qBinTest2()
    qWholeTest()
    
    