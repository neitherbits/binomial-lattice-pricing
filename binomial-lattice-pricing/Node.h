#pragma once

namespace node {

// NOTE: where does this belong? Calls and puts live in a Node, so I put it here for now.
enum class OptionStyle { American, European };
enum class OptionType { Call, Put };

struct Node {
  double S = 0;
  double Delta = 0;
  double B = 0;
  int period = 0;  // Is this needed?
  double call = 0;
  double put = 0;
};

template <OptionStyle T>
struct Node2 {};

template <>
struct Node2<OptionStyle::American> {
  double S = 0;
  int period = 0;  // Is this needed?
  double value = 0;
};

template <>
struct Node2<OptionStyle::European> {
  // double S = 0;
  int period = 0;  // Is this needed?
  double value = 0;
};

// struct AmericanNode {
//   double S = 0;
//   int period = 0;  // Is this needed?
//   double value = 0;
// };

// struct EuropeanNode {
//   int period = 0;  // Is this needed?
//   double value = 0;
// };

}  // namespace node
