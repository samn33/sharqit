#include <gtest/gtest.h>

#include "sharq.h"

TEST(Phase, Constructor) {
  Sharq::Phase phase_A;
  EXPECT_EQ(0, phase_A.frac().numerator());
  EXPECT_EQ(1, phase_A.frac().denominator());
  Sharq::Phase phase_B(Sharq::Fraction(1, -3));
  EXPECT_EQ(-1, phase_B.frac().numerator());
  EXPECT_EQ(3, phase_B.frac().denominator());
  Sharq::Phase phase_C("2*PI/3");
  EXPECT_EQ(2, phase_C.frac().numerator());
  EXPECT_EQ(3, phase_C.frac().denominator());
  Sharq::Phase phase_D("-2*PI/3");
  EXPECT_EQ(-2, phase_D.frac().numerator());
  EXPECT_EQ(3, phase_D.frac().denominator());
}

TEST(Phase, ToString1) {
  Sharq::Phase phase_A;
  EXPECT_EQ("0", phase_A.to_string());
  Sharq::Phase phase_B(Sharq::Fraction(1, -3));
  EXPECT_EQ("5π/3", phase_B.to_string());
  Sharq::Phase phase_C("2*PI/3");
  EXPECT_EQ("2π/3", phase_C.to_string());
  Sharq::Phase phase_D("-2*PI/3");
  EXPECT_EQ("4π/3",phase_D.to_string());
}

TEST(Phase, ToString2) {
  Sharq::Phase phase_A;
  EXPECT_EQ("0", phase_A.to_string(false));
  Sharq::Phase phase_B(Sharq::Fraction(1, -3));
  EXPECT_EQ("5/3", phase_B.to_string(false));
  Sharq::Phase phase_C("2*PI/3");
  EXPECT_EQ("2/3", phase_C.to_string(false));
  Sharq::Phase phase_D("-2*PI/3");
  EXPECT_EQ("4/3",phase_D.to_string(false));
}

TEST(Phase, IsZero) {
  Sharq::Phase phase_A(0);
  EXPECT_EQ(true, phase_A.is_zero());
  Sharq::Phase phase_B(1, 2);
  EXPECT_EQ(false, phase_B.is_zero());
  Sharq::Phase phase_C(1, -2);
  EXPECT_EQ(false, phase_C.is_zero());
  Sharq::Phase phase_D(-1, -2);
  EXPECT_EQ(false, phase_D.is_zero());
}

TEST(Phase, IsPositive) {
  Sharq::Phase phase_A(0);
  EXPECT_EQ(false, phase_A.is_positive());
  Sharq::Phase phase_B(1, 2);
  EXPECT_EQ(true, phase_B.is_positive());
  Sharq::Phase phase_C(1, -2);
  EXPECT_EQ(false, phase_C.is_positive());
  Sharq::Phase phase_D(-1, -2);
  EXPECT_EQ(true, phase_D.is_positive());
}

TEST(Phase, IsNegative) {
  Sharq::Phase phase_A(0);
  EXPECT_EQ(false, phase_A.is_negative());
  Sharq::Phase phase_B(1, 2);
  EXPECT_EQ(false, phase_B.is_negative());
  Sharq::Phase phase_C(1, -2);
  EXPECT_EQ(true, phase_C.is_negative());
  Sharq::Phase phase_D(-1, -2);
  EXPECT_EQ(false, phase_D.is_negative());
}

TEST(Phase, Reduce) {
  Sharq::Phase phase_A(6, 20);
  EXPECT_EQ(3, phase_A.frac().numerator());
  EXPECT_EQ(10, phase_A.frac().denominator());
  Sharq::Phase phase_B(-6, 20);
  EXPECT_EQ(-3, phase_B.frac().numerator());
  EXPECT_EQ(10, phase_B.frac().denominator());
  Sharq::Phase phase_C(-6, -20);
  EXPECT_EQ(3, phase_C.frac().numerator());
  EXPECT_EQ(10, phase_C.frac().denominator());
  Sharq::Phase phase_D(50, 6);
  EXPECT_EQ(25, phase_D.frac().numerator());
  EXPECT_EQ(3, phase_D.frac().denominator());
  Sharq::Phase phase_E(50, -6);
  EXPECT_EQ(-25, phase_E.frac().numerator());
  EXPECT_EQ(3, phase_E.frac().denominator());
}

TEST(Phase, Add) {
  Sharq::Phase phase_A(1, 3);
  Sharq::Phase phase_B(1, 4);
  Sharq::Phase phase_C = phase_A + phase_B;
  EXPECT_EQ(7, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C += phase_B;
  EXPECT_EQ(5, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
  phase_A = Sharq::Phase(-1, 3);
  phase_B = Sharq::Phase(-1, 4);
  phase_C = phase_A + phase_B;
  EXPECT_EQ(-7, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C += phase_B;
  EXPECT_EQ(-5, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
}

TEST(Phase, Sub) {
  Sharq::Phase phase_A(1, 3);
  Sharq::Phase phase_B(1, 4);
  Sharq::Phase phase_C = phase_A - phase_B;
  EXPECT_EQ(1, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C -= phase_B;
  EXPECT_EQ(-1, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
  phase_A = Sharq::Phase(-1, 3);
  phase_B = Sharq::Phase(-1, 4);
  phase_C = phase_A - phase_B;
  EXPECT_EQ(-1, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C -= phase_B;
  EXPECT_EQ(1, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
}

TEST(Phase, Mul) {
  Sharq::Phase phase_A(1, 6);
  Sharq::Phase phase_B = phase_A * (-15);
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
  phase_B = (-15) * phase_A;
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
  phase_B *= -15;
  EXPECT_EQ(75, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
}

TEST(Phase, Div) {
  Sharq::Phase phase_A(15, -6);
  Sharq::Phase phase_B = phase_A / (-3);
  EXPECT_EQ(5, phase_B.frac().numerator());
  EXPECT_EQ(6, phase_B.frac().denominator());
  phase_B /= (-3);
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(18, phase_B.frac().denominator());
}

TEST(Phase, Sign) {
  Sharq::Phase phase_A(15, -6);
  Sharq::Phase phase_B = +phase_A;
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
  phase_B = -phase_A;
  EXPECT_EQ(-5, phase_A.frac().numerator());
  EXPECT_EQ(2, phase_A.frac().denominator());
  EXPECT_EQ(5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
}

TEST(Phase, Equal) {
  Sharq::Phase phase_A(15, -6);
  Sharq::Phase phase_B(-5, 2);
  EXPECT_EQ(true, phase_A == phase_B);
  phase_A = Sharq::Phase(15, -6);
  phase_B = Sharq::Phase(3, 2);
  EXPECT_EQ(true, phase_A == phase_B);
  phase_A = Sharq::Phase(15, -6);
  phase_B = Sharq::Phase(3, -2);
  EXPECT_EQ(false, phase_A == phase_B);
}

TEST(Phase, NotEqual) {
  Sharq::Phase phase_A(15, -6);
  Sharq::Phase phase_B(-5, 2);
  EXPECT_EQ(false, phase_A != phase_B);
  phase_A = Sharq::Phase(15, -6);
  phase_B = Sharq::Phase(3, 2);
  EXPECT_EQ(false, phase_A != phase_B);
  phase_A = Sharq::Phase(15, -6);
  phase_B = Sharq::Phase(3, -2);
  EXPECT_EQ(true, phase_A != phase_B);
}

