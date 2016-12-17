#include <rapidcheck.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "WatermanEggert/WatermanEggert.h"
#include "test_utils.h"

using namespace std;
using namespace rc;

inline string aString() { return *gen::container<std::string>(gen::inRange('a', 'e')); }

int main() {
  check("fill_matrix is properly restricted",
    []() {
      const auto a = aString(), b = aString();
      const size_t row_begin = *gen::resize(kNominalSize, gen::inRange(0UL, a.size()+1)),
                   row_end   = *gen::resize(kNominalSize, gen::inRange(row_begin, a.size()+1)),
                   col_begin = *gen::resize(kNominalSize, gen::inRange(0UL, b.size()+1)),
                   col_end   = *gen::resize(kNominalSize, gen::inRange(col_begin, b.size()+1));
      Matrix matrix = alloc_fill_matrix(a, b, row_begin, row_end, col_begin, col_end);

      bool any = false;
      for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < b.size(); j++) {
        RC_ASSERT(
          (i >= row_begin && i < row_end && j >= col_begin && j < col_end) ||
          (matrix(i,j) == 0)
        );
        any = any || matrix(i,j);
      }
      }
      RC_TAG(any ? "not all zero" : "all zero");
    });

  check("find_alignment properties",
    []() {
      const auto a = aString(), b = aString();
      Matrix matrix = alloc_fill_matrix(a, b);
      Matrix alignment = find_alignment(scoring, a, b, matrix);

      // Property: if all entries are zero, the alignment is empty
      if(matrix.all_le(0)) {
        RC_TAG("all zero");
        RC_ASSERT(alignment.row_begin == alignment.row_end);
        RC_ASSERT(alignment.col_begin == alignment.col_end);
        // for the other properties, we assume that there are non-zero entries
        // Thus, return.
        return;
      }
      RC_TAG("not all zero");
      // cout << alignment.row_begin << " " << alignment.row_end << " " << alignment.col_begin << " " << alignment.col_end << endl;
      RC_ASSERT(!alignment.empty());

      // alignment should end on a maximum value
      auto elements = matrix.elements();
      RC_ASSERT(matrix(alignment.row_end-1,alignment.col_end-1) ==
        *max_element(elements.begin(), elements.end()));
    });

  return 0;
}
