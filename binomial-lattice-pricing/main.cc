#include <iostream>

#include "binomial-lattice-pricing/model.h"

int main(int argc, char* argv[]) {
  model::Model m = model::Model(68.04, 0.0118, 0.0180, 0.2433, 67.50, 173 / 365.0);
  model::DumbTree vs = m.getRecombiningTree(4);
  node::Node n = vs[3].at(0);
  std::cout << "Size: " << vs[3].size() << std::endl;
  std::cout << "Node: " << n.B << " " << n.C_A << " " << n.C_E << " " << n.Delta << " " << n.P_A
            << " " << n.P_E << " " << std::endl;

  return 0;
};
