#include "sharq.h"

int main()
{
  Sharq::QCirc qc_in;
  // random circuit (qubit-num=5, gate-num=20, X:T:CX=4:5:3)
  qc_in.add_random(5, 20, {{"X", 4},{"T", 5},{"CX", 3}});

  qc_in.show(); // show the circuit
  qc_in.save("in.sqc"); // save to text file
  std::cout << "T-count (in) = " << qc_in.t_count() << std::endl;

  Sharq::Optimizer opt;
  Sharq::QCirc qc_out = opt.execute(qc_in); // circuit optimization

  qc_out.show(); // show the circuit
  qc_out.save("out.sqc"); // save to text file
  std::cout << "T-count (out) = " << qc_out.t_count() << std::endl;

  return 0;
}
