/**
 * @file dag.h
 * @brief include file of DAG (directed acyclic graph) related classes (DAGNode, DAGEdge, DAGCirc)
 */

#ifndef DAG_H
#define DAG_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "qgate.h"
#include "qcirc.h"

namespace Sharqit {

  class QCirc;

  //! node kind of the DAG
  enum DAGNodeKind {
		    OpNode, //!< operation node (= quantum gate)
		    InNode, //!< input node
		    OutNode //!< ouput node
  };
  //! edge kind of the DAG
  enum DAGEdgeKind {
		    Forward, //!< forward direction
		    Backward //!< backward direction
  };

  /**
   * @class DAGNode
   * @brief node of DAG (directed acyclic graph)
   * @see DAGEdge
   * @see DAGCirc
  */
  class DAGNode
  {
  private:
    DAGNodeKind kind_; //!< kind of the node
    QGate qgate_; //!< quantum gate corresponding to the node
  public:
    /**
     * @brief constructor of the DAGNode
     * @param [in] kind kind of the node
     * @param [in] qgate the QGate object
     */
    DAGNode(const DAGNodeKind kind = DAGNodeKind::OpNode, const QGate qgate = QGate())
      : kind_(kind), qgate_(qgate) {}
    /**
     * @brief copy constructor of the ZXNode
     * @param [in] node DAGNode object
     */
    DAGNode(const DAGNode& node) : kind_(node.kind_), qgate_(node.qgate_) {}
    //! getter of the kind_
    DAGNodeKind kind() const { return kind_; }
    //! getter of the qgate_
    QGate qgate() const { return qgate_; }
    //! setter of the kind_
    void kind(const DAGNodeKind kind) { kind_ = kind; }
    //! setter of the qgate_
    void qgate(const QGate qgate) { qgate_ = qgate; }
    /**
     * @brief get a name string of the DAGNode
     * @return name string of the DAGNode
     */
    std::string name() const;
    /**
     * @brief get a number of the qubits related to the quantum gate of the node (1 or 2)
     * @return number of the qubits
     */
    uint32_t qubit_num() const { return qgate_.qid().size(); }
    /**
     * @brief get a vector of the qubits' indexes related to the quantum gate of the node
     * @return vector of the qubits' indexes
     */
    std::vector<uint32_t> qid() { return qgate_.qid(); }
    /**
     * @brief whether the node is an input node or not
     * @return true if the node is an input node, false otherwise
     */
    bool is_innode() const { return kind_ == Sharqit::DAGNodeKind::InNode; }
    /**
     * @brief whether the node is an output node or not
     * @return true if the node is an output node, false otherwise
     */
    bool is_outnode() const { return kind_ == Sharqit::DAGNodeKind::OutNode; }
    /**
     * @brief whether the node is an operation node (quantum gate) or not
     * @return true if the node is an operation node (quantum gate), false otherwise
     */
    bool is_opnode() const { return kind_ == Sharqit::DAGNodeKind::OpNode; }
    /**
     * @brief whether the node is an identity node (identity gate) or not
     * @return true if the node is an identity node, false otherwise
     */
    bool is_identity() const
    { return (qgate_.kind() == Sharqit::QGateKind::Id || qgate_.kind() == Sharqit::QGateKind::Id2); }
    /**
     * @brief whether the node is an hadamard gate or not
     * @return true if the node is an hadamard gate, false otherwise
     */
    bool is_hadamard() const { return qgate_.is_H_gate(); }
    /**
     * @brief whether the node is a rotation gate or not
     * @return true if the node is a rotation gate, false otherwise
     */
    bool is_rotation() const { return qgate_.is_RZ_gate(); }
    /**
     * @brief whether the node is a CNOT gate or not
     * @return true if the node is a CNOT gate, false otherwise
     */
    bool is_cnot() const { return qgate_.is_CX_gate(); }
    /**
     * @brief whether the node of quantum gate includes the qubit index or not
     * @param [in] q qubit index
     * @return true if the node of quantum gate includes the qubit index, false otherwise
     */
    bool is_included(const uint32_t q) const
    {
      for (auto& i:qgate_.qid()) { if (i == q) return true; }
      return false;
    }
    /**
     * @brief whether two nodes of quantum gates are mergeable or not
     * @param [in] other DAGNode object
     * @return true if two nodes of quantum gates are mergeable, false otherwise
     */
    bool mergeable(const DAGNode& other) const { return qgate_.mergeable(other.qgate()); }
    /**
     * @brief whether the two nodes of quantum gates are commutable or not
     * @param [in] other DAGNode object
     * @return true if the two nodes of quantum gates are commutable, false otherwise
     */
    bool commutable(const DAGNode& other) const { return qgate_.commutable(other.qgate()); }
    /**
     * @brief the node of the quanteum gate is merged to the other's
     * @param [in] other DAGNode object
     */
    void merge(const DAGNode& other) { qgate_.merge(other.qgate()); }
  };

