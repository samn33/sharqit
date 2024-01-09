#include "dag.h"

/**
 *  member functions
 */

Sharq::DAGCirc::DAGCirc(uint32_t qubit_num)
{
  qubit_num_ = qubit_num;
  inputs_.resize(qubit_num);
  outputs_.resize(qubit_num);

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_in(Sharq::DAGNodeKind::InNode, Sharq::QGate(Sharq::QGateKind::Id, {i}));
    inputs_[i] = append_node(node_in);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_out(Sharq::DAGNodeKind::OutNode, Sharq::QGate(Sharq::QGateKind::Id, {i}));
    outputs_[i] = append_node(node_out);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    connect_nodes(inputs_[i], outputs_[i], i);
  }
}

Sharq::DAGCirc::DAGCirc(const QCirc& qc)
{
  uint32_t qubit_num = qc.qubit_num();
  qubit_num_ = qubit_num;
  inputs_.resize(qubit_num);
  outputs_.resize(qubit_num);

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_in(Sharq::DAGNodeKind::InNode, Sharq::QGate(Sharq::QGateKind::Id, {i}));
    inputs_[i] = append_node(node_in);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_out(Sharq::DAGNodeKind::OutNode, Sharq::QGate(Sharq::QGateKind::Id, {i}));
    outputs_[i] = append_node(node_out);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    connect_nodes(inputs_[i], outputs_[i], i);
  }

  for (auto& qgate:qc.qgates()) add_qgate(qgate);
}

std::string Sharq::DAGCirc::to_string() const
{
  std::stringstream ss;
  std::vector<Sharq::DAGNode> nodes = nodes_;
  
  ss << "qubit_num: " << qubit_num_ << std::endl;
  ss << "adj_mat:" << std::endl;
  for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
    ss << " [" << i << "] " << nodes[i].name() + " ";
    for (auto& edge:adj_mat_[i]) {
      ss << edge.name() + ",";
    }
    ss << std::endl;
  }
  std::string s = ss.str();
  s.pop_back();
  return s;
}

uint32_t Sharq::DAGCirc::append_node(const Sharq::DAGNode node)
{
  nodes_.push_back(node);
  adj_mat_.resize(nodes_.size());
  uint32_t node_idx = nodes_.size() - 1;
  return node_idx;
}

uint32_t Sharq::DAGCirc::prev_node(const uint32_t idx, const uint32_t q)
{
  if (!nodes_[idx].is_included(q)) {
    throw std::runtime_error("invalid node id or qubit id.");
  }

  uint32_t prev_idx = 0;
  for (auto& e:adj_mat_[idx]) {
    if (e.is_backward() && e.q() == q) {
	prev_idx = e.to();
	break;
    }
  }
  return prev_idx;
}

uint32_t Sharq::DAGCirc::next_node(const uint32_t idx, const uint32_t q)
{
  if (!nodes_[idx].is_included(q)) {
    throw std::runtime_error("invalid node id or qubit id.");
  }

  uint32_t next_idx = 0;
  for (auto& e:adj_mat_[idx]) {
    if (e.is_forward() && e.q() == q) {
	next_idx = e.to();
	break;
    }
  }
  return next_idx;
}

std::vector<uint32_t> Sharq::DAGCirc::prev_nodes(const uint32_t idx)
{
  uint32_t nid_first = 0; // previous node id for first qubit
  uint32_t nid_second = 0; // previous node id for second qubit (only for 2-qubit node)
  for (auto& e:adj_mat_[idx]) {
    if (e.is_backward()) {
      if (e.q() == nodes_[idx].qid()[0]) nid_first = e.to();
      else nid_second = e.to();
    }
  }

  uint32_t qubit_num = nodes_[idx].qubit_num();
  std::vector<uint32_t> prev(qubit_num);
  if (qubit_num == 1) {
    prev[0] = nid_first;
  }
  else if (qubit_num == 2) {
    prev[0] = nid_first;
    prev[1] = nid_second;
  }
  
  return prev;
}

