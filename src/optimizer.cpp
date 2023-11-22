#include "sharq.h"

/**
 *  member functions
 */

std::string Sharq::Optimizer::to_string() const
{
  std::map<std::string, uint32_t> stats_in = stats_in_;
  std::map<std::string, uint32_t> stats_out = stats_out_;
  std::stringstream ss;

  if (stats_in["gate_count"] == 0 && stats_out["gate_count"] == 0) {
    return "Optimization has not executed yet.";
  }

  ss << "X-count  = " << stats_in["x_count"] << " -> " << stats_out["x_count"] << std::endl;
  ss << "Z-count  = " << stats_in["z_count"] << " -> " << stats_out["z_count"] << std::endl;
  ss << "H-count  = " << stats_in["h_count"] << " -> " << stats_out["h_count"] << std::endl;
  ss << "S-count  = " << stats_in["s_count"] << " -> " << stats_out["s_count"] << std::endl;
  ss << "T-count  = " << stats_in["t_count"] << " -> " << stats_out["t_count"] << std::endl;
  ss << "RZ-count = " << stats_in["rz_count"] << " -> " << stats_out["rz_count"] << std::endl;
  ss << "CX-count = " << stats_in["cx_count"] << " -> " << stats_out["cx_count"] << std::endl;
  ss << "gate-count = " << stats_in["gate_count"] << " -> " << stats_out["gate_count"] << std::endl;
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
  
  /* gate cancellation */
  Sharq::QCirc qc = qc_in;
  qc.gate_cancel();

  /* convert to ZX-Diagram */
  Sharq::ZXDiagram zx = qc.to_zxdiagram();

  /* simplify */
  zx.simplify();

  /* extract circuit */
  Sharq::QCirc qc_out = zx.extract_qcirc();

  /* gate cancellation */
  qc_out.gate_cancel();

  /* stats (out) */
  stats_out_ = qc_out.stats();
  
  auto end = std::chrono::system_clock::now();
  auto dur = end - start;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  proc_time_ = msec / 1000.;

  return qc_out;
}
