#pragma once

#include <cmath>
#include <map>
#include <vector>

#include "binomial-lattice-pricing/Node.h"

namespace model {

enum class Style {
  JR,
  JR_risk_neutral,
  CRR_classic,
  CRR_drift,

};

struct Model {
public:
  double S{};
  double delta{};
  double r{};
  double sigma{};
  double K{};
  double T{};
};

}  // namespace model
