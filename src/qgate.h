/**
 * @file qgate.h
 * @brief include file of QGate class
 */

#ifndef QGATE_H
#define QGATE_H

#include <iostream>
#include <string>
#include <vector>
#include <complex>

#include "phase.h"

namespace Sharq {

  //! kind of quantum gate
  enum QGateKind {
		  X, //!< pauli-X gate
		  Z, //!< pauli-Z gage
		  S, //!< S gate
		  Sdg, //!< S+ gate (hermitian conjugagte of S gate)
		  T, //!< T gate
		  Tdg, //!< T+ gate (hermitian conjugagte of T gate)
		  H, //!< H (hadamard) gate
		  RZ, //!< RZ gate
		  CX, //!< CNOT gate
		  CZ, //!< CZ gate
		  Id, //!< single-qubit identity gate
		  Id2 //!< two-qubit identity gate
  };

  /**
   * @class QGate
   * @brief quantum gate
  */
  class QGate
  {
  private:
    QGateKind kind_; //!< kind of quantum gate
    std::vector<uint32_t> qid_; //!< vector of qubit indexes
    Phase phase_; //!< phase of rotation gate
    std::vector<std::vector<std::complex<double>>> op_; //!< complex matrix for the quantum gate operation
    /**
     * @brief get an operation matrix from kind of the quantum gate
     * @param [in] kind kind of the quantum gate
     * @param [in] phase phase of the rotation
     * @return operation matrix of the quantum gate
     */
    static std::vector<std::vector<std::complex<double>>> kind_to_op(const QGateKind kind, const Phase& phase = 0);
  public:
    /**
     * @brief constructor of the QGate
     * @param [in] kind kind of the quantum gate
     * @param [in] qid vector of qubit indexes of the quantum gate
     * @param [in] phase phase of the rotation gate
     */
    QGate(const QGateKind kind = QGateKind::Id, const std::vector<uint32_t>& qid = {0}, const Phase& phase = 0);
    /**
     * @brief constructor of the QGate
     * @param [in] qgate_str string representation of the quantum gate
     * @note [examples] @n qgate_str = "x 1", "CX 0 1", "RZ(1/4) 2" ...
     */
    QGate(const std::string& qgate_str);
    /**
     * @brief copy constructor of the QGate
     * @param [in] qgate QGate object
     */
    QGate(const QGate& qgate) : kind_(qgate.kind_), qid_(qgate.qid_), phase_(qgate.phase_), op_(qgate.op_) {}
    //! getter of the kind_
    QGateKind kind() const { return kind_; }
    //! getter of the qid_
    std::vector<uint32_t> qid() const { return qid_; }
    //! getter of the phase_
    Phase phase() const { return phase_; }
    //! getter of the op_
    std::vector<std::vector<std::complex<double>>> op() const { return op_; }
    //! setter of the kind_
    void kind(const QGateKind kind) { kind_ = kind; }
    //! setter of the qid_
    void qid(const std::vector<uint32_t>& qid) { qid_ = qid; }
    //! setter of the phase_
    void phase(const Phase& phase) { phase_ = phase; }
    //! setter of the op_
    void op(const std::vector<std::vector<std::complex<double>>>& op) { op_ = op; }
    /**
     * @brief get a name of the quantum gates
     * @param [in] pi_str whether to include a pi string
     * @return name of the quantum gates
     */
    std::string name(bool pi_str = true) const;
    /**
     * @brief get a string of the QGate object
     * @param [in] pi_str whether to include a pi string
     * @return string of the quantum gate
     */
    std::string to_string(bool pi_str = true) const;
    /**
     * @brief get a number of qubits related to the quantum gate (1 or 2)
     * @return number of qubits
     */
    uint32_t qubit_num() const { return qid_.size(); }
    /**
     * @brief single-qubit identity gate or not
     * @return true if it's an single-qubit identity gate, false otherwise
     */
    bool is_Id_gate() const { return (kind_ == QGateKind::Id); }
    /**
     * @brief two-qubit identity gate or not
     * @return true if it's an two-qubit identity gate, false otherwise
     */
    bool is_Id2_gate() const { return (kind_ == QGateKind::Id2); }
    /**
     * @brief pauli-X gate or not
     * @return true if it's a pauli-X gate, false otherwise
     */
    bool is_X_gate() const { return (kind_ == QGateKind::X); }
    /**
     * @brief pauli-Z gate or not
     * @return true if it's a pauli-Z gate or a RZ gate with pi-phase, false otherwise
     */
    bool is_Z_gate() const
    {
      if (kind_ == QGateKind::Z) return true;
      else if (kind_ == QGateKind::RZ && phase_ == Phase(1)) return true;
      return false;
    }
    /**
     * @brief H (hadamard) gate or not
     * @return true if it's a H gate, false otherwise
     */
    bool is_H_gate() const { return (kind_ == QGateKind::H); }
    /**
     * @brief S,S+ gate or not
     * @return true if it's a S gate or a RZ gate with pi/2-phase, 3*pi/2-phase, false otherwise
     */
    bool is_S_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,2) || phase_ == Phase(-1,2)) ans = true;
      }
      else if (kind_ == QGateKind::S || kind_ == QGateKind::Sdg) { ans = true; }
      return ans;
    }
    /**
     * @brief S gate or not
     * @return true if it's a S gate or a RZ gate with pi/2-phase, false otherwise
     */
    bool is_S1_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ && phase_ == Phase(1,2)) ans = true;
      else if (kind_ == QGateKind::S) ans = true;
      return ans;
    }
    /**
     * @brief S+ gate or not
     * @return true if it's a S+ gate or a RZ gate with 3*pi/2-phase, false otherwise
     */
    bool is_S3_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ && phase_ == Phase(3,2)) ans = true;
      else if (kind_ == QGateKind::Sdg) ans = true;
      return ans;
    }
    /**
     * @brief T,T+ gate or not
     * @return true if it's a T gate or a RZ gate with pi/4-phase, 7*pi/4-phase, false otherwise
     */
    bool is_T_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,4) || phase_ == Phase(-1,4)) ans = true;
      }
      else if (kind_ == QGateKind::T || kind_ == QGateKind::Tdg) { ans = true; }
      return ans;
    }
    /**
     * @brief T gate or not
     * @return true if it's a T gate or a RZ gate with pi/4-phase, false otherwise
     */
    bool is_T1_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,4)) ans = true;
      }
      else if (kind_ == QGateKind::T) { ans = true; }
      return ans;
    }
    /**
     * @brief T+ gate or not
     * @return true if it's a T+ gate or a RZ gate with 7*pi/4-phase, false otherwise
     */
    bool is_T7_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(7,4)) ans = true;
      }
      else if (kind_ == QGateKind::Tdg) { ans = true; }
      return ans;
    }
    /**
     * @brief CX (CNOT) gate or not
     * @return true if it's a CX gate, false otherwise
     */
    bool is_CX_gate() const { return (kind_ == QGateKind::CX); }
    /**
     * @brief CZ gate or not
     * @return true if it's a CZ gate, false otherwise
     */
    bool is_CZ_gate() const { return (kind_ == QGateKind::CZ); }
    /**
     * @brief RZ gate or not
     * @return true if it's a RZ gate, false otherwise
     */
    bool is_RZ_gate() const { return (kind_ == QGateKind::Z || kind_ == QGateKind::S || kind_ == QGateKind::Sdg ||
				      kind_ == QGateKind::T || kind_ == QGateKind::Tdg || kind_ == QGateKind::RZ ||
				      kind_ == QGateKind::Id); }
    /**
     * @brief pauli gate or not
     * @return true if it's a pauli gate, false otherwise
     */
    bool is_pauli_gate() const { return (is_X_gate() || is_Z_gate() || is_Id_gate()); }
    /**
     * @brief proper clifford gate or not
     * @return true if it's a proper clifford gate, false otherwise
     */
    bool is_proper_clifford_gate() const { return is_S_gate(); }
    /**
     * @brief clifford gate or not
     * @return true if it's a clifford gate, false otherwise
     */
    bool is_clifford_gate() const{ return (is_pauli_gate() || is_H_gate() || is_CX_gate() || is_S_gate()); }
    /**
     * @brief non-clifford gate or not
     * @return true if it's a non-clifford gate, false otherwise
     */
    bool is_non_clifford_gate() const { return !(is_clifford_gate()); }
    /**
     * @brief include the qubit id or not
     * @param [in] q qubit id
     * @return true if it includes the qubit id, false otherwise
     */
    bool is_included(const uint32_t q) const
    {
      for (auto& i:qid_) { if (i == q) return true; }
      return false;
    }
    /**
     * @brief get an inverse gate
     * @return inverse gate
     */
    QGate inverse() const;
    /**
     * @brief is the quantum gate identical to the other
     * @param [in] other quantum gate
     * @return true if the quantum gate is identical to the other, false otherwise
     */
    bool is_identical(QGate& other) const;
    /**
     * @brief overlap or not
     * @param [in] other quantum gate
     * @return true if the quantum gate is overlap to the other, false otherwise
     */
    bool overlap(const Sharq::QGate& other) const;
    /**
     * @brief mergeable or not
     * @param [in] other quantum gate
     * @return true if the quantum gate is mergeable to the other, false otherwise
     */
    bool mergeable(const QGate& other) const;
    /**
     * @brief commutable or not
     * @param [in] other quantum gate
     * @return true if the quantum gate is commutable to the other, false otherwise
     */
    bool commutable(const QGate& other) const;
    /**
     * @brief merge to the other quantum gate
     * @param [in] other quantum gate
     */
    void merge(const QGate& other);
    /**
     * @brief get a kind and phase from the quantum gate string
     * @param [in] str quantum gate string
     * @return tuple of the kind and the phase
     */
    static std::tuple<QGateKind, Phase> kind_phase(const std::string& str);
    friend std::ostream& operator<<(std::ostream& ost, const QGate& qgate) { ost << qgate.to_string(); return ost; }
  };
}

#endif
