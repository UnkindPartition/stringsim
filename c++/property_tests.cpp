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

  check("find_alignment properties",
    []() {
      const auto a = aString(), b = aString();
      size_t a_begin, a_end, b_begin, b_end;
      vector<long> matrix = alloc_fill_matrix(a, b);
      find_alignment(scoring, a, b, matrix, a_begin, a_end, b_begin, b_end);

      // Property: if all entries are zero, the alignment is empty
      if(none_of(matrix.begin(), matrix.end(), [](long x){return x;})) {
        RC_TAG("all zero");
        RC_ASSERT(a_begin == a_end);
        RC_ASSERT(b_begin == b_end);
        // for the other properties, we assume that there are non-zero entries
        // Thus, return.
        return;
      }
      RC_TAG("not all zero");
      // cout << a_begin << " " << a_end << " " << b_begin << " " << b_end << endl;
      RC_ASSERT(a_begin < a_end && b_begin < b_end); // alignment should be non-empty

      // alignment should end on a maximum value
      RC_ASSERT(matrix[(a_end-1) * b.size() + (b_end-1)] == *max_element(matrix.begin(), matrix.end()));
    });

  return 0;
}
