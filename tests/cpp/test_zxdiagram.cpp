#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(ZXDiagram, ToDotFile) {
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
  Sharqit::ZXDiagram zx = qc.to_zxdiagram();
  zx.to_dot_file("../data/zxdiagram_test.dot");
  int ret = system("diff ../data/zxdiagram.dot ../data/zxdiagram_test.dot");
  EXPECT_EQ(0, ret);
}

TEST(ZXDiagram, Stats) {
  Sharqit::Phase PI = Sharqit::Phase("PI");
  Sharqit::Optimizer opt;
  Sharqit::QCirc qc;
  qc.x(0).z(1).s(0).cz(1,2).sdg(1).h(0).t(0).tdg(1).h(2).rz(0,PI/2).rz(2,PI/4).cx(0,1);

  Sharqit::ZXDiagram zx = qc.to_zxdiagram();
  std::map<std::string, uint32_t> sts = zx.stats();

  EXPECT_EQ(sts["xspider"], 3);
  EXPECT_EQ(sts["zspider"], 9);
  EXPECT_EQ(sts["hadamard"], 2);
}
