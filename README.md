# Quantum Programming: dann5 version 3

The *dann5* project aims to simplify the programming of quantum computing models and to demonstrate ways of faster development of Python programs, which take advantage of distributed quantum computing resources, such as IBM Qiskit and D-Wave Leap. The interactive examples in *dann5 notebooks* are designed to help traditional developers to learn how to formulate and solve problems on quantum computers by using quantum programing types and operations provided by *dann5* libraries. 

The *d5* library in *dann5* package provides means of defining problems in the form of one, or in a sequence of bitwise, logical, or mathematical statements, which constitute a human easily understandable quantum program. The *dann5* package provides libraries which assist developers in converting a quantum program into a designated quantum computing language, such as QUBO/BQM, in case of D-Wave’s, or Qiskit circuits, in case of IBM’s, quantum solvers or simulators. Also, *dann5* libraries provide simple ways of employing local quantum simulators, or simple ways of connecting to remote quantum solvers and simulators to compute and retrieve solutions for a given problem described by the quantum program. The *dann5* quantum programs can be executed on IBM’s analog quantum gate computers, or on D-Wave’s quantum annealers or hybrid computers, or on Azure quantum simulators using Azure Quantum. 

To write a quantum program, a Python or C++ developer uses dann5 predefined quantum types and operations, in a same way as using the language native types and corresponding operations. A developer can specify undefined and defined variables as a quantum-bit, q-boolean, q-binary, q-whole, q-integer, q-positive-rational and q-rational types. The specified quantum variables can be linked into quantum-statements such as q-expressions and q-assignments by applying type appropriate quantum-operations to describe a problem. The quantum-statements can be executed on different quantum computing resources or further organized into a q-block, q-routine, or a q-function forming a quantum program, which describes a more complex problem.

In a way *dann5* is 3rd generation quantum programming language with built-in virtual machine that interprets a quantum program in a real time and executes a specific quantum assembler interpretation on a targeted quantum computing resource. In the case of IBM, *dann5 virtual code (d5vc)* is converted into a Qiskit interpretation forming a bespoke quantum-circuit. The bespoke Qiskit circuit can be executed on targets such as QasmSimulator or any IBM quantum computing backend. Also, dann5 virtual code can be translated into a QUBO interpretation and as such executed using D-Wave Ocean SDK on D-Wave’s quantum simulator, annealer and hybrid computing resources, such as Advantage2, or Hybrid Solver.

