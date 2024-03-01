/**
 * @file zxnode.cpp
 * @brief member functions of ZXNode class
 */

#include "zx.h"

std::string Sharq::ZXNode::name() const
{
  std::string s;

  if (kind_ == Sharq::ZXNodeKind::XSpider) {
    s = "X(" + phase_.to_string() + ")";
  }
  else if (kind_ == Sharq::ZXNodeKind::ZSpider) {
    s = "Z(" + phase_.to_string() + ")";
  }
  else if (kind_ == Sharq::ZXNodeKind::Input) {
    s = "in_" + std::to_string(q_);
  }
  else if (kind_ == Sharq::ZXNodeKind::Output) {
    s = "out_" + std::to_string(q_);
  }
  
  return s;
}