std::vector<uint32_t> Sharq::DAGCirc::next_nodes(const uint32_t idx)
{
  uint32_t nid_first = 0; // next node id for first qubit
  uint32_t nid_second = 0; // next node id for second qubit (only for 2-qubit node)
  for (auto& e:adj_mat_[idx]) {
    if (e.is_forward()) {
      if (e.q() == nodes_[idx].qid()[0]) nid_first = e.to();
      else nid_second = e.to();
    }
  }

  uint32_t qubit_num = nodes_[idx].qubit_num();
  std::vector<uint32_t> next(qubit_num);
  if (qubit_num == 1) {
    next[0] = nid_first;
  }
  else if (qubit_num == 2) {
    next[0] = nid_first;
    next[1] = nid_second;
  }
  
  return next;
}

void Sharq::DAGCirc::connect_nodes(const uint32_t a, const uint32_t b, const uint32_t q)
{
  /* check two nodes (two gates) include q (qubit id) */
  if (!nodes_[a].is_included(q) && !nodes_[b].is_included(q)) {
    throw std::runtime_error("can't connect two nodes have different qid.");
  }
  /* connect two nodes */
  adj_mat_[a].push_back(Sharq::DAGEdge(Sharq::DAGEdgeKind::Forward, q, b));
  adj_mat_[b].push_back(Sharq::DAGEdge(Sharq::DAGEdgeKind::Backward, q, a));
}

void Sharq::DAGCirc::remove_node(const uint32_t idx)
{
  adj_mat_.erase(adj_mat_.begin() + idx);
  for (auto& node:adj_mat_) {
    for (auto it = node.begin(); it != node.end();) {
      if (it->to() == idx) {
  	it = node.erase(it);
      }
      else if (it->to() > idx) {
  	it->to(it->to() - 1);
	++it;
      }
      else {
	++it;
      }
    }
  }
  nodes_.erase(nodes_.begin() + idx);

  /* update Inputs and Outputs */
  for (uint32_t i = 0; i < inputs_.size(); ++i) {
    if (inputs_[i] > idx) {
      inputs_[i]--;
    }
  }
  for (uint32_t i = 0; i < outputs_.size(); ++i) {
    if (outputs_[i] > idx) {
      outputs_[i]--;
    }
  }
}

void Sharq::DAGCirc::remove_edge(const uint32_t a, const uint32_t b)
{
  if (a == b) return;
  
  for (auto it = adj_mat_[a].begin(); it != adj_mat_[a].end(); ++it) {
    if (it->to() == b) {
      it = adj_mat_[a].erase(it);
      --it;
    }
  }
  for (auto it = adj_mat_[b].begin(); it != adj_mat_[b].end(); ++it) {
    if (it->to() == a) {
      it = adj_mat_[b].erase(it);
      --it;
    }
  }
}

void Sharq::DAGCirc::remove_edges_of_node(const uint32_t idx)
{
  adj_mat_[idx].clear();
  for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
    if (i == idx) continue;
    for (auto it = adj_mat_[i].begin(); it != adj_mat_[i].end(); ++it) {
      if (it->to() == idx) {
	it = adj_mat_[i].erase(it);
	--it;
      }
    }
  }
}

void Sharq::DAGCirc::remove_isolated_nodes()
{
  uint32_t size = adj_mat_.size();
  for (uint32_t i = 0; i < size; ++i) {
    bool find = false;
    for (uint32_t j = 0; j < adj_mat_.size(); ++j) {
      if (adj_mat_[j].size() == 0) {
	remove_node(j);
	find = true;
	break;
      }
    }
    if (!find) break;
  }
}

Sharq::DAGCirc& Sharq::DAGCirc::add_qgate(const QGate& qgate)
{
  DAGNode node(Sharq::DAGNodeKind::OpNode, qgate);
  nodes_.push_back(node);
  uint32_t new_idx = nodes_.size() - 1;
  adj_mat_.push_back({});

  /* X,Z,S,Sdg,T,Tdg,RX,RZ */
  if (qgate.qubit_num() == 1) {
    uint32_t q = qgate.qid()[0];
    uint32_t from = adj_mat_[outputs_[q]][0].to();
    uint32_t to = outputs_[q];
    remove_edge(from, to);
    connect_nodes(from, new_idx, q);
    connect_nodes(new_idx, to, q);
  }
  else if (qgate.qubit_num() == 2) {
    uint32_t c = qgate.qid()[0];
    uint32_t t = qgate.qid()[1];
    uint32_t from_c = adj_mat_[outputs_[c]][0].to();
    uint32_t from_t = adj_mat_[outputs_[t]][0].to();
    uint32_t to_c = outputs_[c];
    uint32_t to_t = outputs_[t];
    remove_edge(from_c, to_c);
    remove_edge(from_t, to_t);
    connect_nodes(from_c, new_idx, c);
    connect_nodes(from_t, new_idx, t);
    connect_nodes(new_idx, to_c, c);
    connect_nodes(new_idx, to_t, t);
  }
  else {
    throw std::runtime_error("number of qubit must be 1, or 2.");
  }

  return *this;
}

