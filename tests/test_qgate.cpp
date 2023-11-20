#include <gtest/gtest.h>

#include "sharq.h"

TEST(QGate, X) {
  Sharq::QGate qgate(Sharq::QGateKind::X, {1});
  EXPECT_EQ(Sharq::QGateKind::X, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("X", qgate.name(true));
  EXPECT_EQ("X", qgate.name(false));
  EXPECT_EQ("X 1", qgate.to_string(true));
  EXPECT_EQ("X 1", qgate.to_string(false));
}

TEST(QGate, Z) {
  Sharq::QGate qgate(Sharq::QGateKind::Z, {1});
  EXPECT_EQ(Sharq::QGateKind::Z, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("Z", qgate.name(true));
  EXPECT_EQ("Z", qgate.name(false));
  EXPECT_EQ("Z 1", qgate.to_string(true));
  EXPECT_EQ("Z 1", qgate.to_string(false));
}

TEST(QGate, S) {
  Sharq::QGate qgate(Sharq::QGateKind::S, {1});
  EXPECT_EQ(Sharq::QGateKind::S, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("S", qgate.name(true));
  EXPECT_EQ("S", qgate.name(false));
  EXPECT_EQ("S 1", qgate.to_string(true));
  EXPECT_EQ("S 1", qgate.to_string(false));
}

TEST(QGate, Sdg) {
  Sharq::QGate qgate(Sharq::QGateKind::Sdg, {1});
  EXPECT_EQ(Sharq::QGateKind::Sdg, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("S+", qgate.name(true));
  EXPECT_EQ("S+", qgate.name(false));
  EXPECT_EQ("S+ 1", qgate.to_string(true));
  EXPECT_EQ("S+ 1", qgate.to_string(false));
}

TEST(QGate, T) {
  Sharq::QGate qgate(Sharq::QGateKind::T, {1});
  EXPECT_EQ(Sharq::QGateKind::T, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("T", qgate.name(true));
  EXPECT_EQ("T", qgate.name(false));
  EXPECT_EQ("T 1", qgate.to_string(true));
  EXPECT_EQ("T 1", qgate.to_string(false));
}

TEST(QGate, Tdg) {
  Sharq::QGate qgate(Sharq::QGateKind::Tdg, {1});
  EXPECT_EQ(Sharq::QGateKind::Tdg, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("T+", qgate.name(true));
  EXPECT_EQ("T+", qgate.name(false));
  EXPECT_EQ("T+ 1", qgate.to_string(true));
  EXPECT_EQ("T+ 1", qgate.to_string(false));
}

TEST(QGate, H) {
  Sharq::QGate qgate(Sharq::QGateKind::H, {1});
  EXPECT_EQ(Sharq::QGateKind::H, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("H", qgate.name(true));
  EXPECT_EQ("H", qgate.name(false));
  EXPECT_EQ("H 1", qgate.to_string(true));
  EXPECT_EQ("H 1", qgate.to_string(false));
}

//TEST(QGate, RX) {
//  Sharq::Phase PI("PI");
//  Sharq::QGate qgate(Sharq::QGateKind::RX, {1}, 3*PI/4);
//  EXPECT_EQ(Sharq::QGateKind::RX, qgate.kind());
//  EXPECT_EQ(1, qgate.qid()[0]);
//  EXPECT_EQ(true, qgate.phase() == 3*PI/4);
//  EXPECT_EQ("RX(3π/4)", qgate.name(true));
//  EXPECT_EQ("RX(3/4)", qgate.name(false));
//  EXPECT_EQ("RX(3π/4) 1", qgate.to_string(true));
//  EXPECT_EQ("RX(3/4) 1", qgate.to_string(false));
//}

TEST(QGate, RZ) {
  Sharq::Phase PI("PI");
  Sharq::QGate qgate(Sharq::QGateKind::RZ, {1}, 3*PI/4);
  EXPECT_EQ(Sharq::QGateKind::RZ, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(true, qgate.phase() == 3*PI/4);
  EXPECT_EQ("RZ(3π/4)", qgate.name(true));
  EXPECT_EQ("RZ(3/4)", qgate.name(false));
  EXPECT_EQ("RZ(3π/4) 1", qgate.to_string(true));
  EXPECT_EQ("RZ(3/4) 1", qgate.to_string(false));
}

TEST(QGate, CX) {
  Sharq::QGate qgate(Sharq::QGateKind::CX, {1,3});
  EXPECT_EQ(Sharq::QGateKind::CX, qgate.kind());
  EXPECT_EQ(1, qgate.qid()[0]);
  EXPECT_EQ(3, qgate.qid()[1]);
  EXPECT_EQ(true, qgate.phase() == Sharq::Phase(0));
  EXPECT_EQ("CX", qgate.name(true));
  EXPECT_EQ("CX", qgate.name(false));
  EXPECT_EQ("CX 1 3", qgate.to_string(true));
  EXPECT_EQ("CX 1 3", qgate.to_string(false));
}

//TEST(QGate, KindPhase) {
//  std::string str = "RX(2*PI/3)";
//  std::tuple<Sharq::QGateKind, Sharq::Phase> kp = Sharq::QGate::kind_phase(str);
//  EXPECT_EQ(Sharq::QGateKind::RX, std::get<0>(kp));
//  EXPECT_EQ(true, Sharq::Phase(2,3) == std::get<1>(kp));
//  str = "RX(-2*PI/3)";
//  kp = Sharq::QGate::kind_phase(str);
//  EXPECT_EQ(Sharq::QGateKind::RX, std::get<0>(kp));
//  EXPECT_EQ(true, Sharq::Phase(-2,3) == std::get<1>(kp));
//}
