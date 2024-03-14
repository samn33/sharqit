#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(BinaryMatrix, Constructor01) {
  Sharqit::BinaryMatrix bmat;
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 0);
  EXPECT_EQ(bmat.col_num(), 0);

  EXPECT_EQ(elements.empty(), true);
  EXPECT_EQ(row_indexes.empty(), true);
}

TEST(BinaryMatrix, Constructor02) {
  Sharqit::BinaryMatrix bmat(2,3);
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 2);
  EXPECT_EQ(bmat.col_num(), 3);

  EXPECT_EQ(row_indexes[0], 0);
  EXPECT_EQ(row_indexes[1], 1);

  EXPECT_EQ(elements[0][0], 0);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 0);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 0);
  EXPECT_EQ(elements[1][2], 0);
}

TEST(BinaryMatrix, Constructor03) {
  Sharqit::BinaryMatrix bmat({{1, 0, 1},
			    {0, 0, 1},
			    {1, 1, 0}});
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 3);
  EXPECT_EQ(bmat.col_num(), 3);

  EXPECT_EQ(row_indexes[0], 0);
  EXPECT_EQ(row_indexes[1], 1);
  EXPECT_EQ(row_indexes[2], 2);

  EXPECT_EQ(elements[0][0], 1);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 1);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 0);
  EXPECT_EQ(elements[1][2], 1);
  EXPECT_EQ(elements[2][0], 1);
  EXPECT_EQ(elements[2][1], 1);
  EXPECT_EQ(elements[2][2], 0);
}

TEST(BinaryMatrix, GaussReduce01) {
  Sharqit::BinaryMatrix bmat({{1, 0, 1},
			    {0, 0, 1},
			    {1, 1, 0}});

  std::vector<std::pair<uint32_t, uint32_t>> row_ops = bmat.gauss_reduce(1000);
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 3);
  EXPECT_EQ(bmat.col_num(), 3);
  
  EXPECT_EQ(row_indexes[0], 0);
  EXPECT_EQ(row_indexes[1], 2);
  EXPECT_EQ(row_indexes[2], 1);

  EXPECT_EQ(elements[0][0], 1);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 0);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 1);
  EXPECT_EQ(elements[1][2], 0);
  EXPECT_EQ(elements[2][0], 0);
  EXPECT_EQ(elements[2][1], 0);
  EXPECT_EQ(elements[2][2], 1);

  EXPECT_EQ(row_ops[0].first, 0);
  EXPECT_EQ(row_ops[0].second, 2);
  EXPECT_EQ(row_ops[1].first, 1);
  EXPECT_EQ(row_ops[1].second, 2);
  EXPECT_EQ(row_ops[2].first, 1);
  EXPECT_EQ(row_ops[2].second, 0);
}

TEST(BinaryMatrix, GaussReduce02) {
  Sharqit::BinaryMatrix bmat({{1, 0, 1},
			    {1, 0, 1},
			    {1, 0, 1}});

  std::vector<std::pair<uint32_t, uint32_t>> row_ops = bmat.gauss_reduce(1000);
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 3);
  EXPECT_EQ(bmat.col_num(), 3);
  
  EXPECT_EQ(row_indexes[0], 0);
  EXPECT_EQ(row_indexes[1], 1);
  EXPECT_EQ(row_indexes[2], 2);
  
  EXPECT_EQ(elements[0][0], 1);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 1);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 0);
  EXPECT_EQ(elements[1][2], 0);
  EXPECT_EQ(elements[2][0], 0);
  EXPECT_EQ(elements[2][1], 0);
  EXPECT_EQ(elements[2][2], 0);

  EXPECT_EQ(row_ops[0].first, 0);
  EXPECT_EQ(row_ops[0].second, 1);
  EXPECT_EQ(row_ops[1].first, 0);
  EXPECT_EQ(row_ops[1].second, 2);
}

TEST(BinaryMatrix, GaussReduce03) {
  Sharqit::BinaryMatrix bmat({{1, 1, 1, 0},
			    {0, 1, 1, 1},
			    {1, 1, 0, 1}});

  std::vector<std::pair<uint32_t, uint32_t>> row_ops = bmat.gauss_reduce(1000);
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 3);
  EXPECT_EQ(bmat.col_num(), 4);
  
  EXPECT_EQ(row_indexes[0], 0);
  EXPECT_EQ(row_indexes[1], 1);
  EXPECT_EQ(row_indexes[2], 2);
  
  EXPECT_EQ(elements[0][0], 1);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 0);
  EXPECT_EQ(elements[0][3], 1);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 1);
  EXPECT_EQ(elements[1][2], 0);
  EXPECT_EQ(elements[1][3], 0);
  EXPECT_EQ(elements[2][0], 0);
  EXPECT_EQ(elements[2][1], 0);
  EXPECT_EQ(elements[2][2], 1);
  EXPECT_EQ(elements[2][3], 1);
  
  EXPECT_EQ(row_ops[0].first, 0);
  EXPECT_EQ(row_ops[0].second, 2);
  EXPECT_EQ(row_ops[1].first, 2);
  EXPECT_EQ(row_ops[1].second, 1);
  EXPECT_EQ(row_ops[2].first, 2);
  EXPECT_EQ(row_ops[2].second, 0);
  EXPECT_EQ(row_ops[3].first, 1);
  EXPECT_EQ(row_ops[3].second, 0);
}

TEST(BinaryMatrix, GaussReduce04) {
  Sharqit::BinaryMatrix bmat({{0, 1, 1},
			    {0, 1, 1},
			    {1, 1, 0},
			    {1, 1, 0},
    });

  std::vector<std::pair<uint32_t, uint32_t>> row_ops = bmat.gauss_reduce(1000);
  std::vector<std::vector<uint8_t>> elements = bmat.elements();
  std::vector<uint32_t> row_indexes = bmat.row_indexes();

  EXPECT_EQ(bmat.row_num(), 4);
  EXPECT_EQ(bmat.col_num(), 3);
  
  EXPECT_EQ(row_indexes[0], 2);
  EXPECT_EQ(row_indexes[1], 1);
  EXPECT_EQ(row_indexes[2], 0);
  EXPECT_EQ(row_indexes[3], 3);
  
  EXPECT_EQ(elements[0][0], 1);
  EXPECT_EQ(elements[0][1], 0);
  EXPECT_EQ(elements[0][2], 1);
  EXPECT_EQ(elements[1][0], 0);
  EXPECT_EQ(elements[1][1], 1);
  EXPECT_EQ(elements[1][2], 1);
  EXPECT_EQ(elements[2][0], 0);
  EXPECT_EQ(elements[2][1], 0);
  EXPECT_EQ(elements[2][2], 0);
  EXPECT_EQ(elements[3][0], 0);
  EXPECT_EQ(elements[3][1], 0);
  EXPECT_EQ(elements[3][2], 0);
  
  EXPECT_EQ(row_ops[0].first, 2);
  EXPECT_EQ(row_ops[0].second, 3);
  EXPECT_EQ(row_ops[1].first, 1);
  EXPECT_EQ(row_ops[1].second, 0);
  EXPECT_EQ(row_ops[2].first, 1);
  EXPECT_EQ(row_ops[2].second, 2);
}