Sharq::QCirc Sharq::DAGCirc::to_qcirc() const
{
  /* topological sort using BFS(Breadth-First Search) */
  std::vector<uint32_t> indim(nodes_.size(), 0);
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    for (auto& e:adj_mat_[i]) {
      if (e.kind() == Sharq::DAGEdgeKind::Forward) indim[e.to()]++;
    }
  }

  std::queue<uint32_t> que;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (indim[i] == 0) que.push(i);
  }

  Sharq::QCirc qc(qubit_num());
  uint32_t q_max = 0;
  while (!que.empty()) {
    uint32_t idx = que.front();
    que.pop();
    if (nodes_[idx].is_opnode()) {
      qc.add_qgate(nodes_[idx].qgate());
      for (auto& q:nodes_[idx].qgate().qid()) { if (q > q_max) q_max = q; }
    }
    for (auto& e:adj_mat_[idx]) {
      if (e.kind() == Sharq::DAGEdgeKind::Forward) {
	indim[e.to()]--;
	if (indim[e.to()] == 0) {
	  que.push(e.to());
	  indim[e.to()]++;
	}
      }
    }
  }

  if (q_max < qubit_num_ - 1) {
    qc.add_qgate(Sharq::QGate(Sharq::QGateKind::Id, {qubit_num_ - 1}));
  }
  
  return qc;
}

void Sharq::DAGCirc::id_removal()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (nodes_[i].is_opnode() && nodes_[i].qgate().is_Id_gate()) {
      uint32_t q = nodes_[i].qgate().qid()[0];
      uint32_t prev = prev_nodes(i)[0];
      uint32_t next = next_nodes(i)[0];
      connect_nodes(prev, next, q);
      remove_edges_of_node(i);
    }
    else if (nodes_[i].is_opnode() && nodes_[i].qgate().is_Id2_gate()) {
      for (uint32_t j = 0; j < nodes_[i].qgate().qid().size(); ++j) {
	uint32_t q = nodes_[i].qgate().qid()[j];
	uint32_t prev = prev_nodes(i)[j];
	uint32_t next = next_nodes(i)[j];
	connect_nodes(prev, next, q);
      }
      remove_edges_of_node(i);
    }
  }
  remove_isolated_nodes();
}

void Sharq::DAGCirc::cx_to_cz_gate()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    uint32_t con = 0;
    uint32_t tar = 0;
    uint32_t idx_p = 0; 
    uint32_t idx_c = 0;
    uint32_t idx_n = 0;
    bool match = false;
    if (nodes_[i].qgate().is_CX_gate()) {
      con = nodes_[i].qgate().qid()[0];
      tar = nodes_[i].qgate().qid()[1];
      idx_c = i;
      idx_p = prev_nodes(i)[1];
      idx_n = next_nodes(i)[1];
      if ((nodes_[idx_p].qgate().is_H_gate() && nodes_[idx_n].qgate().is_H_gate())) {
	match = true;
      }
    }

    if (!match) continue;

    nodes_[idx_p].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
    nodes_[idx_c].qgate(Sharq::QGate(Sharq::QGateKind::CZ, {con,tar}));
    nodes_[idx_n].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
  }

  id_removal();
}

