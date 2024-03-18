sharqit
=======

Quantum Circuit Optimizer

## Feature

- Support two types of gate-count reduction methods:
	- 1) using ZX-calculus
	- 2) using Phase Polynomial
- Provide SDK for both C++ and Python.

## Install

Install the following softwares first.

    $ sudo apt install nlohmann-json3-dev 
	$ sudo apt install graphviz
	$ sudo apt install libeigen3-dev

For Python SDK, need to install the folloing package.

    $ pip install nanobind

### C++ SDK

Install sharqit command, sharqit library and related hedder files as follows,

    $ git clone https://github.com/samn33/sharqit.git
    $ mkdir -p ~/lib ~/bin ~/include/sharqit
    $ cd sharqit/sharqit/cpp
    $ make
    $ make install

Add following lines to your ~/.bashrc. (If you are using another shell, replace as appropriate.)

    export LD_LIBRARY_PATH="${HOME}/lib:$LD_LIBRARY_PATH"
    export PATH="${HOME}/bin:$PATH"

### Python SDK

Install sharqit package as follows,

    $ git clone https://github.com/samn33/sharqit.git
    $ cd sharqit
    $ python setup.py install --user


## Uninstall

### Python SDK

    $ pip uninstall sharqit

### C++ SDK

    $ make uninstall


## Usage

### C++ SDK

#### sharqit command

Prepare the quantum circuit you want to optimize as follows.

    $ cat sample.sqc
    T 1
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    T+ 1

You can display the quantum circuit.

    $ sharqit --show sample.sqc
	q[0] --H-----*--H------
	q[1] --T--H--X--H--T+--

Optimize and display the result.

    $ sharqit --opt sample.sqc > foo.sqc
    $ sharqit --show foo.sqc
    q[0] --X--
    q[1] --*--

Print help message.

    $ sharqit --help

#### sharqit library

An example of C++ code that uses the sharqit c++ library.

    $ cat sample.cpp
    #include "sharqit/sharqit.h"
        
    int main()
    {
      Sharqit::QCirc qc_in;
      qc_in.t(1);
      qc_in.h(0);
      qc_in.h(1);
      qc_in.cx(0,1);
      qc_in.h(0);
      qc_in.h(1);
      qc_in.tdg(1);
      qc_in.show();
    
      Sharqit::Optimizer opt;
      Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, "zx");
      qc_out.show();
        
      return 0;
    }

Build it.

    $ g++ -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample.cpp -lshrqt

Execute a.out.

    $ ./a.out
    q[0] --H-----*--H------
    q[1] --T--H--X--H--T+--
    q[0] --X--
    q[1] --*--

### Python API

#### sharqit package

An example of Python code that uses the sharqit package.

    $ cat sample.py
    from sharqit import Phase, QCirc, Optimizer
    
    qc_in = QCirc()
    qc_in.t(1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.cx(0,1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.tdg(1)
    qc_in.show()
    
    opt = Optimizer()
    qc_out = opt.reduce_gates(qc_in, "zx")
    qc_out.show()

Execute the code.

    $ python sample.py
    q[0] --H-----*--H------
    q[1] --T--H--X--H--T+--
    q[0] --X--
    q[1] --*--

### How to convert from other file format

Sample code converting from qasm file is [here](benchmarks/qasm_to_sqc.py).


## Benchmarks

Processing time, T-count, 2Q-count, Gate-count of 'sharqit' are compared with [PyZX](https://github.com/Quantomatic/pyzx). The 'zx' means the metohd using ZXCalculus, the 'pp' means the method using PhasePolynomial. The operating environment is 13th Gen Intel(R) Core(TM) i7-1355U @5GHz, 16GB RAM.

![benchmarks](/benchmarks/plot.png)

Quantum circuit data used in the benchmarks are from [optimizer: Benchmark quantum circuits before and after optimization](https://github.com/njross/optimizer).


## Documents

- [Tutorial (japanese)](doc/tutorial/jp/main.md)


## References

Papers about T-count reduction using ZX-calculus.

1. Ross Duncan, Aleks Kissinger, Simon Perdrix, John van de Wetering,
"Graph-theoretic Simplification of Quantum Circuits with the ZX-calculus",
[arXiv:1902.03178](https://arxiv.org/abs/1902.03178)

2. Aleks Kissinger, John van de Wetering,
"Reducing T-count with the ZX-calculus",
[arXiv:1903.10477](https://arxiv.org/abs/1903.10477)

3. Miriam Backens, Hector Miller-Bakewell, Giovanni de Felice, Leo Lobski, John van de Wetering,
"There and back again: A circuit extraction tale",
[arXiv:2003.01664](https://arxiv.org/abs/2003.01664)

4. Korbinian Staudacher,
"Optimization Approaches for Quantum Circuits using ZX-calculus"
[Ludwig maximilian university of munich thesis](https://www.mnm-team.org/pub/Diplomarbeiten/stau21/PDF-Version/stau21.pdf)

Papers about gate-count reduction using Phase Polynomial.

5. Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov,
"Automated optimization of large quantum circuits with continuous parameters",
[arXiv:1710.07345](https://arxiv.org/abs/1710.07345)


## Requirements

- Linux (Ubuntu 22.04 LTS)


## Licence

MIT


## Author

[Sam.N](http://github.com/samn33)
