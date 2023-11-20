#include <gtest/gtest.h>

#include "sharq.h"

TEST(Fraction, Constructor) {
  Sharq::Fraction frac;
  EXPECT_EQ(1, frac.numerator());
  EXPECT_EQ(1, frac.denominator());
}

TEST(Fraction, Getter) {
  Sharq::Fraction frac_A(2, 3);
  EXPECT_EQ(2, frac_A.numerator());
  EXPECT_EQ(3, frac_A.denominator());
  Sharq::Fraction frac_B(2, -3);
  EXPECT_EQ(-2, frac_B.numerator());
  EXPECT_EQ(3, frac_B.denominator());
  Sharq::Fraction frac_C(4, 12);
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  Sharq::Fraction frac_D(4, -12);
  EXPECT_EQ(-1, frac_D.numerator());
  EXPECT_EQ(3, frac_D.denominator());
}

TEST(Fraction, Setter) {
  Sharq::Fraction frac_A;
  frac_A.numerator(2);
  frac_A.denominator(3);
  EXPECT_EQ(2, frac_A.numerator());
  EXPECT_EQ(3, frac_A.denominator());
  Sharq::Fraction frac_B;
  frac_B.numerator(2);
  frac_B.denominator(-3);
  EXPECT_EQ(-2, frac_B.numerator());
  EXPECT_EQ(3, frac_B.denominator());
  Sharq::Fraction frac_C;
  frac_C.numerator(4);
  frac_C.denominator(12);
  EXPECT_EQ(1, frac_C.numerator());
  EXPECT_EQ(3, frac_C.denominator());
  Sharq::Fraction frac_D;
  frac_D.numerator(4);
  frac_D.denominator(-12);
  EXPECT_EQ(-1, frac_D.numerator());
  EXPECT_EQ(3, frac_D.denominator());
}

TEST(Fraction, Add) {
  Sharq::Fraction frac_A(1, 3);
  Sharq::Fraction frac_B(1, 4);
  Sharq::Fraction frac_C = frac_A + frac_B;
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

  frac_A = Sharq::Fraction(-1, 3);
  frac_B = Sharq::Fraction(-1, 4);
  frac_C = frac_A + frac_B;
  EXPECT_EQ(-7, frac_C.numerator());
  EXPECT_EQ(12, frac_C.denominator());
}

TEST(Fraction, Sub) {
  Sharq::Fraction frac_A(1, 3);
  Sharq::Fraction frac_B(1, 4);
  Sharq::Fraction frac_C = frac_A - frac_B;
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
  Sharq::Fraction frac_A(1, 3);
  Sharq::Fraction frac_B(1, 4);
  Sharq::Fraction frac_C = frac_A * frac_B;
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
  Sharq::Fraction frac_A(1, 3);
  Sharq::Fraction frac_B(1, 4);
  Sharq::Fraction frac_C = frac_A / frac_B;
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
  Sharq::Fraction frac_A(0);
  EXPECT_EQ(true, frac_A.is_zero());
  Sharq::Fraction frac_B(1, 2);
  EXPECT_EQ(false, frac_B.is_zero());
  Sharq::Fraction frac_C(1, -2);
  EXPECT_EQ(false, frac_C.is_zero());
  Sharq::Fraction frac_D(-1, -2);
  EXPECT_EQ(false, frac_D.is_zero());
}

TEST(Fraction, IsPositive) {
  Sharq::Fraction frac_A(0);
  EXPECT_EQ(false, frac_A.is_positive());
  Sharq::Fraction frac_B(1, 2);
  EXPECT_EQ(true, frac_B.is_positive());
  Sharq::Fraction frac_C(1, -2);
  EXPECT_EQ(false, frac_C.is_positive());
  Sharq::Fraction frac_D(-1, -2);
  EXPECT_EQ(true, frac_D.is_positive());
}

TEST(Fraction, IsNegative) {
  Sharq::Fraction frac_A(0);
  EXPECT_EQ(false, frac_A.is_negative());
  Sharq::Fraction frac_B(1, 2);
  EXPECT_EQ(false, frac_B.is_negative());
  Sharq::Fraction frac_C(1, -2);
  EXPECT_EQ(true, frac_C.is_negative());
  Sharq::Fraction frac_D(-1, -2);
  EXPECT_EQ(false, frac_D.is_negative());
}
