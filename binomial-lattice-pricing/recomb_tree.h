#pragma once

#include "binomial-lattice-pricing/Model.h"

namespace recomb_tree {

// TODO: implement a real tree. I'm just copying the python code for now.
typedef std::map<int, std::vector<node::Node>> DumbTree;
// typedef std::map<int, std::vector<node::AmericanNode>> DumbTreeAmerican;
// typedef std::map<int, std::vector<node::EuropeanNode>> DumbTreeEuropean;

template <node::OptionStyle T>
using DumbTree2 = std::map<int, std::vector<node::Node2<T>>>;

template <node::OptionStyle T>
class RecombinantTree {
private:
  DumbTree2<T> _tree{};

public:
  const model::Model& model;
  model::Style style{};  // TODO: make this a template parameter as well.
  int periods{};

  RecombinantTree(model::Model& _model, model::Style style, int periods);

  DumbTree2<T> tree() { return _tree; };
};
}  // namespace recomb_tree