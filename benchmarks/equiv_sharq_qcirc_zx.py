from fractions import Fraction
import pyzx as zx

def pyzx_qcirc(lines):

    maxq = 0
    for line in lines:
        token = line.split()
        if len(token) == 2 and token[0][0:2] != "RZ":
            qid = [int(token[1])]
        elif len(token) == 2 and token[0][0:2] == "RZ":
            qid = [int(token[1])]
        elif len(token) == 3:
            qid = [int(token[1]), int(token[2])]
        for q in qid:
            if q > maxq:
                maxq = q
    maxq += 1
    
    qc = zx.Circuit(maxq)

    for line in lines:
        token = line.split()
        if len(token) == 2 and token[0][0:2] != "RZ":
            gstr = token[0]
            qid = [int(token[1])]
        elif len(token) == 2 and token[0][0:2] == "RZ":
            gstr = token[0][0:2]
            pstr = token[0][3:len(token[0])-1]
            pstr_frac = pstr.split("/")
            if len(pstr_frac) == 1:
                p_nume = int(pstr_frac[0])
                p_deno = None
            elif len(pstr_frac) == 2:
                p_nume = int(pstr_frac[0])
                p_deno = int(pstr_frac[1])
            qid = [int(token[1])]
        elif len(token) == 3 and token[0] == "CX":
            gstr = token[0]
            qid = [int(token[1]), int(token[2])]
        elif len(token) == 3 and token[0] == "CZ":
            gstr = token[0]
            qid = [int(token[1]), int(token[2])]

        if gstr == "CX":
            qc.add_gate("CNOT", qid[0], qid[1])
        elif gstr == "CZ":
            qc.add_gate("CZ", qid[0], qid[1])
        elif gstr == "RZ":
            if p_deno is None:
                qc.add_gate("ZPhase", qid[0], phase=Fraction(p_nume))
            else:
                qc.add_gate("ZPhase", qid[0], phase=Fraction(p_nume, p_deno))
        elif gstr == "X":
            qc.add_gate("NOT", qid[0])
        elif gstr == "Z":
            qc.add_gate("Z", qid[0])
        elif gstr == "S":
            qc.add_gate("S", qid[0])
        elif gstr == "S+":
            qc.add_gate("S", qid[0], adjoint=True)
        elif gstr == "T":
            qc.add_gate("T", qid[0])
        elif gstr == "T+":
            qc.add_gate("T", qid[0], adjoint=True)
        elif gstr == "H":
            qc.add_gate("HAD", qid[0])

    return qc

def equiv(path_in, path_out):

    with open(path_in, mode='r') as f:
        lines_in = [s.rstrip() for s in f.readlines()]

    with open(path_out, mode='r') as f:
        lines_out = [s.rstrip() for s in f.readlines()]

    qc_in = pyzx_qcirc(lines_in)
    qc_out = pyzx_qcirc(lines_out)

    return qc_in.verify_equality(qc_out)

if __name__ == '__main__':

    with open("qcirc_list.txt") as f:
        lines = [s.rstrip() for s in f.readlines()]

    name_list = []
    path_in_list = []
    path_out_list = []
    for line in lines:
        name = line.split()[0]
        path_in = "qcirc_sharq/" + line.split()[1] + ".sqc"
        path_out = "qcirc_sharq/zx/" + line.split()[2] + ".sqc"
        name_list.append(name)
        path_in_list.append(path_in)
        path_out_list.append(path_out)
        
    for n, pin, pout in zip(name_list, path_in_list, path_out_list):
        ans = equiv(pin, pout)
        print(n, ans)
