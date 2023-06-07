#include <gtest/gtest.h>

#include "binomial-lattice-pricing/Model.h"
#include "binomial-lattice-pricing/Node.h"

TEST(Node, Test) {
  node::Node n{};
  EXPECT_EQ(n.S, 0);
}

TEST(Model, Test) {
  model::Model m = model::Model{1, 1, 1, 1, 1, 1};
  EXPECT_EQ(m.S, 1);
  EXPECT_EQ(m.delta, 1);
}
