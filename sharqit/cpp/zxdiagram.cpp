/**
 * @file zxdiagram.cpp
 * @brief member functions of ZXDiagram class
 */

#include "zx.h"

Sharqit::ZXDiagram::ZXDiagram(uint32_t qubit_num)
{
  kind_ = Sharqit::ZXDiagramKind::General;
  qubit_num_ = qubit_num;
  inputs_.resize(qubit_num);
  outputs_.resize(qubit_num);

  for (uint32_t i = 0; i < qubit_num; ++i) {
    ZXNode node_in(Sharqit::ZXNodeKind::Input, Sharqit::Phase(0), i);
    inputs_[i] = i;
    nodes_.push_back(node_in);
    Sharqit::ZXEdge edge(ZXEdgeKind::Plain, qubit_num + i);
    adj_mat_.push_back({edge});
  }

  for (uint32_t i = 0; i < qubit_num; ++i) {
    ZXNode node_out(Sharqit::ZXNodeKind::Output, Sharqit::Phase(0), i);
    outputs_[i] = qubit_num + i;
    nodes_.push_back(node_out);
    Sharqit::ZXEdge edge(ZXEdgeKind::Plain, i);
    adj_mat_.push_back({edge});
  }

  node_places_.assign(Sharqit::DEF_NODE_PLACES_SIZE, Sharqit::ZXNodePlace::Internal);
}

std::string Sharqit::ZXDiagram::to_string() const
{
  std::stringstream ss;
  std::vector<ZXNode> nodes = nodes_;
  
  ss << "qubit_num: " << qubit_num_ << std::endl;
  ss << "adj_mat:" << std::endl;
  for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
    ss << " [" << i << "] " << nodes[i].name() + " ";
    std::vector<Sharqit::ZXEdge> edges = adj_mat_[i];
    for (auto& edge:edges) {
      ss << edge.name() + ",";
    }
    ss << std::endl;
  }
  std::string s = ss.str();
  s.pop_back();
  return s;
}

std::map<std::string, uint32_t> Sharqit::ZXDiagram::stats() const
{
  std::map<std::string, uint32_t> sts;
  for (auto& node:nodes_) {
    if (node.kind() == Sharqit::ZXNodeKind::XSpider) sts["xspider"] = sts["xspider"] + 1;
    if (node.kind() == Sharqit::ZXNodeKind::ZSpider) sts["zspider"] = sts["zspider"] + 1;
    if (node.phase() == Sharqit::Phase(0) || node.phase() == Sharqit::Phase(1) ||
	node.phase() == Sharqit::Phase(1,2) || node.phase() == Sharqit::Phase(3,2)) {
      sts["clifford"] = sts["clifford"] + 1;
    }
    if (node.phase() != Sharqit::Phase(0) && node.phase() != Sharqit::Phase(1) &&
	node.phase() != Sharqit::Phase(1,2) && node.phase() != Sharqit::Phase(3,2)) {
      sts["non-clifford"] = sts["non-clifford"] + 1;
    }
  }
  uint32_t hadamard_num = 0;
  for (auto& adj:adj_mat_) {
    for (auto& edge:adj) {
      if (edge.kind() == Sharqit::ZXEdgeKind::Hadamard) hadamard_num++;
    }
  }
  sts["hadamard"] = hadamard_num / 2;

  return sts;
}

