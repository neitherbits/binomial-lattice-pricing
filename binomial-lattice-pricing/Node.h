#pragma once

namespace node {

struct Node {
  int id{};
  double S = 0;
  double Delta = 0;
  double B = 0;
  double period = 0;
  /**
   * C (American)
   */
  double C_A = 0;
  /**
   * P (American)
   */
  double P_A = 0;
  /**
   * C (European)
   */
  double C_E = 0;
  /**
   * P (European)
   */
  double P_E = 0;
  bool call_early_exercise = false;
  bool put_early_exercise = false;
};
}  // namespace node
