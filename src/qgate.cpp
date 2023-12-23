#include "qgate.h"

/**
 *  member functions
 */

Sharq::QGate::QGate(const Sharq::QGateKind kind, const std::vector<uint32_t>& qid,
		    const Sharq::Phase& phase)
{
  Sharq::Phase p = phase;
  p.mod_2pi();
  
  switch (kind) {
  case Sharq::QGateKind::Id:
  case Sharq::QGateKind::X:
  case Sharq::QGateKind::Z:
  case Sharq::QGateKind::S:
  case Sharq::QGateKind::Sdg:
  case Sharq::QGateKind::T:
  case Sharq::QGateKind::Tdg:
  case Sharq::QGateKind::H:
    if (qid.size() != 1) {
      throw std::runtime_error("related qubit number must be 1.");
    }
    if (p.is_zero() == false) {
      throw std::runtime_error("phase parameter must be zero.");
    }
    break;
  case Sharq::QGateKind::RZ:
    if (qid.size() != 1) {
      throw std::runtime_error("related qubit number must be 1.");
    }
    break;
  case Sharq::QGateKind::CX:
  case Sharq::QGateKind::CZ:
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

Sharq::QGate::QGate(const std::string& qgate_str)
{
  uint32_t pos = qgate_str.find(" ");
  std::string kind_phase_str = qgate_str.substr(0, pos);
  std::tuple<QGateKind, Phase> kp = kind_phase(kind_phase_str);
  kind_ = std::get<0>(kp);
  phase_ = std::get<1>(kp);

  pos = qgate_str.find(" ");
  std::string s = qgate_str.substr(pos, qgate_str.size()-pos);
  std::string qid_str = Sharq::trim(s);
  for (auto& qstr:Sharq::split(qid_str, ' ')) {
    qid_.push_back(stoi(qstr));
  }
  
  op_ = kind_to_op(kind_, phase_);
}

std::vector<std::vector<std::complex<double>>> Sharq::QGate::kind_to_op(const Sharq::QGateKind kind,
									const Sharq::Phase& phase)
{
  std::vector<std::vector<std::complex<double>>> op;
  std::complex I(0.0, 1.0);
  switch (kind) {
  case Sharq::QGateKind::Id:
    op = {{1.0, 0.0},
	  {0.0, 1.0}};
    break;
  case Sharq::QGateKind::X:
    op = {{0.0, 1.0},
	  {1.0, 0.0}};
    break;
  case Sharq::QGateKind::Z:
    op = {{1.0, 0.0},
	  {0.0, -1.0}};
    break;
  case Sharq::QGateKind::S:
    op = {{1.0, 0.0},
	  {0.0, I}};
    break;
  case Sharq::QGateKind::Sdg:
    op = {{1.0, 0.0},
	  {0.0, -I}};
    break;
  case Sharq::QGateKind::T:
    op = {{1.0, 0.0},
	  {0.0, std::exp(M_PI*I/4.0)}};
    break;
  case Sharq::QGateKind::Tdg:
    op = {{1.0, 0.0},
	  {0.0, std::exp(-M_PI*I/4.0)}};
    break;
  case Sharq::QGateKind::H:
    op = {{1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)},
	  {1.0/std::sqrt(2.0), -1.0/std::sqrt(2.0)}};
    break;
  case Sharq::QGateKind::RZ:
    op = {{std::exp(-I*phase.value()/2.0), 0.0},
	  {0.0, std::exp(I*phase.value()/2.0)}};
    break;
  case Sharq::QGateKind::CX:
    op = {{1.0, 0.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0, 0.0},
	  {0.0, 0.0, 0.0, 1.0},
	  {0.0, 0.0, 1.0, 0.0}};
    break;
  case Sharq::QGateKind::CZ:
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

std::string Sharq::QGate::name(bool pi_str) const
{
  std::vector<std::string> s = { "X", "Z", "S", "S+", "T", "T+", "H", "RZ", "CX", "CZ", "Id" };
  Phase p = phase_;
  std::string p_str = "";
  if (kind_ == QGateKind::RZ) p_str = "(" + p.to_string(pi_str) + ")";
  return s[kind_] + p_str;
}

std::string Sharq::QGate::to_string(bool pi_str) const
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

std::tuple<Sharq::QGateKind, Sharq::Phase> Sharq::QGate::kind_phase(const std::string& str)
{
  Sharq::QGateKind kind;
  Sharq::Phase phase;
  
  if (str == "Id") {
    kind = Sharq::QGateKind::Id;
    phase = Sharq::Phase(0);
  }
  else if (str == "X") {
    kind = Sharq::QGateKind::X;
    phase = Sharq::Phase(0);
  }
  else if (str == "Z") {
    kind = Sharq::QGateKind::Z;
    phase = Sharq::Phase(0);
  }
  else if (str == "S") {
    kind = Sharq::QGateKind::S;
    phase = Sharq::Phase(0);
  }
  else if (str == "Sdg") {
    kind = Sharq::QGateKind::Sdg;
    phase = Sharq::Phase(0);
  }
  else if (str == "S+") {
    kind = Sharq::QGateKind::Sdg;
    phase = Sharq::Phase(0);
  }
  else if (str == "T") {
    kind = Sharq::QGateKind::T;
    phase = Sharq::Phase(0);
  }
  else if (str == "Tdg") {
    kind = Sharq::QGateKind::Tdg;
    phase = Sharq::Phase(0);
  }
  else if (str == "T+") {
    kind = Sharq::QGateKind::Tdg;
    phase = Sharq::Phase(0);
  }
  else if (str == "H") {
    kind = Sharq::QGateKind::H;
    phase = Sharq::Phase(0);
  }
  else if (str == "CX") {
    kind = Sharq::QGateKind::CX;
    phase = Sharq::Phase(0);
  }
  else if (str == "CZ") {
    kind = Sharq::QGateKind::CZ;
    phase = Sharq::Phase(0);
  }
  else if (str.find("RZ") == 0) {
    phase = Sharq::Phase(str.substr(3, str.size() - 4));
    phase.mod_2pi();
    if (str.find("RZ") == 0) kind = Sharq::QGateKind::RZ;
  }
  else {
    throw std::runtime_error(str+" is not supported.");
  }

  return {kind, phase};
}

Sharq::QGate Sharq::QGate::inverse() const
{
  Sharq::QGate qgate_inv;
  switch (kind_) {
  case Sharq::QGateKind::Id:
    qgate_inv.kind(Sharq::QGateKind::Id);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::Id));
    break;
  case Sharq::QGateKind::X:
    qgate_inv.kind(Sharq::QGateKind::X);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::X));
    break;
  case Sharq::QGateKind::Z:
    qgate_inv.kind(Sharq::QGateKind::Z);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::Z));
    break;
  case Sharq::QGateKind::S:
    qgate_inv.kind(Sharq::QGateKind::Sdg);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::Sdg));
    break;
  case Sharq::QGateKind::Sdg:
    qgate_inv.kind(Sharq::QGateKind::S);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::S));
    break;
  case Sharq::QGateKind::T:
    qgate_inv.kind(Sharq::QGateKind::Tdg);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::Tdg));
    break;
  case Sharq::QGateKind::Tdg:
    qgate_inv.kind(Sharq::QGateKind::T);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::T));
    break;
  case Sharq::QGateKind::H:
    qgate_inv.kind(Sharq::QGateKind::H);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::H));
    break;
  case Sharq::QGateKind::RZ:
    qgate_inv.kind(Sharq::QGateKind::RZ);
    qgate_inv.qid(qid_);
    qgate_inv.phase(-phase_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::RZ, -phase_));
    break;
  case Sharq::QGateKind::CX:
    qgate_inv.kind(Sharq::QGateKind::CX);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::CX));
    break;
  case Sharq::QGateKind::CZ:
    qgate_inv.kind(Sharq::QGateKind::CZ);
    qgate_inv.qid(qid_);
    qgate_inv.op(kind_to_op(Sharq::QGateKind::CZ));
    break;
  default:
    throw std::runtime_error("unknown QGateKind.");
  }
  return qgate_inv;
}

