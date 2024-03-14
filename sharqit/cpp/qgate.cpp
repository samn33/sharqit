/**
 * @file qgate.cpp
 * @brief member functions of QGate class
 */

#include "qgate.h"

Sharqit::QGate::QGate(const Sharqit::QGateKind kind, const std::vector<uint32_t>& qid,
		    const Sharqit::Phase& phase)
{
  Sharqit::Phase p = phase;
  p.mod_2pi();
  
  switch (kind) {
  case Sharqit::QGateKind::Id:
  case Sharqit::QGateKind::X:
  case Sharqit::QGateKind::Z:
  case Sharqit::QGateKind::S:
  case Sharqit::QGateKind::Sdg:
  case Sharqit::QGateKind::T:
  case Sharqit::QGateKind::Tdg:
  case Sharqit::QGateKind::H:
    if (qid.size() != 1) {
      throw std::runtime_error("related qubit number must be 1.");
    }
    if (p.is_zero() == false) {
      throw std::runtime_error("phase parameter must be zero.");
    }
    break;
  case Sharqit::QGateKind::RZ:
    if (qid.size() != 1) {
      throw std::runtime_error("related qubit number must be 1.");
    }
    break;
  case Sharqit::QGateKind::Id2:
  case Sharqit::QGateKind::CX:
  case Sharqit::QGateKind::CZ:
    if (qid.size() != 2) {
      throw std::runtime_error("related qubit number must be 2.");
    }
    if (p.is_zero() == false) {
      throw std::runtime_error("phase parameter must be zero.");
    }
    break;
  default:
    throw std::runtime_error("unknown QGateKind.");
  }

  kind_ = kind;
  qid_ = qid;
  phase_ = p;
  op_ = kind_to_op(kind_, phase_);
}

Sharqit::QGate::QGate(const std::string& qgate_str)
{
  uint32_t pos = qgate_str.find(" ");
  std::string kind_phase_str = qgate_str.substr(0, pos);
  std::tuple<QGateKind, Phase> kp = kind_phase(kind_phase_str);
  kind_ = std::get<0>(kp);
  phase_ = std::get<1>(kp);

  pos = qgate_str.find(" ");
  std::string s = qgate_str.substr(pos, qgate_str.size()-pos);
  std::string qid_str = Sharqit::trim(s);
  for (auto& qstr:Sharqit::split(qid_str, ' ')) {
    qid_.push_back(stoi(qstr));
  }
  
  op_ = kind_to_op(kind_, phase_);
}

std::vector<std::vector<std::complex<double>>> Sharqit::QGate::kind_to_op(const Sharqit::QGateKind kind,
									const Sharqit::Phase& phase)
{
  std::vector<std::vector<std::complex<double>>> op;
  std::complex I(0.0, 1.0);
  switch (kind) {
  case Sharqit::QGateKind::Id:
    op = {{1.0, 0.0},
	  {0.0, 1.0}};
    break;
  case Sharqit::QGateKind::X:
    op = {{0.0, 1.0},
	  {1.0, 0.0}};
    break;
  case Sharqit::QGateKind::Z:
    op = {{1.0, 0.0},
	  {0.0, -1.0}};
    break;
  case Sharqit::QGateKind::S:
    op = {{1.0, 0.0},
	  {0.0, I}};
    break;
  case Sharqit::QGateKind::Sdg:
    op = {{1.0, 0.0},
	  {0.0, -I}};
    break;
  case Sharqit::QGateKind::T:
    op = {{1.0, 0.0},
	  {0.0, std::exp(M_PI*I/4.0)}};
    break;
  case Sharqit::QGateKind::Tdg:
    op = {{1.0, 0.0},
	  {0.0, std::exp(-M_PI*I/4.0)}};
    break;
  case Sharqit::QGateKind::H:
    op = {{1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)},
	  {1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)}};
    break;
  case Sharqit::QGateKind::RZ:
    op = {{std::exp(-I*phase.value()/2.0), 0.0},
	  {0.0, std::exp(I*phase.value()/2.0)}};
    break;
  case Sharqit::QGateKind::Id2:
    op = {{1.0, 0.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0, 0.0},
	  {0.0, 0.0, 1.0, 0.0},
	  {0.0, 0.0, 0.0, 1.0}};
    break;
  case Sharqit::QGateKind::CX:
    op = {{1.0, 0.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0, 0.0},
	  {0.0, 0.0, 0.0, 1.0},
	  {0.0, 0.0, 1.0, 0.0}};
    break;
  case Sharqit::QGateKind::CZ:
    op = {{1.0, 0.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0, 0.0},
	  {0.0, 0.0, 1.0, 0.0},
	  {0.0, 0.0, 0.0, -1.0}};
    break;
  default:
    throw std::runtime_error("unknown QGateKind.");
  }
  return op;
}

