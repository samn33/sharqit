/**
 * @file extract_qcirc.cpp
 * @brief member functions of ZXDiagram class related to the algorithm to extract quantum circuit
 */

#include "zx.h"

void Sharqit::ZXDiagram::permutation_as_swap(Sharqit::QCirc& qc)
{
  std::vector<uint32_t> qid_list(qubit_num_);

  for (auto& in:inputs_) {
    qid_list[nodes_[in].q()] = nodes_[adj_mat_[in][0].to()].q();
  }

  for (uint32_t i = 0; i < qubit_num_; ++i) {
    if (qid_list[i] == i) continue;
    for (uint32_t j = i + 1; j < qubit_num_; ++j) {
      if (qid_list[j] == i) {
	std::swap(qid_list[i], qid_list[j]);
	qc.sw(qid_list[i], qid_list[j]);
	break;
      }
    }
  }
}

std::vector<std::pair<uint32_t, uint32_t>> Sharqit::ZXDiagram::extract_2q_connects(std::vector<uint32_t>& frontier) const
{
  /* first element is related to CX gate, others are relate to CZ gate */
  std::vector<std::pair<uint32_t, uint32_t>> connects;

  /* adjacent matrix for frontier nodes connection */
  uint32_t size = frontier.size();
  std::vector<std::vector<uint32_t>> adj_matrix(size, std::vector<uint32_t>(size, 0));

  for (uint32_t i = 0; i < frontier.size(); ++i) {
    for (uint32_t j = i + 1; j < frontier.size(); ++j) {
      if (check_connect(frontier[i], frontier[j])) {
	adj_matrix[i][j] = 1;
	adj_matrix[j][i] = 1;
      }
    }
  }

  uint32_t max_inp = 0; // max of inner product of two rows of the adj_matrix
  std::pair<uint32_t, uint32_t> max_qid;
  std::pair<uint32_t, uint32_t> max_id;
  for (uint32_t i = 0; i < size; ++i) {
    for (uint32_t j = i + 1; j < size; ++j) {
      uint32_t inp = 0;
      for (uint32_t k = 0; k < size; ++k) {
	if (adj_matrix[i][k] != 0 && adj_matrix[j][k] != 0) ++inp;
      }
      if (inp > max_inp) {
	max_inp = inp;
	if (frontier[i] < frontier[j]) {
	  max_qid = {frontier[i], frontier[j]};
	  max_id = {i, j};
	}
	else {
	  max_qid = {frontier[j], frontier[i]};
	  max_id = {j, i};
	}
      }
    }
  }

  if (max_inp == 0) return connects;

  std::vector<uint32_t> overlap_qid;
  for (uint32_t k = 0; k < size; ++k) {
    if (adj_matrix[max_id.first][k] != 0 && adj_matrix[max_id.second][k] != 0) {
      overlap_qid.push_back(frontier[k]);
    }
  }
  
  connects.push_back(max_qid);
  for (auto& q:overlap_qid) {
    connects.push_back({max_qid.second, q});
  }

  return connects;
}

