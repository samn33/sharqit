/**
 * @file simplify.cpp
 * @brief member functions of ZXDiagram class related to the algorithm to simplify ZX-diagram
 */

#include "zx.h"

void Sharqit::ZXDiagram::fuse_spiders()
{
  while (true) {
    update_node_places();
    update_phase_gadget();

    bool done = true;
    uint32_t idx_target;
    uint32_t idx_remove;
  
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (kind_of_node(i) == Sharqit::ZXNodeKind::Input ||
  	  kind_of_node(i) == Sharqit::ZXNodeKind::Output) continue;
  
      bool find_normal = false;
      for (auto& edge:adj_mat_[i]) {
  	if ((edge.kind() == Sharqit::ZXEdgeKind::Plain) &&
	    (kind_of_node(i) == kind_of_node(edge.to())) &&
	    (kind_of_node(edge.to()) != Sharqit::ZXNodeKind::Input &&
	     kind_of_node(edge.to()) != Sharqit::ZXNodeKind::Output)) {
  	  idx_target = std::min(i, edge.to());
  	  idx_remove = std::max(i, edge.to());

	  remove_edge(idx_target, idx_remove);

	  std::vector<uint32_t> removes;
	  for (auto& edge:adj_mat_[idx_remove]) {
	    connect_nodes(idx_target, edge.to(), edge.kind());
	    removes.push_back(edge.to());
	  }
	  for (auto& rm:removes) {
	    remove_edge(idx_remove, rm);
	  }

	  nodes_[idx_target].phase(nodes_[idx_target].phase() + nodes_[idx_remove].phase());
  	  find_normal = true;
  	  break;
  	}
      }
      if (find_normal == true) {
  	done = false;
  	break;
      }
      else {
  	done = true;
  	continue;
      }
    }
  
    /* done if all internal edes are Hadamard */
    if (done == true) break;

    /* removes edges (new) */ // 意味ない？
    for (auto& edge:adj_mat_[idx_remove]) {
      remove_edge(idx_remove, edge.to());
    }
  }
  remove_isolated_spiders();
}

void Sharqit::ZXDiagram::conv_x_to_z()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      if (kind_of_node(i) == Sharqit::ZXNodeKind::XSpider) {
	adj_mat_[i][j].reverse_kind();
      }
      if (kind_of_node(adj_mat_[i][j].to()) == Sharqit::ZXNodeKind::XSpider) {
	adj_mat_[i][j].reverse_kind();
      }
    }
  }
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
     if (kind_of_node(i) == Sharqit::ZXNodeKind::XSpider) {
      nodes_[i].kind(Sharqit::ZXNodeKind::ZSpider);
    }
  }
}

void Sharqit::ZXDiagram::remove_parallel_selfloops_hadamard_edges()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    /* initialize hadamard counts */
    std::map<uint32_t, uint32_t> hadamard_counts;
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      hadamard_counts.insert_or_assign(adj_mat_[i][j].to(), 0);
    }
    /* count hadamard edge */
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      if (adj_mat_[i][j].kind() == Sharqit::ZXEdgeKind::Hadamard) {
	uint32_t cnt = hadamard_counts.at(adj_mat_[i][j].to()) + 1;
	hadamard_counts.insert_or_assign(adj_mat_[i][j].to(), cnt);
      }
    }
    /* remove parallel Hadamard edges */
    for (auto it = adj_mat_[i].begin(); it != adj_mat_[i].end(); ++it) {
      uint32_t hcnt = hadamard_counts.at(it->to());
      if (hcnt != 0) {
	it = adj_mat_[i].erase(it);
	--it;
      }
    }
    for (auto it = hadamard_counts.begin(); it != hadamard_counts.end(); ++it) {
      if (it->second != 0 && it->second % 2 == 1) {
	adj_mat_[i].push_back(Sharqit::ZXEdge(Sharqit::ZXEdgeKind::Hadamard, it->first));
      }
    }
  }
}

