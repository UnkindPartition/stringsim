#include <rapidcheck.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "WatermanEggert.h"
#include "test_utils.h"

using namespace std;
using namespace rc;

inline string aString() { return *gen::container<std::string>(gen::inRange('a', 'e')); }

int main() {
  check("fill_matrix is properly restricted",
    []() {
      const auto a = aString(), b = aString();
      const size_t a_begin = *gen::resize(kNominalSize, gen::inRange(0UL, a.size()+1)),
                   a_end   = *gen::resize(kNominalSize, gen::inRange(a_begin, a.size()+1)),
                   b_begin = *gen::resize(kNominalSize, gen::inRange(0UL, b.size()+1)),
                   b_end   = *gen::resize(kNominalSize, gen::inRange(b_begin, b.size()+1));
      vector<long> matrix = alloc_fill_matrix(a, b, a_begin, a_end, b_begin, b_end);

      bool any = false;
      for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < b.size(); j++) {
        RC_ASSERT(
          (i >= a_begin && i < a_end && j >= b_begin && j < b_end) ||
          (matrix[i * b.size() + j] == 0)
        );
        any = any || matrix[i * b.size() + j];
      }
      }
      RC_TAG(any ? "not all zero" : "all zero");

    });

  return 0;
}
