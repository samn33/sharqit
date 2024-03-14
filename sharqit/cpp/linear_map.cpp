/**
 * @file linear_map.cpp
 * @brief member functions of LinearMap class
 */

#include "linear_map.h"

bool Sharqit::LinearMap::is_zero() const
{
  Sharqit::LinearMap lmap = *this;

  for (uint32_t i = 0; i < row_num(); ++i) {
    for (uint32_t j = 0; j < col_num(); ++j) {
      if (std::abs(lmap(i, j).real()) > Sharqit::EPS) return false;
      else if (std::abs(lmap(i, j).imag()) > Sharqit::EPS) return false;
    }
  }
  return true;
}

bool Sharqit::LinearMap::is_identity() const
{
  if (row_num() != col_num()) return false;
  else if (row_num() == 0 || col_num() == 0) return false;

  Sharqit::LinearMap lmap = *this;
  Sharqit::LinearMap lmap_identity(row_num(), col_num());

  lmap -= lmap_identity;
  return lmap.is_zero();
}

bool Sharqit::LinearMap::is_diagonal() const
{
  if (row_num() != col_num()) return false;
  else if (row_num() == 0 || col_num() == 0) return false;

  Sharqit::LinearMap lmap = *this;
  for (uint32_t i = 0; i < row_num(); ++i) {
    lmap.coeffRef(i, i) = 0.0;
  }
  return lmap.is_zero();
}

bool Sharqit::LinearMap::is_identity_multiple_constant() const
{
  if (row_num() != col_num()) return false;
  else if (row_num() == 0 || col_num() == 0) return false;

  Sharqit::LinearMap lmap = *this;
  if (lmap.is_diagonal() == false) return false;

  for (uint32_t i = 0; i < row_num(); ++i) {
    if (std::abs(lmap(i, i) - lmap(0, 0)) > Sharqit::EPS) return false;
  }
  return true;
}

bool Sharqit::LinearMap::is_unitary() const
{
  if (row_num() != col_num()) return false;
  else if (row_num() == 0 || col_num() == 0) return false;

  Sharqit::LinearMap lmap = *this;
  lmap.adjointInPlace();
  lmap *= (*this);
  return lmap.is_identity();
}

bool Sharqit::LinearMap::is_equal(const LinearMap& other) const
{
  if (row_num() != other.row_num()) return false;
  else if (col_num() != other.col_num()) return false;

  Sharqit::LinearMap lmap = *this;

  lmap -= other;
  return lmap.is_zero();
}

Sharqit::LinearMap& Sharqit::LinearMap::operate_qgate(const QGate& qgate)
{
  if (row_num() != col_num()) {
    throw std::runtime_error("row_num and col_num must be equal.");
  }

  std::vector<std::vector<std::complex<double>>> op = qgate.op();
  Sharqit::LinearMap lmap = *this;
  uint32_t dim = row_num();

  double log2_dim = std::log2((double)dim);
  uint32_t qnum = (uint32_t)log2_dim; // number of qubits
  if ((double)qnum != log2_dim) {
    throw std::runtime_error("invalid row_num or col_num.");
  }

  if ((qgate.kind() != Sharqit::QGateKind::CX) && (qgate.kind() != Sharqit::QGateKind::CZ)) {
  
    uint32_t nn = qnum - qgate.qid()[0] - 1;

    for (uint32_t i = 0; i < dim; ++i) {
      for (uint32_t j = 0; j < dim; ++j) {
	if ((i >> nn) %2 == 0) {
	  coeffRef(i, j) = op[0][0] * lmap(i, j) + op[0][1] * lmap(i + (1 << nn), j);
	}
	else {
	  coeffRef(i, j) = op[1][0] * lmap(i - (1 << nn), j) + op[1][1] * lmap(i, j);
	}
      }
    }
  }
  
  else {

    uint32_t mm = qnum - qgate.qid()[0] - 1;
    uint32_t nn = qnum - qgate.qid()[1] - 1;

    for (uint32_t i = 0; i < dim; ++i) {
      for (uint32_t j = 0; j < dim; ++j) {
	if (((i >> mm) % 2 == 0) && ((i >> nn) % 2 == 0)) {
	  coeffRef(i, j)
	    = op[0][0] * lmap(i, j)
	    + op[0][1] * lmap(i + (1 << nn), j)
	    + op[0][2] * lmap(i + (1 << mm), j)
	    + op[0][3] * lmap(i + (1 << mm) + (1 << nn), j);
	}
	else if (((i >> mm) % 2 == 0) && ((i >> nn) % 2 == 1)) {
	  coeffRef(i, j)
	    = op[1][0] * lmap(i - (1 << nn), j)
	    + op[1][1] * lmap(i, j)
	    + op[1][2] * lmap(i - (1 << nn) + (1 << mm), j)
	    + op[1][3] * lmap(i + (1 << mm), j);
	}
	else if (((i >> mm) % 2 == 1) && ((i >> nn) % 2 == 0)) {
	  coeffRef(i, j)
	    = op[2][0] * lmap(i - (1 << mm), j)
	    + op[2][1] * lmap(i + (1 << nn) - (1 << mm), j)
	    + op[2][2] * lmap(i, j)
	    + op[2][3] * lmap(i + (1 << nn), j);
	}
	else {
	  coeffRef(i, j)
	    = op[3][0] * lmap(i - (1 << nn) - (1 << mm), j)
	    + op[3][1] * lmap(i - (1 << mm), j)
	    + op[3][2] * lmap(i - (1 << nn), j)
	    + op[3][3] * lmap(i, j);
	}
      }
    }
  }
  
  return *this;
}
