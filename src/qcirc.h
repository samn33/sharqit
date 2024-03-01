/**
 * @file qcirc.h
 * @brief include file of QCirc class
 */

#ifndef QCIRC_H
#define QCIRC_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <limits>
#include <nlohmann/json.hpp>

#include "linear_map.h"
#include "qgate.h"
#include "zx.h"
#include "dag.h"

namespace Sharq {

  class ZXDiagram;
  class DAGCirc;
  class BinaryMatrix;
  
  /**
   * @class QCirc
   * @brief quantum circuit
  */
  class QCirc
  {
  private:
    uint32_t qubit_num_; //!< number of qubits
    std::vector<QGate> qgates_; //!< vector of the QGate object
    //void gate_cancel_one_time();
    /**
     * @brief merge rotation gates one time
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void merge_rotation_one_time(const uint32_t start, const uint32_t end,
				 const std::vector<uint32_t>& ppc_qid,
				 const std::vector<std::pair<uint32_t, uint32_t>>& term_border,
				 const std::vector<uint8_t>& cnot_objects);
    /**
     * @brief reach the termination border or not
     * @param [in] idx quantum gate id
     * @param [in] q qubit id
     * @return true if you reach the termination border, false otherwise
     */
    bool is_termination_border(const uint32_t idx, const uint32_t q); // for merge_rotation
  public:
    /**
     * @brief constructor of the QCirc
     * @param [in] qubit_num number of qubits
     */
    QCirc(const uint32_t qubit_num = 0) : qubit_num_(qubit_num) {}
    /**
     * @brief copy constructor of the QCirc
     * @param [in] qc the quantum circuit
     */
    QCirc(const QCirc& qc) : qubit_num_(qc.qubit_num_), qgates_(qc.qgates_) {}
    /**
     * @brief constructor of the QCirc
     * @param [in] qgates vector of the QGate objects
     */
    QCirc(const std::vector<QGate>& qgates) { for (auto& qgate:qgates) add_qgate(qgate); }
    //! getter of the qubit_num_
    uint32_t qubit_num() const { return qubit_num_; }
    //! getter of the qgates_
    std::vector<QGate> qgates() const { return qgates_; }
    //! setter of the qubit_num_
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    //! setter of the qgates_
    void qgates(const std::vector<QGate>& qgates) { qgates_ = qgates; }
    /**
     * @brief get a number of the quantum gates
     * @return number of quantum gates
     */
    uint32_t qgate_num() const { return qgates_.size(); }
    /**
     * @brief save the quantum circuit to a file
     * @param [in] file_name file name you want to save
     */
    void save(const std::string& file_name) const;
    /**
     * @brief load the quantum circuit from a file
     * @param [in] file_name file name you want to load
     */
    void load(const std::string& file_name);
    /**
     * @brief clear the quantum circit
     */
    void clear() { qgates_.clear(); qubit_num_ = 0; }
    /**
     * @brief get stats of the quantum circuit 
     * @return stats of the quantum circuit
     */
    std::map<std::string, uint32_t> stats() const;
    /**
     * @brief get a number of the quantum gates except to identity gates
     * @return number of quantum gates except to identity gates
     */
    uint32_t gate_count() const { return (qgates_.size() - id_count()); }
    /**
     * @brief get a number of identity gates in the quantum circuit
     * @return number of identity gates
     */
    uint32_t id_count() const;
    /**
     * @brief get a number of pauli-X gates in the quantum circuit
     * @return number of pauli-X gates
     */
    uint32_t x_count() const;
    /**
     * @brief get a number of pauli-Z gates in the quantum circuit
     * @return number of pauli-Z gates
     */
    uint32_t z_count() const;
    /**
     * @brief get a number of H (hadamard) gates in the quantum circuit
     * @return number of H (hadamard) gates
     */
    uint32_t h_count() const;
    /**
     * @brief get a number of S and S+ gates in the quantum circuit
     * @return number of S and S+ gates
     */
    uint32_t s_count() const;
    /**
     * @brief get a number of T and T+ gates in the quantum circuit
     * @return number of T and T+ gates
     */
    uint32_t t_count() const;
    /**
     * @brief get a number of RZ gates in the quantum circuit
     * @return number of RZ gates
     */
    uint32_t rz_count() const;
    /**
     * @brief get a number of CX (CNOT) gates in the quantum circuit
     * @return number of CX (CNOT) gates
     */
    uint32_t cx_count() const;
    /**
     * @brief get a number of CZ gates in the quantum circuit
     * @return number of CZ gates
     */
    uint32_t cz_count() const;
    /**
     * @brief get a number of two-qubit gates in the quantum circuit
     * @return number of two-qubit gates
     */
    uint32_t twoq_count() const { return (cx_count() + cz_count()); }
    /**
     * @brief get a depth of the quantum circuit
     * @return depth of the quantum circuit
     */
    uint32_t depth() const;
    /**
     * @brief get a circuit diagram as ascii text of the quantum circuit
     * @return circuit diagram as ascii text
     */
    std::string to_string(const uint32_t width = 100) const;
    /**
     * @brief show the quantum circuit diagram as a ascii text
     * @param [in] width folding width of the diagram
     */
    void show(const uint32_t width = 100) const { std::cout << to_string(width); }
    /**
     * @brief print list of gates in the quantum circuit
     */
    void print_qcirc() const { for (auto& g:qgates_) { std::cout << g.to_string(false) << std::endl; }}
    /**
     * @brief print stats of the quantum circuit
     */
    void print_stats() const;
    /**
     * @brief add a quantum gate to the quantum circuit
     * @param [in] kind kind of the quantum gate
     * @param [in] qid qubit id od the quantum gate ({q} for single-qubit gate, {control,target} for two-qubit gate)
     * @param [in] phase phase of the rotation gate
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& add_qgate(const QGateKind kind, const std::vector<uint32_t>& qid, const Phase& phase = 0);
    /**
     * @brief add a quantum gate to the quantum circuit
     * @param [in] qgate QGate object
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& add_qgate(const QGate& qgate);
    /**
     * @brief add a quantum circuit to the quantum circuit
     * @param [in] other QCirc object you want to add
     * @return reference of the QCirc after adding the quantum circuit
     */
    QCirc& add_qcirc(const QCirc& other);
    /**
     * @brief add a random quantum circuit to the quantum circuit
     * @param [in] qubit_num number of qubits of the quantum circuit
     * @param [in] qgate_num number of quantum gates in the quantum circuit
     * @param [in] probs probability of each quantum gate
     * @return reference of the QCirc after adding the quantum circuit
     * @note example of probs: @n {{"X", 4},{"T", 5},{"CX", 3}} @n {{"H", 0.3},{"S", 0.2},{"RZ(1/4)", 0.4}}
     */
    QCirc& add_random(const uint32_t qubit_num, const uint32_t qgate_num, const nlohmann::json& probs);
    /**
     * @brief get a reverse ordered quantum circuit from the quantum circuit
     * @return reverse ordered quantum circuit
     * @note example of a quantum circuit @n reverse of "q[0] --S--H--T--" is "q[0] --T--H--S--"
     */
    QCirc reverse() const;
    /**
     * @brief get a inverse of the quantum circuit
     * @return inverse of the quantum circuit
     * @note example of a inverse of the quantum circuit. @n
     *       inverse of "q[0] --S--H--T--" is "q[0] --T+--H--S+--".
     */
    QCirc inverse() const;
    /**
     * @brief is the quantum circuit identical to the other
     * @param [in] other quantum circuit
     * @return true if the quantum circuit is identical to the other, false otherwise
     * @note "identical" means that two circuits have same quantum gates list.
     */
    bool is_identical(const QCirc& other) const;
    /**
     * @brief is the quantum circuit equal to the other
     * @param [in] other quantum circuit
     * @return true if the quantum circuit is equal to the other, false otherwise
     * @note "equal" means that two circuits are same unitary operation. @n
     *       "--S--T--T+--S--" and "--Z--" are equal, but not identical.
     */
    bool is_equal(const QCirc& other) const;
    /**
     * @brief save as a dot file
     * @param [in] file_name dot file name
     */
    void to_dot_file(const std::string& file_name) const;
    /**
     * @brief save as a svg file
     * @param [in] file_name svg file name
     */
    void to_svg_file(const std::string& file_name) const;
    /**
     * @brief convert to a ZXDiagram
     * @return ZXDiagram object
     */
    ZXDiagram to_zxdiagram() const;
    /**
     * @brief convert to a linear map
     * @return LinearMap object
     */
    LinearMap to_linearmap() const;
    /**
     * @brief convert to a DAG quantum circuit
     * @return DAGCirc object
     * @note "DAG" means "Directed Acyclic Graph"
     */
    DAGCirc to_dagcirc() const;
    /**
     * @brief remove identity gates
     */
    void remove_id();
    /**
     * @brief replace with RZ gates
     * @details all Z,S,S+,T,T+ gates are replaced with RZ gates
     */
    void replace_with_rz();
    /**
     * @brief merge rotation gates using phase polynomial
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void merge_rotation();
    /**
     * @brief propagate pauli-X gates after CNOT gates
     */
    void propagate_pauli_x();
    /**
     * @brief all CZ gates are converted to CX gates
     */
    void cz_to_cx();
    //void gate_cancel();
    /**
     * @brief add an identity gate to the quantum circuit
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& id(const uint32_t q) { return add_qgate(QGateKind::Id, {q}); }
    /**
     * @brief add a pauli-X gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& x(const uint32_t q) { return add_qgate(QGateKind::X, {q}); }
    /**
     * @brief add a pauli-Z gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& z(const uint32_t q) { return add_qgate(QGateKind::Z, {q}); }
    /**
     * @brief add a S gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& s(const uint32_t q) { return add_qgate(QGateKind::S, {q}); }
    /**
     * @brief add a S+ (hermitian conjugate of S) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& sdg(const uint32_t q) { return add_qgate(QGateKind::Sdg, {q}); }
    /**
     * @brief add a T gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& t(const uint32_t q) { return add_qgate(QGateKind::T, {q}); }
    /**
     * @brief add a T+ (hermitian conjugate of T) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& tdg(const uint32_t q) { return add_qgate(QGateKind::Tdg, {q}); }
    /**
     * @brief add a H (hadamard) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& h(const uint32_t q) { return add_qgate(QGateKind::H, {q}); }
    /**
     * @brief add a RZ gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& rz(const uint32_t q, const Phase& phase) { return add_qgate(QGateKind::RZ, {q}, phase); }
    /**
     * @brief add a two-qubit identity gate to the quantum circuit
     * @param [in] c qubit id (control) to be applied
     * @param [in] t qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& id2(const uint32_t c, const uint32_t t) { return add_qgate(QGateKind::Id2, {c, t}); }
    /**
     * @brief add a CX (CNOT) gate to the quantum circuit
     * @param [in] c qubit id (control) to be applied
     * @param [in] t qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cx(const uint32_t c, const uint32_t t) { return add_qgate(QGateKind::CX, {c, t}); }
    /**
     * @brief add a CZ gate to the quantum circuit
     * @param [in] c qubit id (control) to be applied
     * @param [in] t qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cz(const uint32_t c, const uint32_t t) { return add_qgate(QGateKind::CZ, {c, t}); }
    /**
     * @brief add a RX gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& rx(const uint32_t q, const Phase& phase) { h(q); rz(q,phase); h(q); return *this; }
    /**
     * @brief add a pauli-Y gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& y(const uint32_t q) { x(q); z(q); return *this; }
    /**
     * @brief add a sqrt-X (squre root of pauli-X) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& sx(const uint32_t q) { sdg(q); h(q); sdg(q); return *this;}
    /**
     * @brief add a sqrt-X+ (hermitian conjugate of squre root of pauli-X) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& sxdg(const uint32_t q) { s(q); h(q); s(q); return *this;}
    /**
     * @brief add a RY gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ry(const uint32_t q, const Phase& phase)
    { rx(q, Phase(1,2)); rz(q, phase); rx(q, Phase(-1,2)); return *this; }
    /**
     * @brief add a P (phase shift) gate to the quantum circuit
     * @param [in] q qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& p(const uint32_t q, const Phase& phase) { rz(q,phase); return *this; }
    /**
     * @brief add a CY (controlled-Y) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cy(const uint32_t con, const uint32_t tar) { cz(con,tar); cx(con,tar); s(con); return *this; }
    /**
     * @brief add a controlled-sqrt-X gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& csx(const uint32_t con, const uint32_t tar) { crx(con,tar,Phase(1,2)); t(con); return *this; }
    /**
     * @brief add a hermitian conjugate of controlled-sqrt-X gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& csxdg(const uint32_t con, const uint32_t tar) { tdg(con); crx(con,tar,Phase(-1,2)); return *this; }
    /**
     * @brief add a CH (controlled-hadamard) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ch(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,2)); crx(con,tar,Phase(1,2)); crz(con,tar,Phase(1,2)); s(con); return *this; }
    /**
     * @brief add a CS (controlled-S) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cs(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,2)); t(con); return *this; }
    /**
     * @brief add a CS+ (hermitian conjugate of controlled-S) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& csdg(const uint32_t con, const uint32_t tar)
    { tdg(con); crz(con,tar,Phase(-1,2)); return *this; }
    /**
     * @brief add a CT (controlled-T) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ct(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,4)); rz(con,Phase(1,8)); return *this; }
    /**
     * @brief add a CT+ (hermitian conjugate of controlled-T) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ctdg(const uint32_t con, const uint32_t tar)
    { rz(con,Phase(-1,8)); crz(con,tar,Phase(-1,4)); return *this; }
    /**
     * @brief add a controlled-RX gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& crx(const uint32_t con, const uint32_t tar, const Phase& phase)
    { h(tar); crz(con,tar,phase); h(tar); return *this; }
    /**
     * @brief add a controlled-RY gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cry(const uint32_t con, const uint32_t tar, const Phase& phase)
    { rx(tar,Phase(1,2)); crz(con,tar,phase); rx(tar,Phase(-1,2)); return *this; }
    /**
     * @brief add a controlled-RZ gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& crz(const uint32_t con, const uint32_t tar, const Phase& phase)
    { rz(tar,phase/2); cx(con,tar); rz(tar,-phase/2); cx(con,tar); return *this; }
    /**
     * @brief add a controlled-P gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] tar qubit id (target) to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& cp(const uint32_t con, const uint32_t tar, const Phase& phase)
    { crz(con,tar,phase); rz(con,phase/2); return *this; }
    /**
     * @brief add a ising coupling gate (XX-interaction) to the quantum circuit
     * @param [in] q0 qubit id to be applied
     * @param [in] q1 qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& rxx(const uint32_t q0, const uint32_t q1, const Phase& phase)
    {
      h(q0); h(q1);
      cx(q0,q1); rz(q1,phase); cx(q0,q1);
      h(q0); h(q1);
      return *this;
    }
    /**
     * @brief add a ising coupling gate (YY-interaction) to the quantum circuit
     * @param [in] q0 qubit id to be applied
     * @param [in] q1 qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ryy(const uint32_t q0, const uint32_t q1, const Phase& phase)
    {
      rx(q0,Phase(1,2)); rx(q1,Phase(1,2));
      cx(q0,q1); rz(q1,phase); cx(q0,q1);
      rx(q0,-Phase(1,2)); rx(q1,-Phase(1,2));
      return *this;
    }
    /**
     * @brief add a ising coupling gate (ZZ-interaction) to the quantum circuit
     * @param [in] q0 qubit id to be applied
     * @param [in] q1 qubit id to be applied
     * @param [in] phase phase of the rotation
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& rzz(const uint32_t q0, const uint32_t q1, const Phase& phase)
    { cx(q0,q1); rz(q1,phase); cx(q0,q1); return *this; }
    /**
     * @brief add a swap gate to the quantum circuit
     * @param [in] q0 qubit id to be applied
     * @param [in] q1 qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& sw(const uint32_t q0, const uint32_t q1)
    { cx(q0,q1); cx(q1,q0); cx(q0,q1); return *this; }
    /**
     * @brief add a controlled swap (fredkin) gate to the quantum circuit
     * @param [in] con qubit id (control) to be applied
     * @param [in] q0 qubit id to be applied
     * @param [in] q1 qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& csw(const uint32_t con, const uint32_t q0, const uint32_t q1) // Fredkin gate
    { cx(q1,q0); ccx(con,q0,q1); cx(q1,q0); return *this; }
    /**
     * @brief add a controlled cnot (toffoli) gate to the quantum circuit
     * @param [in] c0 qubit id (control) to be applied
     * @param [in] c1 qubit id (control) to be applied
     * @param [in] tar qubit id to be applied
     * @return reference of the QCirc after adding the quantum gate
     */
    QCirc& ccx(const uint32_t c0, const uint32_t c1, const uint32_t tar) // Toffoli gate
    { csx(c1,tar); cx(c0,c1); csxdg(c1,tar); cx(c0,c1); csx(c0,tar); return *this; }
    QCirc& operator+=(const QCirc& rhs) { return add_qcirc(rhs); }
    friend QCirc operator+(const QCirc& lhs, const QCirc& rhs) { QCirc qc = lhs; return qc.add_qcirc(rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const QCirc& qc) { ost << qc.to_string(); return ost; }
  };
}

#endif
