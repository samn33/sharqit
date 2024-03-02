/**
 * @file linear_map.h
 * @brief include file of LinearMap class
 */

#ifndef LINEAR_MAP_H
#define LINEAR_MAP_H

#include <vector>
#include <cmath>
#include <complex>
#include <Eigen/Dense>

#include "qgate.h"

namespace Sharqit {

  constexpr double EPS = 1.0e-8; //!< epsilon for evaluating very small error

  /**
   * @class LinearMap
   * @brief linear map
  */
  class LinearMap : public Eigen::MatrixXcd
  {
  public:
    /**
     * @brief constructor of the LinearMap
     * @param [in] row_num row number of the linear map (matrix)
     * @param [in] col_num column number of the linear map (matrix)
     * @note construct as an identity matrix
     */
    LinearMap(uint32_t row_num = 0, uint32_t col_num = 0)
      : Eigen::MatrixXcd(Eigen::MatrixXcd::Identity(row_num, col_num)) {}
    /**
     * @brief constructor of the LinearMap
     * @param [in] mat matrix corresponding to the linear map
     */
    LinearMap(const std::vector<std::vector<std::complex<double>>>& mat)
      : Eigen::MatrixXcd(mat.size(), mat[0].size())
    {
      for (uint32_t i = 0; i < mat.size(); ++i) {
	for (uint32_t j = 0; j < mat[0].size(); ++j) {
	  coeffRef(i, j) = mat[i][j];
	}
      }
    }
    /**
     * @brief copy constructor of the LinearMap
     * @param [in] other LinearMap object
     */
    template<typename OtherDerived>
    LinearMap(const Eigen::MatrixBase<OtherDerived>& other) : Eigen::MatrixXcd(other) {}
    /**
     * @brief get a row number of the linear map (matrix)
     * @return row number
     */
    uint32_t row_num() const { return (uint32_t)rows(); }
    /**
     * @brief get a column number of the linear map (matrix)
     * @return column number
     */
    uint32_t col_num() const { return (uint32_t)cols(); }
    /**
     * @brief whether the linear map is zero or not
     * @return true if the linear map is zero, false otherwise
     */
    bool is_zero() const;
    /**
     * @brief whether the linear map is identity or not
     * @return true if the linear map is identity, false otherwise
     */
    bool is_identity() const;
    /**
     * @brief whether the linear map is diagonal or not
     * @return true if the linear map is diagonal, false otherwise
     */
    bool is_diagonal() const;
    /**
     * @brief whether the linear map is identity multiple constant or not
     * @details that is, all diagonal elements are the same and other elements are all zero or not
     * @return true if the linear map is identity multiple constant, false otherwise
     */
    bool is_identity_multiple_constant() const;
    /**
     * @brief whether the linear map is unitary or not
     * @return true if the linear map is unitary, false otherwise
     */
    bool is_unitary() const;
    /**
     * @brief whether the linear map is equal to the other or not
     * @return true if the linear map is equal to the other, false otherwise
     */
    bool is_equal(const LinearMap& other) const;
    /**
     * @brief operate the quantum gate
     * @return result of the operation
     */
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
