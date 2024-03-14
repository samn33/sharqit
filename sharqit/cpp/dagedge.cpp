/**
 * @file dagedge.cpp
 * @brief member functions of DAGEdge class
 */

#include "dag.h"

std::string Sharqit::DAGEdge::name() const
{
  std::string s;
  if (kind_ == Sharqit::DAGEdgeKind::Forward)
    s = "-" + std::to_string(q_) + "->[" + std::to_string(to_) + "]";
  else if (kind_ == Sharqit::DAGEdgeKind::Backward)
    s = "<-" + std::to_string(q_) + "-[" + std::to_string(to_) + "]";
  return s;
}
