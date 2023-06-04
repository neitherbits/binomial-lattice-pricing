#pragma once

#include <string_view>

namespace node {

class Node {
public:
  int id{};
  /**
   * Parse a sample input.
   *
   * @return The number of '(' characters the string starts with.
   */
  static int Parse(std::string_view str);
};

}  // namespace node
