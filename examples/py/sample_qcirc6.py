from sharqit import Phase, QGate, QGateKind, QCirc

def main():

    qc = QCirc()
    qc.rz(0, Phase(3,4));
    qc.h(0)
    qc.h(1)
    qc.cx(0,1)
    qc.h(0)
    qc.h(1)
    qc.rz(1, Phase(1,4));

    qgate_list = qc.get_qgates()

    for qgate in qgate_list:
        if qgate.kind == QGateKind.RZ:
            print("kind: RZ, qid: {}, phase: {}".format(qgate.qid, qgate.phase))
        elif qgate.kind == QGateKind.H:
            print("kind: H, qid: {}".format(qgate.qid))
        elif qgate.kind == QGateKind.CX:
            print("kind: CX, qid: {}".format(qgate.qid))

    print()
    for qgate in qgate_list:
        print(qgate.to_string()) 

if __name__ == "__main__":
    main()
