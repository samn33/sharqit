/**
 * @file zxnode.cpp
 * @brief member functions of ZXNode class
 */

#include "zx.h"

std::string Sharqit::ZXNode::name() const
{
  std::string s;

  if (kind_ == Sharqit::ZXNodeKind::XSpider) {
    s = "X(" + phase_.to_string() + ")";
  }
  else if (kind_ == Sharqit::ZXNodeKind::ZSpider) {
    s = "Z(" + phase_.to_string() + ")";
  }
  else if (kind_ == Sharqit::ZXNodeKind::Input) {
    s = "in_" + std::to_string(q_);
  }
  else if (kind_ == Sharqit::ZXNodeKind::Output) {
    s = "out_" + std::to_string(q_);
  }
  
  return s;
}
