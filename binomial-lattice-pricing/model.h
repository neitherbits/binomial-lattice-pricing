#pragma once

#include <cmath>
#include <map>
#include <vector>

#include "binomial-lattice-pricing/Node.h"

namespace model {

// TODO: implement a real tree. I'm just copying the python code for now.
typedef std::map<int, std::vector<node::Node>> DumbTree;

enum class Style {
  JR,
  JR_risk_neutral,
  CRR_classic,
  CRR_drift,

};

struct Model {
private:
  float _C_E{};
  float _P_E{};
  float _Delta_Call{};
  float _Delta_Put{};
  float _Gamma_Call{};
  float _Gamma_Put{};
  float _vega_Call{};
  float _vega_Put{};
  float _theta_Call{};
  float _theta_Put{};
  float _rho_Call{};
  float _rho_Put{};
  float _Psi_Call{};
  float _Psi_Put{};

public:
  float S{};
  float delta{};
  float r{};
  float sigma{};
  float K{};
  float T{};

  Model(float S, float delta, float r, float sigma, float K, float T);
  DumbTree getRecombiningTree(int periods, Style style = Style::CRR_drift);

  float C_E() { return this->_C_E; };
  float P_E() { return this->_P_E; };
  float Delta_Call() { return this->_Delta_Call; };
  float Delta_Put() { return this->_Delta_Put; };
  float Gamma_Call() { return this->_Gamma_Call; };
  float Gamma_Put() { return this->_Gamma_Put; };
  float vega_Call() { return this->_vega_Call; };
  float vega_Put() { return this->_vega_Put; };
  float theta_Call() { return this->_theta_Call; };
  float theta_Put() { return this->_theta_Put; };
  float rho_Call() { return this->_rho_Call; };
  float rho_Put() { return this->_rho_Put; };
  float Psi_Call() { return this->_Psi_Call; };
  float Psi_Put() { return this->_Psi_Put; };
};

}  // namespace model
