from sharqit import QCirc, QGate

def main():

    qc = QCirc()
    qc.add_qgate("H 0")
    qc.add_qgate("H 1")
    qc.add_qgate("CX 0 1")
    qc.add_qgate("H 0")
    qc.add_qgate("H 1")
    qc.add_qgate("RZ(1/4) 0")
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
