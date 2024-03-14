/**
 * @file merge_rotation.cpp
 * @brief member functions of QCirc class related to the algorithm to propagate pauli X gates
 */

#include "qcirc.h"

/* propagate pauli x gates after cnot gates */
void Sharqit::QCirc::propagate_pauli_x()
{
  /* propagate X gates */
  std::list<QGate> qgates_list(qgates_.begin(), qgates_.end());
  uint32_t q = 0;
  std::vector<uint32_t> qid_buf;
  auto it_x = qgates_list.begin();
  while (true) {
    qid_buf.clear();
    bool x_find = false; // find X gate or not
    bool x_prop = false; // propagate X gate or not
    for (auto it = it_x; it != qgates_list.end(); ++it) {
      if (!x_find && it->is_X_gate()) {
	it_x = it;
	q = it->qid()[0];
	x_find = true;
      }
      else if (x_find && it->is_X_gate() && it->qid()[0] == q) {
	it_x->kind(Sharqit::QGateKind::Id);
	it->kind(Sharqit::QGateKind::Id);
	++it_x;
	x_prop = true;
	break;
      }
      else if (x_find && it->is_RZ_gate() && it->qid()[0] == q) {
	it_x->kind(Sharqit::QGateKind::RZ);
	it_x->phase(-it->phase());
	it->kind(Sharqit::QGateKind::X);
	it->phase(Sharqit::Phase(0));
	++it_x;
	x_prop = true;
	break;
      }
      else if (x_find && it->is_CX_gate() && it->qid()[0] == q &&
	       std::find(qid_buf.begin(), qid_buf.end(), it->qid()[0]) == qid_buf.end()) { // propagate
	uint32_t con = it->qid()[0];
	uint32_t tar = it->qid()[1];
	it_x->kind(Sharqit::QGateKind::Id);
	it_x->qid({q});
	++it;
	it = qgates_list.insert(it, Sharqit::QGate(Sharqit::QGateKind::X, {con}));
	++it;
	it = qgates_list.insert(it, Sharqit::QGate(Sharqit::QGateKind::X, {tar}));
	--it;
	++it_x;
	x_prop = true;
	break;
      }
      else if (x_find && it->is_CX_gate() && it->qid()[1] == q &&
	       std::find(qid_buf.begin(), qid_buf.end(), it->qid()[1]) == qid_buf.end()) { // commute
	uint32_t tar = it->qid()[1];
	it_x->kind(Sharqit::QGateKind::Id);
	it_x->qid({q});
	++it;
	it = qgates_list.insert(it, Sharqit::QGate(Sharqit::QGateKind::X, {tar}));
	++it_x;
	x_prop = true;
	break;
      }
      else if (x_find && it->qid().size() == 1 && it->qid()[0] == q) {
	break;
      }
      else if (x_find && it->qid().size() == 2 && (it->qid()[0] == q || it->qid()[1] == q)) {
	break;
      }
      else if (x_find){
	for (auto& i:it->qid()) qid_buf.push_back(i);
      }
    }
    if (!x_prop) break;
  }

  /* remove RZ(0)'s and Id's */
  remove_id();
}
