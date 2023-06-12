#pragma once

#include "binomial-lattice-pricing/Node.h"

namespace model {

enum class ModelStyle {
  JR,
  JR_risk_neutral,
  CRR_classic,
  CRR_drift,

};

struct TransitionParams {
  double u{};
  double d{};
  double p{};
};

// NOTE: This is a hack so that I don't have to template BlackScholes as well
struct VirtualModel {
public:
  double S{};
  double delta{};
  double r{};
  double sigma{};
  double K{};
  double T{};
};

template <ModelStyle>
struct Model : VirtualModel {};

template <ModelStyle S_>
TransitionParams transition_params(const Model<S_>& model, const double h);

}  // namespace model
