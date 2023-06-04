#include <cstddef>
#include <cstdint>

#include "binomial-lattice-pricing/node.h"

namespace node {

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  auto result = Node::Parse(std::string_view(reinterpret_cast<const char*>(data), size));
  (void)result;

  return 0;
}

}  // namespace node
