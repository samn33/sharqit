#include "qcirc.h"

/**
 *  member functions
 */

//#define DEBUG
//#define SHOW

bool Sharq::QCirc::is_termination_border(const uint32_t idx, const uint32_t q)
{
  if (qgates_[idx].is_included(q) &&
      (!qgates_[idx].is_RZ_gate() && !qgates_[idx].is_X_gate() &&
       !qgates_[idx].is_CX_gate())) {
    return true;
  }
  return false;
}

void Sharq::QCirc::merge_rotation_one_time(const uint32_t start, const uint32_t end,
					   const std::vector<uint32_t>& ppc_qid,
					   const std::vector<std::pair<uint32_t, uint32_t>>& term_border,
					   const std::vector<uint8_t>& cnot_objects)
{
  std::vector<std::vector<uint8_t>> P;
  std::vector<uint32_t> rotation_index;
  std::vector<uint8_t> rotation_index_used;

  BinaryMatrix B(ppc_qid.size(), ppc_qid.size() + 1);
  B.set_identity();

  std::map<uint32_t, uint32_t> qid_map;
  for (uint32_t i = 0; i < ppc_qid.size(); ++i) {
    qid_map[ppc_qid[i]] = i;
  }

  std::vector<uint8_t> qid_flg(qubit_num(), 0);
  for (auto& q:ppc_qid) qid_flg[q] = 1;
#ifdef DEBUG
  std::cerr << "qid_flg = ";
  for (auto& x:qid_flg) std::cerr << std::to_string(x) << " ";
  std::cerr << std::endl;
#endif

  for (uint32_t i = start; i <= end; ++i) {
    if (qgates_[i].is_CX_gate() && cnot_objects[i] == 1) {
      uint32_t con = qgates_[i].qid()[0];
      uint32_t tar = qgates_[i].qid()[1];
      if ((term_border[con].first <= i && i <= term_border[con].second) &&
	  (term_border[tar].first <= i && i <= term_border[tar].second)) {
	B.xor_rows(qid_map[con], qid_map[tar]);
#ifdef DEBUG
	std::cerr << qgates_[i] << std::endl;
	std::cerr << B << std::endl;
#endif
      }
    }
    else if (qgates_[i].is_X_gate() && qid_flg[qgates_[i].qid()[0]] == 1) {
      uint32_t q = qgates_[i].qid()[0];
      if (term_border[q].first <= i && i <= term_border[q].second) {
	B.reverse_element(qid_map[q], ppc_qid.size());
#ifdef DEBUG
	std::cerr << qgates_[i] << std::endl;
	std::cerr << B << std::endl;
#endif
      }
    }
    else if (qgates_[i].is_RZ_gate() && qid_flg[qgates_[i].qid()[0]] == 1) {
      uint32_t q = qgates_[i].qid()[0];
      //if (term_border[q].first < i && i < term_border[q].second) {
      if (term_border[q].first <= i && i <= term_border[q].second) {
	P.push_back(B.elements()[qid_map[q]]);
	rotation_index.push_back(i);
	rotation_index_used.push_back(0);
#ifdef DEBUG
	std::cerr << qgates_[i] << std::endl;
	std::cerr << B << std::endl;
#endif
      }
    }
  }

  if (P.size() <= 1) return;

  for (uint32_t i = 0; i < P.size() - 1; ++i) {
    if (rotation_index_used[i] == 1) continue;
    uint32_t q_i = rotation_index[i];
    for (uint32_t j = i + 1; j < P.size(); ++j) {
      if (std::equal(P[i].cbegin(), P[i].cend(), P[j].cbegin())) {
	uint32_t q_j = rotation_index[j];
#ifdef DEBUG
	std::cerr << "qgates_[" << q_i << "] <- qgates_[" << q_j << "]" << std::endl; 
#endif
	Sharq::Phase phase = qgates_[q_i].phase() + qgates_[q_j].phase();
	Sharq::Phase zero(0);
	qgates_[q_i].phase(phase);
	qgates_[q_j].phase(zero);
	rotation_index_used[j] = 1;
      }
    }
  }
}

