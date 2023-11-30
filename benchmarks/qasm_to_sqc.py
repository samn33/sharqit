import pyzx as zx

## == INPUT: sample.qasm ==
#  OPENQASM 2.0;
#  include "qelib1.inc";
#  qreg q[3];
#  h q[1];
#  s q[0];
#  h q[2];
#  cx q[2], q[0];
#  s q[0];
#  cx q[0], q[1];
#  cx q[1], q[0];
#  h q[2];
#
## == OUTPUT: sample.sqc ==
#  H 1
#  S 0
#  H 2
#  CX 2 0
#  S 0
#  CX 0 1
#  CX 1 0
#  H 2

def gate_str(gate):

    gstr = ""
    if gate.name == "NOT":
        gstr = "X " + str(gate.target)
    elif gate.name == "Z":
        gstr = "Z " + str(gate.target)
    elif gate.name == "S" and gate.adjoint is False:
        gstr = "S " + str(gate.target)
    elif gate.name == "S" and gate.adjoint is True:
        gstr = "S+ " + str(gate.target)
    elif gate.name == "T" and gate.adjoint is False:
        gstr = "T " + str(gate.target)
    elif gate.name == "T" and gate.adjoint is True:
        gstr = "T+ " + str(gate.target)
    elif gate.name == "HAD":
        gstr = "H " + str(gate.target)
    elif gate.name == "CNOT":
        gstr = "CX " + str(gate.control) + " " + str(gate.target)

    return gstr

if __name__ == '__main__':
    
    qc_src = zx.Circuit.load("sample.qasm")
    qc_bas = qc_src.to_basic_gates()
    
    with open("sample.sqc", mode='w') as f:
        for gate in qc_bas.gates:
            f.write(gate_str(gate) + "\n")
