from sharqit import Phase, QGate, QCirc, Optimizer, QGateKind

def main():

    qgate = QGate("X 0")
    print(qgate.to_string())
    print(qgate)

    qgate = QGate("RZ(1/2) 1")
    print(qgate.to_string())
    print(qgate)

    print(qgate.phase)
    qgate.phase = Phase(1,4)
    print(qgate.phase)
    print(qgate)

    print(qgate.qid)
    qgate.qid = [3]
    print(qgate.qid)
    print(qgate)

    print(qgate.kind)
    qgate.kind = QGateKind.X
    print(qgate.kind)

    kind = QGateKind.RZ
    phase = Phase(1,8)
    qid = [1]
    qgate = QGate(kind, qid, phase)
    print(qgate)
    
    kind = QGateKind.CX
    qid = [1,2]
    qgate = QGate(kind, qid)
    print(qgate)
    
if __name__ == "__main__":
    main()