You can download *dann5* libraries as a [Python package](https://pypi.org/project/dann5/), or cross platform (Windows, Mac and Linux) [dann5 C++ source code projects]( https://github.com/voya-voja/dann5.3 ) .

## Quantum programming with *dann5*
To start learning about quantum programming and quantum computing platforms, any developer can very quickly prepare a quantum programming environment by following these seven steps:
1.	Download and install [Python 3.10.11 (64-bit)](https://www.python.org/downloads/release/python-31011/)
2.	Create Python virtual environment for quantum programming
3.	Install and test [dann5](https://pypi.org/project/dann5/) package
4.	Install Use D-Wave [Ocean SDK](https://docs.ocean.D-Wavesys.com/en/latest/overview/install.html)
5.	Set up your D-Wave [Leap](https://cloud.D-Wavesys.com/leap/) account
6.	Install [IBM Qiskit](https://qiskit.org/documentation/getting_started.html)
7.	Download [dann5 notebooks]( https://github.com/voya-voja/dann5.3/tree/master/notebooks) and start emerging into quantum programming

### 1. Download and install [Python 3.10.11 (64-bit)](https://www.python.org/downloads/release/python-31011/)

***Windows installation***

**python.exe** will be installed in *%userprofile%/AppData/Local/Programs/Python/Python310* folder.
Set up Python environment variables:
1. Open *Administrative Tools* (i.e. *Control Panel*)
2. Using *Search Control Panel* box (in top right corner) search for *environment* and press *enter*
3. Click on *Edit the system environment variables* link
4. *System Properties* box will open, click on *Environment variables...* button (at bottom-right corner)
5. *Environment variables* box will open
6. If *PYTHONHOME* already exist in *User variables for ...*, use *Edit..." button to change it, otherwise use *New...* button
7. Set *PYTHONHOME* to *%userprofile%/AppData/Local/Programs/Python/Python310*
8. If *PYTHONPATH* already exist in *User variables for ...*, use *Edit..." button to change it, otherwise use *New...* button
9. Set *PYTHONPATH* to *%PYTHONHOME%*
10. If *PATH* already exist in *User variables for ...*, use *Edit..." button to change it, otherwise use *New...* button
11. Make sure that *PATH* includes *%userprofile%\AppData\Local\Programs\Python\Python310\Scripts\* and %userprofile%\AppData\Local\Programs\Python\Python310\* paths

> python --version

Python 3.10.11

***MAC and Linux installation***

**python3** will be installed in */usr/bin* folder, which is on the path.

> python3 --version

Python 3.10.11 

Now **python** is ready to set up a new virtual environment. To verify all is ready, open *Command Prompt* window and run 

### 2. Create Python virtual environment for quantum programming (VE4QP)
Create a virtual environment for your Quantum work. To create **d5 VE4QP**):
1. Open *Command Prompt* window
2. Go to folder where *d5* virtual environment folder should be created
    - e.g. in case of windows you can create *Envs* folder in *%userprofile%/AppData/Local* folder and run the following command
    > cd %userprofile%/AppData/Local/Envs
    - in case of mac or linux *Envs* folder can be created in user's 'HOME' folder. Make shure the *Envs* is a working directory by moving into it:
    > cd ~/Envs
3. Run the following command to create a virtual environment called **d5**
    - in case of Windows:
    
    > python -m venv d5
    
    - or in case of mac or linux:
    
    > python3 -m venv d5
4. *Activate* the VE4QP by running:
    - in case of Windows:
    > %userprofile%/AppData/Local/Envs/d5/Scripts/activate
    - or in case of mac and linux
    > source ~/Envs/d5/Scripts/activate
5. As a result, the prompt will change to begin with *(d5)*
6. Upgrade *pip* by running:
    > python -m pip install --upgrade pip
7. To be able to execute or create *jupyterlab and notebook* [install jupyter packages](https://jupyter.org/install) by running:
    > pip install --upgrade jupyterlab
    
    > pip install --upgrade notebook

    
***Install Spyder***

To be able to write or debug python code download and install [Spyder 5.5.1](https://www.spyder-ide.org/)
- Spyder comes with selected python 3.7.9 package. To use Python 3.10.11 in your *virtual environment for quantum programming (VE4QP)* 
    1. Install spyder-kernels and matplotlib, using *Command Prompt* in the **active VE4QP**, e.g. *d5* 
    
    > pip install --upgrade matplotlib
    >
    > pip install spyder-kernels==5.5.*
        
    - **Note**: make sure spyder kernel version is correct!
        
    2. Open Spyder from *Windows Start menu*
    3. Change Spyder’s default Python interpreter by click the name of the current environment (i.e. *custom(Python 3.7.5)*) in the status bar, 
    4. then click *Change default environment in Preferences...*, which will open the *Preferences* dialog in the Python interpreter section. 
    5. select the option *Use the following Python interpreter*, 
    6. use the text box or the Select file button to enter the path to the Python interpreter in your VE4QP, e.g.:
        > %userprofile%/AppData/Local/Envs/d5/Scripts/python.exe 
    - The name of the current environment in the status bar should change to *custom(Python 3.10.11)*. 
    - See the [IPython Console](https://docs.spyder-ide.org/current/panes/ipythonconsole.html)  for more information.

**Now your *d5* virtual environment is ready for installation of quantum programming packages!** 

### 3. Quantum programming with [dann5.d5](https://pypi.org/project/dann5/) 
To write a simple quantum program that you can run on a quantum simulator, quantum annealer or quantum computer you should install **dann5 package of libraries**, in your VE4QP, e.g. *active d5 virtual environment*:

> pip install --upgrade dann5

This will install pybind11 and dann5 packages into %userprofile%/AppData/Local/Envs/d5/Lib/site-packages, in case of Windows, or ~/Envs/d5/Lib/site-packages, in case of mac or linux.

To test your local VE4QP, you can run the following code using **python** from a *Command Prompt*, or use **spyder** as an *IDE*.
- The following code finds all possible combinations of 3 numbers that will add to the number 10, where number **p** is *unknown q-whole number with 3 q-bits in superposition state*, while **q** and **r** are two *unknown q-whole numbers with 2 q-bits* each, and where python variable **Sum** references **S** q-whole number with deterministic value 10.

- **sumAssignmnet** is a python variable which references **a quantum assignment of p, q and r addition expression to the S q-whole number**.


> import dann5.d5 as d5
> 
> from dann5.dwave import Solver
> 
> p = d5.Qwhole(3,"p")
> 
> q = d5.Qwhole(2, "q")
> 
> r = d5.Qwhole(2, "r")
> 
> Sum = d5.Qwhole("S", 10)
> 
> sumAssignment = Sum.assign(p + q + r)
> 
> print(sumAssignment)
>
S\4:10\ = ((p\3:U\ + q\2:U\) + r\2:U\)

-  The *sumAssignment.solve()* method uses dann5.d5o quantum annealing simulator to identify all possible solutions for **p, q and r** (shown in code below).

- Before *solve()* methd is called, we need to call *Solver.Active()* to activate the default dann5 solver simulating solutions.

>
> Solver.Active()
>
> sumAssignment.solve()
>
> print("d5o simulation solutions: \n{}".format(sumAssignment.solutions()))

The *sumAssignment.solutions()* method returns line by line all found solutions of expression **S = 10 = p[3] + q[2] + r[2]**, where each variable is presented as 
- *variable_name* ***/*** *#_of_q-bits* ***:*** *varaible_value* ***/***

d5 simulation solutions: 
S\4:10\; _+0\4:13\; p\3:6\; q\2:2\; r\2:2\
S\4:10\; _+0\4:13\; p\3:4\; q\2:3\; r\2:3\
S\4:10\; _+0\4:13\; p\3:6\; q\2:1\; r\2:3\
S\4:10\; _+0\4:13\; p\3:5\; q\2:2\; r\2:3\
S\4:10\; _+0\4:13\; p\3:5\; q\2:3\; r\2:2\
S\4:10\; _+0\4:13\; p\3:7\; q\2:0\; r\2:3\
S\4:10\; _+0\4:13\; p\3:7\; q\2:1\; r\2:2\
S\4:10\; _+0\4:15\; p\3:6\; q\2:3\; r\2:1\
S\4:10\; _+0\4:15\; p\3:7\; q\2:2\; r\2:1\
S\4:10\; _+0\4:15\; p\3:7\; q\2:3\; r\2:0\

The *sumAssignment.solutions()* method returns line by line all found solutions of expression **S = 10 = p[3 qb] + q[2 qb] + r[2 qb]**, where each variable is presented as 
> **variable_name** \\ **#_of_q-bits : varaible_value** \\, e.g. p\\3:6\\; q\\2:2\\; r\\2:2\\.

Additionally, any variable named **'_< sign >#'** (where *#* is a number) is an auxiliary variable. For example, an addition auxiliary variable is **_+0** with **4 qbits** and **value 13**.

### 4. To Use D-Wave Install [Ocean SDK](https://docs.ocean.D-Wavesys.com/en/latest/overview/install.html)
If you would like to develop a quantum solution to be executed on *D-Wave quantum annealer, hybrid-computer or simulator*, you have to create a developer account in *D-Wave Leap* cloud and install *D-Wave Ocean SDK* in local *QVE*.
1. To create D-Wave Leap developer account you need a *[github account](https://github.com/)*. If you don't, [create one](https://docs.github.com/en/get-started/signing-up-for-github/signing-up-for-a-new-github-account).
2. Create a developer account on [*D-Wave Leap*](https://cloud.D-Wavesys.com/leap/signup/).
3. [Log in](https://cloud.D-Wavesys.com/leap/login/?next=/leap/) using your D-Wave Leap developer account.
    - Explore D-Wave Leap landing page and locate *API Token*, which you will need to configure D-Wave Ocean in your local QVE.
    - You can develop/debug D-Wave specialized quantum solutions in Leap, by creating your *Leap IDE*, under *Resources*.
4. [Install](https://docs.ocean.D-Wavesys.com/en/latest/overview/install.html) and  by running following commands using *Command Prompt* in your local **active QVE**
    > pip install --upgrade D-Wave-ocean-sdk
5. [configure](https://docs.ocean.D-Wavesys.com/en/stable/docs_cli.html) D-Wave Ocean in your local QVE by running:
    > D-Wave config create
    1. when prompted *Available profiles: defaults* just press *enter* 
    2. when prompted *Profile (select existing or create new) [defaults]:* just press *enter* 
    3. when prompted to enter *Authentication token [skip]:* past the *API Token* that you have copied from your D-Wave Leap landing page and press *enter*
    - The result should be:
        > Using the simplified configuration flow.
        >
        > Try 'D-Wave config create --full' for more options.
        >
        >
        > Updating existing configuration file: %userprofile%\AppData\Local\D-Wavesystem\D-Wave\D-Wave.conf
        >
        > **Available profiles: defaults**
        >
        > **Profile (select existing or create new) [defaults]:**
        >
        > Updating existing profile: defaults
        >
        > **Authentication token [skip]:** DEV-#########################
        >
        > Configuration saved.

6. [Test communications](https://docs.ocean.D-Wavesys.com/en/latest/overview/sapi.html) with the D-Wave quantum computer by running:
    > D-Wave ping --client qpu
    - If you encounter SSLError, you need to download and past certificates recognized by D-Wave endpoint into *cacert.pem* file located in *Lib\site-packages\certifi\* in your local *QVE* by following these [instructions](https://support.D-Wavesys.com/hc/en-us/community/posts/360018930954-Resolving-SSL-certificate-verify-fails-error-message-from-D-Wave-ping-command). Step-by-step instructions for Windows are one third down the page. Search for *Windows specific instructions* to locate them.

Now your local *QVE* is ready for development of quantum solutions, which you can confirm by submitting a random problem to a remote solver by running following command using *Command Prompt* in **active QVE**.
> D-Wave sample --random-problem

Also, you can use installed *python* and *spyder* IDEs to develop python code and test it on [D-Wave simulators](https://docs.ocean.D-Wavesys.com/en/latest/docs_dimod/reference/sampler_composites/samplers.html), [quantum solvers](https://docs.ocean.D-Wavesys.com/en/stable/overview/qpu.html#using-qpu) or [hybrid  sampler](https://docs.ocean.D-Wavesys.com/en/stable/overview/samplers.html).


### 5. Attach your GitHub account to your D-Wave Leap account: [Leap Link](https://cloud.D-Wavesys.com/leap/)
For your D-Wave license to renew every month for free you will need to pass your GitHub account to the D-Wave account profile you have created. 

Use the link above to sign in and then click on the profile name in the top right corner of the D-Wave leap home page. A dropdown will appear and click on the button labeled "Expand you access".

Once the button is clicked you will be moved to a page with the D-Wave account options, click on the "Explore Developer Access" button in the Developer section and you will be moved to the Developer access page.

On that page you should have the option to input your GitHub username and a repository link, insert any repository you have, save the information and you will get the automatic renewed license for your D-Wave Leap account. 

### 6. Add [IBM Qiskit](https://qiskit.org/documentation/getting_started.html) to local quantum virtual environment 
To be able to use IBM's analog quantum gates computer you will need to create IBM Quantum cloud account, install Qiskit python package and set up your API key. 
1. You can sign in to [IBM Quantum](https://quantum-computing.ibm.com/) using your github account
2. Run the following command in your local virtual environment for quantum programming (VE4QP) to install qiskit package

    > pip install qiskit==0.44.3
    >
    > pip install qiskit-aer==0.13.0
    >
    > pip install qiskit-ibm-provider==0.7.0
    
3. After installation check the version of installed 'qiskit-terra' package is 0.25.3 by running:

    > pip list
    
3. [Instal your IBM Quantum API key](https://subscription.packtpub.com/book/programming/9781838828448/1/ch01lvl1sec06/installing-your-api-key-and-accessing-your-provider)
    1. Copy API token from you IBM Quantum dashboard
    2. From *Command Prompt* with active VE4QP (e.g. d5) run 
        > python
    3. In python run
        > \>>> from qiskit_ibm_provider import IBMProvider
        >
        > \>>> IBMProvider.save_account('#########')
        >
        > \>>> exit()
        
        - NOTE: in the code above replace *#########* with the **API token** that you have copied

Once all is done, you can run the follwing code to execute a block of quantum code **timesBlock** on IBM's qiskit simulator.
> **The problem statement**: When two quantum whole numbers with 2 q-bits each, named x and y, are equal, a multiplication expression of x times y has to be eaqual to z q-whole number.

- **Note**: also *timesBlock.toString(True)* call provides a view into a decomposed dann5 virtual quantum machine code of *timesBlock*.

### 7. Download [dann5 jupyter notebooks](https://github.com/voya-voja/dann5.3/tree/master/notebooks)

1. Create a folder on your local machine
2. Open a remote [dann5 notebooks folder](https://github.com/voya-voja/dann5.3/tree/master/notebooks) in your browser
3. Right click on a jupiter notebook you would like to download and select *'Save Link As'* from the popup menu
4. Save a notebook to the local folder
5. In a comand prompt/terminal window activate your VE4QP, e.g. *d5*
6. Go to the local notebook folder and start a jupiter notebook application
    - In *Windows* the command is:  start jupyter notebook
    - In *Mac* or *Linux* the comand is: (jupyter notebook)&
