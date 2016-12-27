#include "WatermanEggert/WatermanEggert.h"
#include "test_utils.h"

using namespace std;

const Scoring scoring = { 3, -2, -5, -4 };

Matrix alloc_fill_matrix(string a, string b) {
  Matrix matrix(a.size(), b.size());
  fill_matrix(scoring, a, b, matrix);
  return matrix;
}
Matrix alloc_fill_matrix(string a, string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end) {
  Matrix matrix(a.size(), b.size(), a_begin, a_end, b_begin, b_end, 0L);
  fill_matrix(scoring, a, b, matrix);
  return matrix;
}
