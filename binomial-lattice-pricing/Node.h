#pragma once

namespace node {

// NOTE: where does this belong? Calls and puts live in a Node, so I put it here for now.
enum class Option { American, European };

struct Node {
  double S = 0;
  double Delta = 0;
  double B = 0;
  int period = 0;  // Is this needed?
  double call = 0;
  double put = 0;
};
}  // namespace node
