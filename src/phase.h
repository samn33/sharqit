#ifndef PHASE_H
#define PHASE_H

#include <iostream>
#include <string>
#include <cmath>
#include <regex>

#include "util.h"
#include "fraction.h"

namespace Sharq {

  class Phase
  {
  private:
    Fraction frac_;
  public:
    Phase(const int32_t numerator = 0, const int32_t denominator = 1) : frac_(numerator, denominator) {}
    Phase(const Fraction& frac) : frac_(frac.numerator(), frac.denominator()) {}
    Phase(const std::string& str);
    Phase(const Phase& phase) : frac_(phase.frac_) {}
    /* getters */
    Fraction frac() const { return frac_; }
    /* setters */
    void frac(Fraction frac) { frac_ = frac; }
    /* member functions */
    std::string to_string(bool pi_str = true) const;
    double value() const { return M_PI * frac_.numerator() / frac_.denominator(); }
    bool is_zero() const { return (*this == Phase(0)); }
    bool is_positive() const { return frac_.is_positive(); }
    bool is_negative() const { return frac_.is_negative(); }
    void reduce() { frac_.reduce(); }
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