bool Sharq::QGate::is_identical(QGate& other) const
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

bool Sharq::QGate::overlap(const Sharq::QGate& other) const
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

bool Sharq::QGate::mergeable(const Sharq::QGate& other) const
{
  bool ov = overlap(other);
  if (!ov) return false;

  if (qid_.size() == 1 && other.qid().size() == 1) {
    if (kind_ == other.kind()) return true;
    else if (is_RZ_gate() && other.is_RZ_gate()) return true;
  }
  else if (qid_.size() == 2 && other.qid().size() == 2 && kind_ == other.kind()) {
    if (kind_ == Sharq::QGateKind::CX && qid_[0] == other.qid()[0] && qid_[1] == other.qid()[1]) return true;
    else if (kind_ == Sharq::QGateKind::CZ && qid_[0] == other.qid()[0] && qid_[1] == other.qid()[1]) return true;
    else if (kind_ == Sharq::QGateKind::CZ && qid_[0] == other.qid()[1] && qid_[1] == other.qid()[0]) return true;
  }
  return false;
}

bool Sharq::QGate::commutable(const Sharq::QGate& other) const
{
  /* meargeable -> commutable */
  if (Sharq::QGate::mergeable(other)) return true;

  /* not overlap -> commutable */
  bool ov = overlap(other);
  if (!ov) return true;

  if (qid_.size() == 1 && other.qid().size() == 2 && other.kind() == Sharq::QGateKind::CX) {
    if (is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
  }
  if (qid_.size() == 1 && other.qid().size() == 2 && other.kind() == Sharq::QGateKind::CZ) {
    if (is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (is_RZ_gate() && (qid_[0] == other.qid()[1])) return true;
  }

  if (qid_.size() == 2 && other.qid().size() == 1 && kind_ == Sharq::QGateKind::CX) {
    if (other.is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
  }  
  if (qid_.size() == 2 && other.qid().size() == 1 && kind_ == Sharq::QGateKind::CZ) {
    if (other.is_RZ_gate() && (qid_[0] == other.qid()[0])) return true;
    else if (other.is_RZ_gate() && (qid_[1] == other.qid()[0])) return true;
  }  

  if (qid_.size() == 2 && other.qid().size() == 2) {
    if (kind_ == other.kind() && kind_ == Sharq::QGateKind::CX &&
	((qid_[0] == other.qid()[0]) || (qid_[1] == other.qid()[1]))) return true;
    else if (kind_ == other.kind() && kind_ == Sharq::QGateKind::CZ &&
	((qid_[0] == other.qid()[0]) || (qid_[1] == other.qid()[1]))) return true;
    else if (kind_ == other.kind() && kind_ == Sharq::QGateKind::CZ &&
	((qid_[0] == other.qid()[1]) || (qid_[1] == other.qid()[0]))) return true;
  }
  
  return false;
}

void Sharq::QGate::merge(Sharq::QGate other)
{
  if (kind_ == other.kind() && !is_RZ_gate()) {
    kind(Sharq::QGateKind::Id);
  }
  else {
    Sharq::Phase p(0);
    if (kind_ == Sharq::QGateKind::Z) p += Sharq::Phase(1);
    else if (kind_ == Sharq::QGateKind::S) p += Sharq::Phase(1,2);
    else if (kind_ == Sharq::QGateKind::Sdg) p += Sharq::Phase(-1,2);
    else if (kind_ == Sharq::QGateKind::T) p += Sharq::Phase(1,4);
    else if (kind_ == Sharq::QGateKind::Tdg) p += Sharq::Phase(-1,4);
    else if (kind_ == Sharq::QGateKind::RZ) p += phase_;
    else if (kind_ == Sharq::QGateKind::Id) p += Sharq::Phase(0);

    if (other.kind() == Sharq::QGateKind::Z) p += Sharq::Phase(1);
    else if (other.kind() == Sharq::QGateKind::S) p += Sharq::Phase(1,2);
    else if (other.kind() == Sharq::QGateKind::Sdg) p += Sharq::Phase(-1,2);
    else if (other.kind() == Sharq::QGateKind::T) p += Sharq::Phase(1,4);
    else if (other.kind() == Sharq::QGateKind::Tdg) p += Sharq::Phase(-1,4);
    else if (other.kind() == Sharq::QGateKind::RZ) p += other.phase();
    else if (other.kind() == Sharq::QGateKind::Id) p += Sharq::Phase(0);

    if (p == Phase(0)) {
      kind(Sharq::QGateKind::Id);
    }
    else {
      kind(Sharq::QGateKind::RZ);
      phase(p);
    }
  }
}
