#include <gtest/gtest.h>

#include "sharq.h"

#define OPTIMIZER_KIND Sharq::OptimizerKind::ZXCalculus

TEST(OptimizerZX, Execute01) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).cx(0,1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute02) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).cx(0,1).x(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute03) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute04) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);
  qc_in.s(1);
  qc_in.h(3);
  qc_in.cx(1,2);
  qc_in.cz(0,3);
  qc_in.h(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute05) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
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

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute06) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
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

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute07) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
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

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute08) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).h(0).cx(0,1).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute09) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).h(1).t(0).t(1).x(0).cx(0,1).s(0).cx(1,0).t(1).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute10) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).x(1).h(0).cx(1,0).h(0).x(1).x(0).s(1).cx(1,0).h(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute11) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.s(0).s(1).x(0).t(1).h(0).t(1).x(0).s(1).s(0).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute12) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(1,0).cx(1,0).h(0).h(1).cx(0,1).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute13) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).h(0).h(1).cx(0,1).cx(1,0).cx(0,1).s(1).cx(0,1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute14) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).t(1).s(0).x(1).t(1).cx(1,0).h(1).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute15) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(0,1).h(0).t(0).cx(1,0).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute16) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.x(0).h(1).h(0).s(1).cx(1,0).h(0).s(1).x(0).s(1).s(0).s(1).cx(1,0).s(0).s(1).s(0).h(0).cx(1,0).t(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, Execute17) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).s(1).s(2).h(1).s(1).h(1).cx(2,1).z(0).h(1).cx(0,2).x(0).t(1).x(2);
  qc_in.h(2).cx(0,1).h(0).t(1).h(2).h(1).t(2).s(1).s(2).t(1).h(2).t(2).h(2).cx(0,1).cx(1,0);
  qc_in.t(0).s(1).t(2).s(0).h(0).t(0).cx(1,0).cx(2,1).h(0).h(1).s(0).cx(0,2).s(0).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute00) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/00.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute01) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/01.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute02) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/02.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute03) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/03.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute04) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/04.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute05) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/05.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute06) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/06.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute07) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/07.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute08) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/08.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute09) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/09.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute10) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/10.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute11) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/11.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute12) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/12.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute13) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/13.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute14) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/14.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute15) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/15.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute16) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/16.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute17) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/17.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute18) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/18.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute19) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/19.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute20) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/20.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute21) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/21.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute22) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/22.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute23) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/23.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerZX, LoadExecute24) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/24.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}