void Sharqit::ZXDiagram::update_node_places()
{
  if (nodes_.size() > node_places_.size()) {
    node_places_.assign(nodes_.size(), Sharqit::ZXNodePlace::Internal);
  }
  else {
    node_places_.assign(node_places_.size(), Sharqit::ZXNodePlace::Internal);
  }

  for (auto& in:inputs_) {
    node_places_[in] = Sharqit::ZXNodePlace::Terminal;
    uint32_t from = in;
    uint32_t to = adj_mat_[from][0].to(); // index of spider adjacent input node
    node_places_[to] = Sharqit::ZXNodePlace::Boundary;
  }

  for (auto& out:outputs_) {
    node_places_[out] = Sharqit::ZXNodePlace::Terminal;
    uint32_t from = out;
    uint32_t to = adj_mat_[from][0].to(); // index of spider adjacent output node
    node_places_[to] = Sharqit::ZXNodePlace::Boundary;
  }
}

void Sharqit::ZXDiagram::update_phase_gadget()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    nodes_[i].pg_phase(false);
    nodes_[i].pg_root(false);
  }
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_z_spider(i) && degree_of_node(i) == 1 &&
	adj_mat_[i][0].kind() == Sharqit::ZXEdgeKind::Hadamard &&
	check_internal_node(adj_mat_[i][0].to()) &&
	check_z_spider(adj_mat_[i][0].to()) &&
	check_zero_phase_spider(adj_mat_[i][0].to())) {
      nodes_[i].pg_phase(true);
      nodes_[adj_mat_[i][0].to()].pg_root(true);

      for (auto& e:adj_mat_[adj_mat_[i][0].to()]) {
	if (e.to() == i) continue;
	nodes_[e.to()].pg_leaf(true);
      }
    }
  }
}

void Sharqit::ZXDiagram::graph_like()

{
  if (kind() == Sharqit::ZXDiagramKind::GraphLike) return;
  
  /* fuse Spiders */
  fuse_spiders();

  /* convert X spiders to Z spiders */
  conv_x_to_z();

  /* remove parallel and sel-loops Hadamard edges */
  remove_parallel_selfloops_hadamard_edges();

  /* fuse Spiders */
  fuse_spiders();

  /* remove parallel and sel-loops Hadamard edges */
  remove_parallel_selfloops_hadamard_edges();

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
  
  /* if output connected to Hadamard edge, convert Plain edge */
  for (auto& out:outputs_) {
    for (auto& e:adj_mat_[out]) {
      if (e.kind() == Sharqit::ZXEdgeKind::Hadamard) {
	uint32_t to = e.to();
	uint32_t mid = append_node(Sharqit::ZXNode(Sharqit::ZXNodeKind::ZSpider,
						 Sharqit::Phase(0), nodes_[out].q()),
				   Sharqit::ZXEdge(Sharqit::ZXEdgeKind::Plain, out));
	connect_nodes(mid, to, Sharqit::ZXEdgeKind::Hadamard);
      }
    }
  }
  for (auto& out:outputs_) {
    for (auto& e:adj_mat_[out]) {
      if (e.kind() == Sharqit::ZXEdgeKind::Hadamard) {
	remove_edge(out, e.to());
      }
    }
  }
  
  kind(Sharqit::ZXDiagramKind::GraphLike);
}

void Sharqit::ZXDiagram::lcomp_one_time(const uint32_t idx_A)
{
  /* node indexes of neighbours */
  std::vector<uint32_t> idx_neighbours = adjacent_node_indexes(idx_A);
  xor_hadamard_edges(idx_neighbours);

  /* change phases of the edges */
  for (uint32_t i = 0; i < idx_neighbours.size(); ++i) {
    nodes_[idx_neighbours[i]].phase(nodes_[idx_neighbours[i]].phase() - nodes_[idx_A].phase());
  }

  /* remove edges around the target node */
  remove_edges_of_node(idx_A);
}

