#include "sharq.h"

/**
 *  member functions
 */

void Sharq::ZXDiagram::fuse_spiders()
{
  while (true) {
    update_node_places();
    update_phase_gadget();

    bool done = true;
    uint32_t idx_target;
    uint32_t idx_remove;
  
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (kind_of_node(i) == Sharq::ZXNodeKind::Input ||
  	  kind_of_node(i) == Sharq::ZXNodeKind::Output) continue;
  
      bool find_normal = false;
      for (auto& edge:adj_mat_[i]) {
  	if ((edge.kind() == Sharq::ZXEdgeKind::Plain) &&
	    (kind_of_node(i) == kind_of_node(edge.to())) &&
	    (kind_of_node(edge.to()) != Sharq::ZXNodeKind::Input &&
	     kind_of_node(edge.to()) != Sharq::ZXNodeKind::Output)) {
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

void Sharq::ZXDiagram::conv_x_to_z()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      if (kind_of_node(i) == Sharq::ZXNodeKind::XSpider) {
	adj_mat_[i][j].reverse_kind();
      }
      if (kind_of_node(adj_mat_[i][j].to()) == Sharq::ZXNodeKind::XSpider) {
	adj_mat_[i][j].reverse_kind();
      }
    }
  }
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
     if (kind_of_node(i) == Sharq::ZXNodeKind::XSpider) {
      nodes_[i].kind(Sharq::ZXNodeKind::ZSpider);
    }
  }
}

void Sharq::ZXDiagram::remove_parallel_selfloops_hadamard_edges()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    /* initialize hadamard counts */
    std::map<uint32_t, uint32_t> hadamard_counts;
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      hadamard_counts.insert_or_assign(adj_mat_[i][j].to(), 0);
    }
    /* count hadamard edge */
    for (uint32_t j = 0; j < adj_mat_[i].size(); ++j) {
      if (adj_mat_[i][j].kind() == Sharq::ZXEdgeKind::Hadamard) {
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
	adj_mat_[i].push_back(Sharq::ZXEdge(Sharq::ZXEdgeKind::Hadamard, it->first));
      }
    }
  }
}

void Sharq::ZXDiagram::update_node_places()
{
  if (nodes_.size() > node_places_.size()) {
    node_places_.assign(nodes_.size(), Sharq::ZXNodePlace::Internal);
  }
  else {
    node_places_.assign(node_places_.size(), Sharq::ZXNodePlace::Internal);
  }

  for (auto& in:inputs_) {
    node_places_[in] = Sharq::ZXNodePlace::Terminal;
    uint32_t from = in;
    uint32_t to = adj_mat_[from][0].to(); // index of spider adjacent input node
    node_places_[to] = Sharq::ZXNodePlace::Boundary;
  }

  for (auto& out:outputs_) {
    node_places_[out] = Sharq::ZXNodePlace::Terminal;
    uint32_t from = out;
    uint32_t to = adj_mat_[from][0].to(); // index of spider adjacent output node
    node_places_[to] = Sharq::ZXNodePlace::Boundary;
  }
}

