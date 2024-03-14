/**
 * @file dagcirc.cpp
 * @brief member functions of DAGCirc class
 */

#include "dag.h"

Sharqit::DAGCirc::DAGCirc(uint32_t qubit_num)
{
  qubit_num_ = qubit_num;
  inputs_.resize(qubit_num);
  outputs_.resize(qubit_num);

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_in(Sharqit::DAGNodeKind::InNode, Sharqit::QGate(Sharqit::QGateKind::Id, {i}));
    inputs_[i] = append_node(node_in);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_out(Sharqit::DAGNodeKind::OutNode, Sharqit::QGate(Sharqit::QGateKind::Id, {i}));
    outputs_[i] = append_node(node_out);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    connect_nodes(inputs_[i], outputs_[i], i);
  }
}

Sharqit::DAGCirc::DAGCirc(const QCirc& qc)
{
  uint32_t qubit_num = qc.qubit_num();
  qubit_num_ = qubit_num;
  inputs_.resize(qubit_num);
  outputs_.resize(qubit_num);

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_in(Sharqit::DAGNodeKind::InNode, Sharqit::QGate(Sharqit::QGateKind::Id, {i}));
    inputs_[i] = append_node(node_in);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    DAGNode node_out(Sharqit::DAGNodeKind::OutNode, Sharqit::QGate(Sharqit::QGateKind::Id, {i}));
    outputs_[i] = append_node(node_out);
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    connect_nodes(inputs_[i], outputs_[i], i);
  }

  for (auto& qgate:qc.qgates()) add_qgate(qgate);
}

std::string Sharqit::DAGCirc::to_string() const
{
  std::stringstream ss;
  std::vector<Sharqit::DAGNode> nodes = nodes_;
  
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

uint32_t Sharqit::DAGCirc::append_node(const Sharqit::DAGNode node)
{
  nodes_.push_back(node);
  adj_mat_.resize(nodes_.size());
  uint32_t node_idx = nodes_.size() - 1;
  return node_idx;
}

uint32_t Sharqit::DAGCirc::prev_node(const uint32_t idx, const uint32_t q)
{
  if (!nodes_[idx].is_included(q)) {
    throw std::runtime_error("invalid node id or qubit id.");
  }
  else if (nodes_[idx].is_innode()) {
    throw std::runtime_error("previous node isn't exist because it's an input node.");
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

uint32_t Sharqit::DAGCirc::next_node(const uint32_t idx, const uint32_t q)
{
  if (!nodes_[idx].is_included(q)) {
    throw std::runtime_error("invalid node id or qubit id.");
  }
  else if (nodes_[idx].is_outnode()) {
    throw std::runtime_error("next node isn't exist because it's an output node.");
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

void Sharqit::DAGCirc::connect_nodes(const uint32_t a, const uint32_t b, const uint32_t q)
{
  /* check two nodes (two gates) include q (qubit id) */
  if (!nodes_[a].is_included(q) && !nodes_[b].is_included(q)) {
    throw std::runtime_error("can't connect two nodes have different qid.");
  }
  /* connect two nodes */
  adj_mat_[a].push_back(Sharqit::DAGEdge(Sharqit::DAGEdgeKind::Forward, q, b));
  adj_mat_[b].push_back(Sharqit::DAGEdge(Sharqit::DAGEdgeKind::Backward, q, a));
}

void Sharqit::DAGCirc::remove_node(const uint32_t idx)
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

void Sharqit::DAGCirc::remove_edge(const uint32_t a, const uint32_t b)
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

void Sharqit::DAGCirc::remove_edge(const uint32_t a, const uint32_t b, const uint32_t q)
{
  if (a == b) return;
  
  for (auto it = adj_mat_[a].begin(); it != adj_mat_[a].end(); ++it) {
    if (it->to() == b && it->q() == q) {
      it = adj_mat_[a].erase(it);
      --it;
    }
  }
  for (auto it = adj_mat_[b].begin(); it != adj_mat_[b].end(); ++it) {
    if (it->to() == a && it->q() == q) {
      it = adj_mat_[b].erase(it);
      --it;
    }
  }
}

void Sharqit::DAGCirc::remove_edges_of_node(const uint32_t idx)
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

void Sharqit::DAGCirc::remove_isolated_nodes()
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

Sharqit::DAGCirc& Sharqit::DAGCirc::add_qgate(const QGate& qgate)
{
  DAGNode node(Sharqit::DAGNodeKind::OpNode, qgate);
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

Sharqit::QCirc Sharqit::DAGCirc::to_qcirc() const
{
  /* topological sort using BFS(Breadth-First Search) */
  std::vector<uint32_t> indim(nodes_.size(), 0);
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    for (auto& e:adj_mat_[i]) {
      if (e.kind() == Sharqit::DAGEdgeKind::Forward) indim[e.to()]++;
    }
  }

  std::queue<uint32_t> que;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (indim[i] == 0) que.push(i);
  }

  Sharqit::QCirc qc(qubit_num());
  uint32_t q_max = 0;
  while (!que.empty()) {
    uint32_t idx = que.front();
    que.pop();
    if (nodes_[idx].is_opnode()) {
      qc.add_qgate(nodes_[idx].qgate());
      for (auto& q:nodes_[idx].qgate().qid()) { if (q > q_max) q_max = q; }
    }
    for (auto& e:adj_mat_[idx]) {
      if (e.kind() == Sharqit::DAGEdgeKind::Forward) {
	indim[e.to()]--;
	if (indim[e.to()] == 0) {
	  que.push(e.to());
	  indim[e.to()]++;
	}
      }
    }
  }

  if (q_max < qubit_num_ - 1) {
    qc.add_qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {qubit_num_ - 1}));
  }
  
  return qc;
}

void Sharqit::DAGCirc::id_removal()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (nodes_[i].is_opnode() && nodes_[i].qgate().is_Id_gate()) {
      uint32_t q = nodes_[i].qgate().qid()[0];
      uint32_t prev = prev_node(i, q);
      uint32_t next = next_node(i, q);
      connect_nodes(prev, next, q);
      remove_edges_of_node(i);
    }
    else if (nodes_[i].is_opnode() && nodes_[i].qgate().is_Id2_gate()) {
      for (uint32_t j = 0; j < nodes_[i].qgate().qid().size(); ++j) {
	uint32_t q = nodes_[i].qgate().qid()[j];
	uint32_t prev = prev_node(i, q);
	uint32_t next = next_node(i, q);
	connect_nodes(prev, next, q);
      }
      remove_edges_of_node(i);
    }
  }
  remove_isolated_nodes();
}

