#include "binomial-lattice-pricing/BlackScholes.h"

#include "binomial-lattice-pricing/Model.h"

namespace black_scholes {

BlackScholes::BlackScholes(model::VirtualModel& model) {
  // TODO: refactor this to be faster/reuse some calculations
  double d1 = (std::log(model.S / model.K) +
               (model.r - model.delta + 0.5 * std::pow(model.sigma, 2) * model.T)) /
              (model.sigma * std::sqrt(model.T));
  double d2 = (std::log(model.S / model.K) +
               (model.r - model.delta - 0.5 * std::pow(model.sigma, 2) * model.T)) /
              (model.sigma * std::sqrt(model.T));
  this->_Call_Eur = model.S * std::exp(-model.delta * model.T) * norm_cdf(d1) -
                    model.K * std::exp(-model.r * model.T) * norm_cdf(d2);
  this->_Put_Eur = model.K * std::exp(-model.r * model.T) * norm_cdf(-d2) -
                   model.S * std::exp(-model.delta * model.T) * norm_cdf(-d1);
  this->_Delta_Call = std::exp(-model.delta * model.T) * norm_cdf(d1);
  this->_Delta_Put = -std::exp(-model.delta * model.T) * norm_cdf(-d1);
  this->_Gamma_Call = (1.0 / (model.S * model.sigma * std::sqrt(model.T))) *
                      std::exp(-model.delta * model.T) * norm_pdf(d1);
  this->_Gamma_Put = _Gamma_Call;
  // Scaled by 1/100
  this->_vega_Call =
      (model.S * std::exp(-model.delta * model.T) * norm_pdf(d1) * std::sqrt(model.T)) / 100.0;
  this->_vega_Put = _vega_Call;
  // Scaled by 1/365
  this->_theta_Call = (model.delta * model.S * std::exp(-model.delta * model.T) * norm_cdf(d1) -
                       model.r * model.K * std::exp(-model.r * model.T) * norm_cdf(d2) -
                       (model.sigma / (2 * std::sqrt(model.T))) * model.S *
                           std::exp(-model.delta * model.T) * norm_pdf(d1)) /
                      365.0;
  this->_theta_Put = (model.r * model.K * std::exp(-model.r * model.T) * norm_cdf(-d2) -
                      model.delta * model.S * std::exp(-model.delta * model.T) * norm_cdf(-d1) -
                      (model.sigma / (2 * std::sqrt(model.T))) * model.S *
                          std::exp(-model.delta * model.T) * norm_pdf(d1)) /
                     365.0;
  // Scaled 1/10,000
  this->_rho_Call = (model.T * model.K * std::exp(-model.r * model.T) * norm_cdf(d2)) / 10000.0;
  this->_rho_Put = (-model.T * model.K * std::exp(-model.r * model.T) * norm_cdf(-d2)) / 10000.0;
  // Scaled 1/10,000
  this->_Psi_Call =
      (-model.T * model.S * std::exp(-model.delta * model.T) * norm_cdf(d1)) / 10000.0;
  this->_Psi_Put = (model.T * model.S * std::exp(-model.delta * model.T) * norm_cdf(-d1)) / 10000.0;
};

}  // namespace black_scholes