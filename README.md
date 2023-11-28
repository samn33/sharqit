sharq
=====

Quantum Circuit Optimizer

## Feature

- Reducing T-count using ZX-calculus
- Implemented in C++ language

## Install

Install the following software first.

    $ sudo apt install nlohmann-json3-dev 
	$ sudo apt install graphviz
	$ sudo apt install libeigen3-dev

Install the library 'libsharq.so' and 'sharq' command.

	$ git clone https://github.com/samn33/sharq.git
    $ cd sharq/src
    $ mkdir -p ~/lib ~/include ~/bin
    $ make
    $ make install

Add following lines to your ~/.bashrc. (If you are using another shell, replace as appropriate.)

    export LD_LIBRARY_PATH="${HOME}/lib:$LD_LIBRARY_PATH"
    export PATH="${HOME}/bin:$PATH"

## Uninstall

    $ rm ~/lib/libsharq.so ~/include/sharq.h ~/bin/sharq

## Usage

### Sharq library

#### Random quantum circuit

    $ cat sample1.cpp
    #include "sharq.h"
    
    int main()
    {
      Sharq::QCirc qc_in;
      // random circuit (qubit-num=5, gate-num=20, X:T:CX=4:5:3)
      qc_in.add_random(5, 20, {{"X", 4},{"T", 5},{"CX", 3}});
    
      qc_in.show(); // show the circuit
      qc_in.save("in.sqc"); // save to text file
      std::cout << "T-count (in) = " << qc_in.t_count() << std::endl;
    
      Sharq::Optimizer opt;
      Sharq::QCirc qc_out = opt.execute(qc_in); // circuit optimization
    
      qc_out.show(); // show the circuit
      qc_out.save("out.sqc"); // save to text file
      std::cout << "T-count (out) = " << qc_out.t_count() << std::endl;
    
      return 0;
    }

Build and execute it.

    $ g++ -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample1.cpp -lsharq
	$ ./a.out

#### Explicitly specified quantum circuit

    $ cat sample2.cpp
	...
    Sharq::QCirc qc_in;
    qc_in.h(3).h(4).cx(1,4).tdg(4).cx(0,4).t(4).cx(1,4).t(1).tdg(4);
    qc_in.cx(0,4).cx(0,1).t(4).t(0).tdg(1).cx(4,3).cx(0,1).tdg(3);
    qc_in.cx(2,3).t(3).cx(4,3).tdg(3).t(4).cx(2,3).cx(2,4).t(2).t(3);
    qc_in.tdg(4).cx(2,4).h(4).cx(1,4).tdg(4).cx(0,4).t(4).cx(1,4).t(1);
    qc_in.tdg(4).cx(0,4).cx(0,1).t(4).t(0).tdg(1).cx(0,1).h(3).h(4);
    ...
    Sharq::Optimizer opt;
    Sharq::QCirc qc_out = opt.execute(qc_in);
    ...

#### Quantum circuit loaded from file

    $ cat in.sqc
    H 3
    H 4
    CX 1 4
    T+ 4
    CX 0 4
    T 4
    CX 1 4
    T 1
    T+ 4
    CX 0 4
    CX 0 1
    ...

Use 'load' method to load the file.

    $ cat sample2.cpp
	...
    Sharq::QCirc qc_in;
    qc_in.load("in.sqc");
    ...
    Sharq::Optimizer opt;
    Sharq::QCirc qc_out = opt.execute(qc_in);
    ...

### Sharq command

Print help message.

    $ sharq --help
    sharq - quantum circuit optimizer
    [usage]
      sharq [option] [file] (> [file])
    [option]
      --opt FILE    : optimize the circuit file, output to stdout.
      --rand PARAMS : generate a random circuit file, output to stdout.
      --stats FILE  : print stats of the circut file.
      --show FILE   : print the circuit diagram as ascii text.
      --help        : print help message.
      --version     : print version.
    [examples]
      $ sharq --opt foo.sqc > bar.sqc
      $ sharq --rand 3,100,"X":1,"H":2,"T":3.5,"RZ(1/2)":1.5 > bar.sqc # 3 qubits,100 gates
      $ sharq --stats foo.sqc
      $ sharq --show foo.sqc
    ...

## Quantum circuit file format

Sharq supports a simple file format for quantum circuits as follows.

    $ cat foo.sqc"
    H 0
    CX 0 1
    RZ(1/2) 0
	H 2
    T+ 2
    ...

Supported quantum gates are X,Z,H,S,S+,T,T+,CX,RZ.
S+ and T+ are Hermitian conjugates of S and T respectively.
RZ gate have a phase factor denoted by fraction brackled in parentheses.
The unit of phase factor is radian, so 3/4 means 3PI/4, 1 means PI, and so on.


## Benchmarks

Processing time and T-count of 'sharq' are compared with [PyZX](https://github.com/Quantomatic/pyzx). The operating environment is Intel Core i5-3320M CPU @2.60GHz, 16GB RAM.

![benchmarks](/benchmarks/plot.png)

Quantum circuit data used in the benchmarks are from [optimizer: Benchmark quantum circuits before and after optimization](https://github.com/njross/optimizer).


## References

Papers about quantum circuit optimization using ZX-calculus.

1. Ross Duncan, Aleks Kissinger, Simon Perdrix, John van de Wetering,
"Graph-theoretic Simplification of Quantum Circuits with the ZX-calculus",
[arXiv:1902.03178](https://arxiv.org/abs/1902.03178)

2. Aleks Kissinger, John van de Wetering,
"Reducing T-count with the ZX-calculus",
[arXiv:1903.10477](https://arxiv.org/abs/1903.10477)


## Requirements

- Linux (Ubuntu 22.04 LTS)


## Licence

MIT


## Author

[Sam.N](http://github.com/samn33)