uint32_t Sharqit::ZXDiagram::lcomp()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. must be graph-like ZX diagram.");
  }

  std::vector<uint32_t> lc_cand_spiders; // proper clifford spiders
  std::vector<uint32_t> lc_used_spiders; // flags of used spiders or not

  update_node_places();
  update_phase_gadget();

  lc_cand_spiders.clear();
  lc_used_spiders.assign(nodes_.size(), 0);

  /* spiders of lcomp candidate */
  bool find = false;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (lc_used_spiders[i] == 1) continue;
    if ((check_internal_node(i) && check_z_spider(i)) == false) continue;
    if (check_pg_leaf_node(i)) continue;
    if (nodes_[i].phase() == Sharqit::Phase(1,2) ||
	nodes_[i].phase() == Sharqit::Phase(3,2)) {
      bool break_flg = false;
      for (auto& e:adj_mat_[i]) {
	if (lc_used_spiders[e.to()] == 1) {
	  break_flg = true;
	  break;
	}
      }
      if (break_flg) continue;
	
      find = true;
      lc_used_spiders[i] = 1;
      lc_cand_spiders.push_back(i);
      for (auto& e:adj_mat_[i]) {
	lc_used_spiders[e.to()] = 1;
      }
    }
  }

  if (!find) return 0;

  /* execute lcomp */
  for (auto& idx:lc_cand_spiders) {
    lcomp_one_time(idx);
  }
  remove_isolated_spiders();

  return lc_cand_spiders.size();
}

void Sharqit::ZXDiagram::pivot1_one_time(const uint32_t idx_A, const uint32_t idx_B)
{
  std::vector<uint32_t> indexes_A = adjacent_node_indexes(idx_A);
  std::vector<uint32_t> indexes_B = adjacent_node_indexes(idx_B);
  for (auto it = indexes_A.begin(); it != indexes_A.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_A.erase(it);
      --it;
    }
  }
  for (auto it = indexes_B.begin(); it != indexes_B.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_B.erase(it);
      --it;
    }
  }
  
  std::vector<uint32_t> indexes_AandB;
  std::set_intersection(indexes_A.begin(), indexes_A.end(),
			indexes_B.begin(), indexes_B.end(),
			std::inserter(indexes_AandB, indexes_AandB.end()));
  std::vector<uint32_t> indexes_AorB;
  std::set_union(indexes_A.begin(), indexes_A.end(),
		 indexes_B.begin(), indexes_B.end(),
		 std::inserter(indexes_AorB, indexes_AorB.end()));
  std::vector<uint32_t> indexes_AB;
  std::set_difference(indexes_AorB.begin(), indexes_AorB.end(),
		      indexes_AandB.begin(), indexes_AandB.end(),
		      std::inserter(indexes_AB, indexes_AB.end()));

  xor_hadamard_edges(indexes_A);
  xor_hadamard_edges(indexes_B);
  xor_hadamard_edges(indexes_AB);

  /* add phase of idx_A to the phases of all indexes_B */
  Sharqit::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of idx_A to the phases of all indexes_B */
  Sharqit::Phase phase_B = nodes_[idx_B].phase();
  for (auto& idx:indexes_A) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_B);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharqit::Phase(1));
  }

  /* remove edges of idx_A, idx_B */
  remove_edges_of_node(idx_A);
  remove_edges_of_node(idx_B);
}

