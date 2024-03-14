/**
 * @file optimizer.h
 * @brief include file of Optimizer class
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <string>
#include <chrono>

#include "qcirc.h"
#include "zx.h"

namespace Sharqit {

  //! kind of optimizer
  enum OptimizerKind {
		      ZXCalculus, //!< using ZX-calculus
		      PhasePolynomial //! using Phase Polynomial
  };

  /**
   * @class Optimizer
   * @brief quantum circuit optimizer
  */
  class Optimizer
  {
  private:
    OptimizerKind kind_; //!< kind of optimization method
    double proc_time_; //!< processing time
    std::map<std::string, uint32_t> stats_in_; //!< stats of the circuit before optimization
    std::map<std::string, uint32_t> stats_out_; //!< stats of the circuit after optimization
    std::map<std::string, uint32_t> zx_stats_in_; //!< stats of the zx-diagram before simplification
    std::map<std::string, uint32_t> zx_stats_out_; //!< stats of the zx-diagram after simplification
  public:
    //! getter of the kind_
    OptimizerKind kind() const { return kind_; }
    //! getter of the proc_time_
    double proc_time() const { return proc_time_; }
    //! getter of the proc_time_
    double get_proc_time() const { return proc_time_; }
    //! getter of the stats_in_
    std::map<std::string, uint32_t> stats_in() const { return stats_in_; }    
    //! getter of the stats_out_
    std::map<std::string, uint32_t> stats_out() const { return stats_out_; }    
    //! getter of the zx_stats_in_
    std::map<std::string, uint32_t> zx_stats_in() const { return zx_stats_in_; }    
    //! getter of the zx_stats_out_
    std::map<std::string, uint32_t> zx_stats_out() const { return zx_stats_out_; }    
    //! setter of the kind_
    void kind(const OptimizerKind kind) { kind_ = kind; }
    //! setter of the proc_time_
    void proc_time(const double proc_time) { proc_time_ = proc_time; }
    //! setter of the stats_in_
    void stats_in(const std::map<std::string, uint32_t>& stats_in) { stats_in_ = stats_in; }
    //! setter of the stats_out_
    void stats_out(const std::map<std::string, uint32_t>& stats_out) { stats_out_ = stats_out; }
    //! setter of the zx_stats_n_
    void zx_stats_in(const std::map<std::string, uint32_t>& zx_stats_in) { zx_stats_in_ = zx_stats_in; }
    //! setter of the zx_stats_out_
    void zx_stats_out(const std::map<std::string, uint32_t>& zx_stats_out) { zx_stats_out_ = zx_stats_out; }
    /**
     * @brief get a string of the optimizer object
     * @return string of the optimizer
     */
    std::string to_string() const;
    /**
     * @brief get the string of the optimization method
     * @return string of the optimization method
     */
    std::string name() const;
    /**
     * @brief show the optimizer object
     */
    void show() const { std::cout << to_string() << std::endl; }
    /**
     * @brief execute the quantum circuit optimization (reduction of gate-count using ZXCalculus)
     * @param [in] qc_in input quantum circuit
     * @return output quantum circuit
     */
    QCirc reduce_gates_using_zx(const QCirc& qc_in);
    /**
     * @brief execute the quantum circuit optimization (reduction of gate-count using PhasePolynomial)
     * @param [in] qc_in input quantum circuit
     * @return output quantum circuit
     */
    QCirc reduce_gates_using_pp(const QCirc& qc_in);
    /**
     * @brief execute the quantum circuit optimization (reduction of gate-count)
     * @param [in] qc_in input quantum circuit
     * @param [in] method optimization method ("zx" or "pp")
     * @return output quantum circuit
     */
    QCirc reduce_gates(const QCirc& qc_in, const std::string& method);
    /**
     * @brief execute the quantum circuit optimization
     * @param [in] qc_in input quantum circuit
     * @param [in] kind optimization method
     * @return output quantum circuit
     * @note This method will be removed in near future. You should use reduce_gates method instead.
     */
    QCirc execute(const QCirc& qc_in, const OptimizerKind kind = OptimizerKind::ZXCalculus);
    friend std::ostream& operator<<(std::ostream& ost, const Optimizer& opt) { ost << opt.to_string(); return ost; }
   };
}

#endif
