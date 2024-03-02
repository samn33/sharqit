/**
 * @file phase.cpp
 * @brief member functions of Phase class
 */

#include "phase.h"

Sharqit::Phase::Phase(const std::string& str)
{
  std::string s = std::regex_replace(str, std::regex("PI"), "1");
  s = std::regex_replace(s, std::regex(" "), "");
  s = std::regex_replace(s, std::regex("\\*1"), "");

  if (std::regex_match(s, std::regex(R"(-*\d+/*\d*)")) == false) {
    throw std::runtime_error("invalid phase string.");
  }
  
  std::vector<std::string> phase_str_vec = Sharqit::split(s, '/');
  int32_t numerator = stoi(phase_str_vec[0]);
  int32_t denominator = 1;
  if (phase_str_vec.size() > 1) {
    denominator = stoi(phase_str_vec[1]);
  }

  frac_.numerator(numerator);
  frac_.denominator(denominator);

  frac_.reduce();
}

std::string Sharqit::Phase::to_string(bool pi_str) const
{
  std::string pi = "π";
  std::string str = "";
  Sharqit::Phase p = Sharqit::Phase(frac_);
  p.mod_2pi();

  if (pi_str == true) {
    if (p.frac().numerator() == 1 && p.frac().denominator() == 1) {
      str = pi;
    }
    else if (p.frac().numerator() == -1 && p.frac().denominator() == 1) {
      str = "-" + pi;
    }
    else if (p.frac().numerator() == 1 && p.frac().denominator() != 1) {
      str = pi + "/" + std::to_string(p.frac().denominator());
    }
    else if (p.frac().numerator() == -1 && p.frac().denominator() != 1) {
      str = "-" + pi + "/" + std::to_string(p.frac().denominator());
    }
    else if (p.frac().numerator() == 0) {
      str = "0";
    }
    else if (p.frac().denominator() == 1) {
      str = std::to_string(p.frac().numerator()) + pi;
    }
    else {
      str = std::to_string(p.frac().numerator()) + pi + "/" + std::to_string(p.frac().denominator());
    }
  }
  else {
    if (p.frac().numerator() == 1 && p.frac().denominator() == 1) {
      str = "1";
    }
    else if (p.frac().numerator() == -1 && p.frac().denominator() == 1) {
      str = "-1";
    }
    else if (p.frac().numerator() == 1 && p.frac().denominator() != 1) {
      str = "1/" + std::to_string(p.frac().denominator());
    }
    else if (p.frac().numerator() == -1 && p.frac().denominator() != 1) {
      str = "-1/" + std::to_string(p.frac().denominator());
    }
    else if (p.frac().numerator() == 0) {
      str = "0";
    }
    else if (p.frac().denominator() == 1) {
      str = std::to_string(p.frac().numerator());
    }
    else {
      str = std::to_string(p.frac().numerator()) + "/" + std::to_string(p.frac().denominator());
    }
  }
  return str;
}