uint32_t Sharqit::ZXDiagram::pivot1()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes
  std::vector<std::pair<uint32_t, uint32_t>> p1_cand_spiders;
  std::vector<uint32_t> p1_used_spiders;

  update_node_places();
  update_phase_gadget();

  pauli_spiders.clear();
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_pauli_spider(i) && check_internal_node(i)) pauli_spiders.push_back(i);
  }

  p1_cand_spiders.clear();
  p1_used_spiders.assign(nodes_.size(), 0);
  uint32_t idx_A = 0;
  uint32_t idx_B = 0;
  bool find = false;

  for (auto& idx:pauli_spiders) {
    idx_A = idx;
    if (p1_used_spiders[idx_A] == 1) continue;

    bool break_flg = true; // true if idx_A,idx_B have found
    for (auto& e:adj_mat_[idx_A]) {
      if (check_pauli_spider(e.to()) && check_internal_node(e.to()) && p1_used_spiders[e.to()] == 0) {
	break_flg = true;
	idx_B = e.to();
	for (auto& e_A:adj_mat_[idx_A]) {
	  if (p1_used_spiders[e_A.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}
	if (!break_flg) continue;
	for (auto& e_B:adj_mat_[idx_B]) {
	  if (p1_used_spiders[e_B.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}
	if (!break_flg) continue;
	else {
	  p1_cand_spiders.push_back({idx_A, idx_B});
	  break;
	}
      }
      else {
	break_flg = false;
      }
    }

    if (!break_flg) continue;

    find = true;
    p1_used_spiders[idx_A] = 1;
    p1_used_spiders[idx_B] = 1;
    for (auto& e_A:adj_mat_[idx_A]) {
      p1_used_spiders[e_A.to()] = 1;
    }
    for (auto& e_B:adj_mat_[idx_B]) {
      p1_used_spiders[e_B.to()] = 1;
    }
  }

  if (!find) return 0;

  for (auto it = p1_cand_spiders.begin(); it != p1_cand_spiders.end(); ++it) {
    pivot1_one_time(it->first, it->second);
  }
  remove_isolated_spiders();

  return p1_cand_spiders.size();
}

void Sharqit::ZXDiagram::pivot2_one_time(const uint32_t idx_A, const uint32_t idx_B)
{
  uint32_t idx_C = 0;
  uint32_t idx_D = 0;

  /* pivot for pauli spiders pair */
  std::vector<uint32_t> indexes_A = adjacent_node_indexes(idx_A);
  std::vector<uint32_t> indexes_B = adjacent_node_indexes(idx_B);
  for (auto it = indexes_A.begin(); it != indexes_A.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_A.erase(it);
      --it;
    }
  }
  for (auto it = indexes_B.begin(); it != indexes_B.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_B.erase(it);
      --it;
    }
  }
  
  std::vector<uint32_t> indexes_AandB;
  std::set_intersection(indexes_A.begin(), indexes_A.end(),
			indexes_B.begin(), indexes_B.end(),
			std::inserter(indexes_AandB, indexes_AandB.end()));
  std::vector<uint32_t> indexes_AorB;
  std::set_union(indexes_A.begin(), indexes_A.end(),
		 indexes_B.begin(), indexes_B.end(),
		 std::inserter(indexes_AorB, indexes_AorB.end()));
  std::vector<uint32_t> indexes_AB;
  std::set_difference(indexes_AorB.begin(), indexes_AorB.end(),
		      indexes_AandB.begin(), indexes_AandB.end(),
		      std::inserter(indexes_AB, indexes_AB.end()));

  xor_hadamard_edges(indexes_A);
  xor_hadamard_edges(indexes_B);
  xor_hadamard_edges(indexes_AB);

  /* add phase of idx_A to the phases of all indexes_B */
  Sharqit::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharqit::Phase(1));
  }

  /* phase gadget */
  uint32_t idx_tmp = 0;
  for (uint32_t i = 0; i < indexes_A.size(); ++i) {
    if (indexes_A[i] == idx_B) continue;
    else {
      idx_C = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0)),
			  ZXEdge(ZXEdgeKind::Hadamard, indexes_A[i]));
      idx_tmp = indexes_A[i];
      break;
    }
  }
  for (uint32_t i = 0; i < indexes_A.size(); ++i) {
    if (indexes_A[i] == idx_B || indexes_A[i] == idx_tmp) continue;
    else connect_nodes(idx_C, indexes_A[i], ZXEdgeKind::Hadamard);
  }
  Sharqit::Phase phase_B = nodes_[idx_B].phase();
  Sharqit::Fraction frac = phase_A.frac();
  frac.reduce();
  if (std::abs(frac.numerator()) % 2 == 1) {
    phase_B = -phase_B;
    nodes_[idx_B].phase(phase_B);
  }
  idx_D = append_node(ZXNode(ZXNodeKind::ZSpider, phase_B), ZXEdge(ZXEdgeKind::Hadamard, idx_C));
  nodes_[idx_C].pg_root(true);
  nodes_[idx_D].pg_phase(true);

  /* remove edges of idx_A, idx_B */
  remove_edges_of_node(idx_A);
  remove_edges_of_node(idx_B);
}

