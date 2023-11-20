#include "sharq.h"

std::string Sharq::BinaryMatrix::to_string() const
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

void Sharq::BinaryMatrix::xor_rows(const uint32_t a, const uint32_t b)
{
  if (a > row_num_ || b > row_num_) {
    throw std::runtime_error("argument is too large.");
  }
  for (uint32_t i = 0; i < col_num_; ++i) {
    elements_[b][i] = (elements_[b][i] + elements_[a][i]) % 2; // XOR operation
  }
}

void Sharq::BinaryMatrix::swap_rows(const uint32_t a, const uint32_t b)
{
  if (a > row_num_ || b > row_num_) {
    throw std::runtime_error("argument is too large.");
  }
  for (uint32_t i = 0; i < col_num_; ++i) {
    std::swap(elements_[a][i], elements_[b][i]);
  }
  std::swap(row_indexes_[a], row_indexes_[b]);
}

void Sharq::BinaryMatrix::swap_cols(const uint32_t a, const uint32_t b)
{
  //  std::cout << "-- swap_cols --" << std::endl; // test

  if (a > col_num_ || b > col_num_) {
    throw std::runtime_error("argument is too large.");
  }
  for (uint32_t i = 0; i < row_num_; ++i) {
    std::swap(elements_[i][a], elements_[i][b]);
  }
}

bool Sharq::BinaryMatrix::feasible() const
{
  bool ans = false;
  for (auto& row:elements_) {
    uint32_t sum = 0;
    for (auto& e:row) {
      sum += (uint32_t)e;
      if (sum > 1) break;
    }
    if (sum == 1) {
      ans = true;
      break;
    }
  }
  return ans;
}

std::vector<std::pair<uint32_t, uint32_t>> Sharq::BinaryMatrix::gauss_reduce()
{
  std::vector<std::pair<uint32_t, uint32_t>> row_ops;

  //if (feasible()) return row_ops;

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
	xor_rows(i, k);
	row_ops.push_back({row_indexes_[i], row_indexes_[k]});
      }
    }
  }

  /* backward elimination */
  for (int32_t i = (int32_t)row_num_ - 1; i > 0; --i) {
    if (i < (int32_t)col_num_ && elements_[i][i] == 0) continue;
    for (int32_t k = i - 1; k >= 0; --k) {
      if (i < (int32_t)col_num_ && elements_[k][i] == 1) {
	xor_rows(i, k);
	row_ops.push_back({row_indexes_[i], row_indexes_[k]});
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
	  row_ops.push_back({row_indexes_[i], row_indexes_[j]});
	  break;
	}
      }
    }
  }
  
  return row_ops;
}
