#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(Phase, Constructor) {
  Sharqit::Phase phase_A;
  EXPECT_EQ(0, phase_A.frac().numerator());
  EXPECT_EQ(1, phase_A.frac().denominator());
  Sharqit::Phase phase_B(Sharqit::Fraction(1, -3));
  EXPECT_EQ(-1, phase_B.frac().numerator());
  EXPECT_EQ(3, phase_B.frac().denominator());
  Sharqit::Phase phase_C("2*PI/3");
  EXPECT_EQ(2, phase_C.frac().numerator());
  EXPECT_EQ(3, phase_C.frac().denominator());
  Sharqit::Phase phase_D("-2*PI/3");
  EXPECT_EQ(-2, phase_D.frac().numerator());
  EXPECT_EQ(3, phase_D.frac().denominator());
}

TEST(Phase, ToString1) {
  Sharqit::Phase phase_A;
  EXPECT_EQ("0", phase_A.to_string());
  Sharqit::Phase phase_B(Sharqit::Fraction(1, -3));
  EXPECT_EQ("5π/3", phase_B.to_string());
  Sharqit::Phase phase_C("2*PI/3");
  EXPECT_EQ("2π/3", phase_C.to_string());
  Sharqit::Phase phase_D("-2*PI/3");
  EXPECT_EQ("4π/3",phase_D.to_string());
}

TEST(Phase, ToString2) {
  Sharqit::Phase phase_A;
  EXPECT_EQ("0", phase_A.to_string(false));
  Sharqit::Phase phase_B(Sharqit::Fraction(1, -3));
  EXPECT_EQ("5/3", phase_B.to_string(false));
  Sharqit::Phase phase_C("2*PI/3");
  EXPECT_EQ("2/3", phase_C.to_string(false));
  Sharqit::Phase phase_D("-2*PI/3");
  EXPECT_EQ("4/3",phase_D.to_string(false));
}

TEST(Phase, IsZero) {
  Sharqit::Phase phase_A(0);
  EXPECT_EQ(true, phase_A.is_zero());
  Sharqit::Phase phase_B(1, 2);
  EXPECT_EQ(false, phase_B.is_zero());
  Sharqit::Phase phase_C(1, -2);
  EXPECT_EQ(false, phase_C.is_zero());
  Sharqit::Phase phase_D(-1, -2);
  EXPECT_EQ(false, phase_D.is_zero());
}

TEST(Phase, IsPositive) {
  Sharqit::Phase phase_A(0);
  EXPECT_EQ(false, phase_A.is_positive());
  Sharqit::Phase phase_B(1, 2);
  EXPECT_EQ(true, phase_B.is_positive());
  Sharqit::Phase phase_C(1, -2);
  EXPECT_EQ(false, phase_C.is_positive());
  Sharqit::Phase phase_D(-1, -2);
  EXPECT_EQ(true, phase_D.is_positive());
}

TEST(Phase, IsNegative) {
  Sharqit::Phase phase_A(0);
  EXPECT_EQ(false, phase_A.is_negative());
  Sharqit::Phase phase_B(1, 2);
  EXPECT_EQ(false, phase_B.is_negative());
  Sharqit::Phase phase_C(1, -2);
  EXPECT_EQ(true, phase_C.is_negative());
  Sharqit::Phase phase_D(-1, -2);
  EXPECT_EQ(false, phase_D.is_negative());
}

TEST(Phase, Reduce) {
  Sharqit::Phase phase_A(6, 20);
  EXPECT_EQ(3, phase_A.frac().numerator());
  EXPECT_EQ(10, phase_A.frac().denominator());
  Sharqit::Phase phase_B(-6, 20);
  EXPECT_EQ(-3, phase_B.frac().numerator());
  EXPECT_EQ(10, phase_B.frac().denominator());
  Sharqit::Phase phase_C(-6, -20);
  EXPECT_EQ(3, phase_C.frac().numerator());
  EXPECT_EQ(10, phase_C.frac().denominator());
  Sharqit::Phase phase_D(50, 6);
  EXPECT_EQ(25, phase_D.frac().numerator());
  EXPECT_EQ(3, phase_D.frac().denominator());
  Sharqit::Phase phase_E(50, -6);
  EXPECT_EQ(-25, phase_E.frac().numerator());
  EXPECT_EQ(3, phase_E.frac().denominator());
}

