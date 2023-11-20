#include "sharq.h"

//void eval_sharq(const std::string& name, const std::string& path) {
void eval_sharq(const std::string& name, const std::string& path_in, const std::string& path_out) {

  try {
    Sharq::Optimizer opt;

    Sharq::QCirc qc_in;
    qc_in.load(path_in);

    /* clock start */
    auto start = std::chrono::system_clock::now();

    Sharq::QCirc qc_out = opt.execute(qc_in);
    qc_out.save(path_out);

    /* clock end */
    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    double proc_time = msec / 1000.;

    std::cout << name << "," << std::to_string(proc_time) << "," << qc_in.tcount() << ","<< qc_out.tcount() << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

int main(int argc, char** argv)
{
  std::string file_name = "qcirc_list.txt";
  std::ifstream ifs(file_name);
  if (ifs.fail()) {
    throw std::runtime_error("Failed to open file.");
  }

  std::string str;
  std::cout << "name,time[sec],T-count<in>,T-count<out>" << std::endl;
  while (std::getline(ifs, str)) {
    std::vector<std::string> svec = Sharq::split(str, ' ');
    std::string name = svec[0];
    std::string path_in = "qcirc_sharq/" + svec[1] + ".sqc";
    std::string path_out = "qcirc_sharq/" + svec[2] + ".sqc";
    eval_sharq(name, path_in, path_out);
  }

  ifs.close();

  
  //std::cout << "name,time[sec],T-count<in>,T-count<out>" << std::endl;
  //eval_sharq("tof_3", "qcirc_sharq/tof_3_before.sqc", "qcirc_sharq/tof_3_after.sqc");
  //eval_sharq("tof_10", "qcirc_sharq/tof_10_before.sqc", "qcirc_sharq/tof_10_after.sqc");
  //eval_sharq("barenco_tof_10", "qcirc_sharq/barenco_tof_10_before.sqc", "qcirc_sharq/barenco_tof_10_after.sqc");
  //eval_sharq("gf2^8", "qcirc_sharq/gf2^E8_mult_before.sqc", "qcirc_sharq/gf2^E8_mult_after.sqc");
  //eval_sharq("Adder16", "qcirc_sharq/Adder16_before.sqc", "qcirc_sharq/Adder16_after.sqc");

  return 0;
}
