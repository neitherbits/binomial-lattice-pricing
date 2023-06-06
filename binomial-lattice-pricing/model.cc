
#include "binomial-lattice-pricing/model.h"

#include <math.h>

#include <iostream>
#include <stdexcept>

#include "binomial-lattice-pricing/Node.h"

namespace model {

float norm_pdf(float x) {
  return std::exp(-std::pow(x, 2) / 2) / std::sqrt(2 * M_PI);
}

float norm_cdf(float x) {
  return 0.5 * (1.0 + erf(x / std::sqrt(2)));
}

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

// NOTE: This is OOP brain damage. Model really should be a struct and this function should be
// free-standing.
DumbTree Model::getRecombiningTree(int periods, Style style) {
  float h = T / periods;
  float u;
  float d;
  float p;
  switch (style) {
    case Style::JR:
      u = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h + sigma * std::sqrt(h));
      d = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h - sigma * std::sqrt(h));
      p = .5;
      break;
    case Style::JR_risk_neutral:
      u = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h + sigma * std::sqrt(h));
      d = std::exp((r - delta - 0.5 * std::pow(sigma, 2)) * h - sigma * std::sqrt(h));
      p = (std::exp((r - delta) * h) - d) / (u - d);
      break;
    case Style::CRR_classic:
      u = std::exp(sigma * std::sqrt(h));
      d = std::exp(-1 * sigma * std::sqrt(h));
      p = (std::exp((r - delta) * h) - d) / (u - d);
      break;
    case Style::CRR_drift:
      u = std::exp((r - delta) * h + sigma * std::sqrt(h));
      d = std::exp((r - delta) * h - sigma * std::sqrt(h));
      p = (std::exp((r - delta) * h) - d) / (u - d);
      break;
    default: throw std::invalid_argument("received bad style");
  }
  DumbTree levels;

  levels[0] = std::vector<node::Node>{};

  // NOTE: I prefixed the calculated Node properties with 'n' to avoid collision with the Model
  // properties.

  for (int i{0}; i <= periods - 1; ++i) {
    float nS = std::pow(u, i) * std::pow(d, (periods - i - 1)) * S;
    float nC_E = fmax(0, nS - K);
    float nP_E = fmax(0, K - nS);
    float nC_A = nC_E;
    float nP_A = nP_E;
    bool call_early_exercise = nC_A < nS - K;
    bool put_early_exercise = nP_A < K - nS;

    float nperiod = periods - 1;
    node::Node node{
        0, nS, 0, 0, nperiod, nC_A, nP_A, nC_E, nP_E, call_early_exercise, put_early_exercise};
    levels[0].push_back(node);
  };

  for (int i{1}; i < periods; ++i) {
    // NOTE: translation from python:
    // level = levels[i];
    // prevlevel = levels[i-1];
    levels[i] = std::vector<::node::Node>{};
    for (int j{0}; j < periods - i; ++j) {
      float nS = levels[i - 1][j + 1].S / u;
      float discount = std::exp(-r * h);
      float nC_E = discount * ((1 - p) * levels[i - 1][j].C_E + p * levels[i - 1][j + 1].C_E);
      float nP_E = discount * ((1 - p) * levels[i - 1][j].P_E + p * levels[i - 1][j + 1].P_E);
      float nC_A = discount * ((1 - p) * levels[i - 1][j].C_A + p * levels[i - 1][j + 1].C_A);
      float nP_A = discount * ((1 - p) * levels[i - 1][j].P_A + p * levels[i - 1][j + 1].P_A);
      bool ncall_early_exercise = nC_A < nS - K;
      bool nput_early_exercise = nP_A < K - nS;
      nC_A = fmax(nC_A, nS - K);
      nP_A = fmax(nP_A, K - nS);
      float nDelta = std::exp(-delta * h) * (levels[i - 1][j + 1].C_E - levels[i - 1][j].C_E) /
                     (levels[i - 1][j + 1].S - levels[i - 1][j].S);
      float nB = discount * (u * levels[i - 1][j].C_E - d * levels[i - 1][j + 1].C_E) / (u - d);
      float nperiod = periods - i - 1;  // TODO: fix this type, it should be int
      node::Node node{0,
                      nS,
                      nDelta,
                      nB,
                      nperiod,
                      nC_A,
                      nP_A,
                      nC_E,
                      nP_E,
                      ncall_early_exercise,
                      nput_early_exercise};
      levels[i].push_back(node);
    }
  }

  return levels;
};

}  // namespace model
