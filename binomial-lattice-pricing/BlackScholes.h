#pragma once

#include "binomial-lattice-pricing/Model.h"

namespace black_scholes {

inline double norm_pdf(double x) {
  // constexpr double INV_SQRT_2PI = 0.398942280401433;  // 1 / std::sqrt(2 * M_PI);
  return std::exp(std::pow(x, 2) * -0.5) * 0.398942280401433;
}

inline double norm_cdf(double x) {
  // constexpr double INV_SQRT_2 = 0.707106781186548;  // 1 / std::sqrt(2);
  return 0.5 * (1.0 + erf(x * 0.707106781186548));
}

// NOTE: is this the right name for this struct?
struct BlackScholes {
private:
  float _Call_Eur{};
  float _Put_Eur{};
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
  BlackScholes(model::VirtualModel& model);

  float Call_Eur() { return this->_Call_Eur; };
  float Put_Eur() { return this->_Put_Eur; };
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
}  // namespace black_scholes