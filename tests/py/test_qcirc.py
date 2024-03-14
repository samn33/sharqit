# -*- coding: utf-8 -*-
import unittest

from sharqit import Phase, QCirc


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


if __name__ == "__main__":
    unittest.main()
