#pragma once

#include "binomial-lattice-pricing/Model.h"

namespace recomb_tree {

// TODO: implement a real tree. I'm just copying the python code for now.
typedef std::map<int, std::vector<node::Node>> DumbTree;

class RecombinantTree {
private:
  DumbTree _tree{};

public:
  const model::Model& model;
  model::Style style{};
  node::Option option{};
  int periods{};

  RecombinantTree(model::Model& _model, model::Style style, node::Option option, int periods);

  DumbTree tree() { return _tree; };
};
}  // namespace recomb_tree