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

template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b, Matrix &matrix) {
  if (matrix.row_begin >= matrix.row_end || matrix.col_begin >= matrix.col_end) {
    return;
  }

  for (size_t i = matrix.row_begin; i < matrix.row_end; i++) {
  for (size_t j = matrix.col_begin; j < matrix.col_end; j++) {
    matrix(i, j) = compute_matrix_elt(scoring, a, b, matrix, i, j);
  }}
}

template<typename V> Matrix find_alignment(Scoring scoring, const V &a, const V &b, const Matrix &matrix) {
  if (matrix.empty()) {
    return Matrix(matrix, 0, 0, 0, 0);
  }
  size_t row_begin, row_end, col_begin, col_end;

  size_t i, j;
  std::tie(i,j) = matrix.max_element();

  row_begin = row_end = i + 1;
  col_begin = col_end = j + 1;

  long this_value;

  // traceback
  while ((this_value = matrix(i,j)) > 0) {
    row_begin = i;
    col_begin = j;

    if (i > 0 && j > 0 && this_value == matrix(i-1,j-1) + (a[i] == b[j] ? scoring.match_value : scoring.mismatch_value)) {
      i--; j--;
    } else if (i > 0 && this_value == matrix(i-1,j) + scoring.space_value) {
      i--;
    } else if (j > 0 && this_value == matrix(i,j-1) + scoring.space_value) {
      j--;
    } else {
      break; // e.g. we are at (0,3), and this is the beginning
    }
  }
  return Matrix(matrix, row_begin, row_end, col_begin, col_end);
}

// A helper function. Pushed a matrix into a vector unless the matrix is empty.
inline void push_if_not_empty(std::vector<Matrix> &vec, const Matrix &mx) {
  if (!mx.empty()) {
    vec.push_back(mx);
  }
}

// Given a vector of matrices and a local alignment, return:
//
// 1. The vector of matrices that don't intersect the alignment
// 2. The vector of matrices that intercepted the alignment, but split into
//    smaller matrices that do not.
inline std::pair<std::vector<Matrix>,std::vector<Matrix>> remove_alignment(
  std::vector<Matrix> matrices, Matrix alignment) {

  std::vector<Matrix> unaffected, affected;

  for (Matrix mx : matrices) {
    bool rows_intersect = !(mx.row_end <= alignment.row_begin || alignment.row_end <= mx.row_begin),
         cols_intersect = !(mx.col_end <= alignment.col_begin || alignment.col_end <= mx.col_begin);
    if (rows_intersect && cols_intersect) {
      push_if_not_empty(affected, Matrix(mx, mx.row_begin, alignment.row_begin, mx.col_begin, alignment.col_begin));
      push_if_not_empty(affected, Matrix(mx, mx.row_begin, alignment.row_begin, alignment.col_end, mx.col_end));
      push_if_not_empty(affected, Matrix(mx, alignment.row_end, mx.row_end, mx.col_begin, alignment.col_begin));
      push_if_not_empty(affected, Matrix(mx, alignment.row_end, mx.row_end, alignment.col_end, mx.col_end));
    } else if (rows_intersect) {
      push_if_not_empty(affected, Matrix(mx, mx.row_begin, alignment.row_begin, mx.col_begin, mx.col_end));
      push_if_not_empty(affected, Matrix(mx, alignment.row_end, mx.row_end, mx.col_begin, mx.col_end));

    } else if (cols_intersect) {
      push_if_not_empty(affected, Matrix(mx, mx.row_begin, mx.row_end, mx.col_begin, alignment.col_begin));
      push_if_not_empty(affected, Matrix(mx, mx.row_begin, mx.row_end, alignment.col_end, mx.col_end));
    } else {
      unaffected.push_back(mx);
    }
  }
  return std::pair<std::vector<Matrix>,std::vector<Matrix>>(unaffected, affected);
}

// Update a recently cut off part of the matrix
template<typename V> void update_matrix(Scoring scoring, const V &a, const V &b, Matrix &matrix) {
  ssize_t last_updated_col_this = -1, last_updated_col_prev;

  for (size_t i = matrix.row_begin; i < matrix.row_end; i++) {

    last_updated_col_prev = last_updated_col_this;
    last_updated_col_this = -1;

    for (size_t j = matrix.col_begin; j < matrix.col_end; j++) {
      long newval = compute_matrix_elt(scoring, a, b, matrix, i, j);
      if (newval != matrix(i,j)) {
        matrix(i, j) = newval;
        last_updated_col_this = j;
      }
    }
  }
}

template<typename V> long similarity(Scoring scoring, const V &a, const V &b) {
  // TODO
  return 0;
}

#endif // WATERMAN_EGGERT_H
