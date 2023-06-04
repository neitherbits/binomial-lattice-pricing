#include "binomial-lattice-pricing/node.h"

#include <cstdlib>

namespace node {

int Node::Parse(std::string_view str) {
  if (!str.empty()) {
    const char ch = str[0];

    // A simple example that will crash given a large enough input.
    if (ch == '(') {
      const int result = Parse(str.substr(1)) + 1;
      if (result == 100) {
        std::abort();
      }

      return result;
    }
  }

  return 0;
}

}  // namespace node
