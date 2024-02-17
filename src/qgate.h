#ifndef QGATE_H
#define QGATE_H

#include <iostream>
#include <string>
#include <vector>
#include <complex>

#include "phase.h"

namespace Sharq {

  enum QGateKind { X, Z, S, Sdg, T, Tdg, H, RZ, CX, CZ, Id, Id2 };

  class QGate
  {
  private:
    QGateKind kind_;
    std::vector<uint32_t> qid_;
    Phase phase_;
    std::vector<std::vector<std::complex<double>>> op_;
    /* member functions */
    static std::vector<std::vector<std::complex<double>>> kind_to_op(const QGateKind kind, const Phase& phase = 0);
  public:
    QGate(const QGateKind kind = QGateKind::Id, const std::vector<uint32_t>& qid = {0}, const Phase& phase = 0);
    QGate(const std::string& qgate_str);
    QGate(const QGate& qgate) : kind_(qgate.kind_), qid_(qgate.qid_), phase_(qgate.phase_), op_(qgate.op_) {}
    /* getters */
    QGateKind kind() const { return kind_; }
    std::vector<uint32_t> qid() const { return qid_; }
    Phase phase() const { return phase_; }
    std::vector<std::vector<std::complex<double>>> op() const { return op_; }
    /* setters */
    void kind(const QGateKind kind) { kind_ = kind; }
    void qid(const std::vector<uint32_t>& qid) { qid_ = qid; }
    void phase(const Phase& phase) { phase_ = phase; }
    void op(const std::vector<std::vector<std::complex<double>>>& op) { op_ = op; }
    /* member functions */
    std::string name(bool pi_str = true) const;
    std::string to_string(bool pi_str = true) const;
    uint32_t qubit_num() const { return qid_.size(); }
    bool is_Id_gate() const { return (kind_ == QGateKind::Id); }
    bool is_Id2_gate() const { return (kind_ == QGateKind::Id2); }
    bool is_X_gate() const { return (kind_ == QGateKind::X); }
    bool is_Z_gate() const
    {
      if (kind_ == QGateKind::Z) return true;
      else if (kind_ == QGateKind::RZ && phase_ == Phase(1)) return true;
      return false;
    }
    bool is_H_gate() const { return (kind_ == QGateKind::H); }
    bool is_S_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,2) || phase_ == Phase(-1,2)) ans = true;
      }
      else if (kind_ == QGateKind::S || kind_ == QGateKind::Sdg) { ans = true; }
      return ans;
    }
    bool is_S1_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ && phase_ == Phase(1,2)) ans = true;
      else if (kind_ == QGateKind::S) ans = true;
      return ans;
    }
    bool is_S3_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ && phase_ == Phase(3,2)) ans = true;
      else if (kind_ == QGateKind::Sdg) ans = true;
      return ans;
    }
    bool is_T_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,4) || phase_ == Phase(-1,4)) ans = true;
      }
      else if (kind_ == QGateKind::T || kind_ == QGateKind::Tdg) { ans = true; }
      return ans;
    }
    bool is_T1_gate() const // phase=1/4
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,4)) ans = true;
      }
      else if (kind_ == QGateKind::T) { ans = true; }
      return ans;
    }
    bool is_T7_gate() const // phase=7/4=-1/4
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(7,4)) ans = true;
      }
      else if (kind_ == QGateKind::Tdg) { ans = true; }
      return ans;
    }
    bool is_CX_gate() const { return (kind_ == QGateKind::CX); }
    bool is_CZ_gate() const { return (kind_ == QGateKind::CZ); }
    bool is_RZ_gate() const { return (kind_ == QGateKind::Z || kind_ == QGateKind::S || kind_ == QGateKind::Sdg ||
				      kind_ == QGateKind::T || kind_ == QGateKind::Tdg || kind_ == QGateKind::RZ ||
				      kind_ == QGateKind::Id); }
    bool is_pauli_gate() const { return (is_X_gate() || is_Z_gate() || is_Id_gate()); }
    bool is_proper_clifford_gate() const { return is_S_gate(); }
    bool is_clifford_gate() const{ return (is_pauli_gate() || is_H_gate() || is_CX_gate() || is_S_gate()); }
    bool is_non_clifford_gate() const { return !(is_clifford_gate()); }
    bool is_included(const uint32_t q) const
    {
      for (auto& i:qid_) { if (i == q) return true; }
      return false;
    }
    QGate inverse() const;
    bool is_identical(QGate& other) const;
    bool overlap(const Sharq::QGate& other) const;
    bool mergeable(const QGate& other) const;
    bool commutable(const QGate& other) const;
    void merge(const QGate& other);
    static std::tuple<QGateKind, Phase> kind_phase(const std::string& str);
    friend std::ostream& operator<<(std::ostream& ost, const QGate& qgate) { ost << qgate.to_string(); return ost; }
  };
}

#endif
