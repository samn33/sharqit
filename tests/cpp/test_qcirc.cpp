#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(QCirc, Reverse) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);

  Sharqit::QCirc qc_rev = qc.reverse();
  EXPECT_EQ(3, qc_rev.qubit_num());
  EXPECT_EQ("X 0", qc_rev.qgates()[8].to_string());
  EXPECT_EQ("Z 0", qc_rev.qgates()[7].to_string());
  EXPECT_EQ("S 1", qc_rev.qgates()[6].to_string());
  EXPECT_EQ("S+ 2", qc_rev.qgates()[5].to_string());
  EXPECT_EQ("T 1", qc_rev.qgates()[4].to_string());
  EXPECT_EQ("T+ 2", qc_rev.qgates()[3].to_string());
  EXPECT_EQ("RZ(3/4) 1", qc_rev.qgates()[2].to_string(false));
  EXPECT_EQ("CX 0 2", qc_rev.qgates()[1].to_string());
  EXPECT_EQ("H 1", qc_rev.qgates()[0].to_string());
}

TEST(QCirc, Inverse) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);

  Sharqit::QCirc qc_inv = qc.inverse();
  EXPECT_EQ(3, qc_inv.qubit_num());
  EXPECT_EQ("X 0", qc_inv.qgates()[8].to_string());
  EXPECT_EQ("Z 0", qc_inv.qgates()[7].to_string());
  EXPECT_EQ("S+ 1", qc_inv.qgates()[6].to_string());
  EXPECT_EQ("S 2", qc_inv.qgates()[5].to_string());
  EXPECT_EQ("T+ 1", qc_inv.qgates()[4].to_string());
  EXPECT_EQ("T 2", qc_inv.qgates()[3].to_string());
  EXPECT_EQ("RZ(5/4) 1", qc_inv.qgates()[2].to_string(false));
  EXPECT_EQ("CX 0 2", qc_inv.qgates()[1].to_string());
  EXPECT_EQ("H 1", qc_inv.qgates()[0].to_string());
}

TEST(QCirc, AddQGate) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);

  EXPECT_EQ(3, qc.qubit_num());
  EXPECT_EQ("X 0", qc.qgates()[0].to_string());
  EXPECT_EQ("Z 0", qc.qgates()[1].to_string());
  EXPECT_EQ("S 1", qc.qgates()[2].to_string());
  EXPECT_EQ("S+ 2", qc.qgates()[3].to_string());
  EXPECT_EQ("T 1", qc.qgates()[4].to_string());
  EXPECT_EQ("T+ 2", qc.qgates()[5].to_string());
  EXPECT_EQ("RZ(3/4) 1", qc.qgates()[6].to_string(false));
  EXPECT_EQ("CX 0 2", qc.qgates()[7].to_string());
  EXPECT_EQ("H 1", qc.qgates()[8].to_string());
}

TEST(QCirc, Identical01) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc_A;
  qc_A.x(0);
  qc_A.z(0);
  qc_A.s(1);
  qc_A.sdg(2);
  qc_A.t(1);
  qc_A.tdg(2);
  qc_A.rx(0, PI/4);
  qc_A.rz(1, 3*PI/4);
  qc_A.cx(0,2);
  qc_A.h(1);

  Sharqit::QCirc qc_B;
  qc_B.x(0);
  qc_B.z(0);
  qc_B.s(1);
  qc_B.sdg(2);
  qc_B.t(1);
  qc_B.tdg(2);
  qc_B.rx(0, PI/4);
  qc_B.rz(1, 3*PI/4);
  qc_B.cx(0,2);
  qc_B.h(1);

  EXPECT_EQ(qc_A.is_identical(qc_B), true);
}

