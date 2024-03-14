/**
 * @file sharqit.cpp
 * @brief source code of sharqit command
 */

#include "sharqit.h"

/**
 * @brief print help message
 */
void print_help()
{
  std::stringstream ss;
  ss << "sharqit - quantum circuit optimizer" << std::endl;
  ss << "[usage]" << std::endl;
  ss << "  sharqit [option] ([file]..)([params]) (> [file])" << std::endl;
  ss << "[option]" << std::endl;
  ss << "  --opt(=kind) FILE  : optimize the circuit file, output to stdout." << std::endl;
  ss << "                       --opt=zx: T-count reduction using ZX-calculus" << std::endl;
  ss << "                       --opt=pp: gate-count reduction using Phase Polynomials" << std::endl;
  ss << "  --rand PARAMS      : generate a random circuit file, output to stdout." << std::endl;
  ss << "  --eq FILE1 FILE2   : verify two circuits are equal. (can't execute that have too many qubits)" << std::endl;
  ss << "  --stats FILE       : print stats of the circut file." << std::endl;
  ss << "  --show FILE        : print the circuit diagram as ascii text." << std::endl;
  ss << "  --help             : print help message." << std::endl;
  ss << "  --version          : print version." << std::endl;
  ss << "[examples]" << std::endl;
  ss << "  $ sharqit --opt foo.sqc > bar.sqc" << std::endl;
  ss << "  $ sharqit --opt=zx foo.sqc > bar.sqc" << std::endl;
  ss << "  $ sharqit --opt=pp foo.sqc > bar.sqc" << std::endl;
  ss << "  $ sharqit --eq foo.sqc bar.sqc" << std::endl;
  ss << "  $ sharqit --rand 3,100,\"X\":1,\"H\":2,\"T\":3.5,\"RZ(1/2)\":1.5 > bar.sqc # 3 qubits,100 gates" << std::endl;
  ss << "  $ sharqit --stats foo.sqc" << std::endl;
  ss << "  $ sharqit --show foo.sqc" << std::endl;
  ss << "[file format]" << std::endl;
  ss << "  Sharqit supports a simple file format for quantum circuits as follows." << std::endl;
  ss << "  $ cat foo.sqc" << std::endl;
  ss << "  # comment" << std::endl;
  ss << "  H 0" << std::endl;
  ss << "  CX 0 1" << std::endl;
  ss << "  RZ(1/2) 0 # comment" << std::endl;
  ss << "  T+ 1" << std::endl;
  ss << "  RZ(1) 0" << std::endl;
  ss << "  ..." << std::endl;
  ss << "  Supported quantum gates are X,Z,H,S,S+,T,T+,RZ,CX,CZ." << std::endl;
  ss << "  S+ and T+ are Hermitian conjugate of S and T respectively." << std::endl;
  ss << "  RZ gate have one phase factor denoted by fraction brackled in parentheses." << std::endl;
  ss << "  The unit of phase factor is PI radian, so 3/4 means 3PI/4, 1 means PI, and so on." << std::endl;
  std::cerr << ss.str();
}

/**
 * @brief optimize the file
 * @param [in] fin quantum circuit file name
 * @param [in] method optimization method ("zx" or "pp")
 */
