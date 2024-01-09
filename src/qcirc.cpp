#include "qcirc.h"

/**
 *  member functions
 */

void Sharq::QCirc::save(const std::string& file_name) const
{
  std::ofstream ofs;
  ofs.open(file_name, std::ios::out);

  for (auto& g:qgates_) {
    ofs << g.to_string(false) << std::endl;
  }

  ofs.close();
}

void Sharq::QCirc::load(const std::string& file_name)
{
  std::ifstream ifs(file_name);
  if (ifs.fail()) {
    throw std::runtime_error("Failed to open file.");
  }

  std::string str;
  while (std::getline(ifs, str)) {
    Sharq::QGate qgate(str);
    add_qgate(qgate);
  }

  ifs.close();
}

std::map<std::string, uint32_t> Sharq::QCirc::stats() const
{
  std::map<std::string, uint32_t> sts;
  sts["id_count"] = id_count();
  sts["x_count"] = x_count();
  sts["z_count"] = z_count();
  sts["h_count"] = h_count();
  sts["s_count"] = s_count();
  sts["t_count"] = t_count();
  sts["cx_count"] = cx_count();
  sts["2q_count"] = twoq_count();
  sts["rz_count"] = rz_count();
  sts["gate_count"] = gate_count();
  sts["depth"] = depth();
  sts["qubit_num"] = qubit_num();
  return sts;
}

void Sharq::QCirc::print_stats() const
{
  std::map<std::string, uint32_t> sts = Sharq::QCirc::stats();
  std::cout << "X_count  = " << sts["x_count"] << std::endl;
  std::cout << "Z_count  = " << sts["z_count"] << std::endl;
  std::cout << "H_count  = " << sts["h_count"] << std::endl;
  std::cout << "S_count  = " << sts["s_count"] << std::endl;
  std::cout << "T_count  = " << sts["t_count"] << std::endl;
  std::cout << "RZ_count = " << sts["rz_count"] << std::endl;
  std::cout << "CX_count = " << sts["cx_count"] << std::endl;
  std::cout << "2Q_count = " << sts["2q_count"] << std::endl;
  std::cout << "gate_count = " << sts["gate_count"] << std::endl;
  std::cout << "depth      = " << sts["depth"] << std::endl;
  std::cout << "qubit_num  = " << sts["qubit_num"] << std::endl;
}

uint32_t Sharq::QCirc::id_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_Id_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::x_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_X_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::z_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_Z_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::h_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_H_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::s_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_S_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::t_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_T_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::rz_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_RZ_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::cx_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_CX_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::cz_count() const
{
  uint32_t cnt = 0;
  for (auto& qgate:qgates_) if (qgate.is_CZ_gate()) ++cnt;
  return cnt;
}

uint32_t Sharq::QCirc::depth() const
{
  std::vector<uint32_t> depth_count(qubit_num_, 0);

  for (auto& qgate:qgates_) {
    std::vector<uint32_t> qid = qgate.qid();
    uint32_t max_dep = 0;
    for (auto& q:qid) {
      ++depth_count[q];
      if (depth_count[q] > max_dep) {
	max_dep = depth_count[q];
      }
    }
    if (qid.size() > 1) {
      for (auto& q:qid) {
	depth_count[q] = max_dep;
      }
    }
  }
  uint32_t depth = *max_element(depth_count.begin(), depth_count.end());

  return depth;
}

