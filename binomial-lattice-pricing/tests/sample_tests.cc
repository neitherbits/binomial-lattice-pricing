#include <gtest/gtest.h>

#include "binomial-lattice-pricing/Node.h"
#include "binomial-lattice-pricing/model.h"

TEST(Node, Test) {
  node::Node n{1234};
  EXPECT_EQ(n.id, 1234);
  EXPECT_EQ(n.S, 0);
}

TEST(Model, Test) {
  model::Model m = model::Model(1, 1, 1, 1, 1, 1);
  EXPECT_EQ(m.S, 1);
  EXPECT_EQ(m.delta, 1);
  EXPECT_NO_FATAL_FAILURE(m.getRecombiningTree(5));
}