void Sharqit::DAGCirc::cx_to_cz_gate()
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
      idx_p = prev_node(i, tar);
      idx_n = next_node(i, tar);
      if ((nodes_[idx_p].qgate().is_H_gate() && nodes_[idx_n].qgate().is_H_gate())) {
	match = true;
      }
    }

    if (!match) continue;

    nodes_[idx_p].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
    nodes_[idx_c].qgate(Sharqit::QGate(Sharqit::QGateKind::CZ, {con,tar}));
    nodes_[idx_n].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
  }

  id_removal();
}

void Sharqit::DAGCirc::cz_to_cx_gate()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    uint32_t con = 0;
    uint32_t tar = 0;
    uint32_t idx_c = 0;
    uint32_t idx_con_p = 0; 
    uint32_t idx_con_n = 0;
    uint32_t idx_tar_p = 0; 
    uint32_t idx_tar_n = 0;
    if (nodes_[i].qgate().is_CZ_gate()) {
      con = nodes_[i].qgate().qid()[0];
      tar = nodes_[i].qgate().qid()[1];
      idx_c = i;

      idx_con_p = prev_node(i, con);
      idx_con_n = next_node(i, con);
      idx_tar_p = prev_node(i, tar);
      idx_tar_n = next_node(i, tar);
      if ((nodes_[idx_con_p].qgate().is_H_gate() && nodes_[idx_con_n].qgate().is_H_gate())) {
	nodes_[idx_con_p].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {con}));
	nodes_[idx_c].qgate(Sharqit::QGate(Sharqit::QGateKind::CX, {tar, con}));
	nodes_[idx_con_n].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {con}));
      }
      else if ((nodes_[idx_tar_p].qgate().is_H_gate() && nodes_[idx_tar_n].qgate().is_H_gate())) {
	nodes_[idx_tar_p].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
	nodes_[idx_c].qgate(Sharqit::QGate(Sharqit::QGateKind::CX, {con, tar}));
	nodes_[idx_tar_n].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
      }
    }
  }

  id_removal();
}

