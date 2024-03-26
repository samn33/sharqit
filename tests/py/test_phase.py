# -*- coding: utf-8 -*-
import unittest
from sharqit import Phase


class Phase_add(unittest.TestCase):
    """test 'Phase_add'"""

    def test_add_1(self):
        """test 'add_1'"""
        p = Phase(1, 2)
        q = Phase(1, 4)
        s = p + q
        self.assertEqual(s.to_string(), "3/4")

    def test_add_2(self):
        """test 'add_2'"""
        p = Phase(1, 2)
        q = Phase(1, -4)
        s = p + q
        self.assertEqual(s.to_string(), "1/4")

    def test_add_3(self):
        """test 'add_3'"""
        p = Phase(4, 8)
        q = Phase(3, -12)
        s = p + q
        self.assertEqual(s.to_string(), "1/4")

    def test_add_4(self):
        """test 'add_4'"""
        p = Phase(1, 2)
        q = Phase(1, 4)
        p += q
        self.assertEqual(p.to_string(), "3/4")

    def test_add_5(self):
        """test 'add_5'"""
        p = Phase("PI/2")
        q = Phase("PI/4")
        s = p + q
        self.assertEqual(s.to_string(), "3/4")

    def test_add_6(self):
        """test 'add_6'"""
        PI = Phase("PI")
        p = PI / 2
        q = PI / 4
        s = p + q
        self.assertEqual(s.to_string(), "3/4")

    def test_add_7(self):
        """test 'add_7'"""
        PI = Phase("PI")
        p = PI / 2
        q = PI / 4
        s = p + q
        self.assertEqual(s.to_string(), "3/4")


class Phase_sub(unittest.TestCase):
    """test 'Phase_sub'"""

    def test_sub_1(self):
        """test 'sub_1'"""
        p = Phase(1, 2)
        q = Phase(1, 4)
        s = p - q
        self.assertEqual(s.to_string(), "1/4")

    def test_sub_2(self):
        """test 'sub_2'"""
        p = Phase(1, 2)
        q = Phase(1, -4)
        s = p - q
        self.assertEqual(s.to_string(), "3/4")

    def test_sub_3(self):
        """test 'sub_3'"""
        p = Phase(4, 8)
        q = Phase(3, -12)
        s = p - q
        self.assertEqual(s.to_string(), "3/4")

    def test_sub_4(self):
        """test 'sub_4'"""
        p = Phase(1, 2)
        q = Phase(1, 4)
        p -= q
        self.assertEqual(p.to_string(), "1/4")


class Phase_mul(unittest.TestCase):
    """test 'Phase_mul'"""

    def test_mul_1(self):
        """test 'mul_1'"""
        p = Phase(1, 2)
        s = 3 * p
        self.assertEqual(s.to_string(), "3/2")

    def test_mul_2(self):
        """test 'mul_2'"""
        p = Phase(1, 2)
        s = p * 3
        self.assertEqual(s.to_string(), "3/2")

    def test_mul_3(self):
        """test 'mul_3'"""
        p = Phase(1, 2)
        p *= 3
        self.assertEqual(p.to_string(), "3/2")


class Phase_div(unittest.TestCase):
    """test 'Phase_div'"""

    def test_div_1(self):
        """test 'mul_1'"""
        p = Phase(1, 2)
        s = p / 2
        self.assertEqual(s.to_string(), "1/4")

    def test_div_2(self):
        """test 'div_2'"""
        p = Phase(1, 2)
        p /= 3
        self.assertEqual(p.to_string(), "1/6")


if __name__ == "__main__":
    unittest.main()