  /**
   * @class DAGEdge
   * @brief edge of DAG (directed acyclic graph)
   * @see DAGCirc
  */
  class DAGEdge
  {
  private:
    DAGEdgeKind kind_; //!< kind of edge direction (backward or forward)
    uint32_t q_; //!< qubit id related to the edge
    uint32_t to_; //!< node index connected to the edge
  public:
    /**
     * @brief constructor of the DAGEdge
     * @param [in] kind kind of the edge
     * @param [in] q qubit id of the edge
     * @param [in] to node index connected to the edge
     */
    DAGEdge(const DAGEdgeKind kind = Sharqit::DAGEdgeKind::Forward,
	    const uint32_t q = 0, const uint32_t to = 0) : kind_(kind), q_(q), to_(to) {}
    /**
     * @brief copy constructor of the DAGEdge
     * @param [in] edge DAGEdge object
     */
    DAGEdge(const DAGEdge& edge) : kind_(edge.kind_), q_(edge.q_), to_(edge.to_) {}
    //! getter of the kind_
    DAGEdgeKind kind() const { return kind_; }
    //! getter of the q_
    uint32_t q() const { return q_; }
    //! getter of the to_
    uint32_t to() const { return to_; }
    //! setter of the kind_
    void kind(DAGEdgeKind kind) { kind_ = kind; }
    //! setter of the q_
    void q(uint32_t q) { q_ = q; }
    //! setter of the to_
    void to(uint32_t to) { to_ = to; }
    /**
     * @brief get a name of the edge
     * @return name of the edge
     */
    std::string name() const;
    /**
     * @brief whether the edge is a forward one or not
     * @return true if the edge is a forward one, false otherwise
     */
    bool is_forward() const { return kind_ == Sharqit::DAGEdgeKind::Forward; }
    /**
     * @brief whether the edge is a backward one or not
     * @return true if the edge is a backward one, false otherwise
     */
    bool is_backward() const { return kind_ == Sharqit::DAGEdgeKind::Backward; }
  };
  