void Sharqit::DAGCirc::hadamard_gate_reduction_1()
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
      b = next_node(i, q);
      if (nodes_[b].is_outnode()) continue;
      if (nodes_[b].qgate().is_S_gate()) {
	c = next_node(b, q);
	if (nodes_[c].is_outnode()) continue;
	if (nodes_[c].qgate().is_H_gate()) {
	  match = true;
	}
      }
    }

    if (!match) continue;

    Sharqit::QGate s_gate = nodes_[b].qgate();
    if (s_gate.kind() == Sharqit::QGateKind::S ||
	(s_gate.kind() == Sharqit::QGateKind::RZ && s_gate.phase() == Sharqit::Phase(1,2))) {
      nodes_[a].qgate(Sharqit::QGate(Sharqit::QGateKind::Sdg, {q}));
      nodes_[b].qgate(Sharqit::QGate(Sharqit::QGateKind::H, {q}));
      nodes_[c].qgate(Sharqit::QGate(Sharqit::QGateKind::Sdg, {q}));
    }
    else {
      nodes_[a].qgate(Sharqit::QGate(Sharqit::QGateKind::S, {q}));
      nodes_[b].qgate(Sharqit::QGate(Sharqit::QGateKind::H, {q}));
      nodes_[c].qgate(Sharqit::QGate(Sharqit::QGateKind::S, {q}));
    }
  }
}

void Sharqit::DAGCirc::hadamard_gate_reduction_2()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    uint32_t con = 0;
    uint32_t tar = 0;
    uint32_t idx = 0;
    uint32_t idx_con_prev = 0;
    uint32_t idx_tar_prev = 0;
    uint32_t idx_con_next = 0;
    uint32_t idx_tar_next = 0;

    bool match = false;
    if (nodes_[i].qgate().is_CX_gate()) {
      con = nodes_[i].qgate().qid()[0];
      tar = nodes_[i].qgate().qid()[1];
      idx = i;
      idx_con_prev = prev_node(i, con);
      idx_tar_prev = prev_node(i, tar);
      idx_con_next = next_node(i, con);
      idx_tar_next = next_node(i, tar);

      if (nodes_[idx_con_prev].qgate().is_H_gate() &&
	  nodes_[idx_tar_prev].qgate().is_H_gate() &&
	  nodes_[idx_con_next].qgate().is_H_gate() &&
	  nodes_[idx_tar_next].qgate().is_H_gate()) {
	match = true;
      }
    }

    if (!match) continue;

    nodes_[idx_con_prev].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {con}));
    nodes_[idx_tar_prev].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
    nodes_[idx].qgate(Sharqit::QGate(Sharqit::QGateKind::CX, {tar,con}));
    nodes_[idx_con_next].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {con}));
    nodes_[idx_tar_next].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
  }

  id_removal();
}

void Sharqit::DAGCirc::hadamard_gate_reduction_3()
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
      idx_p = prev_node(i, tar);
      idx_n = next_node(i, tar);
      if ((nodes_[idx_p].qgate().is_S1_gate() && nodes_[idx_n].qgate().is_S3_gate())) {
	idx_pp = prev_node(idx_p, tar);
	idx_nn = next_node(idx_n, tar);
	if (nodes_[idx_pp].qgate().is_H_gate() && nodes_[idx_nn].qgate().is_H_gate()) {
	  match1 = true;
	  match2 = false;
	}
      }
      else if ((nodes_[idx_p].qgate().is_S3_gate() && nodes_[idx_n].qgate().is_S1_gate())) {
	idx_pp = prev_node(idx_p, tar);
	idx_nn = next_node(idx_n, tar);
	if (nodes_[idx_pp].qgate().is_H_gate() && nodes_[idx_nn].qgate().is_H_gate()) {
	  match1 = false;
	  match2 = true;
	}
      }
    }

    if (!match1 && !match2) continue;

    if (match1) {
      nodes_[idx_pp].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
      nodes_[idx_p].qgate(Sharqit::QGate(Sharqit::QGateKind::Sdg, {tar}));
      nodes_[idx_c].qgate(Sharqit::QGate(Sharqit::QGateKind::CX, {con,tar}));
      nodes_[idx_n].qgate(Sharqit::QGate(Sharqit::QGateKind::S, {tar}));
      nodes_[idx_nn].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
    }
    else if (match2) {
      nodes_[idx_pp].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
      nodes_[idx_p].qgate(Sharqit::QGate(Sharqit::QGateKind::S, {tar}));
      nodes_[idx_c].qgate(Sharqit::QGate(Sharqit::QGateKind::CX, {con,tar}));
      nodes_[idx_n].qgate(Sharqit::QGate(Sharqit::QGateKind::Sdg, {tar}));
      nodes_[idx_nn].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
    }
  }

  id_removal();
}