std::string Sharq::QCirc::to_string(const uint32_t width) const
{
  if (qubit_num_ == 0) {
    throw std::runtime_error("qubit_num must be positive integer.");
  }
  std::vector<std::string> str_list(qubit_num_);

  std::stringstream ss;
  ss << std::dec << qubit_num_ - 1;;
  uint32_t w = ss.str().size() + 6;

  for (uint32_t i = 0; i < qubit_num_; ++i) {
    str_list[i] += "q[" + std::to_string(i) + "] --";
    if (str_list[i].size() < w) {
      str_list[i].insert(str_list[i].size(), w - str_list[i].size(), '-');
    }
  }

  uint32_t max_strlen = str_list[0].size();

  std::vector<uint32_t> qid;
  std::string name;
  for (auto qgate:qgates_) {
    if ((qgate.kind() != Sharq::QGateKind::CX) && (qgate.kind() != Sharq::QGateKind::CZ)) {
      name = qgate.name(false);
      qid = qgate.qid();
      str_list[qid[0]] += name + "--";
      if (str_list[qid[0]].size() > max_strlen) {
	max_strlen = str_list[qid[0]].size();
      }
    }
    else {
      for (auto& str:str_list) {
	if (str.size() < max_strlen) {
	  str.insert(str.size(), max_strlen - str.size(), '-');
	}
      }
      uint32_t c = qgate.qid()[0];
      uint32_t t = qgate.qid()[1];
      for (uint32_t i = 0; i < qubit_num_; ++i) {
	if (i == c) str_list[i] += "*--";
	else if (i == t && qgate.kind() == Sharq::QGateKind::Id2) str_list[i] += "I--";
	else if (i == t && qgate.kind() == Sharq::QGateKind::CX) str_list[i] += "X--";
	else if (i == t && qgate.kind() == Sharq::QGateKind::CZ) str_list[i] += "*--";
	else if (c < t && c < i && i < t) str_list[i] += "|--";
	else if (t < c && t < i && i < c) str_list[i] += "|--";
	else str_list[i] += "---";
      }
      max_strlen = str_list[0].size();
    }
  }
  
  for (auto& str:str_list) {
    if (str.size() < max_strlen) {
      str.insert(str.size(), max_strlen - str.size(), '-');
    }
  }

  std::stringstream sst;

  uint32_t pos = 0;
  uint32_t tmp_strlen = max_strlen;
  uint32_t tmp_width = std::min(width, max_strlen);
  std::string cont = " ...";
  while (pos < max_strlen) {
    if (tmp_strlen < tmp_width || tmp_strlen < width) {
      tmp_width = tmp_strlen;
      cont = "";
    }
    for (uint32_t i = 0; i < qubit_num_; ++i) {
      sst << str_list[i].substr(pos, tmp_width) << cont << std::endl;
    }
    sst << std::endl;
    tmp_strlen -= width;
    pos += width;
  }

  std::string s = sst.str();
  s.pop_back(); // remove last one character ("\n")

  return s;
}

Sharq::QCirc& Sharq::QCirc::add_qgate(const Sharq::QGate& qgate)
{
  QGate qg = qgate;
  qgates_.push_back(qg);
  for (auto& q:qg.qid()) {
    if (q >= qubit_num_) qubit_num_ = q + 1;
  }
  return *this;
}

Sharq::QCirc& Sharq::QCirc::add_qgate(const Sharq::QGateKind kind,
				      const std::vector<uint32_t>& qid, const Sharq::Phase& phase)
{
  Sharq::QGate qgate(kind, qid, phase);
  qgates_.push_back(qgate);

  for (auto& q:qid) {
    if (q >= qubit_num_) qubit_num_ = q + 1;
  }
  return *this;
}

Sharq::QCirc& Sharq::QCirc::add_qcirc(const QCirc& other)
{
  for (auto& qgate:other.qgates()) {
    add_qgate(qgate);
  }
  return *this;
}

/** = example =
  * qc.add_random(5, 10, {{"X", 1},
  *                       {"H", 1},
  *                       {"Sdg", 1},
  *                       {"CX", 3},
  *                       {"CZ", 2},
  *                       {"RZ(1/4)", 1},});
  */
