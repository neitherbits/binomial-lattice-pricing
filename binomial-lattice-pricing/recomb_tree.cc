#include "binomial-lattice-pricing/recomb_tree.h"

#include <stdexcept>

namespace recomb_tree {

RecombinantTree::RecombinantTree(model::Model& _model, model::Style style, node::Option option,
                                 int periods)
    : model(_model) {  // C++ black magic
  // this->_model = _model; // See above
  this->style = style;
  this->periods = periods;

  double h = _model.T / periods;
  double u;
  double d;
  double p;
  switch (style) {
    case model::Style::JR:
      u = std::exp((_model.r - _model.delta - 0.5 * std::pow(_model.sigma, 2)) * h +
                   _model.sigma * std::sqrt(h));
      d = std::exp((_model.r - _model.delta - 0.5 * std::pow(_model.sigma, 2)) * h -
                   _model.sigma * std::sqrt(h));
      p = .5;
      break;
    case model::Style::JR_risk_neutral:
      u = std::exp((_model.r - _model.delta - 0.5 * std::pow(_model.sigma, 2)) * h +
                   _model.sigma * std::sqrt(h));
      d = std::exp((_model.r - _model.delta - 0.5 * std::pow(_model.sigma, 2)) * h -
                   _model.sigma * std::sqrt(h));
      p = (std::exp((_model.r - _model.delta) * h) - d) / (u - d);
      break;
    case model::Style::CRR_classic:
      u = std::exp(_model.sigma * std::sqrt(h));
      d = std::exp(-1 * _model.sigma * std::sqrt(h));
      p = (std::exp((_model.r - _model.delta) * h) - d) / (u - d);
      break;
    case model::Style::CRR_drift:
      u = std::exp((_model.r - _model.delta) * h + _model.sigma * std::sqrt(h));
      d = std::exp((_model.r - _model.delta) * h - _model.sigma * std::sqrt(h));
      p = (std::exp((_model.r - _model.delta) * h) - d) / (u - d);
      break;
    default: throw std::invalid_argument("received bad style");
  }

  // NOTE: this is a very bad implementation of a tree
  _tree[0] = std::vector<node::Node>{};

  for (int i{0}; i <= periods - 1; ++i) {
    double S = std::pow(u, i) * std::pow(d, (periods - i - 1)) * _model.S;
    double call = fmax(0, S - _model.K);
    double put = fmax(0, _model.K - S);

    int period = periods - 1;
    node::Node node{S, 0, 0, period, call, put};
    _tree[0].push_back(node);
  };

  for (int i = 1; i < periods; ++i) {
    /** NOTE: translation from python:
          level = levels[i];
          prevlevel = levels[i-1];
    */
    _tree[i] = std::vector<::node::Node>{};
    for (int j = 0; j < periods - i; ++j) {
      double S = _tree[i - 1][j + 1].S / u;
      double discount = std::exp(-_model.r * h);
      double call = discount * ((1 - p) * _tree[i - 1][j].call + p * _tree[i - 1][j + 1].call);
      double put = discount * ((1 - p) * _tree[i - 1][j].put + p * _tree[i - 1][j + 1].put);

      // Handle American options
      if (option == node::Option::American) {
        call = fmax(call, S - _model.K);
        put = fmax(put, _model.K - S);
      }

      /** ===Potentially Breaking Change===
      NOTE: The Delta and B calculations explicitly used the
      European option in the original algorithm. This will use the American option if the user
      chooses it.
      */
      double Delta = std::exp(-_model.delta * h) *
                     (_tree[i - 1][j + 1].call - _tree[i - 1][j].call) /
                     (_tree[i - 1][j + 1].S - _tree[i - 1][j].S);
      double B = discount * (u * _tree[i - 1][j].call - d * _tree[i - 1][j + 1].call) / (u - d);

      int period = periods - i - 1;

      node::Node node{S, Delta, B, period, call, put};
      _tree[i].push_back(node);
    }
  }
};

}  // namespace recomb_tree