/* merge rotation gates using phase polynomial representation */
void Sharq::QCirc::merge_rotation()
{
  /* CZ to CX */
  cz_to_cx();

  uint32_t pos = 0;
  uint32_t start = 0; // start of phase polynomial circuit
  uint32_t end = 0; // end of phase polynomial circuit

  uint32_t cnot_idx = 0;
  uint32_t con = 0;
  uint32_t tar = 0;

  std::queue<uint32_t> cnot_anchor;
  std::vector<uint8_t> cnot_visits(qgate_num(), 0);
  std::vector<uint32_t> ppc_qid; // qid of the phase polynomial circuit
  std::vector<std::pair<uint32_t, uint32_t>> term_border(qubit_num());

  //while (end < qgate_num() - 1) {
  while (true) {

    start = 100000000;
    //start = std::numeric_limits<uint32_t>::max();
    end = 0;
    ppc_qid.clear();
    for (auto it = term_border.begin(); it != term_border.end(); ++it) {
      it->first = 100000000;
      //it->first = std::numeric_limits<uint32_t>::max();;
      it->second = 0;
    }

    /* find cnot after the starting position */
    bool cnot_find = false;
    for (uint32_t i = pos; i < qgate_num(); ++i) {
      if (qgates_[i].is_CX_gate() && cnot_visits[i] == 0) {
	cnot_anchor.push(i);
	cnot_visits[i] = 1;
	pos = i + 1;
	cnot_find = true;
	break;
      }
    }
    if (!cnot_find) break;

#ifdef DEBUG
    getchar();
    std::cerr << "=== start cycle ===" << std::endl;
#endif

    bool find = false;
    while (!cnot_anchor.empty()) {

      cnot_idx = cnot_anchor.front();
      cnot_anchor.pop();
      con = qgates_[cnot_idx].qid()[0];
      tar = qgates_[cnot_idx].qid()[1];

#ifdef DEBUG
      std::cerr << "cnot_idx = " << cnot_idx << std::endl;
      std::cerr << "con, tar = " << con << ", " << tar << std::endl;
#endif

      /* scan the control qubit, set the termination border */
      find = false;
      for (int32_t j = (int32_t)(cnot_idx - 1); j >= 0; --j) { // scan backwoard
	if (is_termination_border(j, con)) {
	  if ((int32_t)term_border[con].first > j) term_border[con].first = j;
	  if ((int32_t)start > j) start = j;
	  find = true;
	  break;
	}
      }
      if (!find) {
	term_border[con].first = 0;
	start = 0;
      }
      find = false;
      for (uint32_t j = cnot_idx + 1; j < qgate_num(); ++j) { // scan forwoard
	if (is_termination_border(j, con)) {
	  if (term_border[con].second < j) term_border[con].second = j;
	  if (end < j) end = j;
	  find = true;
	  break;
	}
      }
      if (!find) {
	term_border[con].second = qgate_num() - 1;
	end = qgate_num() - 1;
      }
      if (std::find(ppc_qid.begin(), ppc_qid.end(), con) == ppc_qid.end()) ppc_qid.push_back(con);

      /* scan the target qubit, set the termination border */
      find = false;
      for (int32_t j = (int32_t)(cnot_idx-1); j >= 0; --j) { // scan backwoard
	if (is_termination_border(j, tar)) {
	  if ((int32_t)term_border[tar].first > j) term_border[tar].first = j;
	  if ((int32_t)start > j) start = j;
	  find = true;
	  break;
	}
      }
      if (!find) {
	term_border[tar].first = 0;
	start = 0;
      }
      find = false;
      for (uint32_t j = cnot_idx + 1; j < qgate_num(); ++j) { // scan forwoard
	if (is_termination_border(j, tar)) {
	  if (term_border[tar].second < j) term_border[tar].second = j;
	  if (end < j) end = j;
	  find = true;
	  break;
	}
      }
      if (!find) {
	term_border[tar].second = qgate_num() - 1;
	end = qgate_num() - 1;
      }
      if (std::find(ppc_qid.begin(), ppc_qid.end(), tar) == ppc_qid.end()) ppc_qid.push_back(tar);

#ifdef DEBUG
      std::cerr << "ppc_qid = ";
      for (auto& q:ppc_qid) { std::cerr << q << " "; }
      std::cerr << std::endl;
      for (uint32_t i = 0; i < ppc_qid.size(); ++i) {
	std::cerr << "term_border["<< ppc_qid[i] << "] = "
		  << term_border[ppc_qid[i]].first << ", "
		  << term_border[ppc_qid[i]].second << std::endl;
      }
      std::cerr << "start, end = " << start << ", " << end << std::endl;
#endif

      /* add cnot anchors */
      for (int32_t j = (int32_t)(cnot_idx - 1); j >= (int32_t)start; --j) { // scan backwoard
	if (qgates_[j].is_CX_gate() && cnot_visits[j] == 0) {

	  uint32_t q0 = qgates_[j].qid()[0];
	  uint32_t q1 = qgates_[j].qid()[1];
	  if ((((int32_t)term_border[q0].first <= j) && (j <= (int32_t)term_border[q0].second)) ||
	      ((int32_t)term_border[q1].first <= j && (j <= (int32_t)term_border[q1].second))) {

	    if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[0]) == ppc_qid.end() &&
		std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[1]) == ppc_qid.end()) continue; 
	    else {
	      cnot_visits[j] = 1;
	      if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[0]) == ppc_qid.end()) {
		ppc_qid.push_back(qgates_[j].qid()[0]);
		cnot_anchor.push(j);
	      }
	      else if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[1]) == ppc_qid.end()) {
		ppc_qid.push_back(qgates_[j].qid()[1]);
		cnot_anchor.push(j);
	      }
	    }
	  }
	}
      }

      for (uint32_t j = cnot_idx + 1; j <= end; ++j) { // scan forward
	if (qgates_[j].is_CX_gate() && cnot_visits[j] == 0) {

	  uint32_t q0 = qgates_[j].qid()[0];
	  uint32_t q1 = qgates_[j].qid()[1];
	  if (((term_border[q0].first <= j) && (j <= term_border[q0].second)) ||
	      (term_border[q1].first <= j && (j <= term_border[q1].second))) {
	    if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[0]) == ppc_qid.end() &&
		std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[1]) == ppc_qid.end()) continue; 
	    else {
	      cnot_visits[j] = 1;
	      if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[0]) == ppc_qid.end()) {
		ppc_qid.push_back(qgates_[j].qid()[0]);
		cnot_anchor.push(j);
	      }
	      else if (std::find(ppc_qid.begin(), ppc_qid.end(), qgates_[j].qid()[1]) == ppc_qid.end()) {
		ppc_qid.push_back(qgates_[j].qid()[1]);
		cnot_anchor.push(j);
	      }
	    }
	  }
	}
      }

