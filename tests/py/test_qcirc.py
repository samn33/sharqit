# -*- coding: utf-8 -*-
import unittest

from sharqit import Phase, QCirc, QGate, QGateKind


class QCirc_to_string(unittest.TestCase):
    """test 'to_string_1'"""

    def test_to_string_1(self):
        """test 'string_1'"""
        qc = QCirc()
        qc.h(0)
        qc.cx(0, 1)
        s = qc.to_string()
        self.assertEqual(s, "q[0] --H--*--\nq[1] -----X--\n")


class QCirc_stats(unittest.TestCase):
    """test 'stats_1'"""

    def test_stats_1(self):
        """test 'stats_1'"""
        PI = Phase("PI")
        qc = QCirc()
        qc.x(0)
        qc.z(0)
        qc.s(1)
        qc.sdg(2)
        qc.t(1)
        qc.tdg(2)
        qc.rz(1, 3 * PI / 4)
        qc.cx(0, 2)
        qc.cz(0, 2)
        qc.h(1)
        self.assertEqual(qc.x_count(), 1)
        self.assertEqual(qc.z_count(), 1)
        self.assertEqual(qc.s_count(), 2)
        self.assertEqual(qc.t_count(), 2)
        self.assertEqual(qc.rz_count(), 6)
        self.assertEqual(qc.h_count(), 1)
        self.assertEqual(qc.twoq_count(), 2)
        self.assertEqual(qc.gate_count(), 10)
        self.assertEqual(qc.depth(), 4)
        self.assertEqual(qc.qubit_num(), 3)


class QCirc_add_gate(unittest.TestCase):
    """test 'add_gate'"""

    def test_add_gate_1(self):
        """test 'add_gate_1'"""
        qc = QCirc()
        qc.add_qgate("X 0")
        qc.add_qgate("Z 0")
        qc.add_qgate("S 1")
        qc.add_qgate("S+ 2")
        qc.add_qgate("T 1")
        qc.add_qgate("T+ 2")
        qc.add_qgate("RZ(3/4) 1")
        qc.add_qgate("CX 0 2")
        qc.add_qgate("CZ 0 2")
        qc.add_qgate("H 1")
        self.assertEqual(qc.x_count(), 1)
        self.assertEqual(qc.z_count(), 1)
        self.assertEqual(qc.s_count(), 2)
        self.assertEqual(qc.t_count(), 2)
        self.assertEqual(qc.rz_count(), 6)
        self.assertEqual(qc.h_count(), 1)
        self.assertEqual(qc.twoq_count(), 2)
        self.assertEqual(qc.gate_count(), 10)
        self.assertEqual(qc.depth(), 4)
        self.assertEqual(qc.qubit_num(), 3)

    def test_add_gate_2(self):
        """test 'add_gate_2'"""
        PI = Phase("PI")
        qc = QCirc()
        qc.add_qgate(QGate(QGateKind.X, [0]))
        qc.add_qgate(QGate(QGateKind.Z, [0]))
        qc.add_qgate(QGate(QGateKind.S, [1]))
        qc.add_qgate(QGate(QGateKind.Sdg, [2]))
        qc.add_qgate(QGate(QGateKind.T, [1]))
        qc.add_qgate(QGate(QGateKind.Tdg, [2]))
        qc.add_qgate(QGate(QGateKind.RZ, [1], 3 * PI / 4))
        qc.add_qgate(QGate(QGateKind.CX, [0,2]))
        qc.add_qgate(QGate(QGateKind.CZ, [0,2]))
        qc.add_qgate(QGate(QGateKind.H, [1]))
        self.assertEqual(qc.x_count(), 1)
        self.assertEqual(qc.z_count(), 1)
        self.assertEqual(qc.s_count(), 2)
        self.assertEqual(qc.t_count(), 2)
        self.assertEqual(qc.rz_count(), 6)
        self.assertEqual(qc.h_count(), 1)
        self.assertEqual(qc.twoq_count(), 2)
        self.assertEqual(qc.gate_count(), 10)
        self.assertEqual(qc.depth(), 4)
        self.assertEqual(qc.qubit_num(), 3)


class QCirc_get_qgate(unittest.TestCase):
    """test 'get_qgate'"""

    def test_get_qgate_1(self):
        """test 'get_qgate_1'"""
        PI = Phase("PI")
        qc = QCirc()
        qc.x(0)
        qc.z(0)
        qc.s(1)
        qc.sdg(2)
        qc.t(1)
        qc.tdg(2)
        qc.rz(1, 3 * PI / 4)
        qc.cx(0, 2)
        qc.cz(0, 2)
        qc.h(1)
        self.assertEqual(qc.get_qgate(0).to_string(), "X 0")
        self.assertEqual(qc.get_qgate(1).to_string(), "Z 0")
        self.assertEqual(qc.get_qgate(2).to_string(), "S 1")
        self.assertEqual(qc.get_qgate(3).to_string(), "S+ 2")
        self.assertEqual(qc.get_qgate(4).to_string(), "T 1")
        self.assertEqual(qc.get_qgate(5).to_string(), "T+ 2")
        self.assertEqual(qc.get_qgate(6).to_string(), "RZ(3/4) 1")
        self.assertEqual(qc.get_qgate(7).to_string(), "CX 0 2")
        self.assertEqual(qc.get_qgate(8).to_string(), "CZ 0 2")
        self.assertEqual(qc.get_qgate(9).to_string(), "H 1")


class QCirc_get_qgates(unittest.TestCase):
    """test 'get_qgates'"""

    def test_get_qgates_1(self):
        """test 'get_qgates_1'"""
        PI = Phase("PI")
        qc = QCirc()
        qc.x(0)
        qc.z(0)
        qc.s(1)
        qc.sdg(2)
        qc.t(1)
        qc.tdg(2)
        qc.rz(1, 3 * PI / 4)
        qc.cx(0, 2)
        qc.cz(0, 2)
        qc.h(1)
        qgate_list = qc.get_qgates()
        self.assertEqual(qgate_list[0].to_string(), "X 0")
        self.assertEqual(qgate_list[1].to_string(), "Z 0")
        self.assertEqual(qgate_list[2].to_string(), "S 1")
        self.assertEqual(qgate_list[3].to_string(), "S+ 2")
        self.assertEqual(qgate_list[4].to_string(), "T 1")
        self.assertEqual(qgate_list[5].to_string(), "T+ 2")
        self.assertEqual(qgate_list[6].to_string(), "RZ(3/4) 1")
        self.assertEqual(qgate_list[7].to_string(), "CX 0 2")
        self.assertEqual(qgate_list[8].to_string(), "CZ 0 2")
        self.assertEqual(qgate_list[9].to_string(), "H 1")


if __name__ == "__main__":
    unittest.main()
