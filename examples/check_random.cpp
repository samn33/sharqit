#include "sharq.h"

bool test_random(int qubit_num, int gate_num) {

  try {
    Sharq::Optimizer opt;
    Sharq::QCirc qc_in;
    qc_in.add_random((uint32_t)qubit_num, (uint32_t)gate_num,
		     {{"X", 1},
		      {"Z", 1},
		      {"H", 1},
		      {"S", 1},
		      {"T", 1},
		      {"CX", 1},
		      {"CZ", 1}});

    qc_in.save("sandbox/out.sqc");

    Sharq::QCirc qc_out = opt.execute(qc_in);
    std::cout << "qgate_num = " << qc_out.qgate_num() << std::endl;
    std::cout << "equal? " << qc_in.is_equal(qc_out) << std::endl;
    if (!qc_in.is_equal(qc_out)) return false;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
    return false;
  }

  return true;
}

int main(int argc, char** argv)
{
  int trial = atoi(argv[1]);
  int qubit_num = atoi(argv[2]);
  int gate_num = atoi(argv[3]);
  for (int n = 0; n < trial; ++n) {
    std::cout << "** n = " << n << " **" << std::endl;
    if (test_random(qubit_num, gate_num) == false) break;
  }

  return 0;
}