Sharq::QCirc& Sharq::QCirc::add_random(const uint32_t qubit_num, const uint32_t qgate_num,
				       const nlohmann::json& probs)
{
  if (qubit_num == 0 || qgate_num == 0) {
    throw std::runtime_error("qubit_num is zero, or qgate_num is zero.");
  }

  /* total probability */
  double total_prob = 0.0;
  for (auto it = probs.begin(); it != probs.end(); ++it) {
    total_prob += (double)it.value();
  }

  /* read probs */
  std::vector<Sharq::QGateKind> kind_list;
  std::vector<double> prob_list;
  std::vector<Sharq::Phase> phase_list;
  double prob_tmp = 0.0;
  if (probs.size() == 0) {
    throw std::runtime_error("no probabilities are specified.");
  }
  for (auto it = probs.begin(); it != probs.end(); ++it) {
    std::string str = it.key();
    std::tuple<Sharq::QGateKind, Sharq::Phase> kp = Sharq::QGate::kind_phase(str);
    kind_list.push_back(std::get<0>(kp));
    phase_list.push_back(std::get<1>(kp));
    prob_tmp += (double)it.value() / total_prob;
    prob_list.push_back(prob_tmp);
    if (std::get<0>(kp) == Sharq::QGateKind::CX && qubit_num == 1) {
      throw std::runtime_error("not allowed CX gate for qubit_num = 1.");
    }
    if (std::get<0>(kp) == Sharq::QGateKind::CZ && qubit_num == 1) {
      throw std::runtime_error("not allowed CZ gate for qubit_num = 1.");
    }
  }
  uint32_t prob_list_num = prob_list.size();
  if (prob_list[prob_list_num - 1] != 1.0) prob_list[prob_list_num - 1];

  /* add random qgate */
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_real_distribution<double> uni(0,1);

  std::vector<uint32_t> q_list;
  for (uint32_t i = 0; i < qgate_num; ++i) {
    q_list.push_back(i % qubit_num);
  }
  std::shuffle(q_list.begin(), q_list.end(), engine);

  for (uint32_t i = 0; i < qgate_num; ++i) {
    /* prob_idx */
    uint32_t prob_idx = 0;
    for (uint32_t j = 0; j < prob_list_num; ++j) {
      double r = uni(engine);
      if (r < prob_list[j]) {
	prob_idx = j;
	break;
      }
    }
    /* kind, qid, phase */
    Sharq::QGateKind kind = kind_list[prob_idx];
    Sharq::Phase phase = phase_list[prob_idx];
    std::vector<uint32_t> qid = {q_list[i]};
    if (kind_list[prob_idx] == Sharq::QGateKind::CX || kind_list[prob_idx] == Sharq::QGateKind::CZ) {
      uint32_t t = (qid[0] + 1 + engine() % (qubit_num - 1)) % qubit_num;
      qid.push_back(t);
    }
    add_qgate(kind, qid, phase);
  }
  return *this;
}

Sharq::QCirc Sharq::QCirc::reverse() const
{
  Sharq::QCirc qc;
  uint32_t qgate_num = qgates_.size();
  for (uint32_t i = 0; i < qgate_num; ++i) {
    qc.add_qgate(qgates_[qgate_num - i - 1]);
  }
  return qc;
}

Sharq::QCirc Sharq::QCirc::inverse() const
{
  Sharq::QCirc qc;
  uint32_t qgate_num = qgates_.size();
  for (uint32_t i = 0; i < qgate_num; ++i) {
    qc.add_qgate(qgates_[qgate_num - i - 1].inverse());
  }
  return qc;
}

bool Sharq::QCirc::is_identical(const QCirc& other) const
{
  uint32_t qgate_num = qgates_.size();
  for (uint32_t i = 0; i < qgate_num; ++i) {
    if (qgates_[i].is_identical(other.qgates()[i]) == false) return false;
  }
  return true;
}

bool Sharq::QCirc::is_equal(const QCirc& other) const
{
  Sharq::LinearMap lmap = to_linearmap();
  Sharq::LinearMap lmap_other = other.inverse().to_linearmap();
  if (qubit_num() != other.qubit_num()) return false;
  lmap *= lmap_other;
  if (lmap.is_identity_multiple_constant()) return true;
  else return false;
}

