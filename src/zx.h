/**
 * @file zx.h
 * @brief include file of ZXCalculus related classes (ZXNode, ZXEdge, ZXDiagram)
 * @sa 1. Ross Duncan, Aleks Kissinger, Simon Perdrix, John van de Wetering, "Graph-theoretic Simplification of Quantum Circuits with the ZX-calculus", [arXiv:1902.03178](https://arxiv.org/abs/1902.03178)
 * @sa 2. Aleks Kissinger, John van de Wetering, "Reducing T-count with the ZX-calculus", [arXiv:1903.10477](https://arxiv.org/abs/1903.10477)
 * @sa 3. Miriam Backens, Hector Miller-Bakewell, Giovanni de Felice, Leo Lobski, John van de Wetering, "There and back again: A circuit extraction tale", [arXiv:2003.01664](https://arxiv.org/abs/2003.01664)
 * @sa 4. Korbinian Staudacher, "Optimization Approaches for Quantum Circuits using ZX-calculus" [Ludwig maximilian university of munich thesis](https://www.mnm-team.org/pub/Diplomarbeiten/stau21/PDF-Version/stau21.pdf)
 */

#ifndef ZX_H
#define ZX_H

#include <iostream>
#include <string>
#include <vector>

#include "binary_matrix.h"
#include "qcirc.h"

namespace Sharqit {

  class QCirc;

  //! node kind of the ZX-diagram
  enum ZXNodeKind {
		   XSpider, //!< X spider
		   ZSpider, //!< Z spider
		   Input, //!< Input node
		   Output //!< output node
  };
  //! edge kind of the ZX-diagram
  enum ZXEdgeKind {
		   None, //!< None (no edges exist)
		   Plain, //!< not a hadamard edge (hadamard gate is not exist)
		   Hadamard //!< hadamard edge (hadamard gate exists)
  };
  //! ZX-diagram kind
  enum ZXDiagramKind {
		      General, //!< general ZX-diagram
		      GraphLike //!< graph-like ZX-diagram
  };
  //! kind of node place
  enum ZXNodePlace {
		    Terminal, //!< node is placed terminal
		    Boundary, //!< node is placed boundary, that is connected a terminal node
		    Internal //!< node is placed internally (not terminal and not boundary)
  };

  constexpr ZXNodeKind DEF_ZXNODEKIND = ZXNodeKind::ZSpider; //!< defalut value of node kind of ZX-diagram
  constexpr ZXEdgeKind DEF_ZXEDGEKIND = ZXEdgeKind::Plain; //!< defalut value of edge kind of ZX-diagram
  constexpr uint32_t DEF_NODE_PLACES_SIZE = 64; //!< defalut value of the node places size

  /**
   * @class ZXNode
   * @brief node of ZX-diagram
   * @see ZXEdge
   * @see ZXDiagram
  */
  class ZXNode
  {
  private:
    ZXNodeKind kind_; //!< kind of the node of ZX-diagram
    Phase phase_; //!< phase of the node of ZX-diagram
    uint32_t q_; //!< related qubit id of the node of ZX-diagram
    bool pg_phase_; //!< whether the node is a phase node of the phase gadget or not
    bool pg_root_; //!< whether the node is a root node of the phase gadget or not
    bool pg_leaf_; //!< whether the node is a leaf node of the phase gadget or not
  public:
    /**
     * @brief constructor of the ZXNode
     * @param [in] kind kind of the node of ZX-diagram
     * @param [in] phase phase of the node of ZX-diagram
     * @param [in] q related qubit id of the node of ZX-diagram
     * @param [in] pg_phase phase node of the phase gadget or not
     * @param [in] pg_root root node of the phase gadget or not
     * @param [in] pg_leaf leaf node of the phase gadget or not
     */
    ZXNode(const ZXNodeKind kind = DEF_ZXNODEKIND, const Phase phase = Phase(0), const uint32_t q = 0,
	   const bool pg_phase = false, const bool pg_root = false, const bool pg_leaf = false)
      : kind_(kind), phase_(phase), q_(q), pg_phase_(pg_phase), pg_root_(pg_root), pg_leaf_(pg_leaf) {}
    /**
     * @brief copy constructor of the ZXNode
     * @param [in] node ZXNode object
     */
    ZXNode(const ZXNode& node) : kind_(node.kind_), phase_(node.phase_), q_(node.q_),
				 pg_phase_(node.pg_phase_), pg_root_(node.pg_root_), pg_leaf_(node.pg_leaf_) {}
    //! getter of the kind_
    ZXNodeKind kind() const { return kind_; }
    //! getter of the phase_
    Phase phase() const { return phase_; }
    //! getter of the q_
    uint32_t q() const { return q_; }
    //! getter of the pg_phase_
    bool pg_phase() const { return pg_phase_; }
    //! getter of the pg_root_
    bool pg_root() const { return pg_root_; }
    //! getter of the pg_leaf_
    bool pg_leaf() const { return pg_leaf_; }
    //! setter of the kind_
    void kind(const ZXNodeKind kind) { kind_ = kind; }
    //! setter of the phase_
    void phase(const Phase phase) { phase_ = phase; }
    //! setter of the q_
    void q(const uint32_t q) { q_ = q; }
    //! setter of the pg_phase_
    void pg_phase(const bool pg_phase) { pg_phase_ = pg_phase; }
    //! setter of the pg_root_
    void pg_root(const bool pg_root) { pg_root_ = pg_root; }
    //! setter of the pg_leaf_
    void pg_leaf(const bool pg_leaf) { pg_leaf_ = pg_leaf; }
    /**
     * @brief get a string of the kind of ZXNode
     * @return string of the kind of ZXNode
     */
    std::string kind_str() const {
      std::vector<std::string> s = { "XSpider", "ZSpider", "Input", "Output" };
      return s[kind_];
    }
    /**
     * @brief get a name string of the ZXNode
     * @return name string of the ZXNode
     */
    std::string name() const;
  };
  