uint32_t Sharqit::DAGCirc::hadamard_gate_reduction()
{
  cx_to_cz_gate();
  cz_to_cx_gate();
  hadamard_gate_reduction_1();
  hadamard_gate_reduction_2();
  hadamard_gate_reduction_3();

  return gate_count();
}

uint32_t Sharqit::DAGCirc::single_qubit_gate_cancellation()
{
  for (uint32_t q = 0; q < qubit_num_; ++q) {

    for (uint32_t ori = next_node(inputs_[q], q); !nodes_[ori].is_outnode(); ori = next_node(ori, q)) {

      if (nodes_[ori].is_outnode()) break;
      if (nodes_[ori].is_identity()) continue;

      if (nodes_[ori].qubit_num() == 1) {
	  
	uint32_t pre = ori;
	uint32_t now = ori;

	do {
	  now = next_node(now, q);
	  if (!nodes_[now].is_outnode() && nodes_[ori].commutable(nodes_[now])) {
	    pre = now;
	    continue;
	  }
	  else if (ori == pre) {
	    break;
	  }
	  else if (nodes_[ori].mergeable(nodes_[pre])) {
	    nodes_[pre].merge(nodes_[ori]);
	    nodes_[ori].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {q}));
	    break;
	  }
	  else if (nodes_[ori].is_rotation()) {
	    DAGNode ins_node = nodes_[ori];
	    uint32_t ins = append_node(ins_node);
	    remove_edge(pre, now, q);
	    connect_nodes(pre, ins, q);
	    connect_nodes(ins, now, q);
	    nodes_[ori].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {q}));
	    break;
	  }
	  else {
	    break;
	  }
	} while (!nodes_[now].is_outnode());

      }
    }
  }
  id_removal();

  /* RZ and phase polynomial commute
     ex)
     q[0] ---------*---------*--
     q[1] --RZ(1)--X--RZ(2)--X--
     is equal to
     q[0] --*---------*---------
     q[1] --X--RZ(2)--X--RZ(1)--
   */
  for (uint32_t q = 0; q < qubit_num_; ++q) {

    for (uint32_t ori = next_node(inputs_[q], q); !nodes_[ori].is_outnode(); ori = next_node(ori, q)) {

      if (nodes_[ori].is_outnode()) break;
      if (nodes_[ori].is_identity()) continue;

      if (nodes_[ori].is_rotation()) {

	uint32_t tar = q;

	for (uint32_t now_tar = next_node(ori, tar); !nodes_[now_tar].is_outnode(); now_tar = next_node(now_tar, tar)) {
	  if (nodes_[ori].commutable(nodes_[now_tar])) {
	    continue;
	  }
	  else if (nodes_[now_tar].is_cnot() && nodes_[now_tar].qgate().qid()[1] == tar) {
	    uint32_t con = nodes_[now_tar].qgate().qid()[0];
	    uint32_t now_con = now_tar;
	    now_tar = next_node(now_tar, tar);
	    if (!nodes_[now_tar].is_outnode() && nodes_[now_tar].is_rotation()) {
	      now_tar = next_node(now_tar, tar);
	      now_con = next_node(now_con, con);
	      if (now_tar == now_con && nodes_[now_tar].is_cnot() && nodes_[now_tar].qgate().qid()[1] == tar) {
		uint32_t next_tar = next_node(now_tar, tar);
		DAGNode ins_node = nodes_[ori];
		uint32_t ins = append_node(ins_node);
		remove_edge(now_tar, next_tar, tar);
		connect_nodes(now_tar, ins, tar);
		connect_nodes(ins, next_tar, tar);
		nodes_[ori].qgate(Sharqit::QGate(Sharqit::QGateKind::Id, {tar}));
	      }
	    }
	  }
	  break;
	}
      }
    }
  }
  id_removal();

  return gate_count();
}