void Sharq::ZXDiagram::update_phase_gadget()
{
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    nodes_[i].pg_phase(false);
    nodes_[i].pg_root(false);
  }
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    if (check_z_spider(i) && degree_of_node(i) == 1 &&
	adj_mat_[i][0].kind() == Sharq::ZXEdgeKind::Hadamard &&
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

void Sharq::ZXDiagram::graph_like()

{
  if (kind() == Sharq::ZXDiagramKind::GraphLike) return;
  
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

  // test
  
  /* if input connected to Hadamard edge, convert Plain edge */
  for (auto& in:inputs_) {
    for (auto& e:adj_mat_[in]) {
      if (e.kind() == Sharq::ZXEdgeKind::Hadamard) {
	uint32_t to = e.to();
	uint32_t mid = append_node(Sharq::ZXNode(Sharq::ZXNodeKind::ZSpider,
						 Sharq::Phase(0), nodes_[in].q()),
				   Sharq::ZXEdge(Sharq::ZXEdgeKind::Plain, in));
	connect_nodes(mid, to, Sharq::ZXEdgeKind::Hadamard);
      }
    }
  }
  for (auto& in:inputs_) {
    for (auto& e:adj_mat_[in]) {
      if (e.kind() == Sharq::ZXEdgeKind::Hadamard) {
	remove_edge(in, e.to());
      }
    }
  }
  
  /* if output connected to Hadamard edge, convert Plain edge */
  for (auto& out:outputs_) {
    for (auto& e:adj_mat_[out]) {
      if (e.kind() == Sharq::ZXEdgeKind::Hadamard) {
	uint32_t to = e.to();
	uint32_t mid = append_node(Sharq::ZXNode(Sharq::ZXNodeKind::ZSpider,
						 Sharq::Phase(0), nodes_[out].q()),
				   Sharq::ZXEdge(Sharq::ZXEdgeKind::Plain, out));
	connect_nodes(mid, to, Sharq::ZXEdgeKind::Hadamard);
      }
    }
  }
  for (auto& out:outputs_) {
    for (auto& e:adj_mat_[out]) {
      if (e.kind() == Sharq::ZXEdgeKind::Hadamard) {
	remove_edge(out, e.to());
      }
    }
  }
  
  kind(Sharq::ZXDiagramKind::GraphLike);
}

void Sharq::ZXDiagram::lcomp_one_time(const uint32_t idx_A)
{
  /* node indexes of neighbours */
  std::vector<uint32_t> idx_neighbours = adjacent_node_indexes(idx_A);
  xor_hadamard_edges(idx_neighbours);

  /* change phases of the edges */
  for (uint32_t i = 0; i < idx_neighbours.size(); ++i) {
    nodes_[idx_neighbours[i]].phase(nodes_[idx_neighbours[i]].phase() - nodes_[idx_A].phase());
  }

  /* remove target node */
  remove_node(idx_A);
}

void Sharq::ZXDiagram::lcomp()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. must be graph-like ZX diagram.");
  }

  while(true) {
    update_node_places();
    update_phase_gadget();
    bool done = true;
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if ((check_internal_node(i) && check_z_spider(i)) == false) continue;
      if (check_pg_leaf_node(i)) continue;
      /* only for intrenal Z spiders */
      if (nodes_[i].phase() == Sharq::Phase(1,2) ||
	  nodes_[i].phase() == Sharq::Phase(3,2)) {
	done = false;
	lcomp_one_time(i);
	break;
      }
    }
    if (done == true) break;
  }

  id_removal();
}

void Sharq::ZXDiagram::pivot1_one_time(const uint32_t idx_A, const uint32_t idx_B)
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
  Sharq::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of idx_A to the phases of all indexes_B */
  Sharq::Phase phase_B = nodes_[idx_B].phase();
  for (auto& idx:indexes_A) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_B);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharq::Phase(1));
  }
}

void Sharq::ZXDiagram::pivot1()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  while (true) {
  
    /* update node_places */
    update_node_places();
    update_phase_gadget();

    /* find internal Pauli spiders pair */
    uint32_t idx_A = 0; // index of internal Pauli spider
    uint32_t idx_B = 0; // index of any internal spider adjacent to the Pauli spider
    std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes

    /* internal pauli spiders */
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (check_pauli_spider(i) && check_internal_node(i)) pauli_spiders.push_back(i);
    }
  
    bool find = false;
    for (auto& idx:pauli_spiders) {
      for (auto& edge:adj_mat_[idx]) {
	if (check_pauli_spider(edge.to()) && check_internal_node(edge.to())) {
	  idx_A = idx;
	  idx_B = edge.to();
	  //pauli_count = 2;
	  find = true;
	  break;
	}
      }
      if (find == true) break;
    }

    if (find) {
      pivot1_one_time(idx_A, idx_B);
      remove_isolated_spiders();
    }
    else break;
  }

  id_removal();
}

