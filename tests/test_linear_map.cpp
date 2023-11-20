#include <gtest/gtest.h>

#include "sharq.h"

TEST(LinearMap, Constructor01) {
  Sharq::LinearMap lmap;
  EXPECT_EQ(lmap.row_num(), 0);
  EXPECT_EQ(lmap.col_num(), 0);
}

TEST(LinearMap, Constructor02) {
  Sharq::LinearMap lmap(2, 2);

  EXPECT_EQ(lmap.row_num(), 2);
  EXPECT_EQ(lmap.col_num(), 2);

  EXPECT_EQ(lmap(0, 0), 1.0);
  EXPECT_EQ(lmap(0, 1), 0.0);
  EXPECT_EQ(lmap(1, 0), 0.0);
  EXPECT_EQ(lmap(1, 1), 1.0);
}

TEST(LinearMap, Constructor03) {
  Sharq::LinearMap lmap({{1.0, 2.0}, {3.0, 4.0}});

  EXPECT_EQ(lmap(0, 0), 1.0);
  EXPECT_EQ(lmap(0, 1), 2.0);
  EXPECT_EQ(lmap(1, 0), 3.0);
  EXPECT_EQ(lmap(1, 1), 4.0);
}

TEST(LinearMap, Substitution) {
  Sharq::LinearMap lmap(2, 2);
  lmap.coeffRef(0, 0) = 10.0;
  lmap.coeffRef(0, 1) = 20.0;
  lmap.coeffRef(1, 0) = 30.0;
  lmap.coeffRef(1, 1) = 40.0;
  
  EXPECT_EQ(lmap(0, 0), 10.0);
  EXPECT_EQ(lmap(0, 1), 20.0);
  EXPECT_EQ(lmap(1, 0), 30.0);
  EXPECT_EQ(lmap(1, 1), 40.0);
}

TEST(LinearMap, Add01) {
  Sharq::LinearMap lmap_A({{10.0, 20.0},
			   {30.0, 40.0}});
  Sharq::LinearMap lmap_B({{1.0, 2.0},
			   {3.0, 4.0}});
  Sharq::LinearMap lmap_C = lmap_A + lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 11.0);
  EXPECT_EQ(lmap_C(0, 1), 22.0);
  EXPECT_EQ(lmap_C(1, 0), 33.0);
  EXPECT_EQ(lmap_C(1, 1), 44.0);
}

TEST(LinearMap, Add02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + I, 2.0 + I},
			   {3.0 + I, 4.0 + I}});
  Sharq::LinearMap lmap_C = lmap_A + lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 11.0 + 3.0 * I);
  EXPECT_EQ(lmap_C(0, 1), 22.0 + 3.0 * I);
  EXPECT_EQ(lmap_C(1, 0), 33.0 + 3.0 * I);
  EXPECT_EQ(lmap_C(1, 1), 44.0 + 3.0 * I);
}

TEST(LinearMap, Add03) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + I, 2.0 + I},
			   {3.0 + I, 4.0 + I}});
  lmap_A += lmap_B;

  EXPECT_EQ(lmap_A(0, 0), 11.0 + 3.0 * I);
  EXPECT_EQ(lmap_A(0, 1), 22.0 + 3.0 * I);
  EXPECT_EQ(lmap_A(1, 0), 33.0 + 3.0 * I);
  EXPECT_EQ(lmap_A(1, 1), 44.0 + 3.0 * I);
}

TEST(LinearMap, Sub01) {
  Sharq::LinearMap lmap_A({{10.0, 20.0},
			   {30.0, 40.0}});
  Sharq::LinearMap lmap_B({{1.0, 2.0},
			   {3.0, 4.0}});
  Sharq::LinearMap lmap_C = lmap_A - lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 9.0);
  EXPECT_EQ(lmap_C(0, 1), 18.0);
  EXPECT_EQ(lmap_C(1, 0), 27.0);
  EXPECT_EQ(lmap_C(1, 1), 36.0);
}

TEST(LinearMap, Sub02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + I, 2.0 + I},
			   {3.0 + I, 4.0 + I}});
  Sharq::LinearMap lmap_C = lmap_A - lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 9.0 + I);
  EXPECT_EQ(lmap_C(0, 1), 18.0 + I);
  EXPECT_EQ(lmap_C(1, 0), 27.0 + I);
  EXPECT_EQ(lmap_C(1, 1), 36.0 + I);
}

