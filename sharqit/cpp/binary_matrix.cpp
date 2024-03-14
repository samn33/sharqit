/**
 * @file binary_matrix.cpp
 * @brief member functions of BinaryMatrix class
 */

#include "binary_matrix.h"

std::string Sharqit::BinaryMatrix::to_string() const
{
  if (elements_.empty()) return "";
  
  std::stringstream ss;

  for (uint32_t i = 0; i < elements_.size(); ++i) {
    ss << "[" << row_indexes_[i] << "] ";
    for (auto e:elements_[i]) {
      ss << std::to_string(e) << " ";
    }
    ss << std::endl;
  }

  std::string s = ss.str();
  s.pop_back();
  
  return s;
}

std::vector<std::pair<uint32_t, uint32_t>> Sharqit::BinaryMatrix::gauss_reduce(const uint32_t num_of_sum1)
{
  std::vector<std::pair<uint32_t, uint32_t>> row_ops;
  uint32_t sum1_count = 0;
  
  /* foreward elimination */
  for (int32_t i = 0; i < (int32_t)row_num_ - 1; ++i) {
    if (i < (int32_t)col_num_ && elements_[i][i] == 0) {
      for (int32_t j = i + 1; j < (int32_t)row_num_; ++j) {
	if (elements_[j][i] == 1) {
	  swap_rows(i, j);
	  break;
	}
      }
    }

    if (i < (int32_t)col_num_ && elements_[i][i] == 0) {
      for (int32_t j = i + 1; j < (int32_t)col_num_; ++j) {
    	if (elements_[i][j] == 1) {
    	  swap_cols(i, j);
    	  break;
    	}
      }
    }

    for (int32_t k = i + 1; k < (int32_t)row_num_; ++k) {
      if (i < (int32_t)col_num_ && elements_[k][i] == 1) {
	uint32_t sum = xor_rows(i, k);
	if (sum == 1) ++sum1_count;
	row_ops.push_back({row_indexes_[i], row_indexes_[k]});
	if (sum1_count >= num_of_sum1) return row_ops;
      }
    }
  }

  /* backward elimination */
  for (int32_t i = (int32_t)row_num_ - 1; i > 0; --i) {
    if (i < (int32_t)col_num_ && elements_[i][i] == 0) continue;
    for (int32_t k = i - 1; k >= 0; --k) {
      if (i < (int32_t)col_num_ && elements_[k][i] == 1) {
	uint32_t sum = xor_rows(i, k);
	if (sum == 1) ++sum1_count;
	row_ops.push_back({row_indexes_[i], row_indexes_[k]});
	if (sum1_count >= num_of_sum1) return row_ops;
      }
    }
  }

  if (!feasible()) {
    std::vector<uint8_t> rows(col_num_, 0);
    for (uint32_t i = 0; i < row_num_; ++i) {
      for (uint32_t j = i + 1; j < row_num_; ++j) {
	uint32_t sum = 0;
	for (uint32_t k = 0; k < col_num_; ++k) {
	  rows[k] = (elements_[i][k] + elements_[j][k]) % 2;
	  sum += (uint32_t)rows[k];
	}
	if (sum == 1) {
	  xor_rows(i, j);
	  ++sum1_count;
	  row_ops.push_back({row_indexes_[i], row_indexes_[j]});
	  if (sum1_count >= 1) return row_ops;
	  else break;
	}
      }
    }
  }
  
  return row_ops;
}
