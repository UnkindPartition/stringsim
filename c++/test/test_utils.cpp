#include "WatermanEggert/WatermanEggert.h"
#include "test_utils.h"

using namespace std;

const Scoring scoring = {
  .match_value = 3,
  .mismatch_value = -2,
  .space_value = -5
};

Cell alloc_fill_matrix(string a, string b) {
  Cell cell(a.size(), b.size());
  fill_matrix(scoring, a, b, cell);
  return cell;
}
Cell alloc_fill_matrix(string a, string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end) {
  Cell cell(Matrix(a.size(), b.size(), 0), a_begin, a_end, b_begin, b_end);
  fill_matrix(scoring, a, b, cell);
  return cell;
}
