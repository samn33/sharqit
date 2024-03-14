# -*- coding: utf-8 -*-
import unittest
from sharqit import Phase, QCirc, Optimizer


class Optimizer_reduce_gates(unittest.TestCase):
    """test 'reduce_gates'"""

    def test_reduce_gates_1(self):
        """test 'reduce_gates_1'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.h(0).cx(0, 1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_2(self):
        """test 'reduce_gates_2'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(1, 0).cx(0, 1).x(0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_3(self):
        """test 'reduce_gates_3'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.rz(0, 3 * PI / 2)
        qc_in.h(0)
        qc_in.s(0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_4(self):
        """test 'reduce_gates_4'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.rz(0, 3 * PI / 2)
        qc_in.h(0)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.h(3)
        qc_in.cx(1, 2)
        qc_in.cz(0, 3)
        qc_in.h(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_5(self):
        """test 'reduce_gates_5'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.rz(0, 3 * PI / 2)
        qc_in.h(0)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.h(3)
        qc_in.cx(1, 2)
        qc_in.cz(0, 3)
        qc_in.h(1)
        qc_in.cx(1, 2)
        qc_in.cx(3, 0)
        qc_in.h(0)
        qc_in.t(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_6(self):
        """test 'reduce_gates_6'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.rz(0, 3 * PI / 2)
        qc_in.h(0)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.h(3)
        qc_in.cx(1, 2)
        qc_in.cz(0, 3)
        qc_in.h(1)
        qc_in.cx(1, 2)
        qc_in.cx(3, 0)
        qc_in.h(0)
        qc_in.t(1)
        qc_in.s(2)
        qc_in.h(3)
        qc_in.t(0)
        qc_in.h(1)
        qc_in.h(2)
        qc_in.rz(3, 3 * PI / 2)
        qc_in.s(2)
        qc_in.x(3)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_7(self):
        """test 'reduce_gates_7'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.rz(0, 3 * PI / 2)
        qc_in.h(0)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.h(3)
        qc_in.cx(1, 2)
        qc_in.cz(0, 3)
        qc_in.h(1)
        qc_in.cx(1, 2)
        qc_in.cx(3, 0)
        qc_in.h(0)
        qc_in.t(1)
        qc_in.s(2)
        qc_in.h(3)
        qc_in.t(0)
        qc_in.h(1)
        qc_in.h(2)
        qc_in.rz(3, 3 * PI / 2)
        qc_in.s(2)
        qc_in.x(3)
        qc_in.cx(1, 2)
        qc_in.h(0)
        qc_in.s(3)
        qc_in.x(3)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_8(self):
        """test 'reduce_gates_8'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_in.cx(0, 1)
        qc_in.cx(1, 0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_9(self):
        """test 'reduce_gates_9'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.h(0)
        qc_in.h(1)
        qc_in.t(0)
        qc_in.t(1)
        qc_in.x(0)
        qc_in.cx(0, 1)
        qc_in.s(0)
        qc_in.cx(1, 0)
        qc_in.t(1)
        qc_in.cx(1, 0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_10(self):
        """test 'reduce_gates_10'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.h(0)
        qc_in.x(1)
        qc_in.h(0)
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_in.x(1)
        qc_in.x(0)
        qc_in.s(1)
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_11(self):
        """test 'reduce_gates_11'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.s(0)
        qc_in.s(1)
        qc_in.x(0)
        qc_in.t(1)
        qc_in.h(0)
        qc_in.t(1)
        qc_in.x(0)
        qc_in.s(1)
        qc_in.s(0)
        qc_in.s(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_12(self):
        """test 'reduce_gates_12'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(0, 1)
        qc_in.cx(0, 1)
        qc_in.cx(1, 0)
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_in.h(1)
        qc_in.cx(0, 1)
        qc_in.s(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_13(self):
        """test 'reduce_gates_13'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_in.h(1)
        qc_in.cx(0, 1)
        qc_in.cx(1, 0)
        qc_in.cx(0, 1)
        qc_in.s(1)
        qc_in.cx(0, 1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_14(self):
        """test 'reduce_gates_14'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(0, 1)
        qc_in.t(1)
        qc_in.s(0)
        qc_in.x(1)
        qc_in.t(1)
        qc_in.cx(1, 0)
        qc_in.h(1)
        qc_in.s(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_15(self):
        """test 'reduce_gates_15'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.cx(0, 1)
        qc_in.cx(0, 1)
        qc_in.cx(0, 1)
        qc_in.h(0)
        qc_in.t(0)
        qc_in.cx(1, 0)
        qc_in.cx(1, 0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_16(self):
        """test 'reduce_gates_16'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.x(0)
        qc_in.h(1)
        qc_in.h(0)
        qc_in.s(1)
        qc_in.cx(1, 0)
        qc_in.h(0)
        qc_in.s(1)
        qc_in.x(0)
        qc_in.s(1)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.cx(1, 0)
        qc_in.s(0)
        qc_in.s(1)
        qc_in.s(0)
        qc_in.h(0)
        qc_in.cx(1, 0)
        qc_in.t(0)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_17(self):
        """test 'reduce_gates_17'"""
        PI = Phase("PI")
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.h(0)
        qc_in.s(1)
        qc_in.s(2)
        qc_in.h(1)
        qc_in.s(1)
        qc_in.h(1)
        qc_in.cx(2, 1)
        qc_in.z(0)
        qc_in.h(1)
        qc_in.cx(0, 2)
        qc_in.x(0)
        qc_in.t(1)
        qc_in.x(2)
        qc_in.h(2)
        qc_in.cx(0, 1)
        qc_in.h(0)
        qc_in.t(1)
        qc_in.h(2)
        qc_in.h(1)
        qc_in.t(2)
        qc_in.s(1)
        qc_in.s(2)
        qc_in.t(1)
        qc_in.h(2)
        qc_in.t(2)
        qc_in.h(2)
        qc_in.cx(0, 1)
        qc_in.cx(1, 0)
        qc_in.t(0)
        qc_in.s(1)
        qc_in.t(2)
        qc_in.s(0)
        qc_in.h(0)
        qc_in.t(0)
        qc_in.cx(1, 0)
        qc_in.cx(2, 1)
        qc_in.h(0)
        qc_in.h(1)
        qc_in.s(0)
        qc_in.cx(0, 2)
        qc_in.s(0)
        qc_in.s(1)
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)


class Optimizer_reduce_gates_load(unittest.TestCase):
    """test 'reduce_gates_load'"""

    def test_reduce_gates_load_0(self):
        """test 'reduce_gates_load_0'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/00.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_1(self):
        """test 'reduce_gates_load_1'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/01.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_2(self):
        """test 'reduce_gates_load_2'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/02.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_3(self):
        """test 'reduce_gates_load_3'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/03.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_4(self):
        """test 'reduce_gates_load_4'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/04.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_5(self):
        """test 'reduce_gates_load_5'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/05.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_6(self):
        """test 'reduce_gates_load_6'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/06.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_7(self):
        """test 'reduce_gates_load_7'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/07.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_8(self):
        """test 'reduce_gates_load_8'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/08.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_9(self):
        """test 'reduce_gates_load_9'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/09.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_10(self):
        """test 'reduce_gates_load_10'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/10.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_11(self):
        """test 'reduce_gates_load_11'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/11.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_12(self):
        """test 'reduce_gates_load_12'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/12.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_13(self):
        """test 'reduce_gates_load_13'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/13.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_14(self):
        """test 'reduce_gates_load_14'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/14.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_15(self):
        """test 'reduce_gates_load_15'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/15.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_16(self):
        """test 'reduce_gates_load_16'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/16.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_17(self):
        """test 'reduce_gates_load_17'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/17.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_18(self):
        """test 'reduce_gates_load_18'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/18.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_19(self):
        """test 'reduce_gates_load_19'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/19.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_20(self):
        """test 'reduce_gates_load_20'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/20.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_21(self):
        """test 'reduce_gates_load_21'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/21.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_22(self):
        """test 'reduce_gates_load_22'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/22.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_23(self):
        """test 'reduce_gates_load_23'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/23.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)

    def test_reduce_gates_load_24(self):
        """test 'reduce_gates_load_24'"""
        opt = Optimizer()
        qc_in = QCirc()
        qc_in.load("../data/24.sqc")
        qc_out = opt.reduce_gates(qc_in, "zx")
        self.assertEqual(qc_in.is_equal(qc_out), True)
        qc_out = opt.reduce_gates(qc_in, "pp")
        self.assertEqual(qc_in.is_equal(qc_out), True)


if __name__ == "__main__":
    unittest.main()
