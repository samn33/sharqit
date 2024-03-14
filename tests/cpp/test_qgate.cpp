#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

TEST(QGate, X) {
  Sharqit::QGate qgate(Sharqit::QGateKind::X, {1});
  EXPECT_EQ(Sharqit::QGateKind::X, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("X", qgate.name(true));
  EXPECT_EQ("X", qgate.name(false));
  EXPECT_EQ("X 1", qgate.to_string(true));
  EXPECT_EQ("X 1", qgate.to_string(false));
}

TEST(QGate, Z) {
  Sharqit::QGate qgate(Sharqit::QGateKind::Z, {1});
  EXPECT_EQ(Sharqit::QGateKind::Z, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("Z", qgate.name(true));
  EXPECT_EQ("Z", qgate.name(false));
  EXPECT_EQ("Z 1", qgate.to_string(true));
  EXPECT_EQ("Z 1", qgate.to_string(false));
}

TEST(QGate, S) {
  Sharqit::QGate qgate(Sharqit::QGateKind::S, {1});
  EXPECT_EQ(Sharqit::QGateKind::S, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("S", qgate.name(true));
  EXPECT_EQ("S", qgate.name(false));
  EXPECT_EQ("S 1", qgate.to_string(true));
  EXPECT_EQ("S 1", qgate.to_string(false));
}

TEST(QGate, Sdg) {
  Sharqit::QGate qgate(Sharqit::QGateKind::Sdg, {1});
  EXPECT_EQ(Sharqit::QGateKind::Sdg, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("S+", qgate.name(true));
  EXPECT_EQ("S+", qgate.name(false));
  EXPECT_EQ("S+ 1", qgate.to_string(true));
  EXPECT_EQ("S+ 1", qgate.to_string(false));
}

TEST(QGate, T) {
  Sharqit::QGate qgate(Sharqit::QGateKind::T, {1});
  EXPECT_EQ(Sharqit::QGateKind::T, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("T", qgate.name(true));
  EXPECT_EQ("T", qgate.name(false));
  EXPECT_EQ("T 1", qgate.to_string(true));
  EXPECT_EQ("T 1", qgate.to_string(false));
}

TEST(QGate, Tdg) {
  Sharqit::QGate qgate(Sharqit::QGateKind::Tdg, {1});
  EXPECT_EQ(Sharqit::QGateKind::Tdg, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("T+", qgate.name(true));
  EXPECT_EQ("T+", qgate.name(false));
  EXPECT_EQ("T+ 1", qgate.to_string(true));
  EXPECT_EQ("T+ 1", qgate.to_string(false));
}

TEST(QGate, H) {
  Sharqit::QGate qgate(Sharqit::QGateKind::H, {1});
  EXPECT_EQ(Sharqit::QGateKind::H, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("H", qgate.name(true));
  EXPECT_EQ("H", qgate.name(false));
  EXPECT_EQ("H 1", qgate.to_string(true));
  EXPECT_EQ("H 1", qgate.to_string(false));
}

TEST(QGate, RZ) {
  Sharqit::Phase PI("PI");
  Sharqit::QGate qgate(Sharqit::QGateKind::RZ, {1}, 3*PI/4);
  EXPECT_EQ(Sharqit::QGateKind::RZ, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == 3*PI/4);
  EXPECT_EQ("RZ(3π/4)", qgate.name(true));
  EXPECT_EQ("RZ(3/4)", qgate.name(false));
  EXPECT_EQ("RZ(3π/4) 1", qgate.to_string(true));
  EXPECT_EQ("RZ(3/4) 1", qgate.to_string(false));
}

TEST(QGate, CX) {
  Sharqit::QGate qgate(Sharqit::QGateKind::CX, {1,3});
  EXPECT_EQ(Sharqit::QGateKind::CX, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(3, qgate.qid()[1]);
  EXPECT_EQ(true, qgate.phase() == Sharqit::Phase(0));
  EXPECT_EQ("CX", qgate.name(true));
  EXPECT_EQ("CX", qgate.name(false));
  EXPECT_EQ("CX 1 3", qgate.to_string(true));
  EXPECT_EQ("CX 1 3", qgate.to_string(false));
}
