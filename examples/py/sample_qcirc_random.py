from sharqit import Phase, QCirc, Optimizer


def main():
    qc = QCirc()
    qc.add_random(3, 100, "X:1,Z:1,H:1,S:1,T:1,CX:1,CZ:1")
    qc.show()
    qc.print_stats()


if __name__ == "__main__":
    main()