TEST(LinearMap, Sub03) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + I, 2.0 + I},
			   {3.0 + I, 4.0 + I}});
  lmap_A -= lmap_B;

  EXPECT_EQ(lmap_A(0, 0), 9.0 + I);
  EXPECT_EQ(lmap_A(0, 1), 18.0 + I);
  EXPECT_EQ(lmap_A(1, 0), 27.0 + I);
  EXPECT_EQ(lmap_A(1, 1), 36.0 + I);
}

TEST(LinearMap, Mul01) {
  Sharq::LinearMap lmap_A({{10.0, 20.0},
			   {30.0, 40.0}});
  Sharq::LinearMap lmap_B({{1.0, 2.0},
			   {3.0, 4.0}});
  Sharq::LinearMap lmap_C = lmap_A * lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 70.0);
  EXPECT_EQ(lmap_C(0, 1), 100.0);
  EXPECT_EQ(lmap_C(1, 0), 150.0);
  EXPECT_EQ(lmap_C(1, 1), 220.0);
}

TEST(LinearMap, Mul02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + I, 2.0 + I},
			   {3.0 + I, 4.0 + I}});
  Sharq::LinearMap lmap_C = lmap_A * lmap_B;

  EXPECT_EQ(lmap_C(0, 0), 66.0 + 38.0 * I);
  EXPECT_EQ(lmap_C(0, 1), 96.0 + 42.0 * I);
  EXPECT_EQ(lmap_C(1, 0), 146.0 + 78.0 * I);
  EXPECT_EQ(lmap_C(1, 1), 216.0 + 82.0 * I);
}

TEST(LinearMap, Mul03) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  std::complex<double> B(1.0 - 2.0 * I);
  Sharq::LinearMap lmap_C = lmap_A * B;

  EXPECT_EQ(lmap_C(0, 0), 14.0 - 18.0 * I);
  EXPECT_EQ(lmap_C(0, 1), 24.0 - 38.0 * I);
  EXPECT_EQ(lmap_C(1, 0), 34.0 - 58.0 * I);
  EXPECT_EQ(lmap_C(1, 1), 44.0 - 78.0 * I);
}

TEST(LinearMap, Mul04) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  std::complex<double> B(1.0 - 2.0 * I);
  Sharq::LinearMap lmap_C = B * lmap_A;

  EXPECT_EQ(lmap_C(0, 0), 14.0 - 18.0 * I);
  EXPECT_EQ(lmap_C(0, 1), 24.0 - 38.0 * I);
  EXPECT_EQ(lmap_C(1, 0), 34.0 - 58.0 * I);
  EXPECT_EQ(lmap_C(1, 1), 44.0 - 78.0 * I);
}

TEST(LinearMap, Mul05) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  std::complex<double> B(1.0 - 2.0 * I);
  lmap_A *= B;

  EXPECT_EQ(lmap_A(0, 0), 14.0 - 18.0 * I);
  EXPECT_EQ(lmap_A(0, 1), 24.0 - 38.0 * I);
  EXPECT_EQ(lmap_A(1, 0), 34.0 - 58.0 * I);
  EXPECT_EQ(lmap_A(1, 1), 44.0 - 78.0 * I);
}

TEST(LinearMap, Div01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{10.0 + 2.0 * I, 20.0 + 2.0 * I},
			   {30.0 + 2.0 * I, 40.0 + 2.0 * I}});
  std::complex<double> B(1.0 - 2.0 * I);
  Sharq::LinearMap lmap_C = lmap_A / B;

  EXPECT_EQ(lmap_C(0, 0), 1.2 + 4.4 * I);
  EXPECT_EQ(lmap_C(0, 1), 3.2 + 8.4 * I);
  EXPECT_EQ(lmap_C(1, 0), 5.2 + 12.4 * I);
  EXPECT_EQ(lmap_C(1, 1), 7.2 + 16.4 * I);
}

TEST(LinearMap, IsZero01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{-1.0 - 2.0 * I, 1.0e-9 + 1.0e-9 * I},
			 {-3.0 - 2.0 * I, -4.0 - 2.0 * I}});

  EXPECT_EQ(lmap.is_zero(), false);
}

TEST(LinearMap, IsZero02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0e-9 + 1.0e-9 * I, 0.0},
			 {0.0, -1.0e-9 - 1.0e-9 * I}});

  EXPECT_EQ(lmap.is_zero(), true);
}

TEST(LinearMap, IsIdentity01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 1.0e-9 * I, -1.0e-9 * I},
			 {0.0, 1.0}});

  EXPECT_EQ(lmap.is_identity(), true);
}

TEST(LinearMap, IsIdentity02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{10.0 + 1.0e-9 * I, -1.0e-9 * I},
			 {0.0, 10.0}});

  EXPECT_EQ(lmap.is_identity(), false);
}

