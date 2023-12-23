#ifndef QCIRC_H
#define QCIRC_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>

#include "linear_map.h"
#include "qgate.h"
#include "zx.h"

namespace Sharq {

  class ZXDiagram;
  
  class QCirc
  {
  private:
    uint32_t qubit_num_;
    std::vector<QGate> qgates_;
    void gate_cancel_one_time();
  public:
    QCirc(const uint32_t qubit_num = 0) : qubit_num_(qubit_num) {}
    QCirc(const QCirc& qc) : qubit_num_(qc.qubit_num_), qgates_(qc.qgates_) {}
    QCirc(const std::vector<QGate>& qgates) { for (auto& qgate:qgates) add_qgate(qgate); }
    /* getters */
    uint32_t qubit_num() const { return qubit_num_; }
    std::vector<QGate> qgates() const { return qgates_; }
    /* setters */
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    void qgates(const std::vector<QGate>& qgates) { qgates_ = qgates; }
    /* member functions */
    uint32_t qgate_num() { return qgates_.size(); }
    void save(const std::string& file_name) const;
    void load(const std::string& file_name);
    std::map<std::string, uint32_t> stats() const;
    uint32_t gate_count() const { return (qgates_.size() - id_count()); }
    uint32_t id_count() const;
    uint32_t x_count() const;
    uint32_t z_count() const;
    uint32_t h_count() const;
    uint32_t s_count() const;
    uint32_t t_count() const;
    uint32_t rz_count() const;
    uint32_t cx_count() const;
    uint32_t cz_count() const;
    uint32_t twoq_count() const { return (cx_count() + cz_count()); }
    uint32_t depth() const;
    std::string to_string(const uint32_t width = 100) const;
    void show(const uint32_t width = 100) const { std::cout << to_string(width); }
    void print_qcirc() const { for (auto& g:qgates_) { std::cout << g.to_string(false) << std::endl; }}
    void print_stats() const;
    QCirc& add_qgate(const QGateKind kind, const std::vector<uint32_t>& qid, const Phase& phase = 0);
    QCirc& add_qgate(const QGate& qgate);
    QCirc& add_qcirc(const QCirc& other);
    QCirc& add_random(const uint32_t qubit_num, const uint32_t qgate_num, const nlohmann::json& probs);
    QCirc reverse() const;
    QCirc inverse() const;
    bool is_identical(const QCirc& other) const;
    bool is_equal(const QCirc& other) const;
    void to_dot_file(const std::string& file_name) const;
    void to_svg_file(const std::string& file_name) const;
    ZXDiagram to_zxdiagram() const;
    LinearMap to_linearmap() const;
    void gate_cancel();
    /* fundamental gates */
    QCirc& id(const uint32_t q) { return add_qgate(QGateKind::Id, {q}); }
    QCirc& x(const uint32_t q) { return add_qgate(QGateKind::X, {q}); }
    QCirc& z(const uint32_t q) { return add_qgate(QGateKind::Z, {q}); }
    QCirc& s(const uint32_t q) { return add_qgate(QGateKind::S, {q}); }
    QCirc& sdg(const uint32_t q) { return add_qgate(QGateKind::Sdg, {q}); }
    QCirc& t(const uint32_t q) { return add_qgate(QGateKind::T, {q}); }
    QCirc& tdg(const uint32_t q) { return add_qgate(QGateKind::Tdg, {q}); }
    QCirc& h(const uint32_t q) { return add_qgate(QGateKind::H, {q}); }
    QCirc& rz(const uint32_t q, const Phase& phase) { return add_qgate(QGateKind::RZ, {q}, phase); }
    QCirc& cx(const uint32_t c, const uint32_t t) { return add_qgate(QGateKind::CX, {c, t}); }
    QCirc& cz(const uint32_t c, const uint32_t t) { return add_qgate(QGateKind::CZ, {c, t}); }
    /* compound gates */
    QCirc& rx(const uint32_t q, const Phase& phase) { h(q); rz(q,phase); h(q); return *this; }
    QCirc& y(const uint32_t q) { x(q); z(q); return *this; }
    QCirc& xr(const uint32_t q) { sdg(q); h(q); sdg(q); return *this;}
    QCirc& xrdg(const uint32_t q) { s(q); h(q); s(q); return *this;}
    QCirc& ry(const uint32_t q, const Phase& phase)
    { rx(q, Phase(1,2)); rz(q, phase); rx(q, Phase(-1,2)); return *this; }
    QCirc& p(const uint32_t q, const Phase& phase) { rz(q,phase); return *this; }
    QCirc& cy(const uint32_t con, const uint32_t tar) { cz(con,tar); cx(con,tar); s(con); return *this; }
    QCirc& cxr(const uint32_t con, const uint32_t tar) { crx(con,tar,Phase(1,2)); t(con); return *this; }
    QCirc& cxrdg(const uint32_t con, const uint32_t tar) { tdg(con); crx(con,tar,Phase(-1,2)); return *this; }
    QCirc& ch(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,2)); crx(con,tar,Phase(1,2)); crz(con,tar,Phase(1,2)); s(con); return *this; }
    QCirc& cs(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,2)); t(con); return *this; }
    QCirc& csdg(const uint32_t con, const uint32_t tar)
    { tdg(con); crz(con,tar,Phase(-1,2)); return *this; }
    QCirc& ct(const uint32_t con, const uint32_t tar)
    { crz(con,tar,Phase(1,4)); rz(con,Phase(1,8)); return *this; }
    QCirc& ctdg(const uint32_t con, const uint32_t tar)
    { rz(con,Phase(-1,8)); crz(con,tar,Phase(-1,4)); return *this; }
    QCirc& crx(const uint32_t con, const uint32_t tar, const Phase& phase)
    { h(tar); crz(con,tar,phase); h(tar); return *this; }
    QCirc& cry(const uint32_t con, const uint32_t tar, const Phase& phase)
    { rx(tar,Phase(1,2)); crz(con,tar,phase); rx(tar,Phase(-1,2)); return *this; }
    QCirc& crz(const uint32_t con, const uint32_t tar, const Phase& phase)
    { rz(tar,phase/2); cx(con,tar); rz(tar,-phase/2); cx(con,tar); return *this; }
    QCirc& cp(const uint32_t con, const uint32_t tar, const Phase& phase)
    { crz(con,tar,phase); rz(con,phase/2); return *this; }
    QCirc& rxx(const uint32_t q0, const uint32_t q1, const Phase& phase)
    {
      h(q0); h(q1);
      cx(q0,q1); rz(q1,phase); cx(q0,q1);
      h(q0); h(q1);
      return *this;
    }
    QCirc& ryy(const uint32_t q0, const uint32_t q1, const Phase& phase)
    {
      rx(q0,Phase(1,2)); rx(q1,Phase(1,2));
      cx(q0,q1); rz(q1,phase); cx(q0,q1);
      rx(q0,-Phase(1,2)); rx(q1,-Phase(1,2));
      return *this;
    }
    QCirc& rzz(const uint32_t q0, const uint32_t q1, const Phase& phase)
    { cx(q0,q1); rz(q1,phase); cx(q0,q1); return *this; }
    QCirc& sw(const uint32_t q0, const uint32_t q1)
    { cx(q0,q1); cx(q1,q0); cx(q0,q1); return *this; }
    QCirc& csw(const uint32_t con, const uint32_t q0, const uint32_t q1) // Fredkin gate
    { cx(q1,q0); ccx(con,q0,q1); cx(q1,q0); return *this; }
    QCirc& ccx(const uint32_t c0, const uint32_t c1, const uint32_t tar) // Toffoli gate
    { cxr(c1,tar); cx(c0,c1); cxrdg(c1,tar); cx(c0,c1); cxr(c0,tar); return *this; }
    /* operator overloadding */
    QCirc& operator+=(const QCirc& rhs) { return add_qcirc(rhs); }
    friend QCirc operator+(const QCirc& lhs, const QCirc& rhs) { QCirc qc = lhs; return qc.add_qcirc(rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const QCirc& qc) { ost << qc.to_string(); return ost; }
  };
}

#endif
