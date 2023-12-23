#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <string>

namespace Sharq {

  class Fraction
  {
  private:
    int32_t numerator_;
    int32_t denominator_;
  public:
    Fraction() : numerator_(1), denominator_(1) {}
    Fraction(const int32_t numerator, const int32_t denominator = 1) :
      numerator_(numerator), denominator_(denominator) { reduce(); }
    Fraction(const Fraction& frac) : numerator_(frac.numerator_), denominator_(frac.denominator_) {}
    /* getters */
    int32_t numerator() const { return numerator_; }
    int32_t denominator() const { return denominator_; }
    /* setters */
    void numerator(const int32_t numerator) { numerator_ = numerator; reduce(); }
    void denominator(const int32_t denominator) { denominator_ = denominator; reduce(); }
    /* member functions */
    std::string to_string() const;
    Fraction add(const Fraction& other) const
    {
      int32_t n = numerator_ * other.denominator() + other.numerator() * denominator_;
      int32_t d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction add(const int32_t other) const { return add(Fraction(other)); }
    Fraction sub(const Fraction& other) const
    {
      int n = numerator_ * other.denominator() - other.numerator() * denominator_;
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction sub(const int32_t other) const { return sub(Fraction(other)); }
    Fraction mul(const Fraction& other) const
    {
      int n = numerator_ * other.numerator();
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction mul(const int32_t other) const { return mul(Fraction(other)); }
    Fraction div(const Fraction& other) const
    {
      int n = numerator_ * other.denominator();
      int d = denominator_ * other.numerator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction div(const int32_t other) const { return div(Fraction(other)); }
    bool is_zero() const { return numerator_ == 0; }
    bool is_positive() const { return numerator_ * denominator_ > 0; }
    bool is_negative() const { return numerator_ * denominator_ < 0; }
    void reduce()
    {
      /* make denominator positive if denominator < 0 */
      if (denominator_ == 0) { throw std::runtime_error("You can't devide by zero."); }
      else if (denominator_ < 0) {
	numerator_ = -numerator_;
	denominator_ = -denominator_;
      }
      /* reduce */
      int32_t fac = std::abs(gcd(numerator_, denominator_));
      if (fac != 0 && fac != 1) {
	numerator_ /= fac;
	denominator_ /= fac;
      }
    }
    static int32_t gcd(const int32_t n, const int32_t d)
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
    Fraction operator+() const { return *this; }
    Fraction operator-() const { return mul(-1); }
    Fraction& operator+=(const Fraction& rhs)
    {
      Fraction frac = add(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator+=(const int32_t rhs) { return operator+=(Fraction(rhs)); }
    Fraction& operator-=(const Fraction& rhs)
    {
      Fraction frac = sub(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator-=(const int32_t rhs) { return operator-=(Fraction(rhs)); }
    Fraction& operator*=(const Fraction& rhs)
    {
      Fraction frac = mul(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator*=(const int32_t rhs) { return operator*=(Fraction(rhs)); }
    Fraction& operator/=(const Fraction& rhs)
    {
      Fraction frac = div(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator/=(const int32_t rhs) { return operator/=(Fraction(rhs)); }
    friend bool operator==(const Fraction& lhs, const Fraction& rhs)
    {
      return lhs.numerator() == rhs.numerator() && lhs.denominator() == rhs.denominator();
    }
    friend bool operator!=(const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs) { return lhs.add(rhs); }
    friend Fraction operator+(const Fraction& lhs, const int32_t rhs) { return lhs.add(Fraction(rhs)); }
    friend Fraction operator+(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).add(rhs); }
    friend Fraction operator-(const Fraction& lhs, const Fraction& rhs) { return lhs.sub(rhs); }
    friend Fraction operator-(const Fraction& lhs, const int32_t rhs) { return lhs.sub(Fraction(rhs)); }
    friend Fraction operator-(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).sub(rhs); }
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs) { return lhs.mul(rhs); }
    friend Fraction operator*(const Fraction& lhs, const int32_t rhs) { return lhs.mul(Fraction(rhs)); }
    friend Fraction operator*(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).mul(rhs); }
    friend Fraction operator/(const Fraction& lhs, const Fraction& rhs) { return lhs.div(rhs); }
    friend Fraction operator/(const Fraction& lhs, const int32_t rhs) { return lhs.div(Fraction(rhs)); }
    friend Fraction operator/(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).div(rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const Fraction& frac) { ost << frac.to_string(); return ost; }
  };
}

#endif
