sharq
=====

Quantum Circuit Optimization Library

## Feature

- Reducing T-count using ZX-calculus
- Implemented in C++ language

## Install

Install the following software first.

    $ sudo apt install nlohmann-json3-dev 
	$ sudo apt install graphviz
	$ sudo apt install libeigen3-dev

Install the library 'sharq'.

	$ git clone https://github.com/samn33/sharq.git
    $ cd sharq/src
    $ mkdir -p ~/lib ~/include
    $ make
    $ make install

Add folloing line to your ~/.bashrc. (If you are using another shell, replace as appropriate.)

    export LD_LIBRARY_PATH="${HOME}/lib:$LD_LIBRARY_PATH"

## Uninstall

    $ rm ~/lib/libsharq.so ~/include/sharq.h

## Usage

### Simple examples

#### Random quantum circuit.

    $ cat sample1.cpp
    #include "sharq.h"
    
    int main()
    {
      Sharq::QCirc qc_in;
      // random circuit (qubit-num=5, gate-num=20, X:T:CX=4:5:3)
      qc_in.add_random(5, 20, {{"X", 4},{"T", 5},{"CX", 3}});
    
      qc_in.show(); // show the circuit
      qc_in.save("in.sqc"); // save to text file
      std::cout << "T-count (in) = " << qc_in.tcount() << std::endl;
    
      Sharq::Optimizer opt;
      Sharq::QCirc qc_out = opt.execute(qc_in); // circuit optimization
    
      qc_out.show(); // show the circuit
      qc_out.save("out.sqc"); // save to text file
      std::cout << "T-count (out) = " << qc_out.tcount() << std::endl;
    
      return 0;
    }

Build and execute it.

    $ g++ -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample1.cpp -lsharq
	$ ./a.out

#### Explicitly specified the quantum circuit

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

#### Load quantum circuit

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

## Benchmarks

![benchmarks](/benchmarks/plot.png)

## Requirements

- Linux (Ubuntu 22.04 LTS)

## Licence

MIT

## Author

[Sam.N](http://github.com/samn33)