void optimize(std::string& fin, std::string& method)
{
  try {
    Sharqit::Optimizer opt;
    Sharqit::QCirc qc_in;
    qc_in.load(fin);
    Sharqit::QCirc qc_out = opt.reduce_gates(qc_in, method);
    qc_out.print_qcirc();
    std::cerr << opt << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

/**
 * @brief verify equality of two quantum circuit
 * @param [in] fin_A quantum circuit file name
 * @param [in] fin_B quantum circuit file name
 */
void verify_equality(std::string& fin_A, std::string& fin_B)
{
  try {
    Sharqit::QCirc qc_A;
    Sharqit::QCirc qc_B;
    qc_A.load(fin_A);
    qc_B.load(fin_B);
    bool eq = qc_A.is_equal(qc_B);
    if (eq) std::cout << "true" << std::endl;
    else std::cout << "false" << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

/**
 * @brief generate random quantum circuit
 * @param [in] params parameters for randomization
 * @details example of the params @n "3,100,CX:1,H:2,T:3,RZ(1/2):5"
 */
void random_qcirc(std::string& params)
{
  try {
    std::vector<std::string> args = Sharqit::split(params, ',');
    uint32_t qubit_num = atoi(args[0].c_str());
    uint32_t gate_num = atoi(args[1].c_str());

    nlohmann::json probs;
    for (uint32_t i = 2; i < args.size(); ++i) {
      std::vector<std::string> key_value = Sharqit::split(args[i], ':');
      std::string gate_str = key_value[0];
      std::string prob_str = key_value[1];
      probs[gate_str] = atof(prob_str.c_str());
    }

    Sharqit::QCirc qc_rand;
    qc_rand.add_random(qubit_num, gate_num, probs);
    qc_rand.print_qcirc();
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

/**
 * @brief print stats
 * @param [in] fin quantum circuit file name
 */
void print_stats(std::string& fin)
{
  try {
    Sharqit::QCirc qc;
    qc.load(fin);
    qc.print_stats();
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

/**
 * @brief show the quantum circuit as an ascii text
 * @param [in] fin quantum circuit file name
 */
void show_qcirc(std::string& fin)
{
  try {
    Sharqit::QCirc qc;
    qc.load(fin);
    qc.show();
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

/**
 * @brief main function of sharqit command
 * @param [in] argc number of arguments
 * @param [in] argv string of arguments
 */
int main(int argc, char** argv)
{
  for (int n=1; n<argc; n++) {
    if (strcmp(argv[n],"--help") == 0) {
      print_help();
      break;
    }
    else if (strcmp(argv[n],"--version") == 0) {
      std::cerr << Sharqit::VERSION << std::endl;
      break;
    }
    else if (strncmp(argv[n],"--opt", 5) == 0) {
      if (argc - n <= 1) {
      	std::cerr << "You must specify a circuit file name." << std::endl;
      	exit(1);
      }
      else if (argc - n > 2) {
      	std::cerr << "Too many arguments." << std::endl;
      	exit(1);
      }
      std::string opt_str = argv[n];
      std::vector<std::string> token = Sharqit::split(opt_str, '=');
      std::string method = "";
      if (token.size() == 1) method = "zx";
      else if (token[1] == "zx") method = "zx";
      else if (token[1] == "pp") method = "pp";
      else {
      	std::cerr << "Invalid kind of optimizer." << std::endl;
      	exit(1);
      }
      std::string fin = argv[++n];
      optimize(fin, method);
      break;
    }
    else if (strcmp(argv[n],"--eq") == 0) {
      if (argc - n <= 1) {
      	std::cerr << "You must specify two circuit file names." << std::endl;
      	exit(1);
      }
      else if (argc - n > 3) {
      	std::cerr << "Too many arguments." << std::endl;
      	exit(1);
      }
      std::string fin_A = argv[++n];
      std::string fin_B = argv[++n];
      verify_equality(fin_A, fin_B);
      break;
    }
    else if (strcmp(argv[n],"--rand") == 0) {
      if (argc - n <= 1) {
      	std::cerr << "You must specify a circuit file name." << std::endl;
      	exit(1);
      }
      else if (argc - n > 2) {
      	std::cerr << "Too many arguments." << std::endl;
      	exit(1);
      }
      std::string params = argv[++n];
      random_qcirc(params);
      break;
    }
    else if (strcmp(argv[n],"--stats") == 0) {
      if (argc - n <= 1) {
	std::cerr << "You must specify a circuit file name." << std::endl;
	exit(1);
      }
      else if (argc - n > 2) {
      	std::cerr << "Too many arguments." << std::endl;
      	exit(1);
      }
      std::string fin = argv[++n];
      print_stats(fin);
      break;
    }
    else if (strcmp(argv[n],"--show") == 0) {
      if (argc - n <= 1) {
	std::cerr << "You must specify a circuit file name." << std::endl;
	exit(1);
      }
      else if (argc - n > 2) {
      	std::cerr << "Too many arguments." << std::endl;
      	exit(1);
      }
      std::string fin = argv[++n];
      show_qcirc(fin);
      break;
    }
  }

  return 0;
}
