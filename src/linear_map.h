#ifndef LINEAR_MAP_H
#define LINEAR_MAP_H

#include <vector>
#include <cmath>
#include <complex>
#include <Eigen/Dense>

#include "qgate.h"

namespace Sharq {

  constexpr double EPS = 1.0e-8;

  class LinearMap : public Eigen::MatrixXcd
  {
  public:
    LinearMap(uint32_t row_num = 0, uint32_t col_num = 0)
      : Eigen::MatrixXcd(Eigen::MatrixXcd::Identity(row_num, col_num)) {}
    LinearMap(const std::vector<std::vector<std::complex<double>>>& mat)
      : Eigen::MatrixXcd(mat.size(), mat[0].size())
    {
      for (uint32_t i = 0; i < mat.size(); ++i) {
	for (uint32_t j = 0; j < mat[0].size(); ++j) {
	  coeffRef(i, j) = mat[i][j];
	}
      }
    }
    template<typename OtherDerived>
    LinearMap(const Eigen::MatrixBase<OtherDerived>& other) : Eigen::MatrixXcd(other) {}
    /* member functions */
    uint32_t row_num() const { return (uint32_t)rows(); }
    uint32_t col_num() const { return (uint32_t)cols(); }
    bool is_zero() const;
    bool is_identity() const;
    bool is_diagonal() const;
    bool is_identity_multiple_constant() const;
    bool is_unitary() const;
    bool is_equal(const LinearMap& other) const;
    LinearMap& operate_qgate(const QGate& qgate);
    template<typename OtherDerived>
    LinearMap& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Eigen::MatrixXcd::operator=(other);
      return *this;
    }
    LinearMap operator+() const { return *this; }
    LinearMap operator-() const { return *this * (-1.0); }
    friend LinearMap operator+(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res += rhs; return res; }
    friend LinearMap operator-(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res -= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const double rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const std::complex<double> rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const double lhs, const LinearMap& rhs) { LinearMap res = rhs; res *= lhs; return res; }
    friend LinearMap operator*(const std::complex<double> lhs, const LinearMap& rhs) { LinearMap res = rhs; res *= lhs; return res; }
    friend LinearMap operator/(const LinearMap& lhs, const double rhs) { LinearMap res = lhs; res /= rhs; return res; }
    friend LinearMap operator/(const LinearMap& lhs, const std::complex<double> rhs) { LinearMap res = lhs; res /= rhs; return res; }
  };
}

#endif
