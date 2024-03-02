#include "sharqit/sharqit.h"

void basic_optimization(const std::string& fin) {

  try {
    Sharqit::QCirc qc_in;
    qc_in.load(fin);
    std::cout << "file : " << fin << std::endl;
    qc_in.show();

    Sharqit::QCirc qc_out = qc_in;

    qc_out.gate_cancel();

    std::cerr << "T-Count:    " << qc_in.t_count() << " --> "<< qc_out.t_count() << std::endl;
    std::cerr << "CNOT-Count: " << qc_in.cx_count() << " --> "<< qc_out.cx_count() << std::endl;
    std::cerr << "Gate-Count: " << qc_in.gate_count() << " --> "<< qc_out.gate_count() << std::endl;
    std::cerr << "depth: " << qc_in.depth() << " --> "<< qc_out.depth() << std::endl;
    //std::cout << "equal? " << qc_in.is_equal(qc_out) << std::endl;

    qc_out.show();
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

int main(int argc, char** argv)
{
  std::string fin = argv[1];
  basic_optimization(fin);

  return 0;
}