uint32_t Sharqit::ZXDiagram::pivot2()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes
  std::vector<std::pair<uint32_t, uint32_t>> p2_cand_spiders;
  std::vector<uint32_t> p2_used_spiders;

  update_node_places();
  update_phase_gadget();

  pauli_spiders.clear();
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_pauli_spider(i) && check_internal_node(i) && !check_phase_gadget_node(i))
      pauli_spiders.push_back(i);
  }
    
  p2_cand_spiders.clear();
  p2_used_spiders.assign(nodes_.size(), 0);
  uint32_t idx_A = 0;
  uint32_t idx_B = 0;
  bool find = false;

  for (auto& idx:pauli_spiders) {
    idx_A = idx;
    if (p2_used_spiders[idx_A] == 1) continue;

    bool break_flg = true; // true if idx_A,idx_B have found
    for (auto& e:adj_mat_[idx_A]) {
      if (check_internal_node(e.to()) && check_non_clifford_spider(e.to()) && !check_phase_gadget_node(e.to())) {
	break_flg = true;
	idx_B = e.to();
	for (auto& e_A:adj_mat_[idx_A]) {
	  if (p2_used_spiders[e_A.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}
	if (!break_flg) continue;
	for (auto& e_B:adj_mat_[idx_B]) {
	  if (p2_used_spiders[e_B.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}
	if (!break_flg) continue;
	else {
	  p2_cand_spiders.push_back({idx_A, idx_B});
	  break;
	}
      }
      else {
	break_flg = false;
      }
    }

    if (!break_flg) continue;

    find = true;
    p2_used_spiders[idx_A] = 1;
    p2_used_spiders[idx_B] = 1;
    for (auto& e_A:adj_mat_[idx_A]) {
      p2_used_spiders[e_A.to()] = 1;
    }
    for (auto& e_B:adj_mat_[idx_B]) {
      p2_used_spiders[e_B.to()] = 1;
    }
  }

  if (!find) return 0;

  for (auto it = p2_cand_spiders.begin(); it != p2_cand_spiders.end(); ++it) {
    pivot2_one_time(it->first, it->second);
  }
  remove_isolated_spiders();

  return p2_cand_spiders.size();
}

void Sharqit::ZXDiagram::pivot3_one_time(const uint32_t idx_A, const uint32_t idx_B)
{
  uint32_t idx_C = 0;
  uint32_t idx_CC = 0;
  uint32_t idx_D = 0;
  uint32_t idx_DD = 0;
  uint32_t idx_term = 0;

  /* terminal node index */
  for (auto& e:adj_mat_[idx_B]) {
    if (check_input_node(e.to()) || check_output_node(e.to())) {
      idx_term = e.to();
      break;
    }
  }
  remove_edge(idx_B, idx_term);

  /* pivot for pauli spiders pair */
  std::vector<uint32_t> indexes_A = adjacent_node_indexes(idx_A);
  std::vector<uint32_t> indexes_B = adjacent_node_indexes(idx_B);
  for (auto it = indexes_A.begin(); it != indexes_A.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_A.erase(it);
      --it;
    }
  }
  for (auto it = indexes_B.begin(); it != indexes_B.end(); ++it) {
    if (check_input_node(*it) || check_output_node(*it)) {
      it = indexes_B.erase(it);
      --it;
    }
  }
  
  std::vector<uint32_t> indexes_AandB;
  std::set_intersection(indexes_A.begin(), indexes_A.end(),
			indexes_B.begin(), indexes_B.end(),
			std::inserter(indexes_AandB, indexes_AandB.end()));
  std::vector<uint32_t> indexes_AorB;
  std::set_union(indexes_A.begin(), indexes_A.end(),
		 indexes_B.begin(), indexes_B.end(),
		 std::inserter(indexes_AorB, indexes_AorB.end()));
  std::vector<uint32_t> indexes_AB;
  std::set_difference(indexes_AorB.begin(), indexes_AorB.end(),
		      indexes_AandB.begin(), indexes_AandB.end(),
		      std::inserter(indexes_AB, indexes_AB.end()));

  xor_hadamard_edges(indexes_A);
  xor_hadamard_edges(indexes_B);
  xor_hadamard_edges(indexes_AB);

  /* add phase of idx_A to the phases of all indexes_B */
  Sharqit::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharqit::Phase(1));
  }

  /* phase gadget */
  uint32_t idx_tmp = 0;
  for (uint32_t i = 0; i < indexes_A.size(); ++i) {
    if (indexes_A[i] == idx_B) continue;
    else {
      idx_C = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0)),
			  ZXEdge(ZXEdgeKind::Hadamard, indexes_A[i]));
      idx_CC = append_node(ZXNode(ZXNodeKind::ZSpider, phase_A),
			  ZXEdge(ZXEdgeKind::Hadamard, indexes_A[i]));
      idx_tmp = indexes_A[i];
      break;
    }
  }
  for (uint32_t i = 0; i < indexes_A.size(); ++i) {
    if (indexes_A[i] == idx_B || indexes_A[i] == idx_tmp) continue;
    else {
      connect_nodes(idx_C, indexes_A[i], ZXEdgeKind::Hadamard);
      connect_nodes(idx_CC, indexes_A[i], ZXEdgeKind::Hadamard);
    }
  }
  Sharqit::Phase phase_B = nodes_[idx_B].phase();
  Sharqit::Fraction frac = phase_A.frac();
  frac.reduce();
  if (std::abs(frac.numerator()) % 2 == 1) {
    phase_B = -phase_B;
    nodes_[idx_B].phase(phase_B);
  }
  idx_D = append_node(ZXNode(ZXNodeKind::ZSpider, phase_B), ZXEdge(ZXEdgeKind::Hadamard, idx_C));
  nodes_[idx_C].pg_root(true);
  nodes_[idx_D].pg_phase(true);

  idx_DD = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0)), ZXEdge(ZXEdgeKind::Hadamard, idx_CC));
  connect_nodes(idx_DD, idx_term, ZXEdgeKind::Plain);

  /* remove edges of idx_A, idx_B */
  remove_edges_of_node(idx_A);
  remove_edges_of_node(idx_B);
}

