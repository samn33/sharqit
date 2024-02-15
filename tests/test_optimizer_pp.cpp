#include <gtest/gtest.h>

#include "sharq.h"

#define OPTIMIZER_KIND Sharq::OptimizerKind::PhasePolynomial

TEST(OptimizerPP, Execute01) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).cx(0,1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute02) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).cx(0,1).x(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute03) {

  Sharq::Optimizer opt;
  Sharq::Phase PI("PI");
  Sharq::QCirc qc_in;
  qc_in.rz(0, 3*PI/2);
  qc_in.h(0);
  qc_in.s(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute04) {

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

TEST(OptimizerPP, Execute05) {

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

TEST(OptimizerPP, Execute06) {

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

TEST(OptimizerPP, Execute07) {

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

TEST(OptimizerPP, Execute08) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).h(0).cx(0,1).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute09) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).h(1).t(0).t(1).x(0).cx(0,1).s(0).cx(1,0).t(1).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute10) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).x(1).h(0).cx(1,0).h(0).x(1).x(0).s(1).cx(1,0).h(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute11) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.s(0).s(1).x(0).t(1).h(0).t(1).x(0).s(1).s(0).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute12) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(1,0).cx(1,0).h(0).h(1).cx(0,1).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute13) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(1,0).h(0).h(1).cx(0,1).cx(1,0).cx(0,1).s(1).cx(0,1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute14) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).t(1).s(0).x(1).t(1).cx(1,0).h(1).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute15) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.cx(0,1).cx(0,1).cx(0,1).h(0).t(0).cx(1,0).cx(1,0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute16) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.x(0).h(1).h(0).s(1).cx(1,0).h(0).s(1).x(0).s(1).s(0).s(1).cx(1,0).s(0).s(1).s(0).h(0).cx(1,0).t(0);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, Execute17) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.h(0).s(1).s(2).h(1).s(1).h(1).cx(2,1).z(0).h(1).cx(0,2).x(0).t(1).x(2);
  qc_in.h(2).cx(0,1).h(0).t(1).h(2).h(1).t(2).s(1).s(2).t(1).h(2).t(2).h(2).cx(0,1).cx(1,0);
  qc_in.t(0).s(1).t(2).s(0).h(0).t(0).cx(1,0).cx(2,1).h(0).h(1).s(0).cx(0,2).s(0).s(1);

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute00) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/00.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute01) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/01.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute02) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/02.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute03) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/03.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute04) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/04.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute05) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/05.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute06) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/06.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute07) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/07.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute08) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/08.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute09) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/09.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute10) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/10.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute11) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/11.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute12) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/12.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute13) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/13.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute14) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/14.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute15) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/15.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute16) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/16.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute17) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/17.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute18) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/18.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute19) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/19.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute20) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/20.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute21) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/21.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute22) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/22.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute23) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/23.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}

TEST(OptimizerPP, LoadExecute24) {

  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load("data/24.sqc");

  Sharq::QCirc qc_out = opt.execute(qc_in, OPTIMIZER_KIND);
  
  EXPECT_EQ(qc_in.is_equal(qc_out), true);
}
