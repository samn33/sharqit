# -*- coding: utf-8 -*-
import unittest

from sharqit import QGate, QGateKind


class QGate_to_string(unittest.TestCase):
    """test 'to_string_1'"""

    def test_to_string_1(self):
        """test 'string_1'"""
        qgate = QGate("H 0")
        self.assertEqual(qgate.to_string(), "H 0")

    def test_to_string_2(self):
        """test 'string_2'"""
        qgate = QGate("RZ(1/3) 0")
        self.assertEqual(qgate.to_string(), "RZ(1/3) 0")

    def test_to_string_3(self):
        """test 'string_3'"""
        qgate = QGate("CX 0 1")
        self.assertEqual(qgate.to_string(), "CX 0 1")

class QGate_property(unittest.TestCase):
    """test 'to_property'"""

    def test_to_property_1(self):
        """test 'property_1'"""
        qgate = QGate("H 0")
        self.assertEqual(qgate.kind, QGateKind.H)
        self.assertEqual(qgate.qid, [0])

    def test_to_property_2(self):
        """test 'property_2'"""
        qgate = QGate("RZ(1/3) 0")
        self.assertEqual(qgate.kind, QGateKind.RZ)
        self.assertEqual(qgate.qid, [0])
        self.assertEqual(qgate.phase.to_string(), "1/3")

    def test_to_property_3(self):
        """test 'property_3'"""
        qgate = QGate("CX 0 1")
        self.assertEqual(qgate.to_string(), "CX 0 1")
        self.assertEqual(qgate.kind, QGateKind.CX)
        self.assertEqual(qgate.qid, [0,1])


if __name__ == "__main__":
    unittest.main()