void Sharq::QCirc::to_dot_file(const std::string& file_name) const
{
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
  ofs << "    style = \"filled\"," << std::endl;
  ofs << "    fontsize = 12," << std::endl;
  ofs << "    fontname = \"Migu 1M\"," << std::endl;
  ofs << "    fixedsize = true," << std::endl;
  ofs << "    ];" << std::endl;
  ofs << "  edge [" << std::endl;
  ofs << "    arrowhead = normal," << std::endl; 
  ofs << "    ];" << std::endl;

  ofs << std::endl;

  /* node definition */

  std::vector<std::string> nodes;
  std::vector<std::string> lasts(qubit_num_);
  std::vector<std::tuple<std::string, std::string>> edges;
  std::vector<std::tuple<std::string, std::string>> same_ranks;

  std::string label = "";
  std::string shape = "";
  std::string width = "";
  std::string height = "";
  std::string color = "";
  std::string fillcolor = "";
  std::string fontcolor = "";
  
  /* inputs */
  for (uint32_t i = 0; i < qubit_num_; ++i) {
    label = "q_" + std::to_string(i);;
    shape = "box";
    width = "0.5";
    height = "0.3";
    color = "white";
    fillcolor = "white";
    fontcolor = "black";

    ofs << "  node_" << i << " [label = " << "\"" << label << "\"";
    ofs << ", shape = " << shape;
    ofs << ", width = " << width << ", height = " << height << ", color = " << color;
    ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;

    lasts[i] = "node_" + std::to_string(i);
    nodes.push_back("node_" + std::to_string(i));
    if (i == 0) continue;
    edges.push_back({"node_"+ std::to_string(i - 1), "node_"+ std::to_string(i)});
  }
  
  /* outputs */
  for (uint32_t i = 0; i < qubit_num_; ++i) {
    label = "";
    shape = "box";
    width = "0.5";
    height = "0.3";
    color = "white";
    fillcolor = "white";
    fontcolor = "black";

    ofs << "  node_" << i + qubit_num_ << " [label = " << "\"" << label << "\"";
    ofs << ", shape = " << shape;
    ofs << ", width = " << width << ", height = " << height << ", color = " << color;
    ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;

    nodes.push_back("node_" + std::to_string(i + qubit_num_));
    if (i == 0) continue;
    edges.push_back({"node_"+ std::to_string(qubit_num_ + i - 1), "node_"+ std::to_string(qubit_num_ + i)});
  }

  uint32_t cnt = 0;
  for (uint32_t i = 0; i < qgates_.size(); ++i) {
    uint32_t offset = 2 * qubit_num_;
    Sharq::QGate qgate = qgates_[i];
    if ((qgate.kind() != Sharq::QGateKind::CX) && (qgate.kind() != Sharq::QGateKind::CZ)) {
      label = qgate.name();
      shape = "box";
      width = "0.5";
      height = "0.3";
      color = "black";
      fillcolor = "white";
      fontcolor = "black";
      if (qgate.kind() == Sharq::QGateKind::RZ) width = "0.8";

      edges.push_back({lasts[qgate.qid()[0]], "node_"+ std::to_string(cnt + offset)});
      lasts[qgate.qid()[0]] = "node_"+ std::to_string(cnt + offset);
      nodes.push_back("node_" + std::to_string(cnt + offset));

      ofs << "  node_" << cnt + offset << " [label = " << "\"" << label << "\"";
      ofs << ", shape = " << shape;
      ofs << ", width = " << width << ", height = " << height << ", color = " << color;
      ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;
    }
    else {
      /* control */
      label = "";
      shape = "circle";
      width = "0.1";
      height = "0.1";
      color = "black";
      fillcolor = "black";
      fontcolor = "black";

      nodes.push_back("node_" + std::to_string(cnt + offset));
      edges.push_back({lasts[qgate.qid()[0]], "node_"+ std::to_string(cnt + offset)});
      lasts[qgate.qid()[0]] = "node_"+ std::to_string(cnt + offset);

      ofs << "  node_" << cnt + offset << " [label = " << "\"" << label << "\"";
      ofs << ", shape = " << shape;
      ofs << ", width = " << width << ", height = " << height << ", color = " << color;
      ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;
      ++cnt;
      
      /* target */
      if (qgate.kind() == Sharq::QGateKind::CX) {
	label = "+";
	shape = "circle";
	width = "0.2";
	height = "0.2";
	color = "black";
	fillcolor = "white";
	fontcolor = "black";
      }
      else {
	label = "";
	shape = "circle";
	width = "0.1";
	height = "0.1";
	color = "black";
	fillcolor = "black";
	fontcolor = "black";
      }

      nodes.push_back("node_" + std::to_string(cnt + offset));
      edges.push_back({lasts[qgate.qid()[1]], "node_"+ std::to_string(cnt + offset)});
      lasts[qgate.qid()[1]] = "node_"+ std::to_string(cnt + offset);

      ofs << "  node_" << cnt + offset << " [label = " << "\"" << label << "\"";
      ofs << ", shape = " << shape;
      ofs << ", width = " << width << ", height = " << height << ", color = " << color;
      ofs << ", fillcolor = " << fillcolor << ", fontcolor = " << fontcolor << "];" << std::endl;

      if (qgate.qid()[0] < qgate.qid()[1]) {
	edges.push_back({"node_"+ std::to_string(cnt + offset - 1), "node_"+ std::to_string(cnt + offset)});
      }
      else { 
	edges.push_back({"node_"+ std::to_string(cnt + offset), "node_"+ std::to_string(cnt + offset - 1)});
      }
      same_ranks.push_back({"node_"+ std::to_string(cnt + offset - 1), "node_"+ std::to_string(cnt + offset)});
    }
    ++cnt;
  }
  for (uint32_t i = 0; i < qubit_num_; ++i) {
    edges.push_back({lasts[i], "node_" + std::to_string(qubit_num_ + i)});
  }
  ofs << std::endl;

  /* edge definition */

  for (uint32_t i = 0; i < edges.size(); ++i) {
    std::string style = "solid";
    std::string color = "black";
    if (i < 2 * (qubit_num_ - 1)) color = "white";
    ofs << "  " << std::get<0>(edges[i]) << " -- " << std::get<1>(edges[i]);
    ofs << " [style = " << style << ", color = " << color << "];" << std::endl;
  }
  ofs << std::endl;

  /* rank definition */

  ofs << "  {rank = min; ";
  for (uint32_t i = 0; i < qubit_num_; ++i) {
    ofs << nodes[i] << "; ";
  }
  ofs << "}" << std::endl;

  ofs << "  {rank = max; ";
  for (uint32_t i = 0; i < qubit_num_; ++i) {
    ofs << nodes[i + qubit_num_] << "; ";
  }
  ofs << "}" << std::endl;

  for (auto& same:same_ranks) {
    ofs << "  {rank = same; " << std::get<0>(same) << "; " << std::get<1>(same) << " }" << std::endl;
  }
  
  ofs << "}" << std::endl;

  ofs.close();
}

