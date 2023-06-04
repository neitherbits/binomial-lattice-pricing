#pragma once

struct Node {
  int id{};
  float S = 0;
  float Delta = 0;
  float B = 0;
  float period = 0;
  /**
   * C (American)
   */
  float C_A = 0;
  /**
   * P (American)
   */
  float P_A = 0;
  /**
   * C (European)
   */
  float C_E = 0;
  /**
   * P (European)
   */
  float P_E = 0;
  bool call_early_exercise = false;
  bool put_early_exercise = false;
};