std::string Sharqit::QGate::name(bool pi_str) const
{
  std::vector<std::string> s = { "X", "Z", "S", "S+", "T", "T+", "H", "RZ", "CX", "CZ", "I", "I2" };
  Phase p = phase_;
  std::string p_str = "";
  if (kind_ == QGateKind::RZ) p_str = "(" + p.to_string(pi_str) + ")";
  return s[kind_] + p_str;
}

std::string Sharqit::QGate::to_string(bool pi_str) const
{
  std::stringstream ss;
  ss << name(pi_str) << " ";
  if (qid()[0] != qid()[1]) {
    for (auto& q:qid()) {
      ss << q << " ";
    }
  }
  else {
    ss << qid()[0] << " ";
  }

  std::string s = ss.str();
  s.pop_back();

  return s;
}

std::tuple<Sharqit::QGateKind, Sharqit::Phase> Sharqit::QGate::kind_phase(const std::string& str)
{
  Sharqit::QGateKind kind;
  Sharqit::Phase phase;
  
  if (str == "I") {
    kind = Sharqit::QGateKind::Id;
    phase = Sharqit::Phase(0);
  }
  else if (str == "X") {
    kind = Sharqit::QGateKind::X;
    phase = Sharqit::Phase(0);
  }
  else if (str == "Z") {
    kind = Sharqit::QGateKind::Z;
    phase = Sharqit::Phase(0);
  }
  else if (str == "S") {
    kind = Sharqit::QGateKind::S;
    phase = Sharqit::Phase(0);
  }
  else if (str == "Sdg") {
    kind = Sharqit::QGateKind::Sdg;
    phase = Sharqit::Phase(0);
  }
  else if (str == "S+") {
    kind = Sharqit::QGateKind::Sdg;
    phase = Sharqit::Phase(0);
  }
  else if (str == "T") {
    kind = Sharqit::QGateKind::T;
    phase = Sharqit::Phase(0);
  }
  else if (str == "Tdg") {
    kind = Sharqit::QGateKind::Tdg;
    phase = Sharqit::Phase(0);
  }
  else if (str == "T+") {
    kind = Sharqit::QGateKind::Tdg;
    phase = Sharqit::Phase(0);
  }
  else if (str == "H") {
    kind = Sharqit::QGateKind::H;
    phase = Sharqit::Phase(0);
  }
  else if (str == "I2") {
    kind = Sharqit::QGateKind::Id2;
    phase = Sharqit::Phase(0);
  }
  else if (str == "CX") {
    kind = Sharqit::QGateKind::CX;
    phase = Sharqit::Phase(0);
  }
  else if (str == "CZ") {
    kind = Sharqit::QGateKind::CZ;
    phase = Sharqit::Phase(0);
  }
  else if (str.find("RZ") == 0) {
    phase = Sharqit::Phase(str.substr(3, str.size() - 4));
    phase.mod_2pi();
    if (str.find("RZ") == 0) kind = Sharqit::QGateKind::RZ;
  }
  else {
    throw std::runtime_error(str+" is not supported.");
  }

  return {kind, phase};
}

