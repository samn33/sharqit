import pyzx as zx

def make_gate_str(gate):

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

def convert(path_in, path_out):
    
    qc_ori = zx.Circuit.load(path_in)
    qc_in = qc_ori.to_basic_gates()
    with open(path_out, mode='w') as f:
        for gate in qc_in.gates:
            f.write(make_gate_str(gate) + "\n")
    
if __name__ == '__main__':

    with open("qcirc_list.txt") as f:
        lines = [s.rstrip() for s in f.readlines()]

    path_in_list = []
    path_out_list = []
    for line in lines:
        fname = line.split()[1]
        path_in_list.append("qcirc_pyzx/" + fname)
        path_out_list.append("qcirc_sharqit/" + fname + ".sqc")
        
    for pin, pout in zip(path_in_list, path_out_list):
        convert(pin, pout)