uint32_t Sharqit::DAGCirc::two_qubit_gate_cancellation()
{
  for (uint32_t q = 0; q < qubit_num_; ++q) {

    for (uint32_t ori = next_node(inputs_[q], q); !nodes_[ori].is_outnode(); ori = next_node(ori, q)) {

      if (nodes_[ori].is_identity()) continue;

      if (nodes_[ori].qubit_num() > 1) {
	  
	uint32_t now_con = ori;
	uint32_t now_tar = ori;
	uint32_t con = nodes_[ori].qid()[0];
	uint32_t tar = nodes_[ori].qid()[1];

	now_con = next_node(now_con, con);
	now_tar = next_node(now_tar, tar);
	if (nodes_[now_con].is_outnode() || nodes_[now_tar].is_outnode()) continue;

	do {
	  if (nodes_[ori].mergeable(nodes_[now_con]) && nodes_[ori].mergeable(nodes_[now_tar])) {
	    nodes_[now_con].merge(nodes_[ori]);
	    nodes_[ori].qgate(Sharqit::QGate(Sharqit::QGateKind::Id2, {con, tar}));
	    break;
	  }
	  else if (nodes_[ori].commutable(nodes_[now_con]) && nodes_[ori].mergeable(nodes_[now_tar])) {
	    now_con = next_node(now_con, con);
	    continue;
	  }
	  else if (nodes_[ori].mergeable(nodes_[now_con]) && nodes_[ori].commutable(nodes_[now_tar])) {
	    now_tar = next_node(now_tar, tar);
	    continue;
	  }
	  else if (nodes_[ori].commutable(nodes_[now_con]) && nodes_[ori].commutable(nodes_[now_tar])) {
	    now_con = next_node(now_con, con);
	    now_tar = next_node(now_tar, tar);
	    continue;
	  }
	  else {
	    break;
	  }
	} while (!nodes_[now_con].is_outnode() && !nodes_[now_tar].is_outnode());
      }
    }
  }
  id_removal();

  /*
      q[0] --*-----------
      q[1] --X--H--*--H--
      q[2] --------X-----
      is equal to
      q[0] -----------*--
      q[1] --H--*--H--X--
      q[2] -----X--------    
   */
  for (uint32_t q = 0; q < qubit_num_; ++q) {

    for (uint32_t ori = next_node(inputs_[q], q); !nodes_[ori].is_outnode(); ori = next_node(ori, q)) {

      if (nodes_[ori].is_outnode()) break;
      if (nodes_[ori].is_identity()) continue;

      if (nodes_[ori].is_cnot()) {
	  
	uint32_t con = nodes_[ori].qid()[0];
	uint32_t tar = nodes_[ori].qid()[1];
	uint32_t now_tar = next_node(ori, tar);

	if (!nodes_[now_tar].is_outnode() && nodes_[now_tar].is_hadamard()) {
	  now_tar = next_node(now_tar, tar);
	  if (!nodes_[now_tar].is_outnode() && nodes_[now_tar].is_cnot() && nodes_[now_tar].qid()[0] == tar) {
	    now_tar = next_node(now_tar, tar);
	    if (!nodes_[now_tar].is_outnode() && nodes_[now_tar].is_hadamard()) {
	      uint32_t now_con = next_node(ori, con);
	      if (!nodes_[now_con].is_outnode() && now_con == now_tar && nodes_[now_con].mergeable(nodes_[ori])) {
		nodes_[now_con].merge(nodes_[ori]);
		nodes_[ori].qgate(Sharqit::QGate(Sharqit::QGateKind::Id2, {con, tar}));
	      }
	    }
	  }
	}
      }
    }
  }
  id_removal();

  return gate_count();
}

void Sharqit::DAGCirc::rule_based_gate_reduction()
{
  hadamard_gate_reduction();
  two_qubit_gate_cancellation();
  single_qubit_gate_cancellation();
  two_qubit_gate_cancellation();
  hadamard_gate_reduction();
  single_qubit_gate_cancellation();
}
