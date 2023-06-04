#include <gtest/gtest.h>

#include "binomial-lattice-pricing/node.h"

namespace node {

TEST(Node, Parse) {
  EXPECT_EQ(Node::Parse(""), 0);
  EXPECT_EQ(Node::Parse("("), 1);
  EXPECT_EQ(Node::Parse("(((("), 4);
  EXPECT_EQ(Node::Parse(".(((("), 0);
  EXPECT_EQ(Node::Parse("((.(("), 2);
  EXPECT_DEATH(Node::Parse("((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((("
                           "(((((((((((((((((((((((((((((((("),
               ".*");
  Node n{1234};
  EXPECT_EQ(n.id, 1234);
}

}  // namespace node