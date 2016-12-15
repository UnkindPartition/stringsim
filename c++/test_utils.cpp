#include "WatermanEggert.h"
#include "test_utils.h"

using namespace std;

const Scoring scoring = {
  .match_value = 3,
  .mismatch_value = -2,
  .space_value = -5
};

vector<long> alloc_fill_matrix(string a, string b) {
  vector<long> matrix(a.size() * b.size());
  fill_matrix(scoring, a, b, matrix);
  return matrix;
}
vector<long> alloc_fill_matrix(string a, string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end) {
  vector<long> matrix(a.size() * b.size(), 0);
  fill_matrix(scoring, a, b, a_begin, a_end, b_begin, b_end, matrix);
  return matrix;
}
