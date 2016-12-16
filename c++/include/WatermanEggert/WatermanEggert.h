#ifndef WATERMAN_EGGERT_H
#define WATERMAN_EGGERT_H

#include <vector>
#include <algorithm>
#include "WatermanEggert/matrix.h"

struct Scoring {
  long match_value,
         mismatch_value,
         space_value;
};

template<typename V> inline long compute_matrix_elt(Scoring scoring, const V &a, const V &b,
  const Matrix &matrix, size_t i, size_t j) {

  long upper = i == matrix.row_begin ? 0 : matrix(i-1, j),
       left  = j == matrix.col_begin ? 0 : matrix(i, j-1),
       upper_left = (i == matrix.row_begin || j == matrix.col_begin) ? 0 : matrix(i-1, j-1);

  long match_mistmatch_value = a[i] == b[j] ? scoring.match_value : scoring.mismatch_value;

  return std::max({
      0L,
      upper_left + match_mistmatch_value,
      left + scoring.space_value,
      upper + scoring.space_value
  });
}

// Fill a sub-matrix row_begin to row_end, col_begin to col_end (half-open ranges)
template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b, Matrix &matrix) {
  if (matrix.row_begin >= matrix.row_end || matrix.col_begin >= matrix.col_end) {
    return;
  }

  for (size_t i = matrix.row_begin; i < matrix.row_end; i++) {
  for (size_t j = matrix.col_begin; j < matrix.col_end; j++) {
    matrix(i, j) = compute_matrix_elt(scoring, a, b, matrix, i, j);
  }}
}

// row_start and row_end are *half-open* ends of the aligned subsequence
template<typename V> void find_alignment(Scoring scoring, const V &a, const V &b,
  const std::vector<long> &matrix,
  size_t &row_begin, size_t &row_end, size_t &col_begin, size_t &col_end) {

  const size_t m = a.size(), // rows
               n = b.size(); // columns
  if (m == 0 || n == 0) {
    row_begin = row_end = col_begin = col_end = 0;
    return;
  }

  std::vector<long>::const_iterator end = std::max_element(matrix.begin(), matrix.end());

  size_t
    i = (end - matrix.begin()) / n,
    j = (end - matrix.begin()) % n;

  row_begin = row_end = i + 1;
  col_begin = col_end = j + 1;

  long this_value;

  // traceback
  while ((this_value = matrix[i*n+j]) > 0) {
    row_begin = i;
    col_begin = j;

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
static void clear_alignment(size_t m, size_t n, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end,
  std::vector<long> &matrix) {
  // first, clear the horizontal strip from row_begin to row_end
  for (size_t i = row_begin; i < row_end; i++) {
  for (size_t j = 0; j < n ; j++) {
    matrix[i * n + j] = 0;
  }}
  // clear the top part of the vertical strip
  for (size_t i = 0; i < row_begin; i++) {
  for (size_t j = col_begin; j < col_end ; j++) {
    matrix[i * n + j] = 0;
  }}
  // clear the bottom part of the vertical strip
  for (size_t i = row_end; i < m; i++) {
  for (size_t j = col_begin; j < col_end ; j++) {
    matrix[i * n + j] = 0;
  }}
}

// Update a part of the matrix after the alignment is cleared
template<typename V> void update_matrix(Scoring scoring, const V &a, const V &b,
  size_t row_begin, size_t row_end, size_t col_begin, size_t col_end,
  std::vector<long> &matrix) {

  const size_t m = a.size(), // rows
               n = b.size(); // columns

  ssize_t last_updated_col_this = -1, last_updated_col_prev;

  for (size_t i = row_begin; i < row_end; i++) {

    last_updated_col_prev = last_updated_col_this;
    last_updated_col_this = -1;

    for (size_t j = col_begin; j < col_end; j++) {
      long newval = compute_matrix_elt(scoring, a, b, m, n, row_begin, col_begin, matrix, i, j);
      if (newval != matrix[i*n + j]) {
        matrix[i*n + j] = newval;
        last_updated_col_this = j;
      }
    }
  }
}

// Update the whole matrix after the alignment is cleared. Unlike in
// update_matrix_all, here row_begin etc. refer to the alignment, not the
// submatrix to be updated!
template<typename V> void update_matrix_all(Scoring scoring, const V &a, const V &b,
  size_t row_begin, size_t row_end, size_t col_begin, size_t col_end,
  std::vector<long> &matrix) {

  const size_t m = a.size(), // rows
               n = b.size(); // columns

  // update upper-right corner
  update_matrix(scoring, a, b, 0, row_begin, col_end, n, matrix);
  // update lower-left corner
  update_matrix(scoring, a, b, row_end, m, 0, col_begin, matrix);
  // update lower-right corner
  update_matrix(scoring, a, b, row_end, m, col_end, n, matrix);
}

template<typename V> void clear_alignment_and_update(Scoring scoring, const V &a, const V &b,
  size_t row_begin, size_t row_end, size_t col_begin, size_t col_end,
  std::vector<long> &matrix) {

  clear_alignment(a.size(), b.size(), row_begin, row_end, col_begin, col_end, matrix);
  update_matrix_all(scoring, a, b, row_begin, row_end, col_begin, col_end, matrix);
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
