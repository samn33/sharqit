#include "sharq.h"

void print_help()
{
  std::stringstream ss;
  ss << "sharq - quantum circuit optimizer" << std::endl;
  ss << "[usage]" << std::endl;
  ss << "  sharq [option] [file] (> [file])" << std::endl;
  ss << "[option]" << std::endl;
  ss << "  --opt     : optimize the circuit file, output to stdout." << std::endl;
  ss << "  --stats   : print stats of the circut file." << std::endl;
  ss << "  --show    : print the circuit diagram as ascii text." << std::endl;
  ss << "  --help    : print help message." << std::endl;
  ss << "  --version : print version." << std::endl;
  ss << "[examples]" << std::endl;
  ss << "  $ sharq --opt foo.sqc > bar.sqc" << std::endl;
  ss << "  $ sharq --stats foo.sqc" << std::endl;
  ss << "  $ sharq --show foo.sqc" << std::endl;
  ss << "[file format]" << std::endl;
  ss << "  Sharq supports a simple file format for quantum circuits as follows." << std::endl;
  ss << "  $ cat foo.sqc" << std::endl;
  ss << "  H 0" << std::endl;
  ss << "  CX 0 1" << std::endl;
  ss << "  RZ(1/2) 0" << std::endl;
  ss << "  T+ 1" << std::endl;
  ss << "  RZ(1) 0" << std::endl;
  ss << "  ..." << std::endl;
  ss << "  Supported quantum gates are X,Z,H,S,S+,T,T+,CX,RZ." << std::endl;
  ss << "  S+ and T+ are Hermitian conjugate of S and T respectively." << std::endl;
  ss << "  RZ gate have one phase factor denoted by fraction brackled in parentheses." << std::endl;
  ss << "  The unit of phase factor is radian, so 3/4 means 3PI/4, 1 means PI, and so on." << std::endl;
  std::cerr << ss.str();
}

void optimize(std::string& fin)
{
  Sharq::Optimizer opt;
  Sharq::QCirc qc_in;
  qc_in.load(fin);
  Sharq::QCirc qc_out = opt.execute(qc_in);
  qc_out.print_qcirc();
}

void print_stats(std::string& fin)
{
  Sharq::QCirc qc;
  qc.load(fin);
  qc.print_stats();
}

void show_qcirc(std::string& fin)
{
  Sharq::QCirc qc;
  qc.load(fin);
  qc.show();
}

int main(int argc, char** argv)
{
  std::string fin;
  
  for (int n=1; n<argc; n++) {
    if (strcmp(argv[n],"--help") == 0) {
      print_help();
      break;
    }
    else if (strcmp(argv[n],"--version") == 0) {
      std::cerr << Sharq::VERSION << std::endl;
      break;
    }
    else if (strcmp(argv[n],"--opt") == 0) {
      if (argc == 2) {
	std::cerr << "You must specify a circuit file name." << std::endl;
	exit(1);
      }
      else if (argc > 3) {
	std::cerr << "Too many arguments." << std::endl;
	exit(1);
      }
      fin = argv[++n];
      optimize(fin);
      break;
    }
    else if (strcmp(argv[n],"--stats") == 0) {
      if (argc != 3) {
	std::cerr << "You must specify a circuit file name." << std::endl;
	exit(1);
      }
      fin = argv[++n];
      print_stats(fin);
      break;
    }
    else if (strcmp(argv[n],"--show") == 0) {
      if (argc != 3) {
	std::cerr << "You must specify a circuit file name." << std::endl;
	exit(1);
      }
      fin = argv[++n];
      show_qcirc(fin);
      break;
    }
  }

  return 0;
}
