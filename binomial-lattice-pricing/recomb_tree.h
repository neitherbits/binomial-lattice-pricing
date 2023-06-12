#pragma once

#include <map>
#include <vector>

#include "binomial-lattice-pricing/Model.h"

namespace recomb_tree {

// TODO: implement a real tree. I'm just copying the python code for now.
template <node::OptionStyle S, node::OptionType T>
using DumbTree = std::map<int, std::vector<node::Node<S, T>>>;

template <node::OptionType T>
inline double clamp_value(double S, double K);

template <node::OptionStyle S_, node::OptionType T = node::OptionType::Call,
          model::ModelStyle M = model::ModelStyle::JR>
class RecombinantTree {
private:
  DumbTree<S_, T> tree_{};

public:
  const model::Model<M>& model_;
  const int periods{};

  RecombinantTree(model::Model<M>& model, int periods);

  DumbTree<S_, T> tree() { return tree_; };
};
}  // namespace recomb_tree