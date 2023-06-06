#include "binomial-lattice-pricing/recomb_tree.h"

namespace recomb_tree {

RecombinantTree::RecombinantTree(model::Model& _model, model::Style style, int periods)
    : _model(_model) {
  // this->_model = _model;
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

  // DumbTree _tree;

  _tree[0] = std::vector<node::Node>{};

  // NOTE: I prefixed the calculated Node properties with 'n' to avoid collision with the Model
  // properties.

  for (int i{0}; i <= periods - 1; ++i) {
    double nS = std::pow(u, i) * std::pow(d, (periods - i - 1)) * _model.S;
    double nC_E = fmax(0, nS - _model.K);
    double nP_E = fmax(0, _model.K - nS);
    double nC_A = nC_E;
    double nP_A = nP_E;
    bool call_early_exercise = nC_A < nS - _model.K;
    bool put_early_exercise = nP_A < _model.K - nS;

    double nperiod = periods - 1;
    node::Node node{
        0, nS, 0, 0, nperiod, nC_A, nP_A, nC_E, nP_E, call_early_exercise, put_early_exercise};
    _tree[0].push_back(node);
  };

  for (int i = 1; i < periods; ++i) {
    // NOTE: translation from python:
    // level = levels[i];
    // prevlevel = levels[i-1];
    _tree[i] = std::vector<::node::Node>{};
    for (int j = 0; j < periods - i; ++j) {
      double nS = _tree[i - 1][j + 1].S / u;
      double discount = std::exp(-_model.r * h);
      double nC_E = discount * ((1 - p) * _tree[i - 1][j].C_E + p * _tree[i - 1][j + 1].C_E);
      double nP_E = discount * ((1 - p) * _tree[i - 1][j].P_E + p * _tree[i - 1][j + 1].P_E);
      double nC_A = discount * ((1 - p) * _tree[i - 1][j].C_A + p * _tree[i - 1][j + 1].C_A);
      double nP_A = discount * ((1 - p) * _tree[i - 1][j].P_A + p * _tree[i - 1][j + 1].P_A);
      bool ncall_early_exercise = nC_A < nS - _model.K;
      bool nput_early_exercise = nP_A < _model.K - nS;
      nC_A = fmax(nC_A, nS - _model.K);
      nP_A = fmax(nP_A, _model.K - nS);
      double nDelta = std::exp(-_model.delta * h) *
                      (_tree[i - 1][j + 1].C_E - _tree[i - 1][j].C_E) /
                      (_tree[i - 1][j + 1].S - _tree[i - 1][j].S);
      double nB = discount * (u * _tree[i - 1][j].C_E - d * _tree[i - 1][j + 1].C_E) / (u - d);
      double nperiod = periods - i - 1;  // TODO: fix this type, it should be int
      node::Node node{0,
                      nS,
                      nDelta,
                      nB,
                      nperiod,
                      nC_A,
                      nP_A,
                      nC_E,
                      nP_E,
                      ncall_early_exercise,
                      nput_early_exercise};
      _tree[i].push_back(node);
    }
  }

  // this->_tree = _tree;
};

}  // namespace recomb_tree