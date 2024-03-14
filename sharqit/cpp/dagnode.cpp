/**
 * @file dagnode.cpp
 * @brief member functions of DAGNode class
 */

#include "dag.h"

std::string Sharqit::DAGNode::name() const
{
  std::string s;
  if (kind_ == Sharqit::DAGNodeKind::InNode) s = "Input";
  else if (kind_ == Sharqit::DAGNodeKind::OutNode) s = "Output";
  else s = qgate_.name();
  return s;
}
