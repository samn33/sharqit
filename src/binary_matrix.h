#ifndef BINARY_MATRIX_H
#define BINARY_MATRIX_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Sharq {

  class BinaryMatrix
  {
  private:
    uint32_t row_num_;
    uint32_t col_num_;
    std::vector<std::vector<uint8_t>> elements_;
    std::vector<uint32_t> row_indexes_;
  public:
    BinaryMatrix(const uint32_t row_num = 0, const uint32_t col_num = 0, const uint8_t val = 0)
      : row_num_(row_num), col_num_(col_num), elements_(row_num, std::vector<uint8_t>(col_num, val))
    {
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    BinaryMatrix(const std::vector<std::vector<uint8_t>>& elements)
    {
      elements_ = elements;
      row_num_ = elements.size();
      col_num_ = elements[0].size();
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    BinaryMatrix(const BinaryMatrix& bm)
      : row_num_(bm.row_num_), col_num_(bm.col_num_), elements_(bm.elements_), row_indexes_(bm.row_indexes_) {}
    /* getter */
    uint32_t row_num() const { return row_num_; }
    uint32_t col_num() const { return col_num_; }
    std::vector<std::vector<uint8_t>> elements() const { return elements_; };
    std::vector<uint32_t> row_indexes() const { return row_indexes_; };
    /* setter */
    void row_num(const uint32_t row_num) { row_num_ = row_num; }
    void col_num(const uint32_t col_num) { col_num_ = col_num; }
    void elements(const std::vector<std::vector<uint8_t>>& elements) { elements_ = elements; }
    void row_indexes(const std::vector<uint32_t>& row_indexes) { row_indexes_ = row_indexes; }
    /* member functions */
    std::string to_string() const;
    uint32_t xor_rows(const uint32_t a, const uint32_t b)
    {
      if (a > row_num_ || b > row_num_) {
    	throw std::runtime_error("argument is too large.");
      }
      uint32_t sum = 0;
      for (uint32_t i = 0; i < col_num_; ++i) {
    	elements_[b][i] = (elements_[b][i] + elements_[a][i]) % 2; // XOR operation
    	sum += elements_[b][i];
      }
      return sum;
    }
    void swap_rows(const uint32_t a, const uint32_t b)
    {
      if (a > row_num_ || b > row_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < col_num_; ++i) {
	std::swap(elements_[a][i], elements_[b][i]);
      }
      std::swap(row_indexes_[a], row_indexes_[b]);
    }
    void swap_cols(const uint32_t a, const uint32_t b)
    {
      if (a > col_num_ || b > col_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < row_num_; ++i) {
	std::swap(elements_[i][a], elements_[i][b]);
      }
    }
    bool feasible() const
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
    std::vector<std::pair<uint32_t, uint32_t>> gauss_reduce(const uint32_t num_of_sum1 = 5);
    friend std::ostream& operator<<(std::ostream& ost, const BinaryMatrix& bmat) { ost << bmat.to_string(); return ost; }
  };
}

#endif
