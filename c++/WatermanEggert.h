#ifndef WATERMAN_EGGERT_H
#define WATERMAN_EGGERT_H

#include <vector>
#include <algorithm>

struct Scoring {
  long match_value,
         mismatch_value,
         space_value;
};

// Fill a sub-matrix a_begin to a_end, b_begin to b_end (half-open ranges)
template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end,
  std::vector<long> &matrix) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns
  if (a_begin >= a_end || b_begin >= b_end) {
    return;
  }

  // fill upper-left corner
  matrix[a_begin * n + b_begin] = std::max(
    a[a_begin] == b[b_begin] ? scoring.match_value : scoring.mismatch_value,
    0L
  );

  // fill upper row
  for (size_t j = b_begin + 1; j < b_end; j++) {
    matrix[a_begin * n + j] = std::max({
      matrix[j-1] + scoring.space_value,
      a[a_begin] == b[j] ? scoring.match_value : scoring.mismatch_value,
      0L});
  }
  // fill leftmost column
  for (size_t i = a_begin + 1; i < a_end; i++) {
    matrix[i * n + b_begin] = std::max({
      matrix[(i-1) * n] + scoring.space_value,
      a[i] == b[b_begin] ? scoring.match_value : scoring.mismatch_value,
      0L
    });
  }
  // fill the rest of the matrix
  for (size_t i = a_begin + 1; i < a_end; i++) {
  for (size_t j = b_begin + 1; j < b_end; j++) {
    matrix[i * n + j] = std::max({
      0L,
      matrix[(i-1) * n + (j-1)] + (a[i] == b[j] ? scoring.match_value : scoring.mismatch_value),
      matrix[(i-1) * n + j] + scoring.space_value,
      matrix[i * n + (j - 1)] + scoring.space_value
      });
  }}
}

// Unrestricted version of fill_matrix
template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b,
  std::vector<long> &matrix) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns
  fill_matrix(scoring, a, b, 0, m, 0, n, matrix);
}

// a_start and a_end are *half-open* ends of the aligned subsequence
template<typename V> void find_alignment(Scoring scoring, const V &a, const V &b,
  const std::vector<long> &matrix,
  size_t &a_begin, size_t &a_end, size_t &b_begin, size_t &b_end) {

  const size_t m = a.size(), // rows
               n = b.size(); // columns

  std::vector<long>::const_iterator end = std::max_element(matrix.begin(), matrix.end());

  size_t
    i = (end - matrix.begin()) / n,
    j = (end - matrix.begin()) % n;

  a_begin = a_end = i + 1;
  b_begin = b_end = j + 1;

  long this_value;

  // traceback
  while ((i > 0 || j > 0) && (this_value = matrix[i*n+j]) > 0) {
    a_begin = i;
    b_begin = j;

    if (i > 0 && j > 0 && this_value == matrix[(i-1) * n + (j-1)] + (a[i] == b[j] ? scoring.match_value : scoring.mismatch_value)) {
      i--; j--;
    } else if (i > 0 && this_value == matrix[(i-1) * n + j] + scoring.space_value) {
      i--;
    } else if (j > 0 && this_value == matrix[i * n + (j - 1)] + scoring.space_value) {
      j--;
    } else {
      break; // e.g. we are at (0,3), and this is the beginning
    }
  }
}

template<typename V> long similarity(Scoring scoring, const V &a, const V &b) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns

  if (m == 0 || n == 0) {
    return 0;
  }

  std::vector<long> matrix(m * n); // row-major

  fill_matrix(scoring, a, b, matrix);

  return *(std::max_element(matrix.begin(), matrix.end()));
}

#endif // WATERMAN_EGGERT_H
