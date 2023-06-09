#include "binomial-lattice-pricing/recomb_tree.h"

#include <stdexcept>

namespace recomb_tree {

template <>
RecombinantTree<node::OptionStyle::American>::RecombinantTree(model::Model& _model,
                                                              model::Style style,
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
  _tree[0] = std::vector<node::Node2<node::OptionStyle::American>>{};

  for (int i{0}; i <= periods - 1; ++i) {
    double S = std::pow(u, i) * std::pow(d, (periods - i - 1)) * _model.S;
    double call = fmax(0, S - _model.K);
    // double put = fmax(0, _model.K - S);

    double value = call;  // FIXME

    int period = periods - 1;
    node::Node2<node::OptionStyle::American> node{S, period, value};
    _tree[0].push_back(node);
  };

  for (int i = 1; i < periods; ++i) {
    /** NOTE: translation from python:
          level = levels[i];
          prevlevel = levels[i-1];
    */
    _tree[i] = std::vector<::node::Node2<node::OptionStyle::American>>{};
    for (int j = 0; j < periods - i; ++j) {
      double discount = std::exp(-_model.r * h);
      double value = discount * ((1 - p) * _tree[i - 1][j].value + p * _tree[i - 1][j + 1].value);

      // Handle American options
      double S = _tree[i - 1][j + 1].S / u;

      value = fmax(value, S - _model.K);  // call FIXME
      // put = fmax(value, _model.K - S);
      // ^^^^^

      int period = periods - i - 1;

      node::Node2<node::OptionStyle::American> node{S, period, value};
      _tree[i].push_back(node);
    }
  }
};

}  // namespace recomb_tree