/**
 * @file binary_matrix.h
 * @brief include file of BinaryMatrix class
 */

#ifndef BINARY_MATRIX_H
#define BINARY_MATRIX_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Sharqit {

  /**
   * @class BinaryMatrix
   * @brief binary matrix
  */
  class BinaryMatrix
  {
  private:
    uint32_t row_num_; //!< row number of the matrix
    uint32_t col_num_; //!< column number of the matrix
    std::vector<std::vector<uint8_t>> elements_; //!< elements of the matrix, each element is 0 or 1
    std::vector<uint32_t> row_indexes_; //!< vector of qubit id corresponding to each row
  public:
    /**
     * @brief constructor of the BinaryMatrix
     * @param [in] row_num row number of the matrix
     * @param [in] col_num column number of the matrix
     * @param [in] val value of each element of the matrix
     */
    BinaryMatrix(const uint32_t row_num = 0, const uint32_t col_num = 0, const uint8_t val = 0)
      : row_num_(row_num), col_num_(col_num), elements_(row_num, std::vector<uint8_t>(col_num, val))
    {
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    /**
     * @brief constructor of the BinaryMatrix
     * @param [in] elements elements of the matrix
     */
    BinaryMatrix(const std::vector<std::vector<uint8_t>>& elements)
    {
      elements_ = elements;
      row_num_ = elements.size();
      col_num_ = elements[0].size();
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    /**
     * @brief copy constructor of the BinaryMatrix
     * @param [in] bm the binary matrix
     */
    BinaryMatrix(const BinaryMatrix& bm)
      : row_num_(bm.row_num_), col_num_(bm.col_num_), elements_(bm.elements_), row_indexes_(bm.row_indexes_) {}
    //! getter of the row_num_
    uint32_t row_num() const { return row_num_; }
    //! getter of the col_num_
    uint32_t col_num() const { return col_num_; }
    //! getter of the elements_
    std::vector<std::vector<uint8_t>> elements() const { return elements_; };
    //! getter of the row_indexes_
    std::vector<uint32_t> row_indexes() const { return row_indexes_; };
    //! setter of the row_num_
    void row_num(const uint32_t row_num) { row_num_ = row_num; }
    //! setter of the col_num_
    void col_num(const uint32_t col_num) { col_num_ = col_num; }
    //! setter of the elements_
    void elements(const std::vector<std::vector<uint8_t>>& elements) { elements_ = elements; }
    //! setter of the row_indexes_
    void row_indexes(const std::vector<uint32_t>& row_indexes) { row_indexes_ = row_indexes; }
    /**
     * @brief get a string of the BinaryMatrix object
     * @return string of the binary matrix
     */
    std::string to_string() const;
    /**
     * @brief set identity matrix
     * @note If row_num_ and col_num_ are different, the diagonal components from (0,0) to (A,A)-elment will be 1, @n
     * where A is min(row_num_, col_num_)
     */
    void set_identity()
    {
      for (uint32_t i = 0; i < row_num_; ++i) {
	for (uint32_t j = 0; j < col_num_; ++j) {
	  elements_[i][j] = 0;
	}
      }
      for (uint32_t i = 0; i < std::min(row_num_, col_num_); ++i) elements_[i][i] = 1;
    }
    /**
     * @brief reverse an element of the matrix (0 to 1 or 1 to 0)
     * @param [in] i index of row
     * @param [in] j index of column
     */
    void reverse_element(const uint32_t i, const uint32_t j)
    {
      if (i >= row_num_ || j >= col_num_) throw std::runtime_error("argument is too large.");
      elements_[i][j] = (elements_[i][j] + 1) % 2;
    }
    /**
     * @brief XOR operation of the two rows
     * @details bth row <= ath row + bth row
     * @param [in] a index of row
     * @param [in] b index of row
     * @return sum of the bth row of the result of operation (1 number included in the row)
     */
    uint32_t xor_rows(const uint32_t a, const uint32_t b)
    {
      if (a >= row_num_ || b >= row_num_) {
    	throw std::runtime_error("argument is too large.");
      }
      uint32_t sum = 0;
      for (uint32_t i = 0; i < col_num_; ++i) {
    	elements_[b][i] = (elements_[b][i] + elements_[a][i]) % 2; // XOR operation
    	sum += elements_[b][i];
      }
      return sum;
    }
    /**
     * @brief swap of two rows
     * @param [in] a index of row
     * @param [in] b index of row
     */
    void swap_rows(const uint32_t a, const uint32_t b)
    {
      if (a >= row_num_ || b >= row_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < col_num_; ++i) {
	std::swap(elements_[a][i], elements_[b][i]);
      }
      std::swap(row_indexes_[a], row_indexes_[b]);
    }
    /**
     * @brief swap of two columns
     * @param [in] a index of column
     * @param [in] b index of column
     */
    void swap_cols(const uint32_t a, const uint32_t b)
    {
      if (a >= col_num_ || b >= col_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < row_num_; ++i) {
	std::swap(elements_[i][a], elements_[i][b]);
      }
    }
    /**
     * @brief feasible or not
     * @return true if the sum of at least one row is 1, false otherwise
     */
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
    /**
     * @brief gauss-reduce algorithm for circuit extraction from zx-diagram
     * @param [in] num_of_sum1 number of rows in which the sum of the elements becomes 1
     * @return vector of qubit index pairs corresponding to row operations in the process of the gauss-reduce algorithm
     */
    std::vector<std::pair<uint32_t, uint32_t>> gauss_reduce(const uint32_t num_of_sum1 = 5);
    friend std::ostream& operator<<(std::ostream& ost, const BinaryMatrix& bmat) { ost << bmat.to_string(); return ost; }
  };
}

#endif
