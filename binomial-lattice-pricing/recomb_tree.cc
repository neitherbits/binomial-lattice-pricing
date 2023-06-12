#include "binomial-lattice-pricing/recomb_tree.h"

#include <stdexcept>

#include "binomial-lattice-pricing/Model.h"

// NOTE: this shouldn't be implemented here. I'm just lazy...
namespace model {

template <>
TransitionParams transition_params<ModelStyle::JR>(const Model<ModelStyle::JR>& model,
                                                   const double h) {
  return TransitionParams{std::exp((model.r - model.delta - 0.5 * std::pow(model.sigma, 2)) * h +
                                   model.sigma * std::sqrt(h)),
                          std::exp((model.r - model.delta - 0.5 * std::pow(model.sigma, 2)) * h -
                                   model.sigma * std::sqrt(h)),
                          0.5};
};

TransitionParams transition_params(const Model<ModelStyle::JR_risk_neutral>& model,
                                   const double h) {
  const double u = std::exp((model.r - model.delta - 0.5 * std::pow(model.sigma, 2)) * h +
                            model.sigma * std::sqrt(h));
  const double d = std::exp((model.r - model.delta - 0.5 * std::pow(model.sigma, 2)) * h -
                            model.sigma * std::sqrt(h));
  return TransitionParams{u, d, (std::exp((model.r - model.delta) * h) - d) / (u - d)};
};

TransitionParams transition_params(const Model<ModelStyle::CRR_classic>& model, const double h) {
  const double u = std::exp(model.sigma * std::sqrt(h));
  const double d = std::exp(-1 * model.sigma * std::sqrt(h));
  return TransitionParams{u, d, (std::exp((model.r - model.delta) * h) - d) / (u - d)};
};

TransitionParams transition_params(const Model<ModelStyle::CRR_drift>& model, const double h) {
  const double u = std::exp((model.r - model.delta) * h + model.sigma * std::sqrt(h));
  const double d = std::exp((model.r - model.delta) * h - model.sigma * std::sqrt(h));
  return TransitionParams{u, d, (std::exp((model.r - model.delta) * h) - d) / (u - d)};
};
}  // namespace model

namespace recomb_tree {

template <>
inline double clamp_value<node::OptionType::Call>(double S, double K) {
  return fmax(0, S - K);
};

template <>
inline double clamp_value<node::OptionType::Put>(double S, double K) {
  return fmax(0, K - S);
};

template <node::OptionType T, model::ModelStyle M>
class RecombinantTree<node::OptionStyle::European, T, M> {
private:
  DumbTree<node::OptionStyle::European, T> tree_{};

public:
  const model::Model<M>& model_;
  const int periods{};
  RecombinantTree(model::Model<M>& model_, int periods) : model_(model_), periods(periods) {
    const double h = model_.T / periods;

    const model::TransitionParams tps = model::transition_params(model_, h);

    // NOTE: this is a very bad implementation of a tree
    tree_[0] = std::vector<node::Node<node::OptionStyle::European, T>>{};

    for (int i{0}; i <= periods - 1; ++i) {
      const double S = std::pow(tps.u, i) * std::pow(tps.d, (periods - i - 1)) * model_.S;
      const double value = clamp_value<T>(S, model_.K);

      const int period = periods - 1;
      const node::Node<node::OptionStyle::European, T> node{S, period, value};
      tree_[0].push_back(node);
    };

    for (int i = 1; i < periods; ++i) {
      /** NOTE: translation from python:
            level = levels[i];
            prevlevel = levels[i-1];
      */
      tree_[i] = std::vector<::node::Node<node::OptionStyle::European, T>>{};
      for (int j = 0; j < periods - i; ++j) {
        const double S = tree_[i - 1][j + 1].S / tps.u;
        const double value = std::exp(-model_.r * h) * ((1 - tps.p) * tree_[i - 1][j].value +
                                                        tps.p * tree_[i - 1][j + 1].value);

        const int period = periods - i - 1;

        const node::Node<node::OptionStyle::European, T> node{S, period, value};
        tree_[i].push_back(node);
      }
    }
  };
};

// TODO: Implement RecombinantTree<node::OptionStyle::American, T, M>

// NOTE: Explicitly listing all valid templates as a hack.
// https://stackoverflow.com/a/495056
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Call,
                               model::ModelStyle::JR>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Call,
                               model::ModelStyle::JR_risk_neutral>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Call,
                               model::ModelStyle::CRR_classic>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Call,
                               model::ModelStyle::CRR_drift>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Put,
                               model::ModelStyle::JR>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Put,
                               model::ModelStyle::JR_risk_neutral>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Put,
                               model::ModelStyle::CRR_classic>;
template class RecombinantTree<node::OptionStyle::European, node::OptionType::Put,
                               model::ModelStyle::CRR_drift>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Call,
                               model::ModelStyle::JR>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Call,
                               model::ModelStyle::JR_risk_neutral>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Call,
                               model::ModelStyle::CRR_classic>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Call,
                               model::ModelStyle::CRR_drift>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Put,
                               model::ModelStyle::JR>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Put,
                               model::ModelStyle::JR_risk_neutral>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Put,
                               model::ModelStyle::CRR_classic>;
template class RecombinantTree<node::OptionStyle::American, node::OptionType::Put,
                               model::ModelStyle::CRR_drift>;

}  // namespace recomb_tree