#ifdef DEBUG
      std::queue<uint32_t> tmp = cnot_anchor;
      std::cerr << "cnot_anchor:";
      while (!tmp.empty()) {
	std::cerr << tmp.front() << " ";
	tmp.pop();
      }
      std::cerr << std::endl;
#endif
    }

#ifdef DEBUG
    std::cerr << "=== done ===" << std::endl;
    std::cerr << "start, end = " << start << ", " << end << std::endl;
    std::cerr << "subcircuit = " << start << ", " << end << std::endl;
#endif
    
    /* rules of removing cnot and changing termination borders */
    std::vector<uint8_t> cnot_objects = cnot_visits;
    for (uint32_t i = start; i <= end; ++i) {
      if (qgates_[i].is_CX_gate() && cnot_visits[i] == 1) {
	con = qgates_[i].qid()[0];
	tar = qgates_[i].qid()[1];
	if ((term_border[con].first <= i) && (i <= term_border[con].second) &&
	    (term_border[tar].first <= i) && (i <= term_border[tar].second)) {
	  continue;
	}
	else if ((term_border[con].first <= i) && (i <= term_border[con].second)) {
	  cnot_objects[i] = 0;
	}
	else if ((term_border[tar].first <= i) && (i <= term_border[tar].second)) {
	  term_border[tar].second = i;
	}
      }
    }

#ifdef DEBUG
    std::cerr << "=== after applying the rule ===" << std::endl;
    for (uint32_t i = 0; i < ppc_qid.size(); ++i) {
      std::cerr << "term_border["<< ppc_qid[i] << "] = "
		<< term_border[ppc_qid[i]].first << ", "
		<< term_border[ppc_qid[i]].second << std::endl;
    }
    std::cerr << "start, end = " << start << ", " << end << std::endl;
    std::cerr << "cnot_visit =";
    for (auto& x:cnot_visits) std::cerr << std::to_string(x) << " ";
    std::cerr << std::endl;
    std::cerr << "cnot_objects =";
    for (auto& x:cnot_objects) std::cerr << std::to_string(x) << " ";
    std::cerr << std::endl;
#endif

    if (start >= end) break;

    /* execute merging rotation gates */
#ifdef DEBUG
    std::cerr << "=== merge rotation one time ===" << std::endl;
#endif
    merge_rotation_one_time(start, end, ppc_qid, term_border, cnot_objects);
  }


#ifdef SHOW
  std::cerr << to_string() << std::endl;
  std::cerr << std::endl;
#endif

  uint32_t maxq = 0;
  for (auto& qgate:qgates_) {
    for (auto& q:qgate.qid()) {
      if (q > maxq) maxq = q;
    }
  }
  if (maxq < qubit_num_ - 1) id(qubit_num_ - 1); 

#ifdef SHOW
  std::cerr << qc.to_string() << std::endl;
  std::cerr << std::endl;
#endif

  /* remove RZ(0)'s and Id's */
  remove_id();
}