void Sharqit::ZXDiagram::to_dot_file(const std::string& file_name) const
{
  std::vector<ZXNode> nodes = nodes_;
  std::string label = "";
  std::ofstream ofs;
  ofs.open(file_name, std::ios::out);

  ofs << "graph graph_name {" << std::endl;
  ofs << "  graph [" << std::endl;
  ofs << "    charset = \"UTF-8\";" << std::endl;
  ofs << "    labelloc = \"t\"," << std::endl;
  ofs << "    labeljust = \"c\"," << std::endl;
  ofs << "    bgcolor = white," << std::endl;
  ofs << "    fontcolor = black," << std::endl;
  ofs << "    fontsize = 14," << std::endl;
  ofs << "    style = \"filled\"," << std::endl;
  ofs << "    rankdir = LR," << std::endl;
  ofs << "    splines = spline," << std::endl;
  ofs << "    ];" << std::endl;
  ofs << "  node [" << std::endl;
  ofs << "    colorscheme = \"rdylgn11\"," << std::endl;
  ofs << "    shape = ellipse, " << std::endl;
  ofs << "    style = \"filled\"," << std::endl;
  ofs << "    fontsize = 10," << std::endl;
  ofs << "    fontcolor = 6," << std::endl;
  ofs << "    fontname = \"Migu 1M\"," << std::endl;
  ofs << "    fixedsize = true," << std::endl;
  ofs << "    height = 0.2," << std::endl;
  ofs << "    ];" << std::endl;
  ofs << "  edge [" << std::endl;
  ofs << "    arrowhead = normal," << std::endl; 
  ofs << "    ];" << std::endl;

  ofs << std::endl;

  /* node definition */
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    ZXNode node = nodes_[i];
    std::string label = "";
    std::string width = "";
    std::string height = "";
    std::string color = "";
    std::string fillcolor = "";
    std::string fontcolor = "";
    if (node.kind() == Sharqit::ZXNodeKind::Input || node.kind() == Sharqit::ZXNodeKind::Output) {
      label = node.name();
      width = "0.5";
      height = "0.2";
      color = "white";
      fillcolor = "white";
      fontcolor = "black";
    }
    else if (node.kind() == Sharqit::ZXNodeKind::XSpider) {
      color = "black";
      fillcolor = "\"#ff8888\"";
      fontcolor = "black";
      if (node.phase().is_zero() == true) {
	width = "0.15";
	height = "0.15";
      }
      else {
	width = "0.3";
	height = "0.3";
	label = node.phase().to_string();
      }
    }
    else if (node.kind() == Sharqit::ZXNodeKind::ZSpider) {
      color = "black";
      fillcolor = "\"#ddffdd\"";
      fontcolor = "black";
      if (node.phase().is_zero() == true) {
	width = "0.15";
	height = "0.15";
      }
      else {
	width = "0.3";
	height = "0.3";
	label = node.phase().to_string();
      }
    }

    ofs << "  node_" << i << " [label = " << "\"" << label << "\"";
    ofs << ", width = " << width << ", height = " << height << ", color = " << color;
    ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;
  }
  ofs << std::endl;

  /* edge definition */
  for (uint32_t i = 0; i < adj_mat_.size(); ++i) {
    std::vector<Sharqit::ZXEdge> edges = adj_mat_[i];
    for (auto& edge:edges) {
      if (i >= edge.to()) continue;
      std::string style = "solid";
      std::string color = "black";
      if (edge.kind() == Sharqit::ZXEdgeKind::Hadamard) {
	style = "dashed";
	color = "blue";
      }
      ofs << "  node_" << i << " -- " << "node_" << edge.to();
      ofs << " [style = " << style << ", color = " << color << "];" << std::endl;
    }
  }
  /* virtual edge definition (inputs) */
  for (uint32_t i = 0; i < inputs_.size() - 1; ++i) {
    ofs << "  node_" << inputs_[i] << " -- " << "node_" << inputs_[i+1];
    ofs << " [style = solid, color = white];" << std::endl;
  }
  /* virtual edge definition (outputs) */
  for (uint32_t i = 0; i < outputs_.size() - 1; ++i) {
    ofs << "  node_" << outputs_[i] << " -- " << "node_" << outputs_[i+1];
    ofs << " [style = solid, color = white];" << std::endl;
  }
  
  ofs << std::endl;

  /* rank definition */
  std::vector<std::string> input_nodes;
  std::vector<std::string> output_nodes;
  for (uint32_t i = 0; i < nodes_.size(); ++i) {
    std::string node_name = "node_" + std::to_string(i);
    if (nodes_[i].kind() == Sharqit::ZXNodeKind::Input) {
      input_nodes.push_back(node_name);
    }
    else if (nodes_[i].kind() == Sharqit::ZXNodeKind::Output) {
      output_nodes.push_back(node_name);
    }
  }

  ofs << "  {rank = min; ";
  for (auto& input_node:input_nodes) {
    ofs << input_node << "; ";
  }
  ofs << "}" << std::endl;
  ofs << "  {rank = max; ";
  for (auto& output_node:output_nodes) {
    ofs << output_node << "; ";
  }
  ofs << "}" << std::endl;
  
  ofs << "}" << std::endl;

  ofs.close();
}

