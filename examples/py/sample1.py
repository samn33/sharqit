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
    qc_in.show()

    opt = Optimizer()
    qc_out = opt.reduce_gates(qc_in, "zx")
    qc_out.show()

    print(qc_in.is_equal(qc_out))


if __name__ == "__main__":
    main()