void Sharq::DAGCirc::hadamard_gate_reduction_1()
{
  uint32_t a = 0; // node id
  uint32_t b = 0; // node id
  uint32_t c = 0; // node id
  uint32_t q = 0; // qubit id
  std::vector<uint32_t> next;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    bool match = false;
    if (nodes_[i].qgate().is_H_gate()) {
      a = i;
      q = nodes_[i].qgate().qid()[0];
      next = next_nodes(i);
      if (next.empty()) continue;
      if (nodes_[next[0]].qgate().is_S_gate()) {
	b = next[0];
	next = next_nodes(next[0]);
	if (next.empty()) continue;
	if (nodes_[next[0]].qgate().is_H_gate()) {
	  c = next[0];
	  match = true;
	}
      }
    }

    if (!match) continue;

    Sharq::QGate s_gate = nodes_[b].qgate();
    if (s_gate.kind() == Sharq::QGateKind::S ||
	(s_gate.kind() == Sharq::QGateKind::RZ && s_gate.phase() == Sharq::Phase(1,2))) {
      nodes_[a].qgate(Sharq::QGate(Sharq::QGateKind::Sdg, {q}));
      nodes_[b].qgate(Sharq::QGate(Sharq::QGateKind::H, {q}));
      nodes_[c].qgate(Sharq::QGate(Sharq::QGateKind::Sdg, {q}));
    }
    else {
      nodes_[a].qgate(Sharq::QGate(Sharq::QGateKind::S, {q}));
      nodes_[b].qgate(Sharq::QGate(Sharq::QGateKind::H, {q}));
      nodes_[c].qgate(Sharq::QGate(Sharq::QGateKind::S, {q}));
    }
  }
}

void Sharq::DAGCirc::hadamard_gate_reduction_2()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    uint32_t con = 0;
    uint32_t tar = 0;
    uint32_t idx = 0;
    std::vector<uint32_t> prev;
    std::vector<uint32_t> next;

    bool match = false;
    if (nodes_[i].qgate().is_CX_gate()) {
      prev = prev_nodes(i);
      next = next_nodes(i);
      if (nodes_[prev[0]].qgate().is_H_gate() &&
	  nodes_[prev[1]].qgate().is_H_gate() &&
	  nodes_[next[0]].qgate().is_H_gate() &&
	  nodes_[next[1]].qgate().is_H_gate()) {
	match = true;
	idx = i;
	con = nodes_[i].qgate().qid()[0];
	tar = nodes_[i].qgate().qid()[1];
      }
    }

    if (!match) continue;

    nodes_[prev[0]].qgate(Sharq::QGate(Sharq::QGateKind::Id, {con}));
    nodes_[prev[1]].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
    nodes_[idx].qgate(Sharq::QGate(Sharq::QGateKind::CX, {tar,con}));
    nodes_[next[0]].qgate(Sharq::QGate(Sharq::QGateKind::Id, {con}));
    nodes_[next[1]].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
  }

  id_removal();
}

void Sharq::DAGCirc::hadamard_gate_reduction_3()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    uint32_t con = 0;
    uint32_t tar = 0;
    uint32_t idx_pp = 0;
    uint32_t idx_p = 0; 
    uint32_t idx_c = 0;
    uint32_t idx_n = 0;
    uint32_t idx_nn = 0;
    
    bool match1 = false;
    bool match2 = false;
    if (nodes_[i].qgate().is_CX_gate()) {
      con = nodes_[i].qgate().qid()[0];
      tar = nodes_[i].qgate().qid()[1];
      idx_c = i;
      idx_p = prev_nodes(i)[1];
      idx_n = next_nodes(i)[1];
      if ((nodes_[idx_p].qgate().is_S1_gate() && nodes_[idx_n].qgate().is_S3_gate())) {
	idx_pp = prev_nodes(idx_p)[0];
	idx_nn = next_nodes(idx_n)[0];
	if (nodes_[idx_pp].qgate().is_H_gate() && nodes_[idx_nn].qgate().is_H_gate()) {
	  match1 = true;
	  match2 = false;
	}
      }
      else if ((nodes_[idx_p].qgate().is_S3_gate() && nodes_[idx_n].qgate().is_S1_gate())) {
	idx_pp = prev_nodes(idx_p)[0];
	idx_nn = next_nodes(idx_n)[0];
	if (nodes_[idx_pp].qgate().is_H_gate() && nodes_[idx_nn].qgate().is_H_gate()) {
	  match1 = false;
	  match2 = true;
	}
      }
    }

    if (!match1 && !match2) continue;

    if (match1) {
      nodes_[idx_pp].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
      nodes_[idx_p].qgate(Sharq::QGate(Sharq::QGateKind::Sdg, {tar}));
      nodes_[idx_c].qgate(Sharq::QGate(Sharq::QGateKind::CX, {con,tar}));
      nodes_[idx_n].qgate(Sharq::QGate(Sharq::QGateKind::S, {tar}));
      nodes_[idx_nn].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
    }
    else if (match2) {
      nodes_[idx_pp].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
      nodes_[idx_p].qgate(Sharq::QGate(Sharq::QGateKind::S, {tar}));
      nodes_[idx_c].qgate(Sharq::QGate(Sharq::QGateKind::CX, {con,tar}));
      nodes_[idx_n].qgate(Sharq::QGate(Sharq::QGateKind::Sdg, {tar}));
      nodes_[idx_nn].qgate(Sharq::QGate(Sharq::QGateKind::Id, {tar}));
    }
  }

  id_removal();
}