void Sharqit::ZXDiagram::to_svg_file(const std::string& file_name) const
{
  std::string tmpfile_name = std::filesystem::temp_directory_path().native() + "/sharq_zxdiagram_to_svg_file.dot";
  to_dot_file(tmpfile_name);
  std::string com = "dot -Kdot -Tsvg " + tmpfile_name + " -o" + file_name + "; rm " + tmpfile_name;

  const char* com_char = com.c_str();
  int ret = system(com_char);
  if (ret != 0) {
    throw std::runtime_error("fail to execute dot command.");
  }
}

void Sharqit::ZXDiagram::add_qgate(const QGate& qgate)
{
  std::vector<uint32_t> qid = qgate.qid();

  /* X,Z,S,Sdg,T,Tdg,RX,RZ */
  if (qgate.kind() != Sharqit::QGateKind::H && qgate.kind() != Sharqit::QGateKind::CX) {
    uint32_t from = outputs_[qid[0]];
    uint32_t to = 0;
    Sharqit::ZXNodeKind kind = Sharqit::ZXNodeKind::XSpider;
    Sharqit::Phase phase = Sharqit::Phase(0);

    if (qgate.kind() == Sharqit::QGateKind::X) {
      kind = Sharqit::ZXNodeKind::XSpider;
      phase = Sharqit::Phase(1);
    }
    else if (qgate.kind() == Sharqit::QGateKind::Z) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = Sharqit::Phase(1);
    }
    else if (qgate.kind() == Sharqit::QGateKind::S) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = Sharqit::Phase(1, 2);
    }
    else if (qgate.kind() == Sharqit::QGateKind::Sdg) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = Sharqit::Phase(3, 2);
    }
    else if (qgate.kind() == Sharqit::QGateKind::T) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = Sharqit::Phase(1, 4);
    }
    else if (qgate.kind() == Sharqit::QGateKind::Tdg) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = Sharqit::Phase(7, 4);
    }
    else if (qgate.kind() == Sharqit::QGateKind::RZ) {
      kind = Sharqit::ZXNodeKind::ZSpider;
      phase = qgate.phase();
    }

    nodes_[from].kind(kind);
    nodes_[from].phase(phase);
    nodes_[from].q(qid[0]);

    Sharqit::ZXNode out(Sharqit::ZXNodeKind::Output, Sharqit::Phase(0), qid[0]);
    nodes_.push_back(out);
    to = nodes_.size() - 1;
    outputs_[qid[0]] = to;

    Sharqit::ZXEdge edge_forward(ZXEdgeKind::Plain, to);
    adj_mat_[from].push_back(edge_forward);
    Sharqit::ZXEdge edge_backward(ZXEdgeKind::Plain, from);
    adj_mat_.push_back({edge_backward});
  }
  /* Hadamard */
  else if (qgate.kind() == Sharqit::QGateKind::H) {
    uint32_t to = outputs_[qid[0]];
    if (adj_mat_[to].size() != 1) throw std::runtime_error("error.");
    if (adj_mat_[to][0].kind() == Sharqit::ZXEdgeKind::Plain) {
      adj_mat_[to][0].kind(Sharqit::ZXEdgeKind::Hadamard);
    }
    else {
      adj_mat_[to][0].kind(Sharqit::ZXEdgeKind::Plain);
    }

    uint32_t from = adj_mat_[to][0].to();
    uint32_t e = 0;
    bool flg = false;
    for (uint32_t i = 0; i < adj_mat_[from].size(); ++i) {
      if (kind_of_node(adj_mat_[from][i].to()) == Sharqit::ZXNodeKind::Output) {
	e = i;
	flg = true;
	break;
      }
    }
    if (flg == false) {
      throw std::runtime_error("error.");
    }

    if (adj_mat_[from][e].kind() == Sharqit::ZXEdgeKind::Plain) {
      adj_mat_[from][e].kind(Sharqit::ZXEdgeKind::Hadamard);
    }
    else {
      adj_mat_[from][e].kind(Sharqit::ZXEdgeKind::Plain);
    }
    
  }
  /* CNOT */
  else if (qgate.kind() == Sharqit::QGateKind::CX) {
    /* 2 outputs -> control and target */
    uint32_t from_c = outputs_[qid[0]];
    uint32_t from_t = outputs_[qid[1]];
    Sharqit::ZXNodeKind kind_c = Sharqit::ZXNodeKind::ZSpider;
    Sharqit::ZXNodeKind kind_t = Sharqit::ZXNodeKind::XSpider;
    Sharqit::Phase phase_c = Sharqit::Phase(0);
    Sharqit::Phase phase_t = Sharqit::Phase(0);

    nodes_[from_c].kind(kind_c);
    nodes_[from_c].phase(phase_c);
    nodes_[from_c].q(qid[0]);

    nodes_[from_t].kind(kind_t);
    nodes_[from_t].phase(phase_t);
    nodes_[from_t].q(qid[1]);

    /* edges between control and target */
    Sharqit::ZXEdge edge_c_t(ZXEdgeKind::Plain, from_t);
    adj_mat_[from_c].push_back(edge_c_t);
    Sharqit::ZXEdge edge_t_c(ZXEdgeKind::Plain, from_c);
    adj_mat_[from_t].push_back(edge_t_c);

    /* new output (control) */
    Sharqit::ZXNode out_c(Sharqit::ZXNodeKind::Output, Sharqit::Phase(0), qid[0]);
    nodes_.push_back(out_c);
    uint32_t to_c = nodes_.size() - 1;
    outputs_[qid[0]] = to_c;

    /* edges between new control and new output(control) */
    Sharqit::ZXEdge edge_forward_c(ZXEdgeKind::Plain, to_c);
    adj_mat_[from_c].push_back(edge_forward_c);
    Sharqit::ZXEdge edge_backward_c(ZXEdgeKind::Plain, from_c);
    adj_mat_.push_back({edge_backward_c});

    /* new output (target) */
    Sharqit::ZXNode out_t(Sharqit::ZXNodeKind::Output, Sharqit::Phase(0), qid[1]);
    nodes_.push_back(out_t);
    uint32_t to_t = nodes_.size() - 1;
    outputs_[qid[1]] = to_t;

    /* edges between new target and new output(target) */
    Sharqit::ZXEdge edge_forward_t(ZXEdgeKind::Plain, to_t);
    adj_mat_[from_t].push_back(edge_forward_t);
    Sharqit::ZXEdge edge_backward_t(ZXEdgeKind::Plain, from_t);
    adj_mat_.push_back({edge_backward_t});

  }
  else {
    throw std::runtime_error("unknown qgate kind.");
  }
}