void Sharq::QCirc::to_svg_file(const std::string& file_name) const
{
  std::string tmpfile_name = std::filesystem::temp_directory_path().native() + "/sharq_qcirc_to_svg_file.dot";
  to_dot_file(tmpfile_name);
  std::string com = "dot -Kdot -Tsvg " + tmpfile_name + " -o" + file_name + "; rm " + tmpfile_name;
  const char* com_char = com.c_str();
  int ret = system(com_char);
  if (ret != 0) {
    throw std::runtime_error("fail to execute dot command.");
  }
}

Sharq::ZXDiagram Sharq::QCirc::to_zxdiagram() const
{
  ZXDiagram zx(qubit_num_);
  for (auto qgate:qgates_) {
    if (qgate.is_CZ_gate()) {
      uint32_t c = qgate.qid()[0];
      uint32_t t = qgate.qid()[1];
      zx.add_qgate(Sharq::QGate(Sharq::QGateKind::H, {t}));
      zx.add_qgate(Sharq::QGate(Sharq::QGateKind::CX, {c,t}));
      zx.add_qgate(Sharq::QGate(Sharq::QGateKind::H, {t}));
    }
    else {
      zx.add_qgate(qgate);
    }
  }
  return zx;
}

Sharq::LinearMap Sharq::QCirc::to_linearmap() const
{
  Sharq::LinearMap lmap(1<<qubit_num_, 1<<qubit_num_);
  for (auto qgate:qgates_) {
    lmap.operate_qgate(qgate);
  }
  return lmap;
}

Sharq::DAGCirc Sharq::QCirc::to_dagcirc() const
{
  Sharq::DAGCirc dc(*this);
  return dc;
}

void Sharq::QCirc::gate_cancel_one_time()
{
  std::vector<Sharq::QGate> qgates_out;

  for (auto& qgate:qgates_) {

    bool merge = false;
    uint32_t pos = 0;
    for (int32_t i = qgates_out.size() - 1; i >= 0; --i) {
      if (qgates_out[i].mergeable(qgate)) {
	merge = true;
	pos = i;
	break;
      }
      else if (qgates_out[i].commutable(qgate)) {
	continue;
      }
      else {
	merge = false;
	break;
      }
    }

    if (merge) qgates_out[pos].merge(qgate);
    else qgates_out.push_back(qgate);
  }

  /* remove Id gates */
  uint32_t gnum_last_qubit = 0;
  for (auto it = qgates_out.begin(); it != qgates_out.end(); ++it) {
    if (it->kind() == Sharq::QGateKind::Id) {
      it == qgates_out.erase(it);
      --it;
    }
    else if (it->qid().size() == 1 && it->qid()[0] == qubit_num_-1) {
      ++gnum_last_qubit;
    }
    else if (it->qid().size() == 2 && (it->qid()[0] == qubit_num_-1 || it->qid()[1] == qubit_num_-1)) {
      ++gnum_last_qubit;
    }
  }
  if (gnum_last_qubit == 0) {
    qgates_out.push_back(Sharq::QGate(Sharq::QGateKind::Id, {qubit_num_-1}));
  }
  
  qgates(qgates_out);
}

void Sharq::QCirc::gate_cancel()
{
  while (true) {
    uint32_t qgate_num = qgates_.size();
    gate_cancel_one_time();
    if (qgates_.size() == qgate_num) break;
  }
}
