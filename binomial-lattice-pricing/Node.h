#pragma once

namespace node {

// NOTE: where does this belong? Calls and puts live in a Node, so I put it here for now.
enum class OptionStyle { American, European };

enum class OptionType { Call, Put };

template <OptionStyle S_, OptionType T = OptionType::Call>
struct Node {
  double S = 0;
  // double Delta = 0;
  // double B = 0;
  int period = 0;  // Is this needed?
  double value = 0;
  // double put = 0;
};
}  // namespace node
