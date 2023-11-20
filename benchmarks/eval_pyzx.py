from time import time
import pyzx as zx

def eval_pyzx(file_in):
    
    qc_ori = zx.Circuit.load(file_in)

    start = time()

    qc_in = qc_ori.to_basic_gates()
    zx.optimize.basic_optimization(qc_in)
    g = qc_in.to_graph()
    zx.simplify.full_reduce(g)
    qc_out = zx.extract_circuit(g.copy())

    ptime = time() - start

    return (ptime, qc_in.tcount(), qc_out.tcount())
    
if __name__ == '__main__':

    with open("qcirc_list.txt") as f:
        lines = [s.rstrip() for s in f.readlines()]

    name_list = []
    path_list = []
    for line in lines:
        name = line.split()[0]
        path = "qcirc_pyzx/" + line.split()[1]
        name_list.append(name)
        path_list.append(path)
    
    #name_list = ["tof_3", "gf2^8", "Adder16"]
    #path_list = ["qcirc_pyzx/tof_3_before",
    #             "qcirc_pyzx/gf2^E8_mult_before",
    #             "qcirc_pyzx/Adder16_before"]

    print("name,time[sec],T-count<in>,T-count<out>")
    for n,p in zip(name_list, path_list):
        (ptime, tcout_in, tcout_out) = eval_pyzx(p)
        s = "{},{},{},{}".format(n, ptime, tcout_in, tcout_out)
        print(s)