void Sharq::DAGCirc::hadamard_gate_reduction()
{
  hadamard_gate_reduction_1();
  hadamard_gate_reduction_2();
  hadamard_gate_reduction_3();
}

void Sharq::DAGCirc::single_qubit_gate_cancellation()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (!nodes_[i].is_opnode() || nodes_[i].qubit_num() > 1) continue;
    uint32_t q = nodes_[i].qid()[0];
    uint32_t idx = next_node(i, q);
    while (true) {
      if (nodes_[idx].is_outnode()) break;

      if (nodes_[i].mergeable(nodes_[idx])) {
	nodes_[idx].merge(nodes_[i]);
	nodes_[i].qgate(Sharq::QGate(Sharq::QGateKind::Id, {q}));
	break;
      }
      else if (nodes_[i].commutable(nodes_[idx])) {
	idx = next_node(idx, q);
	if (nodes_[idx].is_outnode()) break;
      }
      else {
	break;
      }
    }
  }
  id_removal();
}

void Sharq::DAGCirc::two_qubit_gate_cancellation()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (!nodes_[i].is_opnode() || nodes_[i].qubit_num() <= 1) continue;

    uint32_t con = nodes_[i].qid()[0];
    uint32_t tar = nodes_[i].qid()[1];
    /* previous node id and next node id adjacent to control qubit of nodes_[i] */
    uint32_t idx_con_prev = prev_node(i, con);
    uint32_t idx_con_next = next_node(i, con);
    /* previous node id and next node id adjacent to target qubit of nodes_[i] */
    uint32_t idx_tar_prev = prev_node(i, tar);
    uint32_t idx_tar_next = next_node(i, tar);

    uint32_t idx_con = idx_con_next;
    uint32_t idx_tar = idx_tar_next;
    while (true) {
      /* while loop is done if reach to the output nodes */
      if (nodes_[idx_con].is_outnode() && nodes_[idx_tar].is_outnode()) break;

      /* commute and merge */
      if (nodes_[i].mergeable(nodes_[idx_con]) && nodes_[i].mergeable(nodes_[idx_tar])) {
	nodes_[idx_tar].merge(nodes_[i]);
	remove_edges_of_node(i);
	connect_nodes(idx_con_prev, idx_con_next, con);
	connect_nodes(idx_tar_prev, idx_tar_next, tar);
      }
      else if (nodes_[i].mergeable(nodes_[idx_con]) && nodes_[i].commutable(nodes_[idx_tar])) {
	idx_tar = next_node(idx_tar, tar);
      }
      else if (nodes_[i].commutable(nodes_[idx_con]) && nodes_[i].mergeable(nodes_[idx_tar])) {
	idx_con = next_node(idx_con, con);
      }
      else if (nodes_[i].commutable(nodes_[idx_con]) && nodes_[i].commutable(nodes_[idx_tar])) {
	if (!nodes_[idx_con].is_outnode()) idx_con = next_node(idx_con, con);
	if (!nodes_[idx_tar].is_outnode()) idx_tar = next_node(idx_tar, tar);
	if (nodes_[idx_con].is_outnode() && nodes_[idx_tar].is_outnode()) break;
      }
      else { /* while loop is done if reach to a non-commutable node */
	break;
      }
    }
  }
  
  id_removal();
}

void Sharq::DAGCirc::rule_based_gate_reduction()
{
  cx_to_cz_gate();
  hadamard_gate_reduction();
  two_qubit_gate_cancellation();
  single_qubit_gate_cancellation();
  two_qubit_gate_cancellation();
  hadamard_gate_reduction();
  single_qubit_gate_cancellation();
}
