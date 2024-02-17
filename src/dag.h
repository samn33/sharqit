#ifndef DAG_H
#define DAG_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "qgate.h"
#include "qcirc.h"

namespace Sharq {

  class QCirc;

  enum DAGNodeKind { OpNode, InNode, OutNode };
  enum DAGEdgeKind { Forward, Backward };

  class DAGNode
  {
  private:
    DAGNodeKind kind_;
    QGate qgate_;
  public:
    DAGNode(const DAGNodeKind kind = DAGNodeKind::OpNode, const QGate qgate = QGate())
      : kind_(kind), qgate_(qgate) {}
    DAGNode(const DAGNode& node) : kind_(node.kind_), qgate_(node.qgate_) {}
    /* getters */
    DAGNodeKind kind() const { return kind_; }
    QGate qgate() const { return qgate_; }
    /* setters */
    void kind(const DAGNodeKind kind) { kind_ = kind; }
    void qgate(const QGate qgate) { qgate_ = qgate; }
    /* member functions */
    std::string name() const;
    uint32_t qubit_num() const { return qgate_.qid().size(); }
    std::vector<uint32_t> qid() { return qgate_.qid(); }
    bool is_innode() const { return kind_ == Sharq::DAGNodeKind::InNode; }
    bool is_outnode() const { return kind_ == Sharq::DAGNodeKind::OutNode; }
    bool is_opnode() const { return kind_ == Sharq::DAGNodeKind::OpNode; }
    bool is_identity() const
    { return (qgate_.kind() == Sharq::QGateKind::Id || qgate_.kind() == Sharq::QGateKind::Id2); }
    bool is_hadamard() const { return qgate_.is_H_gate(); }
    bool is_rotation() const { return qgate_.is_RZ_gate(); }
    bool is_cnot() const { return qgate_.is_CX_gate(); }
    bool is_included(const uint32_t q) const
    {
      for (auto& i:qgate_.qid()) { if (i == q) return true; }
      return false;
    }
    bool mergeable(const DAGNode& other) const { return qgate_.mergeable(other.qgate()); }
    bool commutable(const DAGNode& other) const { return qgate_.commutable(other.qgate()); }
    void merge(const DAGNode& other) { qgate_.merge(other.qgate()); }
  };

  class DAGEdge
  {
  private:
    DAGEdgeKind kind_; // edge direction (backward or forward)
    uint32_t q_; // qubit id
    uint32_t to_; // node id
  public:
    DAGEdge(const DAGEdgeKind kind = Sharq::DAGEdgeKind::Forward,
	    const uint32_t q = 0, const uint32_t to = 0) : kind_(kind), q_(q), to_(to) {}
    DAGEdge(const DAGEdge& edge) : kind_(edge.kind_), q_(edge.q_), to_(edge.to_) {}
    /* getters */
    DAGEdgeKind kind() const { return kind_; }
    uint32_t q() const { return q_; }
    uint32_t to() const { return to_; }
    /* setters */
    void kind(DAGEdgeKind kind) { kind_ = kind; }
    void q(uint32_t q) { q_ = q; }
    void to(uint32_t to) { to_ = to; }
    /* member functions */
    std::string name() const;
    bool is_forward() const { return kind_ == Sharq::DAGEdgeKind::Forward; }
    bool is_backward() const { return kind_ == Sharq::DAGEdgeKind::Backward; }
  };
  
  class DAGCirc
  {
  private:
    uint32_t qubit_num_;
    std::vector<DAGNode> nodes_;
    std::vector<uint32_t> inputs_;
    std::vector<uint32_t> outputs_;
    std::vector<std::vector<DAGEdge>> adj_mat_;
    /* member functions */
    void cx_to_cz_gate();
    void cz_to_cx_gate();
    void hadamard_gate_reduction_1();
    void hadamard_gate_reduction_2();
    void hadamard_gate_reduction_3();
  public:
    DAGCirc(const uint32_t qubit_num = 1);
    DAGCirc(const QCirc& qc);
    DAGCirc(const DAGCirc& dc)
      : qubit_num_(dc.qubit_num_), nodes_(dc.nodes_),
    	inputs_(dc.inputs_), outputs_(dc.outputs_), adj_mat_(dc.adj_mat_) {}
    /* getters */
    uint32_t qubit_num() const { return qubit_num_; }
    std::vector<DAGNode> nodes() const { return nodes_; }
    std::vector<uint32_t> inputs() const { return inputs_; }
    std::vector<uint32_t> outputs() const { return outputs_; }
    std::vector<std::vector<DAGEdge>> adj_mat() const { return adj_mat_; }
    /* setters */
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    void nodes(const std::vector<DAGNode>& nodes) { nodes_ = nodes; }
    void inputs(const std::vector<uint32_t>& inputs) { inputs_ = inputs; }
    void outputs(const std::vector<uint32_t>& outputs) { outputs_ = outputs; }
    void adj_mat(const std::vector<std::vector<DAGEdge>>& adj_mat) { adj_mat_ = adj_mat; }
    /* member functions */
    std::string to_string() const;
    void show() const { std::cout << to_string(); }
    uint32_t gate_count() const { return nodes_.size() - inputs_.size() - outputs_.size(); }
    uint32_t append_node(const Sharq::DAGNode node);
    uint32_t prev_node(const uint32_t idx, const uint32_t q);
    uint32_t next_node(const uint32_t idx, const uint32_t q);
    void connect_nodes(const uint32_t a, const uint32_t b, const uint32_t q);
    void remove_node(const uint32_t idx);
    void remove_edge(const uint32_t a, const uint32_t b);
    void remove_edge(const uint32_t a, const uint32_t b, const uint32_t q);
    void remove_edges_of_node(const uint32_t idx);
    void remove_isolated_nodes();
    DAGCirc& add_qgate(const QGate& qgate);
    QCirc to_qcirc() const;
    void id_removal();
    uint32_t hadamard_gate_reduction();
    uint32_t single_qubit_gate_cancellation();
    uint32_t two_qubit_gate_cancellation();
    void rule_based_gate_reduction();
    friend std::ostream& operator<<(std::ostream& ost, const DAGCirc& dc) { ost << dc.to_string(); return ost; }
  };
}

#endif
