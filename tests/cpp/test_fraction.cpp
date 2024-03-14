#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(Fraction, Constructor) {
  Sharqit::Fraction frac;
  EXPECT_EQ(1, frac.numerator());
  EXPECT_EQ(1, frac.denominator());
}

TEST(Fraction, Getter) {
  Sharqit::Fraction frac_A(2, 3);
  EXPECT_EQ(2, frac_A.numerator());
  EXPECT_EQ(3, frac_A.denominator());
  Sharqit::Fraction frac_B(2, -3);
  EXPECT_EQ(-2, frac_B.numerator());
  EXPECT_EQ(3, frac_B.denominator());
  Sharqit::Fraction frac_C(4, 12);
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  Sharqit::Fraction frac_D(4, -12);
  EXPECT_EQ(-1, frac_D.numerator());
  EXPECT_EQ(3, frac_D.denominator());
}

TEST(Fraction, Setter) {
  Sharqit::Fraction frac_A;
  frac_A.numerator(2);
  frac_A.denominator(3);
  EXPECT_EQ(2, frac_A.numerator());
  EXPECT_EQ(3, frac_A.denominator());
  Sharqit::Fraction frac_B;
  frac_B.numerator(2);
  frac_B.denominator(-3);
  EXPECT_EQ(-2, frac_B.numerator());
  EXPECT_EQ(3, frac_B.denominator());
  Sharqit::Fraction frac_C;
  frac_C.numerator(4);
  frac_C.denominator(12);
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  Sharqit::Fraction frac_D;
  frac_D.numerator(4);
  frac_D.denominator(-12);
  EXPECT_EQ(-1, frac_D.numerator());
  EXPECT_EQ(3, frac_D.denominator());
}

TEST(Fraction, Add) {
  Sharqit::Fraction frac_A(1, 3);
  Sharqit::Fraction frac_B(1, 4);
  Sharqit::Fraction frac_C = frac_A + frac_B;
  EXPECT_EQ(7, frac_C.numerator());
  EXPECT_EQ(12, frac_C.denominator());
  frac_C += frac_B;
  EXPECT_EQ(5, frac_C.numerator());
  EXPECT_EQ(6, frac_C.denominator());
  frac_C = frac_A + 1;
  EXPECT_EQ(4, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  frac_C = 1 + frac_A;
  EXPECT_EQ(4, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());

  frac_A = Sharqit::Fraction(-1, 3);
  frac_B = Sharqit::Fraction(-1, 4);
  frac_C = frac_A + frac_B;
  EXPECT_EQ(-7, frac_C.numerator());
  EXPECT_EQ(12, frac_C.denominator());
}

TEST(Fraction, Sub) {
  Sharqit::Fraction frac_A(1, 3);
  Sharqit::Fraction frac_B(1, 4);
  Sharqit::Fraction frac_C = frac_A - frac_B;
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(12, frac_C.denominator());
  frac_C -= frac_B;
  EXPECT_EQ(-1, frac_C.numerator());
  EXPECT_EQ(6, frac_C.denominator());
  frac_C = frac_A - 1;
  EXPECT_EQ(-2, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  frac_C = 1 - frac_A;
  EXPECT_EQ(2, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
}

TEST(Fraction, Mul) {
  Sharqit::Fraction frac_A(1, 3);
  Sharqit::Fraction frac_B(1, 4);
  Sharqit::Fraction frac_C = frac_A * frac_B;
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(12, frac_C.denominator());
  frac_C *= frac_B;
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(48, frac_C.denominator());
  frac_C = frac_A * 5;
  EXPECT_EQ(5, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  frac_C = 5 * frac_A;
  EXPECT_EQ(5, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
}

TEST(Fraction, Div) {
  Sharqit::Fraction frac_A(1, 3);
  Sharqit::Fraction frac_B(1, 4);
  Sharqit::Fraction frac_C = frac_A / frac_B;
  EXPECT_EQ(4, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  frac_C /= frac_B;
  EXPECT_EQ(16, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  frac_C = frac_A / 5;
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(15, frac_C.denominator());
  frac_C = 5 / frac_A;
  EXPECT_EQ(15, frac_C.numerator());
  EXPECT_EQ(1, frac_C.denominator());
}

TEST(Fraction, IsZero) {
  Sharqit::Fraction frac_A(0);
  EXPECT_EQ(true, frac_A.is_zero());
  Sharqit::Fraction frac_B(1, 2);
  EXPECT_EQ(false, frac_B.is_zero());
  Sharqit::Fraction frac_C(1, -2);
  EXPECT_EQ(false, frac_C.is_zero());
  Sharqit::Fraction frac_D(-1, -2);
  EXPECT_EQ(false, frac_D.is_zero());
}

TEST(Fraction, IsPositive) {
  Sharqit::Fraction frac_A(0);
  EXPECT_EQ(false, frac_A.is_positive());
  Sharqit::Fraction frac_B(1, 2);
  EXPECT_EQ(true, frac_B.is_positive());
  Sharqit::Fraction frac_C(1, -2);
  EXPECT_EQ(false, frac_C.is_positive());
  Sharqit::Fraction frac_D(-1, -2);
  EXPECT_EQ(true, frac_D.is_positive());
}

TEST(Fraction, IsNegative) {
  Sharqit::Fraction frac_A(0);
  EXPECT_EQ(false, frac_A.is_negative());
  Sharqit::Fraction frac_B(1, 2);
  EXPECT_EQ(false, frac_B.is_negative());
  Sharqit::Fraction frac_C(1, -2);
  EXPECT_EQ(true, frac_C.is_negative());
  Sharqit::Fraction frac_D(-1, -2);
  EXPECT_EQ(false, frac_D.is_negative());
}