  /**
   * @class DAGCirc
   * @brief DAG (directed acyclic graph) for quantum circuit
   * @see DAGNode
   * @see DAGEdge
  */
  class DAGCirc
  {
  private:
    uint32_t qubit_num_; //!< number of qubits
    std::vector<DAGNode> nodes_; //!< vector of the DAGNode objects
    std::vector<uint32_t> inputs_; //!< vector of the input node indexes
    std::vector<uint32_t> outputs_; //!< vector of the output node indexes
    std::vector<std::vector<DAGEdge>> adj_mat_; //!< adjacent matrix of the DAGCirc
    /**
     * @brief convert CX to CZ gate
     * @details if there are H gates before and after the target X gate of the CX gate, convert the CX gate and H gates to CZ gate.
     */
    void cx_to_cz_gate();
    /**
     * @brief convert CZ to CX gate
     * @details if there are H gates before and after the control or the target gate of the CZ gate, convert the CZ gate and H gates to CX gate.
     */
    void cz_to_cx_gate();
    /**
     * @brief reduce the hadamard gates by the hadamard reduction rule 1
     * @details [hadamard reduction rule 1] @n
     * `--H--S--H-- => --S+--H--S+--` @n
     * or @n
     * `--H--S+--H-- => --S--H--S--`
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void hadamard_gate_reduction_1();
    /**
     * @brief reduce the hadamard gates by the hadamard reduction rule 2
     * @details [hadamard reduction rule 2] @n
     * `--H--*--H-- => --X--` @n
     * `--H--X--H-- => --*--`
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void hadamard_gate_reduction_2();
    /**
     * @brief reduce the hadamard gates by the hadamard reduction rule 2
     * @details [hadamard reduction rule 3] @n
     * `--------*--------- => ------*-----` @n
     * `--H--S--X--S+--H-- => --S+--X--S--` @n
     * or @n
     * `---------*-------- => -----*------` @n
     * `--H--S+--X--S--H-- => --S--X--S+--`
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void hadamard_gate_reduction_3();
  public:
    /**
     * @brief constructor of the DAGCirc
     * @param [in] qubit_num number of qubits
     */
    DAGCirc(const uint32_t qubit_num = 1);
    /**
     * @brief constructor of the DAGCirc
     * @param [in] qc QCirc object
     */
    DAGCirc(const QCirc& qc);
    /**
     * @brief copy constructor of the DAGCirc
     * @param [in] dc DAGCirc object
     */
    DAGCirc(const DAGCirc& dc)
      : qubit_num_(dc.qubit_num_), nodes_(dc.nodes_),
    	inputs_(dc.inputs_), outputs_(dc.outputs_), adj_mat_(dc.adj_mat_) {}
    //! getter of the qubit_num_
    uint32_t qubit_num() const { return qubit_num_; }
    //! getter of the nodes_
    std::vector<DAGNode> nodes() const { return nodes_; }
    //! getter of the inputs_
    std::vector<uint32_t> inputs() const { return inputs_; }
    //! getter of the outputs_
    std::vector<uint32_t> outputs() const { return outputs_; }
    //! getter of the adj_mat_
    std::vector<std::vector<DAGEdge>> adj_mat() const { return adj_mat_; }
    //! setter of the qubit_num_
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    //! setter of the nodes_
    void nodes(const std::vector<DAGNode>& nodes) { nodes_ = nodes; }
    //! setter of the inputs_
    void inputs(const std::vector<uint32_t>& inputs) { inputs_ = inputs; }
    //! setter of the outputs_
    void outputs(const std::vector<uint32_t>& outputs) { outputs_ = outputs; }
    //! setter of the adj_mat_
    void adj_mat(const std::vector<std::vector<DAGEdge>>& adj_mat) { adj_mat_ = adj_mat; }
    /**
     * @brief get a string of the DAGCirc object
     * @return string of the DAG quantum circuit
     */
    std::string to_string() const;
    /**
     * @brief show the DAG quantum circuit
     */
    void show() const { std::cout << to_string(); }
    /**
     * @brief get a gate-count
     */
    uint32_t gate_count() const { return nodes_.size() - inputs_.size() - outputs_.size(); }
    /**
     * @brief append a DAG node
     * @param [in] node a DAG node
     */
    uint32_t append_node(const Sharqit::DAGNode node);
    /**
     * @brief get a previous index of a DAG node related to the qubit id
     * @param [in] idx index of a DAG node
     * @param [in] q qubit id
     * @return previous index of the DAG node
     */
    uint32_t prev_node(const uint32_t idx, const uint32_t q);
    /**
     * @brief get a next index of a DAG node related to the qubit id
     * @param [in] idx index of a DAG node
     * @param [in] q qubit id
     * @return next index of the DAG node
     */
    uint32_t next_node(const uint32_t idx, const uint32_t q);
    /**
     * @brief connect two nodes specified in the indexes related to the qubit id
     * @param [in] a index of a DAG node
     * @param [in] b index of a DAG node
     * @param [in] q qubit id
     */
    void connect_nodes(const uint32_t a, const uint32_t b, const uint32_t q);
    /**
     * @brief remove a node specified in the index
     * @param [in] idx index of a DAG node
     */
    void remove_node(const uint32_t idx);
    /**
     * @brief remove an edge specified in the indexes
     * @param [in] a index of a DAG node
     * @param [in] b index of a DAG node
     */
    void remove_edge(const uint32_t a, const uint32_t b);
    /**
     * @brief remove an edge specified in the indexes related to the qubit id
     * @param [in] a index of a DAG node
     * @param [in] b index of a DAG node
     * @param [in] q qubit id
     */
    void remove_edge(const uint32_t a, const uint32_t b, const uint32_t q);
    /**
     * @brief remove all edges connected to the node specified in the index
     * @param [in] idx index of a DAG node
     */
    void remove_edges_of_node(const uint32_t idx);
    /**
     * @brief remove isolated DAG nodes
     */
    void remove_isolated_nodes();
    /**
     * @brief add a quantum gate to the DAG circuit
     * @param [in] qgate quantum gate added
     * @return reference of the DAGCirc object
     */
    DAGCirc& add_qgate(const QGate& qgate);
    /**
     * @brief convert to a QCirc object
     * @return QCirc object
     */
    QCirc to_qcirc() const;
    /**
     * @brief remove all identity gates
     */
    void id_removal();
    /**
     * @brief reduce the hadamard gates
     * @return total gate-count after reduced
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    uint32_t hadamard_gate_reduction();
    /**
     * @brief cannlel the single-qubit gates
     * @return total gate-count after the cancellation
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    uint32_t single_qubit_gate_cancellation();
    /**
     * @brief cannlel the two-qubit gates
     * @return total gate-count after the cancellation
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    uint32_t two_qubit_gate_cancellation();
    /**
     * @brief execute the rule-based gate reduction
     * @sa Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov, "Automated optimization of large quantum circuits with continuous parameters", [arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
     */
    void rule_based_gate_reduction();
    friend std::ostream& operator<<(std::ostream& ost, const DAGCirc& dc) { ost << dc.to_string(); return ost; }
  };
}

#endif
