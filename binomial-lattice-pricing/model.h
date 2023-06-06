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
  float S{};
  float delta{};
  float r{};
  float sigma{};
  float K{};
  float T{};
  float C_E{};
  float P_E{};
  float Delta_Call{};
  float Delta_Put{};
  float Gamma_Call{};
  float Gamma_Put{};
  float nu_Call{};
  float nu_Put{};
  float zero_Call{};
  float zero_Put{};
  float rho_Call{};
  float rho_Put{};
  float Psi_Call{};
  float Psi_Put{};

  // Model(const Model& rhs);

  // Model& operator=(const Model& rhs);

  Model(float S, float delta, float r, float sigma, float K, float T);
  // DumbTree getRecombiningTree(int periods, Style style = Style::CRR_drift);
};

}  // namespace model
