#include "sharqit/sharqit.h"
    
int main()
{
  Sharqit::QCirc qc_in;
  qc_in.t(1);
  qc_in.h(0);
  qc_in.h(1);
  qc_in.cx(0,1);
  qc_in.h(0);
  qc_in.h(1);
  qc_in.tdg(1);
  qc_in.show();

  Sharqit::Optimizer opt;
  Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, "zx");
  qc_out.show();
    
  return 0;
}
