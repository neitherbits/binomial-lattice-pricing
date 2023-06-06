
#include "binomial-lattice-pricing/model.h"

#include <math.h>

#include <iostream>
#include <stdexcept>

#include "binomial-lattice-pricing/Node.h"

namespace model {

double norm_pdf(double x) {
  return std::exp(-std::pow(x, 2) / 2) / std::sqrt(2 * M_PI);
}

double norm_cdf(double x) {
  return 0.5 * (1.0 + erf(x / std::sqrt(2)));
}

Model::Model(double S, double delta, double r, double sigma, double K, double T) {
  this->S = S;
  this->delta = delta;
  this->r = r;
  this->sigma = sigma;
  this->K = K;
  this->T = T;
  // TODO: refactor this to be faster/reuse some calculations
  double d1 =
      (std::log(S / K) + (r - delta + 0.5 * std::pow(sigma, 2) * T)) / (sigma * std::sqrt(T));
  double d2 =
      (std::log(S / K) + (r - delta - 0.5 * std::pow(sigma, 2) * T)) / (sigma * std::sqrt(T));
  this->_C_E = S * std::exp(-delta * T) * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
  this->_P_E = K * std::exp(-r * T) * norm_cdf(-d2) - S * std::exp(-delta * T) * norm_cdf(-d1);
  this->_Delta_Call = std::exp(-delta * T) * norm_cdf(d1);
  this->_Delta_Put = -std::exp(-delta * T) * norm_cdf(-d1);
  this->_Gamma_Call = (1.0 / (S * sigma * std::sqrt(T))) * std::exp(-delta * T) * norm_pdf(d1);
  this->_Gamma_Put = _Gamma_Call;
  // Scaled by 1/100
  this->_vega_Call = (S * std::exp(-delta * T) * norm_pdf(d1) * std::sqrt(T)) / 100.0;
  this->_vega_Put = _vega_Call;
  // Scaled by 1/365
  this->_theta_Call =
      (delta * S * std::exp(-delta * T) * norm_cdf(d1) - r * K * std::exp(-r * T) * norm_cdf(d2) -
       (sigma / (2 * std::sqrt(T))) * S * std::exp(-delta * T) * norm_pdf(d1)) /
      365.0;
  this->_theta_Put =
      (r * K * std::exp(-r * T) * norm_cdf(-d2) - delta * S * std::exp(-delta * T) * norm_cdf(-d1) -
       (sigma / (2 * std::sqrt(T))) * S * std::exp(-delta * T) * norm_pdf(d1)) /
      365.0;
  // Scaled 1/10,000
  this->_rho_Call = (T * K * std::exp(-r * T) * norm_cdf(d2)) / 10000.0;
  this->_rho_Put = (-T * K * std::exp(-r * T) * norm_cdf(-d2)) / 10000.0;
  // Scaled 1/10,000
  this->_Psi_Call = (-T * S * std::exp(-delta * T) * norm_cdf(d1)) / 10000.0;
  this->_Psi_Put = (T * S * std::exp(-delta * T) * norm_cdf(-d1)) / 10000.0;
}

// NOTE: This is OOP brain damage. Model really should be a struct and this function should be
// free-standing.
// DumbTree Model::getRecombiningTree(int periods, Style style) {
//   double h = T / periods;
//   double u;
//   double d;
//   double p;
//   switch (style) {
//     case Style::JR:
//       u = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h + sigma * std::sqrt(h));
//       d = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h - sigma * std::sqrt(h));
//       p = .5;
//       break;
//     case Style::JR_risk_neutral:
//       u = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h + sigma * std::sqrt(h));
//       d = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h - sigma * std::sqrt(h));
//       p = (std::exp((r - delta) * h) - d) / (u - d);
//       break;
//     case Style::CRR_classic:
//       u = std::exp(sigma * std::sqrt(h));
//       d = std::exp(-1 * sigma * std::sqrt(h));
//       p = (std::exp((r - delta) * h) - d) / (u - d);
//       break;
//     case Style::CRR_drift:
//       u = std::exp((r - delta) * h + sigma * std::sqrt(h));
//       d = std::exp((r - delta) * h - sigma * std::sqrt(h));
//       p = (std::exp((r - delta) * h) - d) / (u - d);
//       break;
//     default: throw std::invalid_argument("received bad style");
//   }
//   DumbTree levels;

//   levels[0] = std::vector<node::Node>{};

//   // NOTE: I prefixed the calculated Node properties with 'n' to avoid collision with the Model
//   // properties.

//   for (int i{0}; i <= periods - 1; ++i) {
//     double nS = std::pow(u, i) * std::pow(d, (periods - i - 1)) * S;
//     double nC_E = fmax(0, nS - K);
//     double nP_E = fmax(0, K - nS);
//     double nC_A = nC_E;
//     double nP_A = nP_E;
//     bool call_early_exercise = nC_A < nS - K;
//     bool put_early_exercise = nP_A < K - nS;

//     double nperiod = periods - 1;
//     node::Node node{
//         0, nS, 0, 0, nperiod, nC_A, nP_A, nC_E, nP_E, call_early_exercise, put_early_exercise};
//     levels[0].push_back(node);
//   };

//   for (int i{1}; i < periods; ++i) {
//     // NOTE: translation from python:
//     // level = levels[i];
//     // prevlevel = levels[i-1];
//     levels[i] = std::vector<::node::Node>{};
//     for (int j{0}; j < periods - i; ++j) {
//       double nS = levels[i - 1][j + 1].S / u;
//       double discount = std::exp(-r * h);
//       double nC_E = discount * ((1 - p) * levels[i - 1][j].C_E + p * levels[i - 1][j + 1].C_E);
//       double nP_E = discount * ((1 - p) * levels[i - 1][j].P_E + p * levels[i - 1][j + 1].P_E);
//       double nC_A = discount * ((1 - p) * levels[i - 1][j].C_A + p * levels[i - 1][j + 1].C_A);
//       double nP_A = discount * ((1 - p) * levels[i - 1][j].P_A + p * levels[i - 1][j + 1].P_A);
//       bool ncall_early_exercise = nC_A < nS - K;
//       bool nput_early_exercise = nP_A < K - nS;
//       nC_A = fmax(nC_A, nS - K);
//       nP_A = fmax(nP_A, K - nS);
//       double nDelta = std::exp(-delta * h) * (levels[i - 1][j + 1].C_E - levels[i - 1][j].C_E) /
//                      (levels[i - 1][j + 1].S - levels[i - 1][j].S);
//       double nB = discount * (u * levels[i - 1][j].C_E - d * levels[i - 1][j + 1].C_E) / (u - d);
//       double nperiod = periods - i - 1;  // TODO: fix this type, it should be int
//       node::Node node{0,
//                       nS,
//                       nDelta,
//                       nB,
//                       nperiod,
//                       nC_A,
//                       nP_A,
//                       nC_E,
//                       nP_E,
//                       ncall_early_exercise,
//                       nput_early_exercise};
//       levels[i].push_back(node);
//     }
//   }

//   return levels;
// };

}  // namespace model