uint32_t Sharqit::ZXDiagram::pivot3()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes
  std::vector<std::pair<uint32_t, uint32_t>> p3_cand_spiders;
  std::vector<uint32_t> p3_used_spiders;

  update_node_places();
  update_phase_gadget();

  pauli_spiders.clear();
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_pauli_spider(i) && check_internal_node(i) && !check_phase_gadget_node(i))
      pauli_spiders.push_back(i);
  }
    
  p3_cand_spiders.clear();
  p3_used_spiders.assign(nodes_.size(), 0);
  uint32_t idx_A = 0;
  uint32_t idx_B = 0;
  bool find = false;

  for (auto& idx:pauli_spiders) {
    idx_A = idx;
    if (p3_used_spiders[idx_A] == 1) continue;

    bool break_flg = true; // true if idx_A,idx_B have found
    for (auto& e:adj_mat_[idx_A]) {
      if (check_boundary_node(e.to()) && check_non_clifford_spider(e.to()) && !check_phase_gadget_node(e.to())) {
	break_flg = true;
	idx_B = e.to();
	for (auto& e_A:adj_mat_[idx_A]) {
	  if (p3_used_spiders[e_A.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}
	if (!break_flg) continue;
	for (auto& e_B:adj_mat_[idx_B]) {
	  if (p3_used_spiders[e_B.to()] == 1) {
	    break_flg = false;
	    break;
	  }
	}

	if (!break_flg) continue;
	else if (check_connect_input_node(idx_B) && check_connect_output_node(idx_B)) {
	  break_flg = false;
	  continue;
	}
	else if ((check_connect_phase_gadget(idx_A) || check_connect_phase_gadget(idx_B))) {
	  break_flg = false;
	  continue;
	}
	else {
	  break_flg = true;
	  p3_cand_spiders.push_back({idx_A, idx_B});
	  break;
	}
      }
      else {
	break_flg = false;
      }
    }

    if (!break_flg) continue;

    find = true;
    p3_used_spiders[idx_A] = 1;
    p3_used_spiders[idx_B] = 1;
    for (auto& e_A:adj_mat_[idx_A]) {
      p3_used_spiders[e_A.to()] = 1;
    }
    for (auto& e_B:adj_mat_[idx_B]) {
      p3_used_spiders[e_B.to()] = 1;
    }
  }

  if (!find) return 0;

  for (auto it = p3_cand_spiders.begin(); it != p3_cand_spiders.end(); ++it) {
    pivot3_one_time(it->first, it->second);
  }
  remove_isolated_spiders();

  return p3_cand_spiders.size();
}

void Sharqit::ZXDiagram::id_removal()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }
  
  while(true) {
  
    /* update node_places */
    update_node_places();
    update_phase_gadget();

    /* find internal zero-phase spiders */
    uint32_t idx_A = 0; // internal zero-phase spider
    uint32_t idx_B = 0; // adjacent to the zero-pahse spider
    uint32_t idx_C = 0; // adjacent to the zero-pahse spider
    std::vector<uint32_t> zero_phase_spiders; // list of internal zero-pahse spider's indexes
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (check_zero_phase_spider(i) && check_internal_node(i))
	zero_phase_spiders.push_back(i);
    }
  
    /* find idx_B and idx_C such that "... spider(idx_B)---zero-phase-spider(idx_A)---spider(idx_C)---[spiders]" */
    bool find = false;
    for (auto& idx:zero_phase_spiders) {
      if (degree_of_node(idx) == 2) {
	idx_A = idx;
	idx_B = adj_mat_[idx][0].to();
	idx_C = adj_mat_[idx][1].to();
	if (check_connect(idx_B, idx_C)) continue;
	else {
	  find = true;
	  break;
	}
      }
    }

    /* fuse spider(idx_B) and [spiders] directly */
    if (find) {
      nodes_[idx_B].phase(nodes_[idx_B].phase() + nodes_[idx_C].phase());
      for (auto& e:adj_mat_[idx_C]) {
	if (e.to() == idx_A) continue;
	connect_nodes(idx_B, e.to(), e.kind());
      }
      remove_edges_of_node(idx_A);
      remove_edges_of_node(idx_C);
      
      remove_isolated_spiders();
    }
    else break;
  }

  remove_parallel_selfloops_hadamard_edges();
}

