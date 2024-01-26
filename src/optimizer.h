#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <string>
#include <chrono>

#include "qcirc.h"
#include "zx.h"

namespace Sharq {

  enum OptimizerKind { ZXCalculus, PhasePolynomial };

  class Optimizer
  {
  private:
    OptimizerKind kind_;
    double proc_time_;
    std::map<std::string, uint32_t> stats_in_;
    std::map<std::string, uint32_t> stats_out_;
    std::map<std::string, uint32_t> zx_stats_in_;
    std::map<std::string, uint32_t> zx_stats_out_;
  public:
    /* getters */
    OptimizerKind kind() const { return kind_; }
    double proc_time() const { return proc_time_; }
    std::map<std::string, uint32_t> stats_in() const { return stats_in_; }    
    std::map<std::string, uint32_t> stats_out() const { return stats_out_; }    
    std::map<std::string, uint32_t> zx_stats_in() const { return zx_stats_in_; }    
    std::map<std::string, uint32_t> zx_stats_out() const { return zx_stats_out_; }    
    /* setters */
    void kind(const OptimizerKind kind) { kind_ = kind; }
    void proc_time(const double proc_time) { proc_time_ = proc_time; }
    void stats_in(const std::map<std::string, uint32_t>& stats_in) { stats_in_ = stats_in; }
    void stats_out(const std::map<std::string, uint32_t>& stats_out) { stats_out_ = stats_out; }
    void zx_stats_in(const std::map<std::string, uint32_t>& zx_stats_in) { zx_stats_in_ = zx_stats_in; }
    void zx_stats_out(const std::map<std::string, uint32_t>& zx_stats_out) { zx_stats_out_ = zx_stats_out; }
    /* member functions */
    std::string to_string() const;
    std::string name() const;
    void show() const { std::cout << to_string() << std::endl; }
    QCirc execute(const QCirc& qc_in, const OptimizerKind kind = OptimizerKind::ZXCalculus);
    //QCirc execute(const QCirc& qc_in, const OptimizerKind kind = OptimizerKind::PhasePolynomial);
    friend std::ostream& operator<<(std::ostream& ost, const Optimizer& opt) { ost << opt.to_string(); return ost; }
   };
}

#endif
