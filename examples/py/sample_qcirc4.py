from sharqit import Phase, QCirc, QGate, QGateKind

def main():

    qc = QCirc()
    qc.add_qgate(QGate(QGateKind.H, [0]))
    qc.add_qgate(QGate(QGateKind.H, [1]))
    qc.add_qgate(QGate(QGateKind.CX, [0,1]))
    qc.add_qgate(QGate(QGateKind.H, [0]))
    qc.add_qgate(QGate(QGateKind.H, [1]))
    qc.add_qgate(QGate(QGateKind.RZ, [0], Phase(1,4)))
    qc.show()

    gate_list = qc.get_qgates()
    for gate in gate_list:
        print(gate)
        kind = gate.kind
        qid = gate.qid
        phase = gate.phase
        print("kind:{}, qid:{}, phase:{}".format(kind, qid, phase))


if __name__ == "__main__":
    main()
