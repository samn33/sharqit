#ifndef ZX_H
#define ZX_H

#include <iostream>
#include <string>
#include <vector>

#include "binary_matrix.h"
#include "qcirc.h"

namespace Sharq {

  class QCirc;
  
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
    void remove_edges_of_node(const uint32_t node_index) {
      adj_mat_[node_index].clear();
      for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
	if (i == node_index) continue;
	for (auto it = adj_mat_[i].begin(); it != adj_mat_[i].end(); ++it) {
	  if (it->to() == node_index) {
	    it = adj_mat_[i].erase(it);
	    --it;
	  }
	}
      }
    }
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
    void process_frontier(std::vector<uint32_t>& frontier, Sharq::QCirc& qc, const bool opt_cz = false);
    bool update_frontier(std::vector<uint32_t>& frontier, Sharq::QCirc& qc, const bool opt_cz = false);
    bool update_frontier_pg(std::vector<uint32_t>& frontier, Sharq::QCirc& qc);
    void lcomp_one_time(const uint32_t idx_A);
    void pivot1_one_time(const uint32_t idx_A, const uint32_t idx_B);
    void pivot2_one_time(const uint32_t idx_A, const uint32_t idx_B);
    void pivot3_one_time(const uint32_t idx_A, const uint32_t idx_B);
    void gfusion_one_time(const uint32_t idx_A_phase, const uint32_t idx_B_phase);
    std::vector<std::pair<uint32_t, uint32_t>> extract_2q_connects(std::vector<uint32_t>& frontier) const;
  public:
    ZXDiagram(const uint32_t qubit_num = 1);
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
    void nodes(const std::vector<ZXNode>& nodes) { nodes_ = nodes; }
    void inputs(const std::vector<uint32_t>& inputs) { inputs_ = inputs; }
    void outputs(const std::vector<uint32_t>& outputs) { outputs_ = outputs; }
    void adj_mat(std::vector<std::vector<ZXEdge>>& adj_mat) { adj_mat_ = adj_mat; }
    /* member functions */
    std::string to_string() const;
    void show() const { std::cout << to_string() << std::endl; }
    std::map<std::string, uint32_t> stats() const;
    void to_dot_file(const std::string& file_name) const;
    void to_svg_file(const std::string& file_name) const;
    uint32_t spider_count() const { return (nodes_.size() - inputs_.size() - outputs_.size()); }
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
    uint32_t lcomp();
    uint32_t pivot1();
    uint32_t pivot2();
    uint32_t pivot3();
    uint32_t gfusion();
    void id_removal();
    void simplify();
    QCirc extract_qcirc();
    friend std::ostream& operator<<(std::ostream& ost, const ZXDiagram& zx) { ost << zx.to_string(); return ost; }
  };
}

#endif