  /**
   * @class ZXEdge
   * @brief edge of ZX-diagram
   * @see ZXNode
   * @see ZXDiagram
  */
  class ZXEdge
  {
  private:
    ZXEdgeKind kind_; //!< kind of the edge of ZX-diagram
    uint32_t to_;  //!< node index connected to the edge of ZX-diagram
  public:
    /**
     * @brief constructor of the ZXEdge
     * @param [in] kind kind of the edge of ZX-diagram
     * @param [in] to node index connected to the edge of ZX-diagram
     */
    ZXEdge(const ZXEdgeKind kind = DEF_ZXEDGEKIND, const uint32_t to = 0) : kind_(kind), to_(to) {}
    /**
     * @brief copy constructor of the ZXEdge
     * @param [in] edge ZXEdge object
     */
    ZXEdge(const ZXEdge& edge) : kind_(edge.kind_), to_(edge.to_) {}
    //! getter of the kind_
    ZXEdgeKind kind() const { return kind_; }
    //! getter of the to_
    uint32_t to() const { return to_; }
    //! setter of the kind_
    void kind(const ZXEdgeKind kind) { kind_ = kind; }
    //! setter of the to_
    void to(const uint32_t to) { to_ = to; }
    /**
     * @brief get a string of the kind of ZXEdge
     * @return string of the kind of ZXEdge
     */
    std::string kind_str() const {
      std::vector<std::string> s = { "Plain", "Hadamard" };
      return s[kind_];
    }
    /**
     * @brief get a name string of the ZXEdge
     * @return name string of the ZXEdge
     */
    std::string name() const
    {
      std::string s = "";
      if (kind_ == ZXEdgeKind::Plain) s = "--";
      else if (kind_ == ZXEdgeKind::Hadamard) s = "++";
      s += "[" + std::to_string(to_) + "]";
      return s;
    }
    /**
     * @brief reverse the kind
     * @details "Plain" to "Hadamard", "Hadamard" to "Plain"
     */
    void reverse_kind()
    {
      if (kind_ == Plain) kind_ = Hadamard;
      else kind_ = Plain;
    }
  };
  
