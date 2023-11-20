#include "sharq.h"

/**
 *  member functions
 */

std::string Sharq::Fraction::to_string() const
{
  std::string str;
  if (numerator_ == 0) str = "0";
  else if (numerator_ == 1 && denominator_ == 1) str = "1";
  else str = std::to_string(numerator_) + "/" + std::to_string(denominator_);
  return str;
}

Sharq::Fraction Sharq::Fraction::add(const Fraction& other) const
{
  int32_t n = numerator_ * other.denominator() + other.numerator() * denominator_;
  int32_t d = denominator_ * other.denominator();
  return Fraction(n / gcd(n, d), d / gcd(n, d));
}

Sharq::Fraction Sharq::Fraction::sub(const Fraction& other) const
{
  int n = numerator_ * other.denominator() - other.numerator() * denominator_;
  int d = denominator_ * other.denominator();
  return Fraction(n / gcd(n, d), d / gcd(n, d));
}

Sharq::Fraction Sharq::Fraction::mul(const Fraction& other) const
{
  int n = numerator_ * other.numerator();
  int d = denominator_ * other.denominator();
  return Fraction(n / gcd(n, d), d / gcd(n, d));
}

Sharq::Fraction Sharq::Fraction::div(const Fraction& other) const
{
  int n = numerator_ * other.denominator();
  int d = denominator_ * other.numerator();
  return Fraction(n / gcd(n, d), d / gcd(n, d));
}

int32_t Sharq::Fraction::gcd(const int32_t n, const int32_t d)
{
  int32_t remainder;
  int32_t nn = std::abs(n);
  int32_t dd = std::abs(d);
  while (dd != 0) {
    remainder = nn % dd;
    nn = dd;
    dd = remainder;
  }
  return nn;
}
