#pragma once

#include <cmath>

namespace model {
class Model {
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

  Model(float S, float delta, float r, float sigma, float K, float T);
};
}  // namespace model