#include <iostream>

#include "binomial-lattice-pricing/model.h"
#include "binomial-lattice-pricing/recomb_tree.h"

int main(int argc, char* argv[]) {
  model::Model m = model::Model(68.04, 0.0118, 0.0180, 0.2433, 67.50, 173 / 365.0);
  int periods = 1000;
  recomb_tree::RecombinantTree r = recomb_tree::RecombinantTree(m, model::Style::JR, periods);
  std::cout << m.S << " == " << r._model.S << std::endl;
  // model::DumbTree vs = m.getRecombiningTree(4);
  node::Node n = r.tree()[periods - 1].at(0);
  std::cout << "Size: " << r.tree()[periods - 1].size() << std::endl;
  std::cout << "Node: " << n.B << " " << n.C_A << " " << n.C_E << " " << n.Delta << " " << n.P_A
            << " " << n.P_E << " " << std::endl;

  return 0;
};