bool Sharqit::ZXDiagram::update_frontier(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc, const bool opt_cz)
{
  /* frontier adjacent to input node or root of phase gadget */
  std::vector<uint8_t> frontier_prohibit;
  for (auto& idx:frontier) {
    uint8_t flg = 0;
    for (auto& e:adj_mat_[idx]) {
      if (e.kind() == Sharqit::ZXEdgeKind::Plain &&
	  kind_of_node(e.to()) == Sharqit::ZXNodeKind::Input) {
	flg = 1;
	break;
      }
      if (check_pg_root_node(e.to())) {
	flg = 1;
	break;
      }
    }
    frontier_prohibit.push_back(flg);
  }

  /* neighbours of frontier */
  std::vector<uint32_t> neighbours;
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    if (frontier_prohibit[i] == 1) continue;
    for (auto& e:adj_mat_[frontier[i]]) {
      if (check_input_node(e.to())) continue;
      neighbours.push_back(e.to());
    }
  }
  std::sort(neighbours.begin(), neighbours.end());
  neighbours.erase(std::unique(neighbours.begin(), neighbours.end()), neighbours.end());

  if (neighbours.size() == 0) {
    if (update_frontier_pg(frontier, qc)) return true;
    else return false;
  }
  
  /* biadjacency matrix */
  std::vector<std::vector<uint8_t>> elements(frontier.size(), std::vector<uint8_t>(neighbours.size(), 0));
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    for (uint32_t j = 0; j < neighbours.size(); ++j) {
      if (frontier_prohibit[i] == 1) elements[i][j] = 0;
      else if (check_connect(frontier[i], neighbours[j])) elements[i][j] = 1;
    }
  }
  Sharqit::BinaryMatrix bmat(elements);

  /* gauss reduce */
  std::vector<std::pair<uint32_t, uint32_t>> row_ops = bmat.gauss_reduce();

  /* if bmat is not feasible */
  if (!bmat.feasible()) {
    update_frontier_pg(frontier, qc);
    return false;
  }

  /* extract CNOT gates */
  for (auto& op:row_ops) {
    qc.cx(op.second, op.first);
    row_operation(frontier[op.first], frontier[op.second]);
  }

  /* extract Hadamard, Phase gates, update frontier */
  std::vector<uint32_t> frontier_pre = frontier;
  std::vector<uint8_t> neighbours_flg(neighbours.size(), 1);
  for (uint32_t r = 0; r < frontier_pre.size(); ++r) {
    if (frontier_prohibit[r] == 1) continue;
    uint32_t f = frontier_pre[r];
    for (uint32_t s = 0; s < neighbours.size(); ++s) {
      if (neighbours_flg[s] == 0) continue;
      uint32_t n = neighbours[s];
      if (check_connect(f, n) && degree_of_node(f) == 1 &&
	  kind_of_node(n) != Sharqit::ZXNodeKind::Input && !check_pg_root_node(n)) {
	uint32_t q = nodes_[f].q();
	qc.h(q);
	if (nodes_[n].phase().is_zero() == 0) {
	  qc.p(q, nodes_[n].phase());
	  nodes_[n].phase(0); // remove phase
	}

	/* update frontier */
	remove_edge(frontier_pre[r], n);
	frontier[r] = n;
	neighbours_flg[s] = 0;
	nodes_[frontier[r]].q(q);
	break;
      }
    }
  }

  /* extract CZ gates */
  if (opt_cz) {
    std::vector<std::pair<uint32_t, uint32_t>> connects;
    while (true) {
      connects = extract_2q_connects(frontier);
      if (connects.size() <= 3) break;
      auto top = connects.begin();
      qc.cx(nodes_[top->first].q(), nodes_[top->second].q());
      for (auto it = connects.begin() + 1; it != connects.end(); ++it) {
  	qc.cz(nodes_[it->first].q(), nodes_[it->second].q());
  	remove_edge(it->first, it->second);
  	remove_edge(top->first, it->second);
      }
      qc.cx(nodes_[top->first].q(), nodes_[top->second].q());
    }
  }

  for (uint32_t i = 0; i < frontier.size(); ++i) {
    for (uint32_t j = i + 1; j < frontier.size(); ++j) {
      if (check_connect(frontier[i], frontier[j])) {
	qc.cz(nodes_[frontier[i]].q(), nodes_[frontier[j]].q());
	remove_edge(frontier[i], frontier[j]);
      }
    }
  }

  /* if frontier is not changed */
  bool changed = false;
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    if (frontier_pre[i] != frontier[i]) {
      changed = true;
      break;
    }
  }
  if (!changed) {
    update_frontier_pg(frontier, qc);
    return false;
  }

  /* return true if update is done */
  for (auto& f:frontier) {
    for (auto& e:adj_mat_[f]) {
      if (kind_of_node(e.to()) != Sharqit::ZXNodeKind::Input) return false;
    }
  }

  return true;
}

bool Sharqit::ZXDiagram::update_frontier_pg(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc)
{
  uint32_t idx_A = 0;
  uint32_t idx_B = 0;

  /* find a frontier node (idx_A) adjacent to a root of phase gadget (idx_B) */
  bool find = false;
  for (auto& f:frontier) {
    for (auto& e:adj_mat_[f]) {
      if (check_pg_root_node(e.to())) {
	idx_A = f; // frontier node
	idx_B = e.to(); // root node of phase gadget
	find = true;
      }
    }
  }
  if (!find) return true;

  /* insert nodes between the frontier node (idx_A) and output node */
  uint32_t pos = 0;
  uint32_t idx_C = 0;
  uint32_t idx_D = 0;
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    if (frontier[i] == idx_A) {
      pos = i;
      idx_C = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0), pos), ZXEdge(ZXEdgeKind::Hadamard, idx_A));
      idx_D = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0), pos), ZXEdge(ZXEdgeKind::Hadamard, idx_C));
      connect_nodes(idx_D, outputs_[i], Sharqit::ZXEdgeKind::Plain);
    }
    else {
      connect_nodes(frontier[i], outputs_[i], Sharqit::ZXEdgeKind::Plain);
    }
  }

  /* is node idx_A adjacent to input node? */
  bool adj_input = false;
  uint32_t pos_input = 0;
  for (auto& e:adj_mat_[idx_A]) {
    if (nodes_[e.to()].kind() == Sharqit::ZXNodeKind::Input) {
      pos_input = nodes_[e.to()].q();
      adj_input = true;
    }
  }

  /* in the case that node idx_A is adjacent to input node */
  if (adj_input) {
    uint32_t idx_E = 0;
    uint32_t idx_F = 0;
    idx_E = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0), pos_input), ZXEdge(ZXEdgeKind::Hadamard, idx_A));
    idx_F = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0), pos_input), ZXEdge(ZXEdgeKind::Hadamard, idx_E));
    connect_nodes(idx_F, inputs_[pos_input], Sharqit::ZXEdgeKind::Plain);
    remove_edge(pos_input, idx_A);
  }

  /* pivot1 */
  pivot1_one_time(idx_A, idx_B);

  frontier[pos] = idx_D; // new frontier
    
  /* remove edges between frontier and output nodes */
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    remove_edge(frontier[i], outputs_[i]);
  }

  /* update phase gadget info */
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (degree_of_node(i) == 1) nodes_[i].pg_phase(true);
    else nodes_[i].pg_phase(false);
    nodes_[i].pg_root(false);
  }
  for (auto& f:frontier) nodes_[f].pg_phase(false);
  for (auto& in:inputs_) nodes_[in].pg_phase(false);
  for (auto& out:outputs_) nodes_[out].pg_phase(false);
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (nodes_[i].pg_phase()) nodes_[adj_mat_[i][0].to()].pg_root(true);
  }

  return false;
}

