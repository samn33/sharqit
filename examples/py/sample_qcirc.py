from sharqit import Phase, QCirc, Optimizer

def main():
    PI = Phase("PI")
    qc_in = QCirc()
    qc_in.t(1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.cx(0,1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.tdg(1);
    qc_in.show()

    opt = Optimizer()
    qc_out = opt.reduce_gates(qc_in, "zx")
    qc_out.show()


if __name__ == "__main__":
    main()