void Sharqit::ZXDiagram::gfusion_one_time(const uint32_t idx_A_phase, const uint32_t idx_B_phase)
{
  uint32_t idx_B_root = adj_mat_[idx_B_phase][0].to();
  nodes_[idx_A_phase].phase(nodes_[idx_A_phase].phase() + nodes_[idx_B_phase].phase());
  remove_edges_of_node(idx_B_phase);
  remove_edges_of_node(idx_B_root);
}

uint32_t Sharqit::ZXDiagram::gfusion()
{
  if (kind() != Sharqit::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  /* random generation for Zobrist Hash */
  std::random_device rnd;
  std::mt19937 mt(rnd());
  std::uniform_int_distribution<> rand100(0, 1000000);
  std::vector<uint32_t> rand_table(adj_mat_.size());
  for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
    rand_table[i] = rand100(mt);
  }

  std::vector<uint32_t> leafs_A;
  std::vector<uint32_t> leafs_B;

  uint32_t max_degree_pre = 0;

  std::vector<uint32_t> pg_phase_spiders;
  std::map<uint32_t, uint32_t> pg_used_map;
  std::vector<std::pair<uint32_t, uint32_t>> pg_phase_spiders_pairs;

  update_node_places();
  update_phase_gadget();

  /* get phase spiders of phase gadget, initialize used array */
  pg_phase_spiders.clear();
  pg_used_map.clear();
  uint32_t max_degree = 0;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_pg_phase_node(i)) {
      pg_phase_spiders.push_back(i);
      pg_used_map[i] = 0;
      if (degree_of_node(adj_mat_[i][0].to()) > max_degree) {
	max_degree = degree_of_node(adj_mat_[i][0].to());
      }
    }
  }
  
  /* find 2 phase gadget that have same leaf nodes */

  pg_phase_spiders_pairs.clear();    

  if (max_degree > max_degree_pre) {
    leafs_A.resize(max_degree);
    leafs_B.resize(max_degree);
    max_degree_pre = max_degree;
  }

  uint32_t leafs_A_size = 0;
  uint32_t leafs_B_size = 0;
  uint32_t leafs_size = 0;

  uint32_t leafs_A_xor = 0;
  uint32_t leafs_B_xor = 0;

  uint32_t idx_A_phase = 0; // phase gadget (phase node)
  uint32_t idx_B_phase = 0; // phase gadget (phase node)
  uint32_t idx_A_root = 0; // phase gadget (root node)
  uint32_t idx_B_root = 0; // phase gadget (root node)
  std::vector<uint32_t> leafs;

  bool find = false;

  for (uint32_t i = 0; i < pg_phase_spiders.size(); ++i) {

    idx_A_phase = pg_phase_spiders[i];
    idx_A_root = adj_mat_[idx_A_phase][0].to();

    if (pg_used_map[idx_A_phase] == 1) continue;

    leafs_A_xor = 0;
    leafs_A_size = 0;
    for (uint32_t k = 0; k < adj_mat_[idx_A_root].size(); ++k) {
      if (adj_mat_[idx_A_root][k].to() == idx_A_phase) continue;
      leafs_A[leafs_A_size] = adj_mat_[idx_A_root][k].to();
      leafs_A_xor ^= rand_table[adj_mat_[idx_A_root][k].to()];
      ++leafs_A_size;
    }

    for (uint32_t j = i + 1; j < pg_phase_spiders.size(); ++j) {

      idx_B_phase = pg_phase_spiders[j];
      idx_B_root = adj_mat_[idx_B_phase][0].to();

      if (pg_used_map[idx_B_phase] == 1) continue;

      if (adj_mat_[idx_A_root].size() == adj_mat_[idx_B_root].size()) {
	leafs_B_xor = 0;
	leafs_B_size = 0;
	for (uint32_t l = 0; l < adj_mat_[idx_B_root].size(); ++l) {
	  if (adj_mat_[idx_B_root][l].to() == idx_B_phase) continue;
	  leafs_B[leafs_B_size] = adj_mat_[idx_B_root][l].to();
	  leafs_B_xor ^= rand_table[adj_mat_[idx_B_root][l].to()];
	  ++leafs_B_size;
	}
	leafs_size = leafs_A_size;

	if (leafs_A_xor == leafs_B_xor) {
	  std::sort(leafs_A.begin(), leafs_A.begin() + leafs_A_size);
	  std::sort(leafs_B.begin(), leafs_B.begin() + leafs_B_size);
	  bool flg = true;
	  for (uint32_t m = 0; m < leafs_size; ++m) {
	    if (leafs_A[m] != leafs_B[m]) {
	      flg = false;
	      break;
	    }
	  }
	  if (flg) {
	    find = true;
	    pg_phase_spiders_pairs.push_back({idx_A_phase, idx_B_phase});
	    pg_used_map[idx_A_phase] = 1;
	    pg_used_map[idx_B_phase] = 1;
	    break;
	  }
	}
      }
    }
  }
  
  if (!find) return 0;

  for (auto it = pg_phase_spiders_pairs.begin(); it != pg_phase_spiders_pairs.end(); ++it) {
    gfusion_one_time(it->first, it->second);
  }
    
  remove_isolated_spiders();

  return pg_phase_spiders_pairs.size();
}

void Sharqit::ZXDiagram::simplify()
{
  graph_like();

  while (true) {
    uint32_t l = lcomp();
    uint32_t p1 = pivot1();
    if (l + p1 == 0) break;
  }
  id_removal();

  while (true) {
    uint32_t spider_num = spider_count();
    while (true) {
      uint32_t l = lcomp();
      uint32_t p1 = pivot1();
      uint32_t p2 = pivot2();
      uint32_t p3 = pivot3();
      uint32_t gf = gfusion();
      if (l + p1 + p2 + p3 + gf == 0) break;
    }
    id_removal();
    if (spider_num == spider_count()) break;
  }
}