bool Sharqit::ZXDiagram::check_connect(const uint32_t a, const uint32_t b) const
{
  bool ans = false;
  for (auto& edge:adj_mat_[a]) {
    if (edge.to() == b) {
      ans = true;
      break;
    }
  }
  if (ans == false) return false;

  ans = false;
  for (auto& edge:adj_mat_[b]) {
    if (edge.to() == a) {
      ans = true;
      break;
    }
  }
  if (ans == false) return false;
  
  return true;
}

void Sharqit::ZXDiagram::xor_hadamard_edge(const uint32_t a, const uint32_t b)
{
  /* edge of a -> b */
  bool hadamard_edge_exist = false;
  for (auto it = adj_mat_[a].begin(); it != adj_mat_[a].end(); ++it) {
    if (it->to() == b && it->kind() == Sharqit::ZXEdgeKind::Hadamard) {
      it = adj_mat_[a].erase(it);
      --it;
      hadamard_edge_exist = true;
      break;
    }
  }
  if (hadamard_edge_exist == false) {
    Sharqit::ZXEdge h_edge = Sharqit::ZXEdge(Sharqit::ZXEdgeKind::Hadamard, b);
    adj_mat_[a].push_back(h_edge);
  }

  /* edge of b -> a */
  hadamard_edge_exist = false;
  for (auto it = adj_mat_[b].begin(); it != adj_mat_[b].end(); ++it) {
    if (it->to() == a && it->kind() == Sharqit::ZXEdgeKind::Hadamard) {
      it = adj_mat_[b].erase(it);
      --it;
      hadamard_edge_exist = true;
      break;
    }
  }
  if (hadamard_edge_exist == false) {
    Sharqit::ZXEdge h_edge = Sharqit::ZXEdge(Sharqit::ZXEdgeKind::Hadamard, a);
    adj_mat_[b].push_back(h_edge);
  }
}

