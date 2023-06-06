#pragma once

#include "binomial-lattice-pricing/model.h"

namespace recomb_tree {

// TODO: implement a real tree. I'm just copying the python code for now.
typedef std::map<int, std::vector<node::Node>> DumbTree;

class RecombinantTree {
private:
  DumbTree _tree{};

public:
  const model::Model& _model;
  model::Style style{};
  int periods{};

  RecombinantTree(model::Model& _model, model::Style style, int periods);

  DumbTree tree() { return _tree; };
};
}  // namespace recomb_tree