void Sharq::ZXDiagram::pivot2_one_time(const uint32_t idx_A, const uint32_t idx_B)
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
  Sharq::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharq::Phase(1));
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
  Sharq::Phase phase_B = nodes_[idx_B].phase();
  Sharq::Fraction frac = phase_A.frac();
  frac.reduce();
  if (std::abs(frac.numerator()) % 2 == 1) {
    phase_B = -phase_B;
    nodes_[idx_B].phase(phase_B);
  }
  idx_D = append_node(ZXNode(ZXNodeKind::ZSpider, phase_B), ZXEdge(ZXEdgeKind::Hadamard, idx_C));
  nodes_[idx_C].pg_root(true);
  nodes_[idx_D].pg_phase(true);
}

void Sharq::ZXDiagram::pivot2()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  while(true) {
  
    /* update node_places */
    update_node_places();
    update_phase_gadget();

    /* find internal Pauli spiders pair */
    uint32_t idx_A = 0; // index of internal Pauli spider
    uint32_t idx_B = 0; // index of any internal spider adjacent to the Pauli spider
    std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes

    /* internal pauli spiders */
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (check_pauli_spider(i) && check_internal_node(i) && !check_phase_gadget_node(i))
	pauli_spiders.push_back(i);
    }
  
    bool find = false;
    for (auto& idx:pauli_spiders) {
      for (auto& edge:adj_mat_[idx]) {
	if (check_internal_node(edge.to()) && check_non_clifford_spider(edge.to()) && !check_phase_gadget_node(edge.to())) {
	  idx_A = idx; // index of pauli spider
	  idx_B = edge.to(); // index of spider adjacent to the pauli spider
	  find = true;
	  break;
	}
      }
      if (find == true) break;
    }

    if (find) {
      pivot2_one_time(idx_A, idx_B);
      remove_isolated_spiders();
    }
    else break;
  }

  id_removal();
}

void Sharq::ZXDiagram::pivot3_one_time(const uint32_t idx_A, const uint32_t idx_B)
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
  Sharq::Phase phase_A = nodes_[idx_A].phase();
  for (auto& idx:indexes_B) {
    nodes_[idx].phase(nodes_[idx].phase() + phase_A);
  }

  /* add phase of PI to the phases of all indexes_AandB */
  for (auto& idx:indexes_AandB) {
    nodes_[idx].phase(nodes_[idx].phase() + Sharq::Phase(1));
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
  Sharq::Phase phase_B = nodes_[idx_B].phase();
  Sharq::Fraction frac = phase_A.frac();
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
}

void Sharq::ZXDiagram::pivot3()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  while(true) {
  
    /* update node_places */
    update_node_places();
    update_phase_gadget();

    /* find internal Pauli spiders pair */
    uint32_t idx_A = 0; // index of internal Pauli spider
    uint32_t idx_B = 0; // index of any internal spider adjacent to the Pauli spider
    std::vector<uint32_t> pauli_spiders; // list of internal pauli spider's indexes

    /* internal pauli spiders */
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (check_pauli_spider(i) && check_internal_node(i) && !check_phase_gadget_node(i))
	pauli_spiders.push_back(i);
    }
  
    bool find = false;
    for (auto& idx:pauli_spiders) {
      for (auto& edge:adj_mat_[idx]) {
	if (check_boundary_node(edge.to()) && check_non_clifford_spider(edge.to()) && !check_phase_gadget_node(edge.to())) {
	  idx_A = idx; // index of pauli spider
	  idx_B = edge.to(); // index of boundary spider connect to the pauli spider
	  find = true;
	  break;
	}
      }
      if (find) break;
    }

    if (find && check_connect_input_node(idx_B) && check_connect_output_node(idx_B)) break;
    if (find && (check_connect_phase_gadget(idx_A) || check_connect_phase_gadget(idx_B))) break;

    if (find) {
      pivot3_one_time(idx_A, idx_B);
      remove_isolated_spiders();
    }
    else break;
  }

  id_removal();
}

