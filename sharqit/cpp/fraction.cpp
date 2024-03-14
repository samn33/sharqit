/**
 * @file fraction.cpp
 * @brief member functions of Fraction class
 */

#include "fraction.h"

std::string Sharqit::Fraction::to_string() const
{
  std::string str;
  if (numerator_ == 0) str = "0";
  else if (numerator_ == 1 && denominator_ == 1) str = "1";
  else str = std::to_string(numerator_) + "/" + std::to_string(denominator_);
  return str;
}