void Sharqit::ZXDiagram::process_frontier(std::vector<uint32_t>& frontier, Sharqit::QCirc& qc, const bool opt_cz)
{
  std::vector<uint32_t> frontier_pre = frontier;
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    uint32_t q = nodes_[frontier[i]].q();
    uint32_t to = frontier[i];
    uint32_t from = adj_mat_[frontier[i]][0].to();
    if (adj_mat_[to][0].kind() == Sharqit::ZXEdgeKind::Hadamard) {
      qc.h(q);
    }
    if (nodes_[from].phase().is_zero() == false) {
      qc.p(q, nodes_[from].phase());
      nodes_[from].phase(0);
    }
    frontier[i] = from;
    nodes_[frontier[i]].q(q);
  }

  for (uint32_t i = 0; i < frontier_pre.size(); ++i) {
    remove_edge(frontier_pre[i], frontier[i]);
  }

  /* extract CZ gates */
  if (opt_cz) {
    std::vector<std::pair<uint32_t, uint32_t>> connects;
    while (true) {
      connects = extract_2q_connects(frontier);
      if (connects.size() <= 3) break;
      auto top = connects.begin();
      qc.cx(nodes_[top->first].q(), nodes_[top->second].q());
      for (auto it = connects.begin() + 1; it != connects.end(); ++it) {
  	qc.cz(nodes_[it->first].q(), nodes_[it->second].q());
  	remove_edge(it->first, it->second);
  	remove_edge(top->first, it->second);
      }
      qc.cx(nodes_[top->first].q(), nodes_[top->second].q());
    }
  }
  for (uint32_t i = 0; i < frontier.size(); ++i) {
    for (uint32_t j = i + 1; j < frontier.size(); ++j) {
      if (check_connect(frontier[i], frontier[j])) {
	qc.cz(nodes_[frontier[i]].q(), nodes_[frontier[j]].q());
	remove_edge(frontier[i], frontier[j]);
      }
    }
  }
}

Sharqit::QCirc Sharqit::ZXDiagram::extract_qcirc()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't extract circuit not for simplified ZX diagram.");
  }

  /* update node_places */
  update_node_places();
  update_phase_gadget();

  /* if input connected to Hadamard edge, convert Plain edge */
  for (auto& in:inputs_) {
    for (auto& e:adj_mat_[in]) {
      if (e.kind() == Sharqit::ZXEdgeKind::Hadamard) {
	uint32_t to = e.to();
	uint32_t mid = append_node(Sharqit::ZXNode(Sharqit::ZXNodeKind::ZSpider,
						 Sharqit::Phase(0), nodes_[in].q()),
				   Sharqit::ZXEdge(Sharqit::ZXEdgeKind::Plain, in));
	connect_nodes(mid, to, Sharqit::ZXEdgeKind::Hadamard);
      }
    }
  }
  for (auto& in:inputs_) {
    for (auto& e:adj_mat_[in]) {
      if (e.kind() == Sharqit::ZXEdgeKind::Hadamard) {
	remove_edge(in, e.to());
      }
    }
  }

  /* first step */
  Sharqit::QCirc qc;
  std::vector<uint32_t> frontier = outputs_;
  process_frontier(frontier, qc, true);

  /* update frontier */
  while (true) {

    std::vector<uint32_t> frontier_ori = frontier;

    if (update_frontier(frontier, qc, false)) {
      break;
    }
  }
  
  permutation_as_swap(qc);

  for (int32_t q = qubit_num_ - 1; q >= (int32_t)qc.qubit_num(); --q) {
    qc.id(q);
  }

  remove_isolated_spiders();
  
  return qc.reverse();
}
