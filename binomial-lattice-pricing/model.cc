#include "binomial-lattice-pricing/model.h"

#include <math.h>

#include "binomial-lattice-pricing/Node.h"

namespace model {
float norm_pdf(float x) {
  return std::exp(-std::pow(x, 2) / 2) / std::sqrt(2 * M_PI);
}
float norm_cdf(float x) {
  return 0.5 * (1.0 + erf(x / std::sqrt(2)));
};
Model::Model(float _S, float _delta, float _r, float _sigma, float _K, float _T) {
  S = _S;
  delta = _delta;
  r = _r;
  sigma = _sigma;
  K = _K;
  T = _T;
  // TODO: refactor this to be faster
  float d1 =
      (std::log(S / K) + (r - delta + 0.5 * std::pow(sigma, 2) * T)) / (sigma * std::sqrt(T));
  float d2 =
      (std::log(S / K) + (r - delta - 0.5 * std::pow(sigma, 2) * T)) / (sigma * std::sqrt(T));
  C_E = S * std::exp(-delta * T) * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
  P_E = K * std::exp(-r * T) * norm_cdf(-d2) - S * std::exp(-delta * T) * norm_cdf(-d1);
  Delta_Call = std::exp(-delta * T) * norm_cdf(d1);
  Delta_Put = -std::exp(-delta * T) * norm_cdf(-d1);
  Gamma_Call = (1.0 / (S * sigma * std::sqrt(T))) * std::exp(-delta * T) * norm_pdf(d1);
  Gamma_Put = Gamma_Call;
  // Scaled by 1/100
  nu_Call = (S * std::exp(-delta * T) * norm_pdf(d1) * std::sqrt(T)) / 100.0;
  nu_Put = nu_Call;
  // Scaled by 1/365
  zero_Call =
      (delta * S * std::exp(-delta * T) * norm_cdf(d1) - r * K * std::exp(-r * T) * norm_cdf(d2) -
       (sigma / (2 * std::sqrt(T))) * S * std::exp(-delta * T) * norm_pdf(d1)) /
      365.0;
  zero_Put =
      (r * K * std::exp(-r * T) * norm_cdf(-d2) - delta * S * std::exp(-delta * T) * norm_cdf(-d1) -
       (sigma / (2 * std::sqrt(T))) * S * std::exp(-delta * T) * norm_pdf(d1)) /
      365.0;
  // Scaled 1/10,000
  rho_Call = (T * K * std::exp(-r * T) * norm_cdf(d2)) / 10000.0;
  rho_Put = (-T * K * std::exp(-r * T) * norm_cdf(-d2)) / 10000.0;
  // Scaled 1/10,000
  Psi_Call = (-T * S * std::exp(-delta * T) * norm_cdf(d1)) / 10000.0;
  Psi_Put = (T * S * std::exp(-delta * T) * norm_cdf(-d1)) / 10000.0;
}
}  // namespace model