TEST(Phase, Add) {
  Sharqit::Phase phase_A(1, 3);
  Sharqit::Phase phase_B(1, 4);
  Sharqit::Phase phase_C = phase_A + phase_B;
  EXPECT_EQ(7, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C += phase_B;
  EXPECT_EQ(5, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
  phase_A = Sharqit::Phase(-1, 3);
  phase_B = Sharqit::Phase(-1, 4);
  phase_C = phase_A + phase_B;
  EXPECT_EQ(-7, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C += phase_B;
  EXPECT_EQ(-5, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
}

TEST(Phase, Sub) {
  Sharqit::Phase phase_A(1, 3);
  Sharqit::Phase phase_B(1, 4);
  Sharqit::Phase phase_C = phase_A - phase_B;
  EXPECT_EQ(1, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C -= phase_B;
  EXPECT_EQ(-1, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
  phase_A = Sharqit::Phase(-1, 3);
  phase_B = Sharqit::Phase(-1, 4);
  phase_C = phase_A - phase_B;
  EXPECT_EQ(-1, phase_C.frac().numerator());
  EXPECT_EQ(12, phase_C.frac().denominator());
  phase_C -= phase_B;
  EXPECT_EQ(1, phase_C.frac().numerator());
  EXPECT_EQ(6, phase_C.frac().denominator());
}

TEST(Phase, Mul) {
  Sharqit::Phase phase_A(1, 6);
  Sharqit::Phase phase_B = phase_A * (-15);
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
  Sharqit::Phase phase_A(15, -6);
  Sharqit::Phase phase_B = phase_A / (-3);
  EXPECT_EQ(5, phase_B.frac().numerator());
  EXPECT_EQ(6, phase_B.frac().denominator());
  phase_B /= (-3);
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(18, phase_B.frac().denominator());
}

TEST(Phase, Sign) {
  Sharqit::Phase phase_A(15, -6);
  Sharqit::Phase phase_B = +phase_A;
  EXPECT_EQ(-5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
  phase_B = -phase_A;
  EXPECT_EQ(-5, phase_A.frac().numerator());
  EXPECT_EQ(2, phase_A.frac().denominator());
  EXPECT_EQ(5, phase_B.frac().numerator());
  EXPECT_EQ(2, phase_B.frac().denominator());
}

TEST(Phase, Equal) {
  Sharqit::Phase phase_A(15, -6);
  Sharqit::Phase phase_B(-5, 2);
  EXPECT_EQ(true, phase_A == phase_B);
  phase_A = Sharqit::Phase(15, -6);
  phase_B = Sharqit::Phase(3, 2);
  EXPECT_EQ(true, phase_A == phase_B);
  phase_A = Sharqit::Phase(15, -6);
  phase_B = Sharqit::Phase(3, -2);
  EXPECT_EQ(false, phase_A == phase_B);
}

TEST(Phase, NotEqual) {
  Sharqit::Phase phase_A(15, -6);
  Sharqit::Phase phase_B(-5, 2);
  EXPECT_EQ(false, phase_A != phase_B);
  phase_A = Sharqit::Phase(15, -6);
  phase_B = Sharqit::Phase(3, 2);
  EXPECT_EQ(false, phase_A != phase_B);
  phase_A = Sharqit::Phase(15, -6);
  phase_B = Sharqit::Phase(3, -2);
  EXPECT_EQ(true, phase_A != phase_B);
}

