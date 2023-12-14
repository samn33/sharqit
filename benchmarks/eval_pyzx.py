from time import time
import pyzx as zx

def eval_pyzx(file_in):
    
    qc_ori = zx.Circuit.load(file_in)
    qc_in = qc_ori.to_basic_gates()

    start = time()

    zx.optimize.basic_optimization(qc_in)
    g = qc_in.to_graph()
    zx.simplify.full_reduce(g)
    qc_out = zx.extract_circuit(g.copy())

    ptime = time() - start

    stats_in = qc_in.stats_dict()
    stats_out = qc_out.to_basic_gates().stats_dict()
    return (ptime, stats_in['tcount'], stats_out['tcount'], stats_in['cnot'], stats_out['cnot'], stats_in['gates'], stats_out['gates'])
    
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
    
    print("name,time[sec],T-count<in>,T-count<out>")
    for n,p in zip(name_list, path_list):
        (ptime, tcount_in, tcount_out, cxcount_in, cxcount_out, gcount_in, gcount_out) = eval_pyzx(p)
        s = "{},{},{},{},{},{},{},{}".format(n, ptime, tcount_in, tcount_out, cxcount_in, cxcount_out, gcount_in, gcount_out)
        print(s)
