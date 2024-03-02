/**
 * @file fraction.h
 * @brief include file of Fraction class
 */

#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <string>

namespace Sharqit {

  /**
   * @class Fraction
   * @brief fraction to describe a phase
  */
  class Fraction
  {
  private:
    int32_t numerator_; //!< numerator of the fraction
    int32_t denominator_; //!< denominator of the fraction
  public:
    /**
     * @brief constructor of the Fraction
     */
    Fraction() : numerator_(1), denominator_(1) {}
    /**
     * @brief constructor of the Fraction
     * @param [in] numerator numerator of the fraction
     * @param [in] denominator denominator of the fraction
     */
    Fraction(const int32_t numerator, const int32_t denominator = 1) :
      numerator_(numerator), denominator_(denominator) { reduce(); }
    /**
     * @brief copy constructor of the Fraction
     * @param [in] frac the fraction
     */
    Fraction(const Fraction& frac) : numerator_(frac.numerator_), denominator_(frac.denominator_) {}
    //! getter of the numerator_
    int32_t numerator() const { return numerator_; }
    //! getter of the denominator_
    int32_t denominator() const { return denominator_; }
    //! setter of the numerator_
    void numerator(const int32_t numerator) { numerator_ = numerator; reduce(); }
    //! setter of the denominator_
    void denominator(const int32_t denominator) { denominator_ = denominator; reduce(); }
    /**
     * @brief get a string of the Fraction object
     * @return string of the fraction
     */
    std::string to_string() const;
    /**
     * @brief add the fraction
     * @param [in] other fraction to be added
     * @return result of the operation
     */
    Fraction add(const Fraction& other) const
    {
      int32_t n = numerator_ * other.denominator() + other.numerator() * denominator_;
      int32_t d = denominator_ * other.denominator();
      return Fraction(n / Sharqit::Fraction::gcd(n, d), d / Sharqit::Fraction::gcd(n, d));
    }
    /**
     * @brief add the fraction
     * @param [in] other integer to be added
     * @return result of the operation
     */
    Fraction add(const int32_t other) const { return add(Fraction(other)); }
    /**
     * @brief subtract the fraction
     * @param [in] other fraction to be subtracted
     * @return result of the operation
     */
    Fraction sub(const Fraction& other) const
    {
      int n = numerator_ * other.denominator() - other.numerator() * denominator_;
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharqit::Fraction::gcd(n, d), d / Sharqit::Fraction::gcd(n, d));
    }
    /**
     * @brief subtract the fraction
     * @param [in] other integer to be subtracted
     * @return result of the operation
     */
    Fraction sub(const int32_t other) const { return sub(Fraction(other)); }
    /**
     * @brief multiply the fraction
     * @param [in] other fraction to be multiplied
     * @return result of the operation
     */
    Fraction mul(const Fraction& other) const
    {
      int n = numerator_ * other.numerator();
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharqit::Fraction::gcd(n, d), d / Sharqit::Fraction::gcd(n, d));
    }
    /**
     * @brief multiply the fraction
     * @param [in] other integer to be multiplied
     * @return result of the operation
     */
    Fraction mul(const int32_t other) const { return mul(Fraction(other)); }
    /**
     * @brief divide the fraction
     * @param [in] other fraction to be divided
     * @return result of the operation
     */
    Fraction div(const Fraction& other) const
    {
      int n = numerator_ * other.denominator();
      int d = denominator_ * other.numerator();
      return Fraction(n / Sharqit::Fraction::gcd(n, d), d / Sharqit::Fraction::gcd(n, d));
    }
    /**
     * @brief divide the fraction
     * @param [in] other the integer to be divided
     * @return result of the operation
     */
    Fraction div(const int32_t other) const { return div(Fraction(other)); }
    /**
     * @brief is the fraction zero or not
     * @return true if the fraction is zero, false otherwise
     */
    bool is_zero() const { return numerator_ == 0; }
    /**
     * @brief is the fraction positive or not
     * @return true if the fraction is positive, false otherwise
     */
    bool is_positive() const { return numerator_ * denominator_ > 0; }
    /**
     * @brief is the fraction negative or not
     * @return true if the fraction is negative, false otherwise
     */
    bool is_negative() const { return numerator_ * denominator_ < 0; }
    /**
     * @brief reduce the fraction
     */
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
    /**
     * @brief greaetst common divisor of two intergers
     * @param [in] n integer
     * @param [in] d integer
     * @return greaetst common divisor
     */
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
