# dann5.ocean version 2 (d5o2)

This project aims to simplify the programming of quantum computing models and demonstrates ways of faster development of interactive examples, which will help you learn to formulate and solve problems on quantum computers by using [dann5 d5o library](https://github.com/voya-voja/dann5) quantum programing types and operations. The d5o library provides means of converting operational assignments into QUBO, which then can be executed on various quantum solvers, being analog quantum gate computers using IBM Qiskit for example, or quantum annealers or hybrid computers using DWave Ocean, or quantum simulators using Azure Quantum. Dann5 d5o library provides means of specifying undefined and defined variables in python of programming types such as q-bit, q-boolean, q-binary, q-whole, q-integer, quantum non-negative rational and q-rational. The specified quantum variables then can be linked into quantum logic assignments by applying appropriate operational expression to describe the problem that needs to be solved. In a way dann5 d5o library is an extension of D-Wave Ocean's stack of tools, which will allow you to develop integrated code, and to submit it to run in a quantum annealing computer (QAC), either D-Wave Advantage, 2000Q or Hybrid Solver.

The goal of Dann5 project and d5o framework is to provide QAC developers with programming constructs, such as:
- standard types:
    - **Qbit**† – quantum bit, where
      > Qbit x ϵ { 0, 1, S(uperposition) }
    - **Qbool**† – quantum boolean, where
      > Qbool x ϵ { T(rue), F(alse), S(uperposition) }
    - **Qbin**† – quantum binary, as an array of Qbit’s
      > Qbin x ≡ Qbit[ xi; where xi ϵ Qbit & 0 <= i <= n ]
    - **Qwhole** – quantum whole numbers (non-negative integer), where 
      > 	Qwhole x ∈ { n | n ≥ 0                                         }
      >                {   | n = U(nknown) if ni = S | ni ϵ Qbit & ni ϵ n} }
    - **Qint**† – quantum integer numbers, where 
      > 	Qint x ∈ Qwhole U -Qwhole                                         }
    - **Qnnr**†† – quantum non-negative rational numbers, where 
      > 	Qnnr x ∈ { r | r ∈ R(ational) & n≥0                          }
      >              {   | r = U(nknown) if ri = S | ri ϵ Qbit & ri ϵ r} }
    - **Qrat**†† – quantum rational numbers, where 
      > 	Qreal x ∈ Qnnr U -Qnnr                                         }
- definitions:
  - **Quantum variable** examples
    > Qbit x(“x”) ⇛ initialize a Q bit variable with the bit in S(uperpostion) state
    
    > Qbool y(“z”, d5o.Qbool.false()) ⇛ initialize a defined Q binary variable with bit z = 0
    
    > Qbin a(3, “a”) ⇛ initialize an undefined Q binary variable with 3 quantum bits in S(uperpostion) state
    
    > Qwhole b(“b”, 6) ⇛ initialize a defined Q whole variable b with value 6, i.e. contains 3 qbits with deterministic values: b0 = 0, b1 = 1 and b2 = 1
    
    > Qint i(3, “i”, -1) ⇛ initialize a defined Q whole variable i with value -1, i.e. contains 3 qbits with deterministic values: i0 = 1 i1 = 1 and i2 = 1
    
    > Qnnr c(“c”, 2.5) ⇛ initialize a defined Q non-negative rational variable c with binary value 10.1, i.e. c-1 = 1, c0 = 0 and c1 = 1 ††
    
    > Qrat a(1.2, “d”) ⇛ initialize a Q rational variable d with 3 quantum bits, as an unknown rational number, i.e. d-2 = S, d-1 = S and d0 = S ††
    
  - **Qassignment** - quantum assignment defines an operational interaction between predefined variables
    > eX = Qwhole(“X”, 15).assign(a * a) ⇛ for Q variable a (defined above) assigns multiplication expression with expected result X -= 15 to eX Q quantum assignment
    - QUBO integration into D-Wave Ocean, Azure Quantum or Qiskit:
      - **qubo()**  – returns QUBO binary quadratic model (BQM) of the assignment
        > eX.qubo() ⇛ for eX Q assignment (defined above) returns a QUBO dictionary object for execution on an exact or QAC solver
      - **add()**  – receives a dictionary of samples returned by a solver 
        > eX.add(lowSamples) ⇛ sets resulting samples returned from a solver into eX Q equation, which has been used to create QUBO BQM
      - **solutions()**  – returns string representation of the assignments solutions based on added samples 
        > eX.solutions() ⇛ for eX Q assignment (defined above) with expected result 15 and b set to 6 returns solutions for undefined Q variable a
      - **reset()**  – resets teh assignment into its inital state, i.e. without any solutions 
        > eX.reset() ⇛ for eX Q assignment (defined above) is set into its initial state
        
  - **Qcondition**†† - quantum condition statement
    > Qcondition cA(a > b) ⇛ creates a Q condition cA that compares (above defined) Q variables a and b
    
    > cA << eX ⇛ adds the (above defined) eX Q equation to be calculated under condition cA, i.e. a is bigger than b
    
  - **Qroutine**†† - quantum routine
    > Qroutine program(“my Q program”) ⇛ creates a Q routine program, which allows correlation of programming Q statements
    
    >	program << cA << eB ⇛ adds the (above defined) cA Q condition and a predefined eB Q equation into my Q program
    
  - **Qfunction**†† - quantum function routine
    >	Qfunction myF(“my F”, a) ⇛ creates a Q function routine my F with Q variable a as a return variable
    
    >	myF << cA << a + b ⇛ adds the (above defined) cA Q condition and addition Q equation of a and b into myF
    
    >	eB *= myF ⇛ updates eB Q equation with a multiplication of its existing expression with a return from myF Q function
    
   - **Qpow**††, **Qmod**††, **Qgcd**†† - specialized quantum function
      >	Qpow a_x3(a, 3) ⇛ creates a power Q function routine a_x3 with Q variable a as a base and exponent 3
   
      >	Qmod b_m3(b, 3) ⇛ creates a modulo Q function routine b_m3 with Q variable b as a dividend and divisor 3
   
      >	Qgcd a_b(a, b) ⇛ creates a greatest common divisor Q function routine a_b for Q variables a and b