  /**
   * @class ZXDiagram
   * @brief ZX-diagram
   * @see ZXNode
   * @see ZXEdge
   * @sa 1. Ross Duncan, Aleks Kissinger, Simon Perdrix, John van de Wetering, "Graph-theoretic Simplification of Quantum Circuits with the ZX-calculus", [arXiv:1902.03178](https://arxiv.org/abs/1902.03178)
   * @sa 2. Aleks Kissinger, John van de Wetering, "Reducing T-count with the ZX-calculus", [arXiv:1903.10477](https://arxiv.org/abs/1903.10477)
   * @sa 3. Miriam Backens, Hector Miller-Bakewell, Giovanni de Felice, Leo Lobski, John van de Wetering, "There and back again: A circuit extraction tale", [arXiv:2003.01664](https://arxiv.org/abs/2003.01664)
   * @sa 4. Korbinian Staudacher, "Optimization Approaches for Quantum Circuits using ZX-calculus" [Ludwig maximilian university of munich thesis](https://www.mnm-team.org/pub/Diplomarbeiten/stau21/PDF-Version/stau21.pdf)
  */
  class ZXDiagram
  {
  private:
    ZXDiagramKind kind_; //!< kind of the ZX-diagram
    uint32_t qubit_num_; //!< number of qubits
    std::vector<ZXNode> nodes_; //!< vector of the ZXNode objects
    std::vector<uint32_t> inputs_; //!< vector of the input node indexes
    std::vector<uint32_t> outputs_; //!< vector of the output node indexes
    std::vector<std::vector<ZXEdge>> adj_mat_; //!< adjacent matrix of the ZX-diagram
    std::vector<ZXNodePlace> node_places_; //!< vector of the node places (Terminal, Boundary or Internal)
    /**
     * @brief get a maximum number of the adjacent nodes
     */
    uint32_t max_adj_num() const
    {
      uint32_t N = 0;
      for (uint32_t i = 0; nodes_.size(); ++i) {
	if (adj_mat_[i].size() > N) N = adj_mat_[i].size();
      }
      return N;
    }
    /**
     * @brief whether the node specified in this index is a X spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a X spider, false otherwise
     */
    bool check_x_spider(const uint32_t node_index) const { return nodes_[node_index].kind() == Sharqit::ZXNodeKind::XSpider; }
    /**
     * @brief whether the node specified in this index is a Z spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a Z spider, false otherwise
     */
    bool check_z_spider(const uint32_t node_index) const { return nodes_[node_index].kind() == Sharqit::ZXNodeKind::ZSpider; }
    /**
     * @brief whether the node specified in this index is a zero phase spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a zero phase spider, false otherwise
     */
    bool check_zero_phase_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(0))) return true;
      else return false;
    }
    /**
     * @brief whether the node specified in this index is a pauli spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a pauli spider, false otherwise
     */
    bool check_pauli_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(0) || nodes_[node_index].phase() == Phase(1))) return true;
      else return false;
    }
    /**
     * @brief whether the node specified in this index is a proper clifford spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a proper clifford spider, false otherwise
     */
    bool check_proper_clifford_spider(const uint32_t node_index) const
    {
      if ((nodes_[node_index].kind() == ZXNodeKind::XSpider || nodes_[node_index].kind() == ZXNodeKind::ZSpider) &&
	  (nodes_[node_index].phase() == Phase(1,2) || nodes_[node_index].phase() == Phase(3,2))) return true;
      else return false;
    }
    /**
     * @brief whether the node specified in this index is a clifford spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a clifford spider, false otherwise
     */
    bool check_clifford_spider(const uint32_t node_index) const
    {
      return (check_pauli_spider(node_index) || check_proper_clifford_spider(node_index));
    }
    /**
     * @brief whether the node specified in this index is a non-clifford spider or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a non-clifford spider, false otherwise
     */
    bool check_non_clifford_spider(const uint32_t node_index) const { return !check_clifford_spider(node_index); }
    /**
     * @brief whether the node specified in this index is a internal node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a internal node, false otherwise
     */
    bool check_internal_node(const uint32_t node_index) const { return node_places_[node_index] == ZXNodePlace::Internal; }
    /**
     * @brief whether the node specified in this index is a boundary node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a boundary node, false otherwise
     */
    bool check_boundary_node(const uint32_t node_index) const { return node_places_[node_index] == ZXNodePlace::Boundary; }
    /**
     * @brief whether the node specified in this index is a phase node of a phase gadget or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a phase node of a phase gadget, false otherwise
     */
    bool check_pg_phase_node(const uint32_t node_index) const { return nodes_[node_index].pg_phase(); }
    /**
     * @brief whether the node specified in this index is a root node of a phase gadget or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a root node of a phase gadget, false otherwise
     */
    bool check_pg_root_node(const uint32_t node_index) const { return nodes_[node_index].pg_root(); }
    /**
     * @brief whether the node specified in this index is a leaf node of a phase gadget or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a leaf node of a phase gadget, false otherwise
     */
    bool check_pg_leaf_node(const uint32_t node_index) const { return nodes_[node_index].pg_leaf(); }
    /**
     * @brief whether the node specified in this index is a node of a phase gadget (phase or root) or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a node of a phase gadget (phase or root), false otherwise
     */
    bool check_phase_gadget_node(const uint32_t node_index) const
    { return (nodes_[node_index].pg_root() || nodes_[node_index].pg_phase()); }
    /**
     * @brief whether the node specified in this index is a input node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a input node, false otherwise
     */
    bool check_input_node(const uint32_t node_index) const { return (nodes_[node_index].kind() == Sharqit::ZXNodeKind::Input); }
    /**
     * @brief whether the node specified in this index is a output node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index is a output node, false otherwise
     */
    bool check_output_node(const uint32_t node_index) const { return (nodes_[node_index].kind() == Sharqit::ZXNodeKind::Output); }
    /**
     * @brief whether the node specified in this index connects a input node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index connects a input node, false otherwise
     */
    bool check_connect_input_node(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (nodes_[e.to()].kind() == Sharqit::ZXNodeKind::Input) return true;
      }
      return false;
    }
    /**
     * @brief whether the node specified in this index connects a output node or not
     * @param [in] node_index node index
     * @return true if the node specified in this index connects a output node, false otherwise
     */
    bool check_connect_output_node(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (nodes_[e.to()].kind() == Sharqit::ZXNodeKind::Output) return true;
      }
      return false;
    }
    /**
     * @brief whether the node specified in this index connects a phase gadget (phase or root) or not
     * @param [in] node_index node index
     * @return true if the node specified in this index connects a phase gadget (phase or root), false otherwise
     */
    bool check_connect_phase_gadget(const uint32_t node_index)
    {
      for (auto& e:adj_mat_[node_index]) {
	if (check_pg_root_node(e.to()) || check_pg_phase_node(e.to())) return true;
      }
      return false;
    }
    /**
     * @brief whether the two nodes specified in these indexes are connected or not
     * @param [in] a node index
     * @param [in] b node index
     * @return true if the two nodes specified in these indexes are connected, false otherwise
     */
    bool check_connect(const uint32_t a, const uint32_t b) const;
    /**
     * @brief operate XOR by the hadamard edge between two nodes specified in these indexes
     * @param [in] a node index
     * @param [in] b node index
     */
    void xor_hadamard_edge(const uint32_t a, const uint32_t b);
    /**
     * @brief operate XOR by the all hadamard edges composed by any pair of two nodes
     * @param [in] node_indexes vector of the node indexes
     */
    void xor_hadamard_edges(std::vector<uint32_t> node_indexes);
    /**
     * @brief remove a node specified in the index
     * @param [in] node_index node index
     * @return kind of the node removed
     */
    ZXNodeKind remove_node(const uint32_t node_index);
    /**
     * @brief remove all edges connected to the node specified in the index
     * @param [in] node_index node index
     */
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
    /**
     * @brief get a degree of the node specified in the index
     * @param [in] node_index node index
     * @return degree of the node
     */
    uint32_t degree_of_node(const uint32_t node_index) { return adj_mat_[node_index].size(); }
    /**
     * @brief get a kind of the node specified in the index
     * @param [in] node_index node index
     * @return kind of the node
     */
    ZXNodeKind kind_of_node(const uint32_t node_index) { return nodes_[node_index].kind(); }
    /**
     * @brief swap two nodes specified in the indexes
     * @param [in] i node index
     * @param [in] j node index
     */
    void swap_nodes(uint32_t i, uint32_t j);
    /**
     * @brief remove isolated spiders
     */
    void remove_isolated_spiders();
    /**
     * @brief execute row operation for the two nodes specified in the indexes
     * @details from the edges of node(a) to the edges of node(b)
     * @param [in] a node index
     * @param [in] b node index
     */
    void row_operation(const uint32_t a, const uint32_t b);
    /**
     * @brief get a vector of nodes adjacent to the node specified in the index
     * @param [in] node_index node index
     * @return vector of nodes adjacent to the node specified in the index
     */
    std::vector<uint32_t> adjacent_node_indexes(uint32_t node_index);
    /**
     * @brief convert all X spiders to Z spiders with hadamard edges
     */
    void conv_x_to_z();
    /**
     * @brief fuse all spiders
     */
    void fuse_spiders();
    /**
     * @brief remove all parallel hadamard edges and self-looped hadamard edges
     */
    void remove_parallel_selfloops_hadamard_edges();
    /**
     * @brief add swap gates to reorder the swapped qubit indexes to the original order in process of extracting the quantum circuit
     * @param [in] qc the quantum circuit that qubit indexes is swapped
     */
    void permutation_as_swap(QCirc& qc);
    /**
     * @brief update the frontier spiders initially
     * @param [in] frontier indexes' vector of frontier spiders introduced in the process of extracting quantum circuit
     * @param [in,out] qc the quantum circuit
     * @param [in] opt_cz optimize CZ or not
     * @sa ZXDiagram::update_frontier
     * @sa ZXDiagram::update_frontier_pg
     */
    void process_frontier(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc, const bool opt_cz = false);
    /**
     * @brief extract quantum gates from the ZX-diagram and update the frontier spiders
     * @param [in,out] frontier indexes' vector of frontier spiders introduced in the process of extracting quantum circuit
     * @param [in,out] qc the quantum circuit
     * @param [in] opt_cz optimize CZ or not
     * @sa ZXDiagram::process_frontier
     * @return updated or not
     */
    bool update_frontier(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc, const bool opt_cz = false);
    /**
     * @brief update frontier spiders in the case that the frontier can't be updated because of invalid biadjacency matrix related to the frontier
     * @param [in,out] frontier indexes' vector of frontier spiders introduced in the process of extracting quantum circuit
     * @param [in,out] qc the quantum circuit
     * @sa ZXDiagram::process_frontier
     * @sa ZXDiagram::update_frontier
     * @return updated or not
     */
    bool update_frontier_pg(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc);
    /**
     * @brief apply a local complimmentation algorithm for the node specified in the index
     * @param [in] idx_A node index
     * @sa ZXDiagram::lcomp
     */
    void lcomp_one_time(const uint32_t idx_A);
    /**
     * @brief apply a pivot1 algorithm for the nodes specified in the indexes
     * @param [in] idx_A node index
     * @param [in] idx_B node index
     * @sa ZXDiagram::pivot1
     */
    void pivot1_one_time(const uint32_t idx_A, const uint32_t idx_B);
    /**
     * @brief apply a pivot2 algorithm for the nodes specified in the indexes
     * @param [in] idx_A node index
     * @param [in] idx_B node index
     * @sa ZXDiagram::pivot2
     */
    void pivot2_one_time(const uint32_t idx_A, const uint32_t idx_B);
    /**
     * @brief apply a pivot3 algorithm for the nodes specified in the indexes
     * @param [in] idx_A node index
     * @param [in] idx_B node index
     * @sa ZXDiagram::pivot3
     */
    void pivot3_one_time(const uint32_t idx_A, const uint32_t idx_B);
    /**
     * @brief apply a gadget fusion algorithm for the nodes specified in the indexes
     * @param [in] idx_A_phase node index
     * @param [in] idx_B_phase node index
     * @sa ZXDiagram::gfusion
     */
    void gfusion_one_time(const uint32_t idx_A_phase, const uint32_t idx_B_phase);
    /**
     * @brief extract 2Q connections from the frontier (for CZ optimization in the update_frontier function)
     * @param [in] frontier indexes' vector of frontier spiders introduced in the process of extracting quantum circuit
     * @return vector of pairs of the indexes
     */
    std::vector<std::pair<uint32_t, uint32_t>> extract_2q_connects(std::vector<uint32_t>& frontier) const;
  public:
    /**
     * @brief constructor of the ZXDiagram
     * @param [in] qubit_num number of qubits
     */
    ZXDiagram(const uint32_t qubit_num = 1);
    /**
     * @brief copy constructor of the ZXDiagram
     * @param [in] zx ZXDiagram object
     */
    ZXDiagram(const ZXDiagram& zx)
      : kind_(zx.kind_), qubit_num_(zx.qubit_num_), nodes_(zx.nodes_),
    	inputs_(zx.inputs_), outputs_(zx.outputs_), adj_mat_(zx.adj_mat_),
    	node_places_(zx.node_places_) {}
    //! getter of the kind_
    ZXDiagramKind kind() const { return kind_; }
    //! getter of the qubit_num_
    uint32_t qubit_num() const { return qubit_num_; }
    //! getter of the nodes_
    std::vector<ZXNode> nodes() const { return nodes_; }
    //! getter of the inputs_
    std::vector<uint32_t> inputs() const { return inputs_; }
    //! getter of the outputs_
    std::vector<uint32_t> outputs() const { return outputs_; }
    //! getter of the adj_mat_
    std::vector<std::vector<ZXEdge>> adj_mat() const { return adj_mat_; }
    //! setter of the kind_
    void kind(const ZXDiagramKind kind) { kind_ = kind; }
    //! setter of the qubit_num_
    void qubit_num(const uint32_t qubit_num) { qubit_num_ = qubit_num; }
    //! setter of the nodes_
    void nodes(const std::vector<ZXNode>& nodes) { nodes_ = nodes; }
    //! setter of the inputs_
    void inputs(const std::vector<uint32_t>& inputs) { inputs_ = inputs; }
    //! setter of the outputs_
    void outputs(const std::vector<uint32_t>& outputs) { outputs_ = outputs; }
    //! setter of the adj_mat_
    void adj_mat(std::vector<std::vector<ZXEdge>>& adj_mat) { adj_mat_ = adj_mat; }
    /* member functions */
    /**
     * @brief get a string of the ZXDiagram object
     * @return string of the ZX-diagram
     */
    std::string to_string() const;
    /**
     * @brief show the ZX-diagram
     */
    void show() const { std::cout << to_string() << std::endl; }
    /**
     * @brief get stats of the ZX-diagram
     * @return stats of the ZX-diagram
     */
    std::map<std::string, uint32_t> stats() const;
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
     * @brief get a spider-count
     */
    uint32_t spider_count() const { return (nodes_.size() - inputs_.size() - outputs_.size()); }
    /**
     * @brief get a non-clifford-count
     */
    uint32_t non_clifford_count() const
    {
      uint32_t cnt = 0;
      for (uint32_t i = 0; i < nodes_.size(); ++i) {
	if (check_non_clifford_spider(i)) ++cnt;
      }
      return cnt;
    }
    /**
     * @brief add a quantum gate to the ZX-diagram
     * @param [in] qgate quantum gate added
     */
    void add_qgate(const QGate& qgate);
    /**
     * @brief append a node of ZX-diagram
     * @param [in] node node of ZX-diagram
     */
    uint32_t append_node(const ZXNode node);
    /**
     * @brief append a node and an edge connected to the node
     * @param [in] node node of ZX-diagram
     * @param [in] edge edge of ZX-diagram
     */
    uint32_t append_node(const ZXNode node, const ZXEdge edge);
    /**
     * @brief make an edge connected two nodes specified the indexes, and assign the kind to the edge
     * @param [in] a node index to be connected
     * @param [in] b node index to be connected
     * @param [in] kind kind of the edge
     */
    void connect_nodes(const uint32_t a, const uint32_t b, const ZXEdgeKind kind);
    /**
     * @brief remove an edge between two nodes specified in the indexes
     * @param [in] a node index to be connected
     * @param [in] b node index to be connected
     * @return kind of the edge removed
     */
    ZXEdgeKind remove_edge(const uint32_t a, const uint32_t b);
    /**
     * @brief update the vector of the node places
     * @details this function is always called after updating the ZX-diagram
     */
    void update_node_places();
    /**
     * @brief update the vector of the phase gadget
     * @details this function is always called after updating the ZX-diagram
     */
    void update_phase_gadget();
    /**
     * @brief convert the ZX-diagram to the graph-like ZX-diagram
     */
    void graph_like();
    /**
     * @brief apply a local complimmentation algorithm
     * @sa ZXDiagram::lcomp_one_time
     */
    uint32_t lcomp();
    /**
     * @brief apply a pivot1 algorithm
     * @sa ZXDiagram::pivot1_one_time
     */
    uint32_t pivot1();
    /**
     * @brief apply a pivot2 algorithm
     * @sa ZXDiagram::pivot2_one_time
     */
    uint32_t pivot2();
    /**
     * @brief apply a pivot3 algorithm
     * @sa ZXDiagram::pivot3_one_time
     */
    uint32_t pivot3();
    /**
     * @brief apply a gadget fusion algorithm
     * @sa ZXDiagram::gfusion_one_time
     */
    uint32_t gfusion();
    /**
     * @brief remove all identity spiders
     */
    void id_removal();
    /**
     * @brief simplify the ZX-diagram
     * @details apply the algorithm of local complimentation, pivot1, pivot2, pivot3, and gadget fusion, for graph-like diagram converted the original ZX-diagram
     */
    void simplify();
    /**
     * @brief extract a quantum circuit from the graph-like ZX-diagram
     */
    QCirc extract_qcirc();
    friend std::ostream& operator<<(std::ostream& ost, const ZXDiagram& zx) { ost << zx.to_string(); return ost; }
  };
}

#endif
