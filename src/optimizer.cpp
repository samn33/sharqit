#include "sharq.h"

/**
 *  member functions
 */

Sharq::QCirc Sharq::Optimizer::execute(const Sharq::QCirc& qc_in)
{
  std::ofstream ofs;
  auto now = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(now);
  if (log_file_ != "") {
    ofs.open(log_file_, std::ios::out);
    ofs << "START: " << std::ctime(&end_time);
  }
  
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

  if (log_file_ != "") {
    now = std::chrono::system_clock::now();
    end_time = std::chrono::system_clock::to_time_t(now);
    ofs << "END: " << std::ctime(&end_time);
    ofs.close();
  }

  return qc_out;
}
