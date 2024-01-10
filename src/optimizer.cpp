#include "optimizer.h"

/**
 *  member functions
 */

std::string Sharq::Optimizer::to_string() const
{
  std::map<std::string, uint32_t> stats_in = stats_in_;
  std::map<std::string, uint32_t> stats_out = stats_out_;
  std::map<std::string, uint32_t> zx_stats_in = zx_stats_in_;
  std::map<std::string, uint32_t> zx_stats_out = zx_stats_out_;
  std::stringstream ss;

  if (stats_in["gate_count"] == 0 && stats_out["gate_count"] == 0) {
    return "Optimization has not executed yet.";
  }

  ss << "[zx diagram]" << std::endl;
  ss << "xspider       = " << zx_stats_in["xspider"] << " -> " << zx_stats_out["xspider"] << std::endl;
  ss << "zspider       = " << zx_stats_in["zspider"] << " -> " << zx_stats_out["zspider"] << std::endl;
  ss << "clifford      = " << zx_stats_in["clifford"] << " -> " << zx_stats_out["clifford"] << std::endl;
  ss << "non-clifford  = " << zx_stats_in["non-clifford"] << " -> " << zx_stats_out["non-clifford"] << std::endl;
  ss << "hadamard      = " << zx_stats_in["hadamard"] << " -> " << zx_stats_out["hadamard"] << std::endl;

  ss << "[quantum circuit]" << std::endl;
  ss << "X-count  = " << stats_in["x_count"] << " -> " << stats_out["x_count"] << std::endl;
  ss << "Z-count  = " << stats_in["z_count"] << " -> " << stats_out["z_count"] << std::endl;
  ss << "H-count  = " << stats_in["h_count"] << " -> " << stats_out["h_count"] << std::endl;
  ss << "S-count  = " << stats_in["s_count"] << " -> " << stats_out["s_count"] << std::endl;
  ss << "T-count  = " << stats_in["t_count"] << " -> " << stats_out["t_count"] << std::endl;
  ss << "RZ-count = " << stats_in["rz_count"] << " -> " << stats_out["rz_count"] << std::endl;
  ss << "CX-count = " << stats_in["cx_count"] << " -> " << stats_out["cx_count"] << std::endl;
  ss << "2Q-count = " << stats_in["2q_count"] << " -> " << stats_out["2q_count"] << std::endl;
  ss << "gate-count = " << stats_in["gate_count"] << " -> " << stats_out["gate_count"] << std::endl;
  ss << "depth      = " << stats_in["depth"] << " -> " << stats_out["depth"] << std::endl;
  ss << "qubit_num  = " << stats_in["qubit_num"] << " -> " << stats_out["qubit_num"] << std::endl;
  ss << "proc time (sec) = " << std::to_string(proc_time_) << std::endl;
  
  std::string s = ss.str();
  s.pop_back();
  return s;
}

Sharq::QCirc Sharq::Optimizer::execute(const Sharq::QCirc& qc_in)
{
  std::ofstream ofs;
  auto start = std::chrono::system_clock::now();

  /* stats (in) */
  stats_in_ = qc_in.stats();

  /* rule-based gate reduction */
  Sharq::DAGCirc dc_in = qc_in.to_dagcirc();
  dc_in.rule_based_gate_reduction();
  Sharq::QCirc qc = dc_in.to_qcirc();

  /* convert to ZX-Diagram */
  Sharq::ZXDiagram zx = qc.to_zxdiagram();
  zx_stats_in_ = zx.stats();

  /* simplify */
  zx.simplify();
  zx_stats_out_ = zx.stats();

  /* extract circuit */
  Sharq::QCirc qc_out = zx.extract_qcirc();

  /* rule-based gate reduction */
  Sharq::DAGCirc dc_out = qc_out.to_dagcirc();
  dc_out.rule_based_gate_reduction();
  qc_out = dc_out.to_qcirc();

  /* stats (out) */
  stats_out_ = qc_out.stats();
  
  auto end = std::chrono::system_clock::now();
  auto dur = end - start;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  proc_time_ = msec / 1000.;

  return qc_out;
}
