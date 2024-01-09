#include "dag.h"

/**
 *  member functions
 */

std::string Sharq::DAGEdge::name() const
{
  std::string s;
  if (kind_ == Sharq::DAGEdgeKind::Forward)
    s = "-" + std::to_string(q_) + "->[" + std::to_string(to_) + "]";
  else if (kind_ == Sharq::DAGEdgeKind::Backward)
    s = "<-" + std::to_string(q_) + "-[" + std::to_string(to_) + "]";
  return s;
}
