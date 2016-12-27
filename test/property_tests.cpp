#include <rapidcheck.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "stringsim/stringsim.h"
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

  check("remove_alignment",
    []() {
      const auto a = aString(), b = aString();
      Matrix matrix = alloc_fill_matrix(a, b);
      Matrix alignment = find_alignment(scoring, a, b, matrix);
      vector<Matrix> unaffected, affected, affected2;
      tie(unaffected, affected) = remove_alignment({matrix}, alignment);
      RC_TAG(affected.size());
      // try to remove the same alignment the second time
      tie(ignore, affected2) = remove_alignment(affected, alignment);
      RC_ASSERT(affected2.empty());
      // check that the sizes add up
      size_t total_size = 0;
      for (Matrix mx : affected) {
        total_size += (mx.row_end - mx.row_begin) * (mx.col_end - mx.col_begin);
      }
      for (Matrix mx : unaffected) {
        total_size += (mx.row_end - mx.row_begin) * (mx.col_end - mx.col_begin);
      }
      total_size +=
        (alignment.row_end - alignment.row_begin) * b.size() +
        (alignment.col_end - alignment.col_begin) * a.size() -
        (alignment.row_end - alignment.row_begin) * (alignment.col_end - alignment.col_begin);
      RC_ASSERT(total_size == matrix.elements().size());
    });

  check("update_matrix is equivalent to fill_matrix",
    []() {
      const auto a = aString(), b = aString();
      Matrix matrix = alloc_fill_matrix(a, b);
      Matrix alignment = find_alignment(scoring, a, b, matrix);
      vector<Matrix> unaffected, affected, affected2;
      tie(unaffected, affected) = remove_alignment({matrix}, alignment);

      for (const Matrix &mx : unaffected) {
        Matrix refilled = mx.duplicate();
        fill_matrix(scoring, a, b, refilled);

        RC_ASSERT(refilled == mx);
      }
      for (const Matrix &mx : affected) {
        Matrix refilled = mx.duplicate();
        Matrix updated = mx.duplicate();
        fill_matrix(scoring, a, b, refilled);
        update_matrix(scoring, a, b, updated);

        RC_ASSERT(refilled == updated);
      }
    });

  return 0;
}
