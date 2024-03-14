#include <gtest/gtest.h>

#include "sharqit/sharqit.h"

#define OPTIMIZER_METHOD "zx"

TEST(OptimizerZX, ReduceGates01) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.h(0).cx(0,1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates02) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(1,0).cx(0,1).x(0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates03) {

  Sharqit::Optimizer opt;
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates04) {

  Sharqit::Optimizer opt;
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);
  qc_in.s(1);
  qc_in.h(3);
  qc_in.cx(1,2);
  qc_in.cz(0,3);
  qc_in.h(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates05) {

  Sharqit::Optimizer opt;
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);
  qc_in.s(1);
  qc_in.h(3);
  qc_in.cx(1,2);
  qc_in.cz(0,3);
  qc_in.h(1);
  qc_in.cx(1,2);
  qc_in.cx(3,0);
  qc_in.h(0);
  qc_in.t(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates06) {

  Sharqit::Optimizer opt;
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);
  qc_in.s(1);
  qc_in.h(3);
  qc_in.cx(1,2);
  qc_in.cz(0,3);
  qc_in.h(1);
  qc_in.cx(1,2);
  qc_in.cx(3,0);
  qc_in.h(0);
  qc_in.t(1);
  qc_in.s(2);
  qc_in.h(3);
  qc_in.t(0);
  qc_in.h(1);
  qc_in.h(2);
  qc_in.rz(3, 3*PI/2);
  qc_in.s(2);
  qc_in.x(3);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates07) {

  Sharqit::Optimizer opt;
  Sharqit::Phase PI("PI");
  Sharqit::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);
  qc_in.s(1);
  qc_in.h(3);
  qc_in.cx(1,2);
  qc_in.cz(0,3);
  qc_in.h(1);
  qc_in.cx(1,2);
  qc_in.cx(3,0);
  qc_in.h(0);
  qc_in.t(1);
  qc_in.s(2);
  qc_in.h(3);
  qc_in.t(0);
  qc_in.h(1);
  qc_in.h(2);
  qc_in.rz(3, 3*PI/2);
  qc_in.s(2);
  qc_in.x(3);
  qc_in.cx(1,2);
  qc_in.h(0);
  qc_in.s(3);
  qc_in.x(3);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates08) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(1,0).h(0).cx(0,1).cx(1,0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates09) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.h(0).h(1).t(0).t(1).x(0).cx(0,1).s(0).cx(1,0).t(1).cx(1,0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates10) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.h(0).x(1).h(0).cx(1,0).h(0).x(1).x(0).s(1).cx(1,0).h(0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates11) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.s(0).s(1).x(0).t(1).h(0).t(1).x(0).s(1).s(0).s(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates12) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(1,0).cx(1,0).h(0).h(1).cx(0,1).s(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates13) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(1,0).h(0).h(1).cx(0,1).cx(1,0).cx(0,1).s(1).cx(0,1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates14) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(0,1).t(1).s(0).x(1).t(1).cx(1,0).h(1).s(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates15) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(0,1).h(0).t(0).cx(1,0).cx(1,0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates16) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.x(0).h(1).h(0).s(1).cx(1,0).h(0).s(1).x(0).s(1).s(0).s(1).cx(1,0).s(0).s(1).s(0).h(0).cx(1,0).t(0);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, ReduceGates17) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.h(0).s(1).s(2).h(1).s(1).h(1).cx(2,1).z(0).h(1).cx(0,2).x(0).t(1).x(2);
  qc_in.h(2).cx(0,1).h(0).t(1).h(2).h(1).t(2).s(1).s(2).t(1).h(2).t(2).h(2).cx(0,1).cx(1,0);
  qc_in.t(0).s(1).t(2).s(0).h(0).t(0).cx(1,0).cx(2,1).h(0).h(1).s(0).cx(0,2).s(0).s(1);

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates00) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/00.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates01) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/01.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates02) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/02.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates03) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/03.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates04) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/04.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates05) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/05.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates06) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/06.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates07) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/07.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates08) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/08.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates09) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/09.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates10) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/10.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates11) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/11.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates12) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/12.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates13) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/13.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates14) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/14.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates15) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/15.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates16) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/16.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates17) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/17.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates18) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/18.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates19) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/19.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates20) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/20.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates21) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/21.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates22) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/22.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates23) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/23.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadReduceGates24) {

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_in;
  qc_in.load("../data/24.sqc");

  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, OPTIMIZER_METHOD);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}