Sharqit::QGate Sharqit::QGate::inverse() const
{
  Sharqit::QGate qgate_inv;
  switch (kind_) {
  case Sharqit::QGateKind::Id:
    qgate_inv.kind(Sharqit::QGateKind::Id);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::Id));
    break;
  case Sharqit::QGateKind::X:
    qgate_inv.kind(Sharqit::QGateKind::X);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::X));
    break;
  case Sharqit::QGateKind::Z:
    qgate_inv.kind(Sharqit::QGateKind::Z);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::Z));
    break;
  case Sharqit::QGateKind::S:
    qgate_inv.kind(Sharqit::QGateKind::Sdg);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::Sdg));
    break;
  case Sharqit::QGateKind::Sdg:
    qgate_inv.kind(Sharqit::QGateKind::S);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::S));
    break;
  case Sharqit::QGateKind::T:
    qgate_inv.kind(Sharqit::QGateKind::Tdg);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::Tdg));
    break;
  case Sharqit::QGateKind::Tdg:
    qgate_inv.kind(Sharqit::QGateKind::T);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::T));
    break;
  case Sharqit::QGateKind::H:
    qgate_inv.kind(Sharqit::QGateKind::H);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::H));
    break;
  case Sharqit::QGateKind::RZ:
    qgate_inv.kind(Sharqit::QGateKind::RZ);
    qgate_inv.qid(qid_);
    qgate_inv.phase(-phase_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::RZ, -phase_));
    break;
  case Sharqit::QGateKind::Id2:
    qgate_inv.kind(Sharqit::QGateKind::Id2);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::Id2));
    break;
  case Sharqit::QGateKind::CX:
    qgate_inv.kind(Sharqit::QGateKind::CX);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::CX));
    break;
  case Sharqit::QGateKind::CZ:
    qgate_inv.kind(Sharqit::QGateKind::CZ);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharqit::QGateKind::CZ));
    break;
  default:
    throw std::runtime_error("unknown QGateKind.");
  }
  return qgate_inv;
}

bool Sharqit::QGate::is_identical(QGate& other) const
{
  if (kind_ != other.kind()) return false;
  if (qid_.size() != other.qid().size()) return false;
  if (qid_.size() == 1 && qid_[0] != other.qid()[0]) return false;
  if (qid_.size() == 2 &&
      (qid_[0] != other.qid()[0] || qid_[1] != other.qid()[1])) return false;
  if (qid_.size() > 2) return false;
  if (phase_ != other.phase()) return false;

  return true;
}

bool Sharqit::QGate::overlap(const Sharqit::QGate& other) const
{
  bool overlap = false;
  for (auto& q_this:qid_) {
    for (auto& q_other:other.qid()) {
      if (q_this == q_other) {
	overlap = true;
	break;
      }
    }
  }
  return overlap;
}

bool Sharqit::QGate::mergeable(const Sharqit::QGate& other) const
{
  bool ov = overlap(other);
  if (!ov) return false;

  if (qid_.size() == 1 && other.qid().size() == 1) {
    if (kind_ == other.kind()) return true;
    else if (is_RZ_gate() && other.is_RZ_gate()) return true;
  }
  else if (qid_.size() == 2 && other.qid().size() == 2 && kind_ == other.kind()) {
    if (kind_ == Sharqit::QGateKind::CX && qid_[0] == other.qid()[0] && qid_[1] == other.qid()[1]) return true;
    else if (kind_ == Sharqit::QGateKind::CZ && qid_[0] == other.qid()[0] && qid_[1] == other.qid()[1]) return true;
    else if (kind_ == Sharqit::QGateKind::CZ && qid_[0] == other.qid()[1] && qid_[1] == other.qid()[0]) return true;
  }
  return false;
}