TEST(QCirc, Identical02) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc_A;
  qc_A.x(0);
  qc_A.z(0);
  qc_A.s(1);
  qc_A.sdg(2);
  qc_A.t(1);
  qc_A.tdg(2);
  qc_A.rx(0, PI/4);
  qc_A.rz(1, 3*PI/4);
  qc_A.cx(0,2);
  qc_A.h(1);

  Sharqit::QCirc qc_B;
  qc_B.x(0);
  qc_B.z(0);
  qc_B.s(1);
  qc_B.sdg(2);
  qc_B.t(1);
  qc_B.tdg(2);
  qc_B.rx(0, 2*PI/4);
  qc_B.rz(1, 3*PI/4);
  qc_B.cx(0,2);
  qc_B.h(1);

  EXPECT_EQ(qc_A.is_identical(qc_B), false);
}

TEST(QCirc, Equal01) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc_A;
  qc_A.x(0);
  qc_A.z(0);
  qc_A.s(1);
  qc_A.sdg(2);
  qc_A.t(1);
  qc_A.tdg(2);
  qc_A.rx(0, PI/4);
  qc_A.rz(1, 3*PI/4);
  qc_A.cx(0,2);
  qc_A.h(1);

  Sharqit::QCirc qc_B;
  qc_B.x(0);
  qc_B.z(0);
  qc_B.s(1);
  qc_B.sdg(2);
  qc_B.t(1);
  qc_B.tdg(2);
  qc_B.rx(0, PI/4);
  qc_B.rz(1, 3*PI/4);
  qc_B.cx(0,2);
  qc_B.h(1);

  EXPECT_EQ(qc_A.is_equal(qc_B), true);
}

TEST(QCirc, Equal02) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc_A;
  qc_A.x(0);
  qc_A.z(0);
  qc_A.s(1);
  qc_A.sdg(2);
  qc_A.t(1);
  qc_A.tdg(2);
  qc_A.rx(0, PI/4);
  qc_A.rz(1, 3*PI/4);
  qc_A.cx(0,2);
  qc_A.h(1);

  Sharqit::QCirc qc_B;
  qc_B.x(0);
  qc_B.z(0);
  qc_B.s(1);
  qc_B.sdg(2);
  qc_B.t(1);
  qc_B.tdg(2);
  qc_B.rx(0, 2*PI/4);
  qc_B.rz(1, 3*PI/4);
  qc_B.cx(0,2);
  qc_B.h(1);

  EXPECT_EQ(qc_A.is_equal(qc_B), false);
}

TEST(QCirc, Equal03) {
  Sharqit::QCirc qc_A;
  qc_A.cx(1, 0);

  Sharqit::QCirc qc_B;
  qc_B.h(0).h(1);
  qc_B.cx(0, 1);
  qc_B.h(0).h(1);

  EXPECT_EQ(qc_A.is_equal(qc_B), true);
}

TEST(QCirc, ToString) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rz(0, PI/4);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);
  std::string s = "";
  s += "q[0] --X--Z--RZ(1/4)--*-----\n";
  s += "q[1] --S--T--RZ(3/4)--|--H--\n";
  s += "q[2] --S+--T+---------X-----\n";
  EXPECT_EQ(s, qc.to_string());

  s = "";
  s += "q[0] --X--Z--RZ(1/4) ...\n";
  s += "q[1] --S--T--RZ(3/4) ...\n";
  s += "q[2] --S+--T+------- ...\n";
  s += "\n";
  s += "--*-----\n";
  s += "--|--H--\n";
  s += "--X-----\n";
  EXPECT_EQ(s, qc.to_string(20));
}

TEST(QCirc, AddQcirc) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc_A;
  qc_A.x(0);
  qc_A.z(0);
  qc_A.s(1);
  qc_A.sdg(2);
  qc_A.t(1);
  qc_A.tdg(2);
  Sharqit::QCirc qc_B;
  qc_B.rz(0, PI/4);
  qc_B.rz(1, 3*PI/4);
  qc_B.cx(0,2);
  qc_B.h(1);
  Sharqit::QCirc qc = qc_A + qc_B;
  std::string s = "";
  s += "q[0] --X--Z--RZ(1/4)--*-----\n";
  s += "q[1] --S--T--RZ(3/4)--|--H--\n";
  s += "q[2] --S+--T+---------X-----\n";
  EXPECT_EQ(s, qc.to_string());

  qc_A += qc_B;
  EXPECT_EQ(s, qc_A.to_string());
}

