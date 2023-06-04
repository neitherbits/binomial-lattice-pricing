#include <gtest/gtest.h>

#include "binomial-lattice-pricing/Node.h"
#include "binomial-lattice-pricing/model.h"

TEST(Node, Test) {
  Node n{1234};
  EXPECT_EQ(n.id, 1234);
  EXPECT_EQ(n.S, 0);
}

namespace model {

TEST(Model, Test) {
  Model m = Model(1, 1, 1, 1, 1, 1);
  EXPECT_EQ(m.S, 1);
  EXPECT_EQ(m.delta, 1);
}

}  // namespace model