bool Sharqit::QGate::commutable(const Sharqit::QGate& other) const
{
  /* meargeable -> commutable */
  if (Sharqit::QGate::mergeable(other)) return true;

  /* pauli gates -> commutable */
  if (is_pauli_gate() && other.is_pauli_gate()) return true;

  /* not overlap -> commutable */
  bool ov = overlap(other);
  if (!ov) return true;

  /* single qubit gate and two qubit gate */
  if (qid_.size() == 1 && other.qid().size() == 2 && other.kind() == Sharqit::QGateKind::CX) {
    if (is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (is_X_gate() && (qid_[0] == other.qid()[1])) return true;
  }
  if (qid_.size() == 1 && other.qid().size() == 2 && other.kind() == Sharqit::QGateKind::CZ) {
    if (is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (is_RZ_gate() && (qid_[0] == other.qid()[1])) return true;
  }

  if (qid_.size() == 2 && other.qid().size() == 1 && kind_ == Sharqit::QGateKind::CX) {
    if (other.is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (other.is_X_gate() && (qid_[1] == other.qid()[0])) return true;
  }  
  if (qid_.size() == 2 && other.qid().size() == 1 && kind_ == Sharqit::QGateKind::CZ) {
    if (other.is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (other.is_RZ_gate() && (qid_[1] == other.qid()[0])) return true;
  }  

  /* two qubit gate and two qubit gate */
  if (qid_.size() == 2 && other.qid().size() == 2) {
    if (kind_ == other.kind() && kind_ == Sharqit::QGateKind::CX &&
	((qid_[0] == other.qid()[0]) || (qid_[1] == other.qid()[1]))) return true;
    else if (kind_ == other.kind() && kind_ == Sharqit::QGateKind::CZ &&
	((qid_[0] == other.qid()[0]) || (qid_[1] == other.qid()[1]))) return true;
    else if (kind_ == other.kind() && kind_ == Sharqit::QGateKind::CZ &&
	((qid_[0] == other.qid()[1]) || (qid_[1] == other.qid()[0]))) return true;
  }
  
  return false;
}

void Sharqit::QGate::merge(const Sharqit::QGate& other)
{
  if (kind_ == other.kind() && !is_RZ_gate()) {
    if (qubit_num() == 1) kind(Sharqit::QGateKind::Id);
    else kind(Sharqit::QGateKind::Id2);
  }
  else if (qubit_num() == 1) {
    Sharqit::Phase p(0);
    if (kind_ == Sharqit::QGateKind::Z) p += Sharqit::Phase(1);
    else if (kind_ == Sharqit::QGateKind::S) p += Sharqit::Phase(1,2);
    else if (kind_ == Sharqit::QGateKind::Sdg) p += Sharqit::Phase(-1,2);
    else if (kind_ == Sharqit::QGateKind::T) p += Sharqit::Phase(1,4);
    else if (kind_ == Sharqit::QGateKind::Tdg) p += Sharqit::Phase(-1,4);
    else if (kind_ == Sharqit::QGateKind::RZ) p += phase_;
    else if (kind_ == Sharqit::QGateKind::Id) p += Sharqit::Phase(0);

    if (other.kind() == Sharqit::QGateKind::Z) p += Sharqit::Phase(1);
    else if (other.kind() == Sharqit::QGateKind::S) p += Sharqit::Phase(1,2);
    else if (other.kind() == Sharqit::QGateKind::Sdg) p += Sharqit::Phase(-1,2);
    else if (other.kind() == Sharqit::QGateKind::T) p += Sharqit::Phase(1,4);
    else if (other.kind() == Sharqit::QGateKind::Tdg) p += Sharqit::Phase(-1,4);
    else if (other.kind() == Sharqit::QGateKind::RZ) p += other.phase();
    else if (other.kind() == Sharqit::QGateKind::Id) p += Sharqit::Phase(0);

    if (p == Phase(0)) {
      kind(Sharqit::QGateKind::Id);
    }
    else {
      kind(Sharqit::QGateKind::RZ);
      phase(p);
    }
  }
}