void Sharqit::ZXDiagram::xor_hadamard_edges(std::vector<uint32_t> node_indexes)
{
  std::sort(node_indexes.begin(), node_indexes.end());

  /* change all edges between neighbours */
  for (uint32_t i = 0; i < node_indexes.size(); ++i) {
    for (uint32_t j = i + 1; j < node_indexes.size(); ++j) {
      xor_hadamard_edge(node_indexes[i], node_indexes[j]);
    }
  }
}

Sharqit::ZXNodeKind Sharqit::ZXDiagram::remove_node(const uint32_t idx)
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
  
  return nodes_[idx].kind();
}

Sharqit::ZXEdgeKind Sharqit::ZXDiagram::remove_edge(const uint32_t a, const uint32_t b)
{
  ZXEdgeKind kind = Sharqit::ZXEdgeKind::None;

  if (a == b) return kind;
  
  for (auto it = adj_mat_[a].begin(); it != adj_mat_[a].end(); ++it) {
    if (it->to() == b) {
      kind = it->kind();
      it = adj_mat_[a].erase(it);
      --it;
    }
  }
  for (auto it = adj_mat_[b].begin(); it != adj_mat_[b].end(); ++it) {
    if (it->to() == a) {
      kind = it->kind();
      it = adj_mat_[b].erase(it);
      --it;
    }
  }

  return kind;
}

uint32_t Sharqit::ZXDiagram::append_node(const Sharqit::ZXNode node)
{
  nodes_.push_back(node);
  adj_mat_.resize(nodes_.size());
  uint32_t node_idx = nodes_.size() - 1;
  return node_idx;
}

uint32_t Sharqit::ZXDiagram::append_node(const Sharqit::ZXNode node, const Sharqit::ZXEdge edge)
{
  nodes_.push_back(node);
  adj_mat_.push_back({edge});

  uint32_t node_idx = nodes_.size() - 1;
  uint32_t to_idx = edge.to();
  ZXEdge edge_rev = ZXEdge(edge.kind(), node_idx);
  adj_mat_[to_idx].push_back(edge_rev);

  return node_idx;
}

void Sharqit::ZXDiagram::connect_nodes(const uint32_t a, const uint32_t b, const Sharqit::ZXEdgeKind kind)
{
  ZXEdge edge_forward(kind, b);
  ZXEdge edge_backward(kind, a);
  adj_mat_[a].push_back(edge_forward);
  adj_mat_[b].push_back(edge_backward);
}

void Sharqit::ZXDiagram::swap_nodes(uint32_t i, uint32_t j)
{
  Sharqit::ZXNodeKind kind_tmp = nodes_[i].kind();
  Sharqit::Phase phase_tmp = nodes_[i].phase();
  uint32_t q_tmp = nodes_[i].q();

  nodes_[i].kind(nodes_[j].kind());
  nodes_[i].phase(nodes_[j].phase());
  nodes_[i].q(nodes_[j].q());
  
  nodes_[j].kind(kind_tmp);
  nodes_[j].phase(phase_tmp);
  nodes_[j].q(q_tmp);
}

void Sharqit::ZXDiagram::remove_isolated_spiders()
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

void Sharqit::ZXDiagram::row_operation(const uint32_t a, const uint32_t b)
{
  for (auto& edge:adj_mat_[a]) {
    if (edge.kind() == Sharqit::ZXEdgeKind::Hadamard) {
      xor_hadamard_edge(b, edge.to());
    }
  }
}

std::vector<uint32_t> Sharqit::ZXDiagram::adjacent_node_indexes(uint32_t node_index)
{
  std::vector<uint32_t> indexes;
  for (auto& edge:adj_mat_[node_index]) {
    if (edge.kind() != Sharqit::ZXEdgeKind::Hadamard) {
      throw std::runtime_error("This ZX Diagram is not a graph-like.");
    }
    indexes.push_back(edge.to());
  }
  std::sort(indexes.begin(), indexes.end());

  return indexes;
}
