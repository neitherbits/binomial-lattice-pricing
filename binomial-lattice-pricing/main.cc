#include <iostream>

#include "binomial-lattice-pricing/Model.h"
#include "binomial-lattice-pricing/recomb_tree.h"

// TODO: make this a legitimate test

int main(int argc, char* argv[]) {
  model::Model m = model::Model{68.04, 0.0118, 0.0180, 0.2433, 67.50, 173 / 365.0};

  int periods = 1000;
  recomb_tree::RecombinantTree r =
      recomb_tree::RecombinantTree(m, model::Style::JR, node::Option::American, periods);

  std::cout << "Model copied into RecombinantTree: (m.S)" << m.S << " == " << r.model.S
            << "(r.model.S)" << std::endl;

  node::Node n = r.tree()[periods - 1].at(0);

  // std::cout << "Size: " << r.tree()[periods - 1].size() << std::endl;
  std::cout << "Final Node: S=" << n.S << " Delta=" << n.Delta << " B=" << n.B << " call=" << n.call
            << " put=" << n.put << std::endl;

  return 0;
};
