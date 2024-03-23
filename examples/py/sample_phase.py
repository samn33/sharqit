from sharqit import Phase, QGate, QCirc, Optimizer, QGateKind

def main():

    p = Phase(1)
    print(p)
    
    p = Phase(1,3)
    print(p)
    
    p = Phase("PI")
    print(p)

    q = Phase(1,2)
    r = p + q
    print(r)

    q = p / 2
    print(q)
    
    q *= 2
    print(q)

    r += Phase(1,8)
    print(r)
    
if __name__ == "__main__":
    main()
