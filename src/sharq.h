#ifndef SHARQ_H
#define SHARQ_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <numeric>
#include <limits>
#include <cstdint>
#include <random>
#include <chrono>
#include <cmath>
#include <complex>
#include <regex>
#include <bitset>
#include <omp.h>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>

namespace Sharq {

  constexpr char VERSION[] = "v0.0.4";
  constexpr double EPS = 1.0e-8;

  class Fraction;
  class Phase;
  class QGate;
  class QCirc;
  class ZXNode;
  class ZXEdge;
  class ZXDiagram;
  class LinearMap;
  class BinaryMatrix;
  
  /* Fraction */

  class Fraction
  {
  private:
    int32_t numerator_;
    int32_t denominator_;
  public:
    Fraction() : numerator_(1), denominator_(1) {}
    Fraction(const int32_t numerator, const int32_t denominator = 1) :
      numerator_(numerator), denominator_(denominator) { reduce(); }
    Fraction(const Fraction& frac) : numerator_(frac.numerator_), denominator_(frac.denominator_) {}
    /* getters */
    int32_t numerator() const { return numerator_; }
    int32_t denominator() const { return denominator_; }
    /* setters */
    void numerator(const int32_t numerator) { numerator_ = numerator; reduce(); }
    void denominator(const int32_t denominator) { denominator_ = denominator; reduce(); }
    /* member functions */
    std::string to_string() const;
    Fraction add(const Fraction& other) const
    {
      int32_t n = numerator_ * other.denominator() + other.numerator() * denominator_;
      int32_t d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction add(const int32_t other) const { return add(Fraction(other)); }
    Fraction sub(const Fraction& other) const
    {
      int n = numerator_ * other.denominator() - other.numerator() * denominator_;
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction sub(const int32_t other) const { return sub(Fraction(other)); }
    Fraction mul(const Fraction& other) const
    {
      int n = numerator_ * other.numerator();
      int d = denominator_ * other.denominator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction mul(const int32_t other) const { return mul(Fraction(other)); }
    //Fraction div(const Fraction& other) const;
    Fraction div(const Fraction& other) const
    {
      int n = numerator_ * other.denominator();
      int d = denominator_ * other.numerator();
      return Fraction(n / Sharq::Fraction::gcd(n, d), d / Sharq::Fraction::gcd(n, d));
    }
    Fraction div(const int32_t other) const { return div(Fraction(other)); }
    bool is_zero() const { return numerator_ == 0; }
    bool is_positive() const { return numerator_ * denominator_ > 0; }
    bool is_negative() const { return numerator_ * denominator_ < 0; }
    void reduce()
    {
      /* make denominator positive if denominator < 0 */
      if (denominator_ == 0) { throw std::runtime_error("You can't devide by zero."); }
      else if (denominator_ < 0) {
	numerator_ = -numerator_;
	denominator_ = -denominator_;
      }
      /* reduce */
      //int32_t fac = std::abs(std::gcd(numerator_, denominator_));
      int32_t fac = std::abs(gcd(numerator_, denominator_));
      if (fac != 0 && fac != 1) {
	numerator_ /= fac;
	denominator_ /= fac;
      }
    }
    //static int32_t gcd(const int32_t n, const int32_t d);
    static int32_t gcd(const int32_t n, const int32_t d)
    {
      int32_t remainder;
      int32_t nn = std::abs(n);
      int32_t dd = std::abs(d);
      while (dd != 0) {
	remainder = nn % dd;
	nn = dd;
	dd = remainder;
      }
      return nn;
    }
    Fraction operator+() const { return *this; }
    Fraction operator-() const { return mul(-1); }
    Fraction& operator+=(const Fraction& rhs)
    {
      Fraction frac = add(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator+=(const int32_t rhs) { return operator+=(Fraction(rhs)); }
    Fraction& operator-=(const Fraction& rhs)
    {
      Fraction frac = sub(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator-=(const int32_t rhs) { return operator-=(Fraction(rhs)); }
    Fraction& operator*=(const Fraction& rhs)
    {
      Fraction frac = mul(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator*=(const int32_t rhs) { return operator*=(Fraction(rhs)); }
    Fraction& operator/=(const Fraction& rhs)
    {
      Fraction frac = div(rhs);
      numerator_ = frac.numerator();
      denominator_ = frac.denominator();
      return *this;
    }
    Fraction& operator/=(const int32_t rhs) { return operator/=(Fraction(rhs)); }
    friend bool operator==(const Fraction& lhs, const Fraction& rhs)
    {
      return lhs.numerator() == rhs.numerator() && lhs.denominator() == rhs.denominator();
    }
    friend bool operator!=(const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs) { return lhs.add(rhs); }
    friend Fraction operator+(const Fraction& lhs, const int32_t rhs) { return lhs.add(Fraction(rhs)); }
    friend Fraction operator+(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).add(rhs); }
    friend Fraction operator-(const Fraction& lhs, const Fraction& rhs) { return lhs.sub(rhs); }
    friend Fraction operator-(const Fraction& lhs, const int32_t rhs) { return lhs.sub(Fraction(rhs)); }
    friend Fraction operator-(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).sub(rhs); }
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs) { return lhs.mul(rhs); }
    friend Fraction operator*(const Fraction& lhs, const int32_t rhs) { return lhs.mul(Fraction(rhs)); }
    friend Fraction operator*(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).mul(rhs); }
    friend Fraction operator/(const Fraction& lhs, const Fraction& rhs) { return lhs.div(rhs); }
    friend Fraction operator/(const Fraction& lhs, const int32_t rhs) { return lhs.div(Fraction(rhs)); }
    friend Fraction operator/(const int32_t lhs, const Fraction& rhs) { return Fraction(lhs).div(rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const Fraction& frac) { ost << frac.to_string(); return ost; }
  };

  /* Phase */

  class Phase
  {
  private:
    Fraction frac_;
  public:
    Phase(const int32_t numerator = 0, const int32_t denominator = 1) : frac_(numerator, denominator) {}
    Phase(const Fraction& frac) : frac_(frac.numerator(), frac.denominator()) {}
    Phase(const std::string& str);
    Phase(const Phase& phase) : frac_(phase.frac_) {}
    /* getters */
    Fraction frac() const { return frac_; }
    /* setters */
    void frac(Fraction frac) { frac_ = frac; }
    /* member functions */
    std::string to_string(bool pi_str = true) const;
    double value() const { return M_PI * frac_.numerator() / frac_.denominator(); }
    bool is_zero() const { return (*this == Phase(0)); }
    bool is_positive() const { return frac_.is_positive(); }
    bool is_negative() const { return frac_.is_negative(); }
    void reduce() { frac_.reduce(); }
    void mod_2pi()
    {
      frac_.reduce();
      if (frac_.is_positive()) {
    	int32_t rounds = frac_.numerator() / (frac_.denominator() * 2);
    	frac_.numerator(frac_.numerator() - 2 * rounds * frac_.denominator());
      }
      else if (frac_.is_negative()) {
    	int32_t nume = -frac_.numerator();
    	int32_t deno_2 = frac_.denominator() * 2;
    	int32_t rounds = (nume + deno_2 - 1 ) / deno_2;
    	frac_.numerator(2 * rounds * frac_.denominator() + frac_.numerator());
      }
    }
    Phase operator+() const { return *this; }
    Phase operator-() const { return Phase(-frac_); }
    Phase& operator+=(const Phase& rhs) { frac_ += rhs.frac(); return *this; }
    Phase& operator-=(const Phase& rhs) { frac_ -= rhs.frac(); return *this; }
    Phase& operator*=(const int32_t rhs) { frac_ *= rhs; return *this; }
    Phase& operator/=(const int32_t rhs) { frac_ /= rhs; return *this; }
    friend bool operator==(const Phase& lhs, const Phase& rhs) {
      Phase lhs_mod = lhs; lhs_mod.mod_2pi();
      Phase rhs_mod = rhs; rhs_mod.mod_2pi();
      return lhs_mod.frac() == rhs_mod.frac();
    }
    friend bool operator!=(const Phase& lhs, const Phase& rhs) { return !(lhs == rhs); }
    friend Phase operator+(const Phase& lhs, const Phase& rhs) { return Phase(lhs.frac() + rhs.frac()); }
    friend Phase operator-(const Phase& lhs, const Phase& rhs) { return Phase(lhs.frac() - rhs.frac()); }
    friend Phase operator*(const Phase& lhs, const int32_t rhs) { return Phase(lhs.frac() * rhs); }
    friend Phase operator*(const int32_t lhs, const Phase& rhs) { return Phase(lhs * rhs.frac()); }
    friend Phase operator/(const Phase& lhs, const int32_t rhs) { return Phase(lhs.frac() / rhs); }
    friend std::ostream& operator<<(std::ostream& ost, const Phase& phase) { ost << phase.to_string(); return ost; }
  };

  /* LinearMap */

  class LinearMap : public Eigen::MatrixXcd
  {
  public:
    LinearMap(uint32_t row_num = 0, uint32_t col_num = 0)
      : Eigen::MatrixXcd(Eigen::MatrixXcd::Identity(row_num, col_num)) {}
    LinearMap(const std::vector<std::vector<std::complex<double>>>& mat)
      : Eigen::MatrixXcd(mat.size(), mat[0].size())
    {
      for (uint32_t i = 0; i < mat.size(); ++i) {
	for (uint32_t j = 0; j < mat[0].size(); ++j) {
	  coeffRef(i, j) = mat[i][j];
	}
      }
    }
    template<typename OtherDerived>
    LinearMap(const Eigen::MatrixBase<OtherDerived>& other) : Eigen::MatrixXcd(other) {}
    /* member functions */
    uint32_t row_num() const { return (uint32_t)rows(); }
    uint32_t col_num() const { return (uint32_t)cols(); }
    bool is_zero() const;
    bool is_identity() const;
    bool is_diagonal() const;
    bool is_identity_multiple_constant() const;
    bool is_unitary() const;
    bool is_equal(const LinearMap& other) const;
    LinearMap& operate_qgate(const QGate& qgate);
    template<typename OtherDerived>
    LinearMap& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Eigen::MatrixXcd::operator=(other);
      return *this;
    }
    LinearMap operator+() const { return *this; }
    LinearMap operator-() const { return *this * (-1.0); }
    friend LinearMap operator+(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res += rhs; return res; }
    friend LinearMap operator-(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res -= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const LinearMap& rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const double rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const LinearMap& lhs, const std::complex<double> rhs) { LinearMap res = lhs; res *= rhs; return res; }
    friend LinearMap operator*(const double lhs, const LinearMap& rhs) { LinearMap res = rhs; res *= lhs; return res; }
    friend LinearMap operator*(const std::complex<double> lhs, const LinearMap& rhs) { LinearMap res = rhs; res *= lhs; return res; }
    friend LinearMap operator/(const LinearMap& lhs, const double rhs) { LinearMap res = lhs; res /= rhs; return res; }
    friend LinearMap operator/(const LinearMap& lhs, const std::complex<double> rhs) { LinearMap res = lhs; res /= rhs; return res; }
  };

  class BinaryMatrix
  {
  private:
    uint32_t row_num_;
    uint32_t col_num_;
    std::vector<std::vector<uint8_t>> elements_;
    std::vector<uint32_t> row_indexes_;
  public:
    BinaryMatrix(const uint32_t row_num = 0, const uint32_t col_num = 0, const uint8_t val = 0)
      : row_num_(row_num), col_num_(col_num), elements_(row_num, std::vector<uint8_t>(col_num, val))
    {
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    BinaryMatrix(const std::vector<std::vector<uint8_t>>& elements)
    {
      elements_ = elements;
      row_num_ = elements.size();
      col_num_ = elements[0].size();
      for (uint32_t i = 0; i < row_num_; ++i) row_indexes_.push_back(i);
    }
    BinaryMatrix(const BinaryMatrix& bm)
      : row_num_(bm.row_num_), col_num_(bm.col_num_), elements_(bm.elements_), row_indexes_(bm.row_indexes_) {}
    /* getter */
    uint32_t row_num() const { return row_num_; }
    uint32_t col_num() const { return col_num_; }
    std::vector<std::vector<uint8_t>> elements() const { return elements_; };
    std::vector<uint32_t> row_indexes() const { return row_indexes_; };
    /* setter */
    void row_num(const uint32_t row_num) { row_num_ = row_num; }
    void col_num(const uint32_t col_num) { col_num_ = col_num; }
    void elements(const std::vector<std::vector<uint8_t>>& elements) { elements_ = elements; }
    void row_indexes(const std::vector<uint32_t>& row_indexes) { row_indexes_ = row_indexes; }
    /* member functions */
    std::string to_string() const;
    void xor_rows(const uint32_t a, const uint32_t b)
    {
      if (a > row_num_ || b > row_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < col_num_; ++i) {
	elements_[b][i] = (elements_[b][i] + elements_[a][i]) % 2; // XOR operation
      }
    }
    void swap_rows(const uint32_t a, const uint32_t b)
    {
      if (a > row_num_ || b > row_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < col_num_; ++i) {
	std::swap(elements_[a][i], elements_[b][i]);
      }
      std::swap(row_indexes_[a], row_indexes_[b]);
    }
    void swap_cols(const uint32_t a, const uint32_t b)
    {
      if (a > col_num_ || b > col_num_) {
	throw std::runtime_error("argument is too large.");
      }
      for (uint32_t i = 0; i < row_num_; ++i) {
	std::swap(elements_[i][a], elements_[i][b]);
      }
    }
    bool feasible() const
    {
      bool ans = false;
      for (auto& row:elements_) {
	uint32_t sum = 0;
	for (auto& e:row) {
	  sum += (uint32_t)e;
	  if (sum > 1) break;
	}
	if (sum == 1) {
	  ans = true;
	  break;
	}
      }
      return ans;
    }
    std::vector<std::pair<uint32_t, uint32_t>> gauss_reduce();
    friend std::ostream& operator<<(std::ostream& ost, const BinaryMatrix& bmat) { ost << bmat.to_string(); return ost; }
  };

  /* Quantum Gate */

  enum QGateKind { X, Z, S, Sdg, T, Tdg, H, RZ, CX, Id };

  class QGate
  {
  private:
    QGateKind kind_;
    std::vector<uint32_t> qid_;
    Phase phase_;
    std::vector<std::vector<std::complex<double>>> op_;
    static std::vector<std::vector<std::complex<double>>> kind_to_op(const QGateKind kind, const Phase& phase = 0);
  public:
    QGate(const QGateKind kind = QGateKind::X, const std::vector<uint32_t>& qid = {0}, const Phase& phase = 0);
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
    bool is_Id_gate() const { return (kind_ == QGateKind::Id); }
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
    bool is_T_gate() const
    {
      bool ans = false;
      if (kind_ == QGateKind::RZ) {
	if (phase_ == Phase(1,4) || phase_ == Phase(-1,4)) ans = true;
      }
      else if (kind_ == QGateKind::T || kind_ == QGateKind::Tdg) { ans = true; }
      return ans;
    }
    bool is_CX_gate() const { return (kind_ == QGateKind::CX); }
    bool is_RZ_gate() const { return (kind_ == QGateKind::Z || kind_ == QGateKind::S || kind_ == QGateKind::Sdg ||
				      kind_ == QGateKind::T || kind_ == QGateKind::Tdg || kind_ == QGateKind::RZ ||
				      kind_ == QGateKind::Id); }
    bool is_pauli_gate() const { return (is_X_gate() || is_Z_gate() || is_Id_gate()); }
    bool is_proper_clifford_gate() const { return is_S_gate(); }
    bool is_clifford_gate() const{ return (is_pauli_gate() || is_H_gate() || is_CX_gate() || is_S_gate()); }
    bool is_non_clifford_gate() const { return !(is_clifford_gate()); }
    QGate inverse() const;
    bool is_identical(QGate& other) const;
    bool mergeable(const QGate& other) const;
    bool commutable(const QGate& other) const;
    void merge(QGate other);
    static std::tuple<QGateKind, Phase> kind_phase(const std::string& str);
    friend std::ostream& operator<<(std::ostream& ost, const QGate& qgate) { ost << qgate.to_string(); return ost; }
  };

  /* Quantum Circuit */
  
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
    uint32_t cx_count() const;
    uint32_t rz_count() const;
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
    /* compound gates */
    QCirc& rx(const uint32_t q, const Phase& phase) { h(q); rz(q,phase); h(q); return *this; }
    QCirc& y(const uint32_t q) { x(q); z(q); return *this; }
    QCirc& xr(const uint32_t q) { sdg(q); h(q); sdg(q); return *this;}
    QCirc& xrdg(const uint32_t q) { s(q); h(q); s(q); return *this;}
    QCirc& ry(const uint32_t q, const Phase& phase)
    { rx(q, Phase(1,2)); rz(q, phase); rx(q, Phase(-1,2)); return *this; }
    QCirc& p(const uint32_t q, const Phase& phase) { rz(q,phase); return *this; }
    QCirc& cy(const uint32_t con, const uint32_t tar) { cz(con,tar); cx(con,tar); s(con); return *this; }
    QCirc& cz(const uint32_t con, const uint32_t tar) { h(tar); cx(con,tar); h(tar); return *this; }
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

  /* ZX-diagram */
    
  enum ZXNodeKind { XSpider, ZSpider, Input, Output };
  enum ZXEdgeKind { None, Plain, Hadamard };
  enum ZXDiagramKind { General, GraphLike };
  enum ZXNodePlace { Terminal, Boundary, Internal };

  constexpr ZXNodeKind DEF_ZXNODEKIND = ZXNodeKind::ZSpider;
  constexpr ZXEdgeKind DEF_ZXEDGEKIND = ZXEdgeKind::Plain;
  constexpr uint32_t DEF_NODE_PLACES_SIZE = 64;

  class ZXNode
  {
  private:
    ZXNodeKind kind_;
    Phase phase_;
    uint32_t q_; // related qubit id for Input or Output
    bool pg_phase_; // phase gadget (phase node)
    bool pg_root_; // phase gadget (root node)
    bool pg_leaf_; // phase gadget (leaf node)
  public:
    ZXNode(const ZXNodeKind kind = DEF_ZXNODEKIND, const Phase phase = Phase(0), const uint32_t q = 0,
	   const bool pg_phase = false, const bool pg_root = false, const bool pg_leaf = false)
      : kind_(kind), phase_(phase), q_(q), pg_phase_(pg_phase), pg_root_(pg_root), pg_leaf_(pg_leaf) {}
    ZXNode(const ZXNode& node) : kind_(node.kind_), phase_(node.phase_), q_(node.q_),
				 pg_phase_(node.pg_phase_), pg_root_(node.pg_root_), pg_leaf_(node.pg_leaf_) {}
    /* getters */
    ZXNodeKind kind() const { return kind_; }
    Phase phase() const { return phase_; }
    uint32_t q() const { return q_; }
    bool pg_phase() const { return pg_phase_; }
    bool pg_root() const { return pg_root_; }
    bool pg_leaf() const { return pg_leaf_; }
    /* setters */
    void kind(const ZXNodeKind kind) { kind_ = kind; }
    void phase(const Phase phase) { phase_ = phase; }
    void q(const uint32_t q) { q_ = q; }
    void pg_phase(const bool pg_phase) { pg_phase_ = pg_phase; }
    void pg_root(const bool pg_root) { pg_root_ = pg_root; }
    void pg_leaf(const bool pg_leaf) { pg_leaf_ = pg_leaf; }
    /* member functions */
    std::string kind_str() const {
      std::vector<std::string> s = { "XSpider", "ZSpider", "Input", "Output" };
      return s[kind_];
    }
    std::string name() const;
  };
  
  class ZXEdge
  {
  private:
    ZXEdgeKind kind_;
    uint32_t to_;  // position of nodes_
  public:
    ZXEdge(const ZXEdgeKind kind = DEF_ZXEDGEKIND, const uint32_t to = 0) : kind_(kind), to_(to) {}
    ZXEdge(const ZXEdge& edge) : kind_(edge.kind_), to_(edge.to_) {}
    /* getters */
    ZXEdgeKind kind() const { return kind_; }
    uint32_t to() const { return to_; }
    /* setters */
    void kind(const ZXEdgeKind kind) { kind_ = kind; }
    void to(const uint32_t to) { to_ = to; }
    /* member functions */
    std::string kind_str() const {
      std::vector<std::string> s = { "Plain", "Hadamard" };
      return s[kind_];
    }
    std::string name() const
    {
      std::string s = "";
      if (kind_ == ZXEdgeKind::Plain) s = "--";
      else if (kind_ == ZXEdgeKind::Hadamard) s = "++";
      s += "[" + std::to_string(to_) + "]";
      return s;
    }
    void reverse_kind()
    {
      if (kind_ == Plain) kind_ = Hadamard;
      else kind_ = Plain;
    }
  };
  
  class ZXDiagram
  {
  private:
    ZXDiagramKind kind_;
    uint32_t qubit_num_;
    std::vector<ZXNode> nodes_;
    std::vector<uint32_t> inputs_;
    std::vector<uint32_t> outputs_;
    std::vector<std::vector<ZXEdge>> adj_mat_;
    std::vector<ZXNodePlace> node_places_;
    uint32_t max_adj_num() const
    {
      uint32_t N = 0;
      for (uint32_t i = 0; nodes_.size(); ++i) {
	if (adj_mat_[i].size() > N) N = adj_mat_[i].size();
      }
      return N;
    }
    bool check_x_spider(const uint32_t node_index) const { return nodes_[node_index].kind() == Sharq::ZXNodeKind::XSpider; }
    bool check_z_spider(const uint32_t node_index) const { return nodes_[node_index].kind() == Sharq::ZXNodeKind::ZSpider; }
    bool check_zero_phase_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(0))) return true;
      else return false;
    }
    bool check_pauli_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(0) || nodes_[node_index].phase() == Phase(1))) return true;
      else return false;
    }
    bool check_proper_clifford_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(1,2) || nodes_[node_index].phase() == Phase(3,2))) return true;
      else return false;
    }
    bool check_clifford_spider(const uint32_t node_index) const
    {
      return (check_pauli_spider(node_index) || check_proper_clifford_spider(node_index));
    }
    bool check_non_clifford_spider(const uint32_t node_index) const { return !check_clifford_spider(node_index); }
    bool check_internal_node(const uint32_t node_index) const { return node_places_[node_index] == ZXNodePlace::Internal; }
    bool check_boundary_node(const uint32_t node_index) const { return node_places_[node_index] == ZXNodePlace::Boundary; }
    bool check_pg_phase_node(const uint32_t node_index) const { return nodes_[node_index].pg_phase(); }
    bool check_pg_root_node(const uint32_t node_index) const { return nodes_[node_index].pg_root(); }
    bool check_pg_leaf_node(const uint32_t node_index) const { return nodes_[node_index].pg_leaf(); }
    bool check_phase_gadget_node(const uint32_t node_index) const
    { return (nodes_[node_index].pg_root() || nodes_[node_index].pg_phase()); }
    bool check_input_node(const uint32_t node_index) const { return (nodes_[node_index].kind() == Sharq::ZXNodeKind::Input); }
    bool check_output_node(const uint32_t node_index) const { return (nodes_[node_index].kind() == Sharq::ZXNodeKind::Output); }
    bool check_connect_input_node(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (nodes_[e.to()].kind() == Sharq::ZXNodeKind::Input) return true;
      }
      return false;
    }
    bool check_connect_output_node(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (nodes_[e.to()].kind() == Sharq::ZXNodeKind::Output) return true;
      }
      return false;
    }
    bool check_connect_phase_gadget(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (check_pg_root_node(e.to()) || check_pg_phase_node(e.to())) return true;
      }
      return false;
    }
    bool check_connect(const uint32_t a, const uint32_t b) const;
    void xor_hadamard_edge(const uint32_t a, const uint32_t b);
    void xor_hadamard_edges(std::vector<uint32_t> node_indexes);
    ZXNodeKind remove_node(const uint32_t node_index);
    void remove_edges_of_node(const uint32_t node_index) { adj_mat_[node_index].clear(); }
    uint32_t degree_of_node(const uint32_t node_index) { return adj_mat_[node_index].size(); }
    ZXNodeKind kind_of_node(const uint32_t node_index) { return nodes_[node_index].kind(); }
    void swap_nodes(uint32_t i, uint32_t j);
    void remove_isolated_spiders();
    void row_operation(const uint32_t a, const uint32_t b);
    std::vector<uint32_t> adjacent_node_indexes(uint32_t node_index);
    void conv_x_to_z();
    void fuse_spiders();
    void remove_parallel_selfloops_hadamard_edges();
    void permutation_as_swap(QCirc& qc);
    void process_frontier(std::vector<uint32_t>& frontier, Sharq::QCirc& qc);
    bool update_frontier(std::vector<uint32_t>& frontier, Sharq::QCirc& qc);
    bool update_frontier_pg(std::vector<uint32_t>& frontier, Sharq::QCirc& qc);
    void lcomp_one_time(const uint32_t idx_A);
    void pivot1_one_time(const uint32_t idx_A, const uint32_t idx_B);
    void pivot2_one_time(const uint32_t idx_A, const uint32_t idx_B);
    void pivot3_one_time(const uint32_t idx_A, const uint32_t idx_B);
  public:
    ZXDiagram(uint32_t qubit_num = 1);
    ZXDiagram(const ZXDiagram& zx)
      : kind_(zx.kind_), qubit_num_(zx.qubit_num_), nodes_(zx.nodes_),
    	inputs_(zx.inputs_), outputs_(zx.outputs_), adj_mat_(zx.adj_mat_),
    	node_places_(zx.node_places_) {}
    /* getters */
    ZXDiagramKind kind() const { return kind_; }
    uint32_t qubit_num() const { return qubit_num_; }
    std::vector<ZXNode> nodes() const { return nodes_; }
    std::vector<uint32_t> inputs() const { return inputs_; }
    std::vector<uint32_t> outputs() const { return outputs_; }
    std::vector<std::vector<ZXEdge>> adj_mat() const { return adj_mat_; }
    /* setters */
    void kind(const ZXDiagramKind kind) { kind_ = kind; }
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    void nodes(const std::vector<ZXNode> nodes) { nodes_ = nodes; }
    void inputs(const std::vector<uint32_t> inputs) { inputs_ = inputs; }
    void outputs(const std::vector<uint32_t> outputs) { outputs_ = outputs; }
    void adj_mat(std::vector<std::vector<ZXEdge>>& adj_mat) { adj_mat_ = adj_mat; }
    /* member functions */
    std::string to_string() const;
    void show() const { std::cout << to_string() << std::endl; }
    std::map<std::string, uint32_t> stats() const;
    void to_dot_file(const std::string& file_name) const;
    void to_svg_file(const std::string& file_name) const;
    uint32_t non_clifford_count() const
    {
      uint32_t cnt = 0;
      for (uint32_t i = 0; i < nodes_.size(); ++i) {
	if (check_non_clifford_spider(i)) ++cnt;
      }
      return cnt;
    }
    void add_qgate(const QGate& qgate);
    uint32_t append_node(const ZXNode node);
    uint32_t append_node(const ZXNode node, const ZXEdge edge);
    void connect_nodes(const uint32_t a, const uint32_t b, const ZXEdgeKind kind);
    ZXEdgeKind remove_edge(const uint32_t a, const uint32_t b);
    void update_node_places();
    void update_phase_gadget();
    void graph_like();
    void lcomp();
    void pivot1();
    void pivot2();
    void pivot3();
    void id_removal();
    void gfusion();
    void simplify();
    QCirc extract_qcirc();
    friend std::ostream& operator<<(std::ostream& ost, const ZXDiagram& zx) { ost << zx.to_string(); return ost; }
  };
  
  class Optimizer
  {
  private:
    double proc_time_;
    std::map<std::string, uint32_t> stats_in_;
    std::map<std::string, uint32_t> stats_out_;
  public:
    /* getters */
    double proc_time() const { return proc_time_; }
    std::map<std::string, uint32_t> stats_in() const { return stats_in_; }    
    std::map<std::string, uint32_t> stats_out() const { return stats_out_; }    
    /* setters */
    void proc_time(const double proc_time) { proc_time_ = proc_time; }
    void stats_in(const std::map<std::string, uint32_t>& stats_in) { stats_in_ = stats_in; }
    void stats_out(const std::map<std::string, uint32_t>& stats_out) { stats_out_ = stats_out; }
    /* member functions */
    std::string to_string() const;
    void show() const { std::cout << to_string() << std::endl; }
    QCirc execute(const QCirc& qc_in);
    friend std::ostream& operator<<(std::ostream& ost, const Optimizer& opt) { ost << opt.to_string(); return ost; }
   };
  
  /* utility functions */
  
  inline std::vector<std::string> split(const std::string& str, const char sep)
  {
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
      v.push_back(buffer);
    }
    return v;
  }
  inline std::string ltrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
  {
    str.erase(0, str.find_first_not_of(whitespace));
    return str;
  }
  inline std::string rtrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
  {
    str.erase(str.find_last_not_of(whitespace) + 1);
    return str;
  }
  inline std::string trim(std::string &str, std::string const &whitespace=" \r\n\t\v\f")
  {
    std::string rstr = rtrim(str, whitespace);
    return ltrim(rstr, whitespace);
  }
  inline std::string stats_to_string(const std::map<std::string, uint32_t>& sts)
  {
    std::stringstream ss;
    for (auto it = sts.begin(); it != sts.end(); ++it) {
      ss << it->first << ": " << it->second << std::endl;
    }
    return ss.str();
  }
}

#endif
