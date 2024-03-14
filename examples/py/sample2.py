from sharqit import Phase, QCirc, Optimizer


def main():
    PI = Phase("PI")
    qc_in = QCirc()
    qc_in.h(0)
    qc_in.h(1)
    qc_in.cx(0, 1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.rz(0, PI / 4)
    qc_in.crz(0, 1, 3 * PI / 4)
    print("[Input quantum circuit]")
    qc_in.show()

    opt = Optimizer()
    qc_out = opt.reduce_gates(qc_in, "zx")
    print("[Output quantum circuit]")
    qc_out.show()

    print("[Stats of the input circuit]")
    print("x_count =", qc_in.x_count())
    print("z_count =", qc_in.z_count())
    print("h_count =", qc_in.h_count())
    print("s_count =", qc_in.s_count())
    print("t_count =", qc_in.t_count())
    print("rz_count =", qc_in.rz_count())
    print("cx_count =", qc_in.cx_count())
    print("twoq_count =", qc_in.twoq_count())
    print("gate_count =", qc_in.gate_count())
    print("depth =", qc_in.depth())
    print("qubit_num =", qc_in.qubit_num())

    print("[Stats of the output circuit]")
    print("x_count =", qc_out.x_count())
    print("z_count =", qc_out.z_count())
    print("h_count =", qc_out.h_count())
    print("s_count =", qc_out.s_count())
    print("t_count =", qc_out.t_count())
    print("rz_count =", qc_out.rz_count())
    print("cx_count =", qc_out.cx_count())
    print("gate_count =", qc_out.gate_count())
    print("depth =", qc_out.depth())
    print("qubit_num =", qc_out.qubit_num())

    print("[Processing time]")
    print("proc_time =", opt.proc_time())

    print("[Print stats of the input circuit]")
    qc_in.print_stats()

    print("[Print stats of the output circuit]")
    qc_out.print_stats()

    print("[Is the output one equal to input one?]")
    print(qc_in.is_equal(qc_out))

    print("[Save and load, show the loaded quantum circuit]")
    qc_in.save("foo.sqc")
    qc_tmp = QCirc()
    qc_tmp.load("foo.sqc")
    qc_tmp.show()


if __name__ == "__main__":
    main()
