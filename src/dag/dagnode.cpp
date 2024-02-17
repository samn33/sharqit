#include "dag.h"

/**
 *  member functions
 */

std::string Sharq::DAGNode::name() const
{
  std::string s;
  if (kind_ == Sharq::DAGNodeKind::InNode) s = "Input";
  else if (kind_ == Sharq::DAGNodeKind::OutNode) s = "Output";
  else s = qgate_.name();
  return s;
}
