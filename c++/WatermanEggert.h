#ifndef WATERMAN_EGGERT_H
#define WATERMAN_EGGERT_H

#include <vector>
#include <algorithm>

struct Scoring {
  long match_value,
         mismatch_value,
         space_value;
};

template<typename V> inline long compute_matrix_elt(Scoring scoring, const V &a, const V &b,
  size_t m, size_t n, size_t a_begin, size_t b_begin,
  const std::vector<long> &matrix, size_t i, size_t j) {

  long upper = i == a_begin ? 0 : matrix[(i-1) * n + j],
       left  = j == b_begin ? 0 : matrix[i * n + (j-1)],
       upper_left = (i == a_begin || j == b_begin) ? 0 : matrix[(i-1) * n + (j-1)];

  long match_mistmatch_value = a[i] == b[j] ? scoring.match_value : scoring.mismatch_value;

  return std::max({
      0L,
      upper_left + match_mistmatch_value,
      left + scoring.space_value,
      upper + scoring.space_value
  });
}

// Fill a sub-matrix a_begin to a_end, b_begin to b_end (half-open ranges)
template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end,
  std::vector<long> &matrix) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns
  if (a_begin >= a_end || b_begin >= b_end) {
    return;
  }

  for (size_t i = a_begin; i < a_end; i++) {
  for (size_t j = b_begin; j < b_end; j++) {
    matrix[i * n + j] = compute_matrix_elt(scoring, a, b, m, n, a_begin, b_begin, matrix, i, j);
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
  if (m == 0 || n == 0) {
    a_begin = a_end = b_begin = b_end = 0;
    return;
  }

  std::vector<long>::const_iterator end = std::max_element(matrix.begin(), matrix.end());

  size_t
    i = (end - matrix.begin()) / n,
    j = (end - matrix.begin()) % n;

  a_begin = a_end = i + 1;
  b_begin = b_end = j + 1;

  long this_value;

  // traceback
  while ((this_value = matrix[i*n+j]) > 0) {
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

// Take a matrix and the alignment coordinates. Clear the matrix entries
// associated with the aligned fragments.
static void clear_alignment(size_t m, size_t n, size_t a_begin, size_t a_end, size_t b_begin, size_t b_end,
  std::vector<long> &matrix) {
  // first, clear the horizontal strip from a_begin to a_end
  for (size_t i = a_begin; i < a_end; i++) {
  for (size_t j = 0; j < n ; j++) {
    matrix[i * n + j] = 0;
  }}
  // clear the top part of the vertical strip
  for (size_t i = 0; i < a_begin; i++) {
  for (size_t j = b_begin; j < b_end ; j++) {
    matrix[i * n + j] = 0;
  }}
  // clear the bottom part of the vertical strip
  for (size_t i = a_end; i < m; i++) {
  for (size_t j = b_begin; j < b_end ; j++) {
    matrix[i * n + j] = 0;
  }}
}

// Update a part of the matrix after the alignment is cleared
template<typename V> void update_matrix(Scoring scoring, const V &a, const V &b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end,
  std::vector<long> matrix) {

  const size_t m = a.size(), // rows
               n = b.size(); // columns

  ssize_t last_updated_col_this = -1, last_updated_col_prev;

  for (size_t i = a_begin; i < a_end; i++) {

    last_updated_col_prev = last_updated_col_this;
    last_updated_col_this = -1;

    for (size_t j = b_begin; j < b_end; j++) {
      long newval = compute_matrix_elt(scoring, a, b, m, n, a_begin, b_begin, matrix, i, j);
      if (newval != matrix[i*n + j]) {
        newval = matrix[i*n + j];
        last_updated_col_this = j;
      }
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