- standard operations:
  - **assignment**  – of quantum variable or expression to the Q equation
  - **unary operators**: ~ (inversion)
  - **binary operators**: +, *, -, /
  - **logical operators**: &, |, ^, nand, nor
  - **comparison operators †**: ==, !=, >, >=, <, <=

    
      **Note** :  † features in development, †† planned features. 
    
For additional information on how to use features of d5o refer to [examples](examples).

## Install & Use

### Installation prerequisites
To install and use Dann5 d5o precompiled library
-	install [Python 3.9.9 (64-bit)](https://www.python.org/downloads/release/python-399/), which has been used to test released functionality
-	install D-Wave Ocean Tools into python environment; follow [Installing Ocean Tools](https://docs.ocean.dwavesys.com/en/stable/overview/install.html)
-	install pybind11 into the same python environment; follow [Installing the library](https://pybind11.readthedocs.io/en/stable/installing.html)

To compile Dann5 d5o source code in addition to above prerequisites
-	install [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html) by following [download instructions](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download)

### Installation
Dann5 d5o library is in active development on Windows x64 as a primary OS. A Mac OS X/Linux version and package installation will be released in the future.

#### Install Windows x64 version from this repository into python project or environment

> pip install --upgrade dann5

#####Install manually
  1.	Download dann5 folder with d5o.cp38-win_amd64.pyd library from [examples folder](examples) 
  2.	Make dann5 a sub-folder of
   -	your python project folder; or
   -	site-packages folder for your python environment, e.g. for anaconda distribution, on windows, the path should be:
   
        > C:\Users\<user_name>\AppData\Local\Continuum\anaconda3\envs\<env_name>\Lib\site-packages
    
  3.	Copy d5o.cp38-win_amd64.pyd into Dann5 sub-folder

#####Install package from source code
  1.	Simply clone or download the dann5 repository 
    -	If using the MS Visual Studio dann5.ocean.sln, make sure dann5 and Eigen are sub-folders in the same folder.
  2.	Build 
    -	d5o.cp38-win_amd64.pyd for windows and python x64
    -	d5o.cp38-win_amd32.pyd for windows and python x32
    -	d5o.so for Mac OS X or Linux
  3.	Deploy d5o library into dann5 folder within your python project or environment as described above in Windows x64 version installation
  
### Use
After installing d5o library as per instructions above, use released features in your python source code:
  > import dann5.d5o2 as d5o
  >
  > a = Qwhole(4, "a")
  >
  > b = Qwhole(3, "b")
  >
  > A = Qwhole("A", 15))
  >
  > eA = A.assign( a + b )
  >
  > print(eA.toString())
  >
  > print(eA.toString(True))
  >
  > print(eA.qubo())

Or use Jupyter Notebook [examples](examples) to review Dann5 d50 version 2 functionality. 

## Bugs, issues and contributing
Contributions to this project are welcome.

**Note**: if you have an issue, please send me the code example.
