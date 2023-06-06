#include "binomial-lattice-pricing/recomb_tree.h"

namespace recomb_tree {

RecombinantTree::RecombinantTree(model::Model& _model, model::Style style, int periods)
    : _model(_model) {
  // this->_model = _model;
  this->style = style;
  this->periods = periods;
};

}  // namespace recomb_tree