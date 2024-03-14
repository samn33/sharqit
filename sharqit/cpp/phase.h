/**
 * @file phase.h
 * @brief include file of Phase class
 */

#ifndef PHASE_H
#define PHASE_H

#include <iostream>
#include <string>
#include <cmath>
#include <regex>

#include "util.h"
#include "fraction.h"

namespace Sharqit {

  /**
   * @class Phase
   * @brief phase of rotation gate
  */
  class Phase
  {
  private:
    Fraction frac_; //!< fraction for describing phase of rotation gate
  public:
    /**
     * @brief constructor of the Phase
     * @param [in] numerator numerator of the fraction of the phase except to pi
     * @param [in] denominator denominator of the fraction of the phase except to pi
     */
    Phase(const int32_t numerator = 0, const int32_t denominator = 1) : frac_(numerator, denominator) {}
    /**
     * @brief constructor of the Phase
     * @param [in] frac fraction of the phase except to pi
     */
    Phase(const Fraction& frac) : frac_(frac.numerator(), frac.denominator()) {}
    /**
     * @brief constructor of the Phase
     * @param [in] str string representation of the phase
     */
    Phase(const std::string& str);
    /**
     * @brief copy constructor of the Phase
     * @param [in] phase phase
     */
    Phase(const Phase& phase) : frac_(phase.frac_) {}
    //! getter of the frac_
    Fraction frac() const { return frac_; }
    //! setter of the frac_
    void frac(Fraction frac) { frac_ = frac; }
    /**
     * @brief get a string of the Phase object
     * @return string of the phase
     */
    std::string to_string(bool pi_str = true) const;
    /**
     * @brief get a value of the phase
     * @return value of the phase
     */
    double value() const { return M_PI * frac_.numerator() / frac_.denominator(); }
    /**
     * @brief is the phase zero or not
     * @return true if the phase is zero, false otherwise
     */
    bool is_zero() const { return (*this == Phase(0)); }
    /**
     * @brief is the phase positive or not
     * @return true if the phase is positive, false otherwise
     */
    bool is_positive() const { return frac_.is_positive(); }
    /**
     * @brief is the phase negative or not
     * @return true if the phase is negative, false otherwise
     */
    bool is_negative() const { return frac_.is_negative(); }
    /**
     * @brief reduce the fraction of the phase except to pi
     */
    void reduce() { frac_.reduce(); }
    /**
     * @brief mod 2pi of the phase
     */
    void mod_2pi()
    {
      frac_.reduce();
      if (frac_.is_positive()) {
    	int32_t rounds = frac_.numerator() / (frac_.denominator() * 2);
    	frac_.numerator(frac_.numerator() - 2 * rounds * frac_.denominator());
      }
      else if (frac_.is_negative()) {
    	int32_t nume = -frac_.numerator();
    	int32_t deno_2 = frac_.denominator() * 2;
    	int32_t rounds = (nume + deno_2 - 1 ) / deno_2;
    	frac_.numerator(2 * rounds * frac_.denominator() + frac_.numerator());
      }
    }
    Phase operator+() const { return *this; }
    Phase operator-() const { return Phase(-frac_); }
    Phase& operator+=(const Phase& rhs) { frac_ += rhs.frac(); return *this; }
    Phase& operator-=(const Phase& rhs) { frac_ -= rhs.frac(); return *this; }
    Phase& operator*=(const int32_t rhs) { frac_ *= rhs; return *this; }
    Phase& operator/=(const int32_t rhs) { frac_ /= rhs; return *this; }
    friend bool operator==(const Phase& lhs, const Phase& rhs) {
      Phase lhs_mod = lhs; lhs_mod.mod_2pi();
      Phase rhs_mod = rhs; rhs_mod.mod_2pi();
      return lhs_mod.frac() == rhs_mod.frac();
    }
    friend bool operator!=(const Phase& lhs, const Phase& rhs) { return !(lhs == rhs); }
    friend Phase operator+(const Phase& lhs, const Phase& rhs) { return Phase(lhs.frac() + rhs.frac()); }
    friend Phase operator-(const Phase& lhs, const Phase& rhs) { return Phase(lhs.frac() - rhs.frac()); }
    friend Phase operator*(const Phase& lhs, const int32_t rhs) { return Phase(lhs.frac() * rhs); }
    friend Phase operator*(const int32_t lhs, const Phase& rhs) { return Phase(lhs * rhs.frac()); }
    friend Phase operator/(const Phase& lhs, const int32_t rhs) { return Phase(lhs.frac() / rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const Phase& phase) { ost << phase.to_string(); return ost; }
  };
}

#endif
