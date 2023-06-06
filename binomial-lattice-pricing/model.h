#pragma once

#include <cmath>
#include <map>
#include <vector>

#include "binomial-lattice-pricing/Node.h"

namespace model {

// TODO: implement a real tree. I'm just copying the python code for now.
// typedef std::map<int, std::vector<node::Node>> DumbTree;

enum class Style {
  JR,
  JR_risk_neutral,
  CRR_classic,
  CRR_drift,

};

struct Model {
private:
  double _C_E{};
  double _P_E{};
  double _Delta_Call{};
  double _Delta_Put{};
  double _Gamma_Call{};
  double _Gamma_Put{};
  double _vega_Call{};
  double _vega_Put{};
  double _theta_Call{};
  double _theta_Put{};
  double _rho_Call{};
  double _rho_Put{};
  double _Psi_Call{};
  double _Psi_Put{};

public:
  double S{};
  double delta{};
  double r{};
  double sigma{};
  double K{};
  double T{};

  Model(double S, double delta, double r, double sigma, double K, double T);
  // DumbTree getRecombiningTree(int periods, Style style = Style::CRR_drift);

  double C_E() { return this->_C_E; };
  double P_E() { return this->_P_E; };
  double Delta_Call() { return this->_Delta_Call; };
  double Delta_Put() { return this->_Delta_Put; };
  double Gamma_Call() { return this->_Gamma_Call; };
  double Gamma_Put() { return this->_Gamma_Put; };
  double vega_Call() { return this->_vega_Call; };
  double vega_Put() { return this->_vega_Put; };
  double theta_Call() { return this->_theta_Call; };
  double theta_Put() { return this->_theta_Put; };
  double rho_Call() { return this->_rho_Call; };
  double rho_Put() { return this->_rho_Put; };
  double Psi_Call() { return this->_Psi_Call; };
  double Psi_Put() { return this->_Psi_Put; };
};

}  // namespace model