TEST(LinearMap, IsIdentity03) {
  Sharq::LinearMap lmap(2, 3);
  EXPECT_EQ(lmap.is_identity(), false);
}

TEST(LinearMap, IsEqual01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{1.0 + 2.0 * I, 2.0 + 3.0 * I},
			   {4.0 + 5.0 * I, 6.0 + 7.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + 2.0 * I, 2.0 + 3.0 * I},
			   {4.0 + 5.0 * I, 6.0 + 7.0 * I}});

  EXPECT_EQ(lmap_A.is_equal(lmap_B), true);
}

TEST(LinearMap, IsEqual02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap_A({{1.0 + 2.0 * I, 2.0 + 3.0 * I},
			   {4.1 + 5.0 * I, 6.0 + 7.0 * I}});
  Sharq::LinearMap lmap_B({{1.0 + 2.0 * I, 2.0 + 3.0 * I},
			   {4.0 + 5.0 * I, 6.0 + 7.0 * I}});

  EXPECT_EQ(lmap_A.is_equal(lmap_B), false);
}

TEST(LinearMap, IsDiagonal01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 2.0 * I, 0.0},
			   {0.0, 6.0 + 7.0 * I}});
  EXPECT_EQ(lmap.is_diagonal(), true);
}

TEST(LinearMap, IsDiagonal02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 2.0 * I, -5.0},
			   {0.0, 6.0 + 7.0 * I}});
  EXPECT_EQ(lmap.is_diagonal(), false);
}

TEST(LinearMap, IsIdentityMultipleConstant01) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 2.0 * I, 0.0},
			 {0.0, 1.0 + 2.0 * I}});
  EXPECT_EQ(lmap.is_identity_multiple_constant(), true);
}

TEST(LinearMap, IsIdentityMultipleConstant02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{2.0 + 2.0 * I, 0.0},
			 {0.0, 1.0 + 2.0 * I}});
  EXPECT_EQ(lmap.is_identity_multiple_constant(), false);
}

TEST(LinearMap, IsIdentityMultipleConstant03) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 2.0 * I, 3.0},
			 {0.0, 1.0 + 2.0 * I}});
  EXPECT_EQ(lmap.is_identity_multiple_constant(), false);
}

TEST(LinearMap, IsUnitary01) {
  Sharq::QCirc qc;
  qc.h(0);
  qc.cx(0,1);
  Sharq::LinearMap lmap = qc.to_linearmap();
  EXPECT_EQ(lmap.is_unitary(), true);
}

TEST(LinearMap, IsUnitary02) {
  std::complex<double> I(0.0, 1.0);
  Sharq::Phase PI = Sharq::Phase("PI");
  Sharq::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rx(0, PI/4);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);
  Sharq::LinearMap lmap = qc.to_linearmap();
  EXPECT_EQ(lmap.is_unitary(), true);
}

TEST(LinearMap, IsUnitary03) {
  std::complex<double> I(0.0, 1.0);
  Sharq::LinearMap lmap({{1.0 + 2.0 * I, 3.0},
			 {0.0, 1.0 + 2.0 * I}});
  EXPECT_EQ(lmap.is_unitary(), false);
}

TEST(LinearMap, OperateQgate01) {

  Sharq::QCirc qc;
  qc.h(0);
  qc.cx(0,1);
  Sharq::LinearMap lmap = qc.to_linearmap();

  std::complex<double> sqrt2_inv = 1.0 / std::sqrt(2.0);

  EXPECT_EQ(lmap(0, 0), sqrt2_inv);
  EXPECT_EQ(lmap(0, 1), 0.0);
  EXPECT_EQ(lmap(0, 2), sqrt2_inv);
  EXPECT_EQ(lmap(0, 3), 0.0);
  EXPECT_EQ(lmap(1, 0), 0.0);
  EXPECT_EQ(lmap(1, 1), sqrt2_inv);
  EXPECT_EQ(lmap(1, 2), 0.0);
  EXPECT_EQ(lmap(1, 3), sqrt2_inv);
  EXPECT_EQ(lmap(2, 0), 0.0);
  EXPECT_EQ(lmap(2, 1), sqrt2_inv);
  EXPECT_EQ(lmap(2, 2), 0.0);
  EXPECT_EQ(lmap(2, 3), -sqrt2_inv);
  EXPECT_EQ(lmap(3, 0), sqrt2_inv);
  EXPECT_EQ(lmap(3, 1), 0.0);
  EXPECT_EQ(lmap(3, 2), -sqrt2_inv);
  EXPECT_EQ(lmap(3, 3), 0.0);
}