void Sharq::ZXDiagram::id_removal()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
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

void Sharq::ZXDiagram::gfusion()
{
  if (kind() != Sharq::ZXDiagramKind::GraphLike) {
    throw std::runtime_error("can't execute local complementation for general ZX diagram. it must be graph-like ZX diagram.");
  }

  while(true) {
  
    update_node_places();
    update_phase_gadget();
  
    std::vector<uint32_t> pg_phase_spiders;
    for (uint32_t i = 0; i < nodes_.size(); ++i) {
      if (check_pg_phase_node(i)) pg_phase_spiders.push_back(i);
    }
  
    /* find 2 phase gadget that have same leaf nodes */
    uint32_t idx_A_phase = 0; // phase gadget (phase node)
    uint32_t idx_B_phase = 0; // phase gadget (phase node)
    uint32_t idx_A_root = 0; // phase gadget (root node)
    uint32_t idx_B_root = 0; // phase gadget (root node)
    std::vector<uint32_t> leafs;
    bool find = false;
    for (uint32_t i = 0; i < pg_phase_spiders.size(); ++i) {
      for (uint32_t j = i + 1; j < pg_phase_spiders.size(); ++j) {
  	idx_A_phase = pg_phase_spiders[i];
  	idx_B_phase = pg_phase_spiders[j];
  	idx_A_root = adj_mat_[idx_A_phase][0].to();
  	idx_B_root = adj_mat_[idx_B_phase][0].to();
  	if (adj_mat_[idx_A_root].size() == adj_mat_[idx_B_root].size()) {
	  uint32_t leaf_num = 0; // number of leaf nodes
  	  leaf_num = adj_mat_[idx_A_root].size() - 1;

	  std::vector<uint32_t> leafs_A;
	  std::vector<uint32_t> leafs_B;
	  for (uint32_t i = 0; i < adj_mat_[idx_A_root].size(); ++i) {
	    if (adj_mat_[idx_A_root][i].to() == idx_A_phase) continue;
	    leafs_A.push_back(adj_mat_[idx_A_root][i].to());
	  }
	  for (uint32_t i = 0; i < adj_mat_[idx_B_root].size(); ++i) {
	    if (adj_mat_[idx_B_root][i].to() == idx_B_phase) continue;
	    leafs_B.push_back(adj_mat_[idx_B_root][i].to());
	  }
	  std::sort(leafs_A.begin(), leafs_A.end());
	  std::sort(leafs_B.begin(), leafs_B.end());
	  leafs = leafs_A;
  
  	  find = true;
	  for (uint32_t i = 0; i < leaf_num; ++i) {
	    if (leafs_A[i] != leafs_B[i]) {
	      find = false;
	      break;
	    }
	  }
	  if (find) break;
  	}
      }
      if (find) break;
    }
  
    if (!find) break;
  
    uint32_t idx_C_phase = 0;
    uint32_t idx_C_root = 0;
  
    Sharq::Phase p = nodes_[idx_A_phase].phase() + nodes_[idx_B_phase].phase();
    idx_C_phase = append_node(ZXNode(ZXNodeKind::ZSpider, p));
    idx_C_root = append_node(ZXNode(ZXNodeKind::ZSpider, Phase(0)), ZXEdge(ZXEdgeKind::Hadamard, idx_C_phase));
    //for (auto& n:leafs_A) connect_nodes(idx_C_root, n, ZXEdgeKind::Hadamard);
    for (auto& n:leafs) connect_nodes(idx_C_root, n, ZXEdgeKind::Hadamard);
    remove_edges_of_node(idx_A_phase);
    remove_edges_of_node(idx_A_root);
    remove_edges_of_node(idx_B_phase);
    remove_edges_of_node(idx_B_root);
    
    remove_isolated_spiders();
  }

  id_removal();
}

void Sharq::ZXDiagram::simplify()
{
  graph_like();
  lcomp();
  pivot1();
  pivot2();
  pivot3();
  gfusion();
}