TEST(QCirc, ToDotFile) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::QCirc qc;
  qc.x(0);
  qc.z(0);
  qc.s(1);
  qc.sdg(2);
  qc.t(1);
  qc.tdg(2);
  qc.rz(0, PI/4);
  qc.rz(1, 3*PI/4);
  qc.cx(0,2);
  qc.h(1);
  qc.to_dot_file("../data/qcirc_test.dot");
  int ret = system("diff ../data/qcirc.dot ../data/qcirc_test.dot");
  EXPECT_EQ(0, ret);
}

/* fundamental gates */

TEST(QCirc, X) {
  Sharqit::QCirc qc;
  qc.x(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{0.0, 1.0},
				{1.0, 0.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Z) {
  Sharqit::QCirc qc;
  qc.z(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, -1.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, S) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.s(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, I}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Sdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.sdg(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, -I}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, T) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.t(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, std::exp(I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Tdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.tdg(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, std::exp(-I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, H) {
  Sharqit::QCirc qc;
  qc.h(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)},
				{1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Rx) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.rx(0, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0/std::sqrt(2.0), -I/std::sqrt(2.0)},
				{-I/std::sqrt(2.0), 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Rz) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.rz(0, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{std::exp(-I*M_PI/4.0), 0.0},
				{0.0, std::exp(I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CX) {
  Sharqit::QCirc qc;
  qc.cx(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 1.0},
				{0.0, 0.0, 1.0, 0.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

/* compound gates */

TEST(QCirc, Y) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.y(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{0.0, -I},
				{I, 0.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) -1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, SX) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.sx(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{I, 1.0},
				{1.0, I}});
  lmap_expect *= (1.0 - I)/2.0;
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) -1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, SXdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.sxdg(0);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{-I, 1.0},
				{1.0, -I}});
  lmap_expect *= (1.0 + I)/2.0;
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) -1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Ry) {
  Sharqit::QCirc qc;
  qc.ry(0, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)},
				{1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, P) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.p(0, Sharqit::Phase(1,4));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0},
				{0.0, std::exp(I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CY) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.cy(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, -I},
				{0.0, 0.0, I, 0.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CZ) {
  Sharqit::QCirc qc;
  qc.cz(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, -1.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CSX) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.csx(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, I*(1.0-I)/2.0, (1.0-I)/2.0},
				{0.0, 0.0, (1.0-I)/2.0, I*(1.0-I)/2.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CSXdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.csxdg(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, -I*(1.0+I)/2.0, (1.0+I)/2.0},
				{0.0, 0.0, (1.0+I)/2.0, -I*(1.0+I)/2.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CH) {
  Sharqit::QCirc qc;
  qc.ch(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)},
				{0.0, 0.0, 1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CS) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.cs(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, I}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CSdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.csdg(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, -I}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CT) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.ct(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, std::exp(I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CTdg) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.ctdg(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, std::exp(-I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CRx) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.crx(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0/std::sqrt(2.0), -I/std::sqrt(2.0)},
				{0.0, 0.0, -I/std::sqrt(2.0), 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CRy) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.cry(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)},
				{0.0, 0.0, 1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CRz) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.crz(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, std::exp(-I*M_PI/4.0), 0.0},
				{0.0, 0.0, 0.0, std::exp(I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CP) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.cp(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, std::exp(I*M_PI/2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Rxx) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.rxx(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0/std::sqrt(2.0), 0.0, 0.0, -I/std::sqrt(2.0)},
				{0.0, 1.0/std::sqrt(2.0), -I/std::sqrt(2.0), 0.0},
				{0.0, -I/std::sqrt(2.0), 1.0/std::sqrt(2.0), 0.0},
				{-I/std::sqrt(2.0), 0.0, 0.0, 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Ryy) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.ryy(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0/std::sqrt(2.0), 0.0, 0.0, I/std::sqrt(2.0)},
				{0.0, 1.0/std::sqrt(2.0), -I/std::sqrt(2.0), 0.0},
				{0.0, -I/std::sqrt(2.0), 1.0/std::sqrt(2.0), 0.0},
				{I/std::sqrt(2.0), 0.0, 0.0, 1.0/std::sqrt(2.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, Rzz) {
  std::complex<double> I(0.0, 1.0);
  Sharqit::QCirc qc;
  qc.rzz(0, 1, Sharqit::Phase(1,2));
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{std::exp(-I*M_PI/4.0), 0.0, 0.0, 0.0},
				{0.0, std::exp(I*M_PI/4.0), 0.0, 0.0},
				{0.0, 0.0, std::exp(I*M_PI/4.0), 0.0},
				{0.0, 0.0, 0.0, std::exp(-I*M_PI/4.0)}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, SW) {
  Sharqit::QCirc qc;
  qc.sw(0, 1);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0},
				{0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 1.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CSW) {
  Sharqit::QCirc qc;
  qc.csw(0, 1, 2);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, CCX) {
  Sharqit::QCirc qc;
  qc.ccx(0, 1, 2);
  Sharqit::LinearMap lmap_actual = qc.to_linearmap();
  Sharqit::LinearMap lmap_expect({{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
				{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}});
  lmap_expect.adjointInPlace();
  Sharqit::LinearMap lmap = lmap_expect * lmap_actual;
  EXPECT_EQ((std::abs(std::abs(lmap(0,0)) - 1.0) < Sharqit::EPS) && lmap.is_identity_multiple_constant(), true);
}

TEST(QCirc, SaveLoad) {
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_A;
  qc_A.h(0).x(0).z(1).cx(0,1).s(0).t(1).rz(0,PI/4).rx(1,-PI/4).cz(0,1).sdg(1).tdg(1);
  qc_A.save("hoge.qc");

  Sharqit::QCirc qc_B;
  qc_B.load("hoge.qc");

  int ret = system("rm -f hoge.qc");

  EXPECT_EQ(qc_A.is_identical(qc_B), true);
}

TEST(QCirc, Stats) {
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc;

  qc.x(0).z(1).s(0).sdg(1).t(0).tdg(1).h(2).rz(0,PI/2).id(3).rz(1,PI/4);
  std::map<std::string, uint32_t> sts = qc.stats();

  EXPECT_EQ(sts["id_count"], 1);
  EXPECT_EQ(sts["x_count"], 1);
  EXPECT_EQ(sts["z_count"], 1);
  EXPECT_EQ(sts["h_count"], 1);
  EXPECT_EQ(sts["s_count"], 3);
  EXPECT_EQ(sts["t_count"], 3);
  EXPECT_EQ(sts["rz_count"], 8);
  EXPECT_EQ(sts["gate_count"], 9);
  EXPECT_EQ(sts["depth"], 4);
  EXPECT_EQ(sts["qubit_num"], 4);
}

TEST(QCirc, Stats00) {
  Sharqit::QCirc qc;

  qc.load("../data/00.sqc");
  std::map<std::string, uint32_t> sts = qc.stats();

  EXPECT_EQ(sts["id_count"], 0);
  EXPECT_EQ(sts["x_count"], 2);
  EXPECT_EQ(sts["z_count"], 0);
  EXPECT_EQ(sts["h_count"], 10);
  EXPECT_EQ(sts["s_count"], 7);
  EXPECT_EQ(sts["t_count"], 2);
  EXPECT_EQ(sts["rz_count"], 9);
  EXPECT_EQ(sts["gate_count"], 26);
  EXPECT_EQ(sts["depth"], 11);
}
