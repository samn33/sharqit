#include "sharqit/sharqit.h"

bool test_random(int qubit_num, int gate_num, std::string& mode) {

  try {
    Sharqit::Optimizer opt;
    Sharqit::QCirc qc_in;
    qc_in.add_random((uint32_t)qubit_num, (uint32_t)gate_num,
    		     {{"X", 1},
    		      {"Z", 1},
    		      {"H", 1},
    		      {"S", 1},
    		      {"T", 1},
    		      {"CX", 1},
    		      {"CZ", 1}});

    qc_in.save("sandbox/out.sqc");

    Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, mode);
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

bool test_random_str(int qubit_num, int gate_num, std::string& mode) {

  try {
    Sharqit::Optimizer opt;
    Sharqit::QCirc qc_in;
    qc_in.add_random_str((uint32_t)qubit_num, (uint32_t)gate_num,
			 "X:1,Z:1,H:1,S:1,T:1,CX:1,CZ:1");

    qc_in.save("sandbox/out.sqc");

    Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, mode);
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
  /*
    $ ./check_random [mode] [trial] [qubit_num] [gate_num]
    ex) $ ./check_random zx 1000 3 100
    ex) $ ./check_random pp 1000 3 100
   */
  std::string mode = argv[1];
  int trial = atoi(argv[2]);
  int qubit_num = atoi(argv[3]);
  int gate_num = atoi(argv[4]);

  for (int n = 0; n < trial; ++n) {
    std::cout << "** n = " << n << " **" << std::endl;
    if (test_random(qubit_num, gate_num, mode) == false) break;
    //if (test_random_str(qubit_num, gate_num, mode) == false) break;
  }

  return 0;
}
