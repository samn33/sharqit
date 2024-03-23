/**
 * @file nb_sharqit.cpp
 * @brief definition of python-wrapper using nanobind
 */

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/set.h>
#include <nanobind/stl/list.h>

namespace nb = nanobind;

#include "sharqit.h"

NB_MODULE(sharqit_base, m) {
  nb::class_<Sharqit::Phase>(m, "Phase")
    .def(nb::init<const int32_t>())
    .def(nb::init<const int32_t, const int32_t>())
    .def(nb::init<const std::string&>())
    .def("to_string", &Sharqit::Phase::to_string, nb::arg("pi_str") = false)
    .def("__str__", &Sharqit::Phase::to_string, nb::arg("pi_str") = false)
    .def(nb::self + nb::self)
    .def(nb::self - nb::self)
    .def(nb::self += nb::self)
    .def(nb::self -= nb::self)
    .def(nb::self *= int32_t())
    .def(nb::self /= int32_t())
    .def(int() * nb::self)
    .def(nb::self * int())
    .def(nb::self / int())
    ;
  nb::enum_<Sharqit::QGateKind>(m, "QGateKind")
    .value("X", Sharqit::QGateKind::X)
    .value("Z", Sharqit::QGateKind::Z)
    .value("S", Sharqit::QGateKind::S)
    .value("Sdg", Sharqit::QGateKind::Sdg)
    .value("T", Sharqit::QGateKind::T)
    .value("Tdg", Sharqit::QGateKind::Tdg)
    .value("H", Sharqit::QGateKind::H)
    .value("RZ", Sharqit::QGateKind::RZ)
    .value("CX", Sharqit::QGateKind::CX)
    .value("CZ", Sharqit::QGateKind::CZ)
    .value("Id", Sharqit::QGateKind::Id)
    .value("Id2", Sharqit::QGateKind::Id2)
    .export_values();
  nb::class_<Sharqit::QGate>(m, "QGate")
    .def(nb::init<const Sharqit::QGateKind, const std::vector<uint32_t>&>())
    .def(nb::init<const Sharqit::QGateKind, const std::vector<uint32_t>&, const Sharqit::Phase&>())
    .def(nb::init<const std::string&>())
    .def("to_string", &Sharqit::QGate::to_string, nb::arg("pi_str") = false)
    .def("__str__", &Sharqit::QGate::to_string, nb::arg("pi_str") = false)
    .def("name", &Sharqit::QGate::name, nb::arg("pi_str") = false)
    .def_prop_rw("kind",
		 [](Sharqit::QGate& qgate) { return qgate.kind() ; },
		 [](Sharqit::QGate& qgate, Sharqit::QGateKind kind) { qgate.kind(kind) ; })
    .def_prop_rw("qid",
		 [](Sharqit::QGate& qgate) { return qgate.qid() ; },
		 [](Sharqit::QGate& qgate, std::vector<uint32_t>& qid) { qgate.qid(qid) ; })
    .def_prop_rw("phase",
		 [](Sharqit::QGate& qgate) { return qgate.phase() ; },
		 [](Sharqit::QGate& qgate, Sharqit::Phase& phase) { qgate.phase(phase) ; })
     ;
  nb::class_<Sharqit::QCirc>(m, "QCirc")
    .def(nb::init<>())
    .def("to_string", &Sharqit::QCirc::to_string, nb::arg("width") = 100)
    .def("__str__", &Sharqit::QCirc::to_string, nb::arg("width") = 100)
    .def("show", &Sharqit::QCirc::show, nb::arg("width") = 100)
    .def("print_stats", &Sharqit::QCirc::print_stats)
    .def("is_equal", &Sharqit::QCirc::is_equal)
    .def("save", &Sharqit::QCirc::save)
    .def("load", &Sharqit::QCirc::load)
    .def("id", &Sharqit::QCirc::id)
    .def("x", &Sharqit::QCirc::x)
    .def("z", &Sharqit::QCirc::z)
    .def("s", &Sharqit::QCirc::s)
    .def("sdg", &Sharqit::QCirc::sdg)
    .def("t", &Sharqit::QCirc::t)
    .def("tdg", &Sharqit::QCirc::tdg)
    .def("h", &Sharqit::QCirc::h)
    .def("rz", &Sharqit::QCirc::rz)
    .def("id2", &Sharqit::QCirc::id2)
    .def("cx", &Sharqit::QCirc::cx)
    .def("cz", &Sharqit::QCirc::cz)
    .def("rx", &Sharqit::QCirc::rx)
    .def("y", &Sharqit::QCirc::y)
    .def("sx", &Sharqit::QCirc::sx)
    .def("sxdg", &Sharqit::QCirc::sxdg)
    .def("ry", &Sharqit::QCirc::ry)
    .def("p", &Sharqit::QCirc::p)
    .def("cy", &Sharqit::QCirc::cy)
    .def("csx", &Sharqit::QCirc::csx)
    .def("csxdg", &Sharqit::QCirc::csxdg)
    .def("ch", &Sharqit::QCirc::ch)
    .def("cs", &Sharqit::QCirc::cs)
    .def("csdg", &Sharqit::QCirc::csdg)
    .def("ct", &Sharqit::QCirc::ct)
    .def("ctdg", &Sharqit::QCirc::ctdg)
    .def("cs", &Sharqit::QCirc::cs)
    .def("crx", &Sharqit::QCirc::crx)
    .def("cry", &Sharqit::QCirc::cry)
    .def("crz", &Sharqit::QCirc::crz)
    .def("cp", &Sharqit::QCirc::cp)
    .def("rxx", &Sharqit::QCirc::rxx)
    .def("ryy", &Sharqit::QCirc::ryy)
    .def("rzz", &Sharqit::QCirc::rzz)
    .def("sw", &Sharqit::QCirc::sw)
    .def("csw", &Sharqit::QCirc::csw)
    .def("ccx", &Sharqit::QCirc::ccx)
    .def("x_count", &Sharqit::QCirc::x_count)
    .def("z_count", &Sharqit::QCirc::z_count)
    .def("h_count", &Sharqit::QCirc::h_count)
    .def("s_count", &Sharqit::QCirc::s_count)
    .def("t_count", &Sharqit::QCirc::t_count)
    .def("rz_count", &Sharqit::QCirc::rz_count)
    .def("cx_count", &Sharqit::QCirc::cx_count)
    .def("twoq_count", &Sharqit::QCirc::twoq_count)
    .def("gate_count", &Sharqit::QCirc::gate_count)
    .def("depth", &Sharqit::QCirc::depth)
    .def("qubit_num", [](Sharqit::QCirc& qc) { return qc.qubit_num(); })
    .def("add_random", &Sharqit::QCirc::add_random_str)
    .def("add_qgate", nb::overload_cast<const std::string&>(&Sharqit::QCirc::add_qgate))
    .def("add_qgate", nb::overload_cast<const Sharqit::QGate&>(&Sharqit::QCirc::add_qgate))
    .def("add_qgate", nb::overload_cast<const Sharqit::QGateKind, const std::vector<uint32_t>&,
	 const Sharqit::Phase&>(&Sharqit::QCirc::add_qgate))
    .def("get_qgate", &Sharqit::QCirc::get_qgate)
    .def("get_qgates", &Sharqit::QCirc::get_qgates)
    ;
  nb::class_<Sharqit::Optimizer>(m, "Optimizer")
    .def(nb::init<>())
    .def("proc_time", &Sharqit::Optimizer::get_proc_time)
    .def("reduce_gates", &Sharqit::Optimizer::reduce_gates)
    ;
}
