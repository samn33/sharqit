#include "sharqit/sharqit.h"

int main()
{
  Sharqit::QCirc qc_in;
  qc_in.h(3).h(4).cx(1,4).tdg(4).cx(0,4).t(4).cx(1,4).t(1).tdg(4);
  qc_in.cx(0,4).cx(0,1).t(4).t(0).tdg(1).cx(4,3).cx(0,1).tdg(3);
  qc_in.cx(2,3).t(3).cx(4,3).tdg(3).t(4).cx(2,3).cx(2,4).t(2).t(3);
  qc_in.tdg(4).cx(2,4).h(4).cx(1,4).tdg(4).cx(0,4).t(4).cx(1,4).t(1);
  qc_in.tdg(4).cx(0,4).cx(0,1).t(4).t(0).tdg(1).cx(0,1).h(3).h(4);
  
  qc_in.show();
  qc_in.save("in.sqc");
  std::cout << "T-count (in) = " << qc_in.t_count() << std::endl;

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_out = opt.execute(qc_in);

  qc_out.show();
  qc_out.save("out.sqc");
  std::cout << "T-count (out) = " << qc_out.t_count() << std::endl;

  return 0;
}
