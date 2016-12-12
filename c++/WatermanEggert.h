#ifndef WATERMAN_EGGERT_H
#define WATERMAN_EGGERT_H

#include <vector>
#include <algorithm>

struct Scoring {
  double match_value,
         mismatch_value,
         space_value;
};

template<typename V> void fill_matrix(Scoring scoring, const V &a, const V &b, std::vector<double> &matrix) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns

  // fill upper-left corner
  matrix[0] = std::max(
    a[0] == b[0] ? scoring.match_value : scoring.mismatch_value,
    0.0
  );

  // fill upper row
  for (size_t j = 1; j < n; j++) {
    matrix[j] = std::max({
      matrix[j-1] + scoring.space_value,
      a[0] == b[j] ? scoring.match_value : scoring.mismatch_value,
      0.0});
  }
  // fill leftmost column
  for (size_t i = 1; i < m; i++) {
    matrix[i * n] = std::max({
      matrix[(i-1) * n] + scoring.space_value,
      a[i] == b[0] ? scoring.match_value : scoring.mismatch_value,
      0.0
    });
  }
  // fill the rest of the matrix
  for (size_t i = 1; i < m; i++) {
  for (size_t j = 1; j < n; j++) {
    matrix[i * n + j] = std::max({
      0.0,
      matrix[(i-1) * n + (j-1)] + (a[i] == b[j] ? scoring.match_value : scoring.mismatch_value),
      matrix[(i-1) * n + j] + scoring.space_value,
      matrix[i * n + (j - 1)] + scoring.space_value
      });
  }}
}

template<typename V> double similarity(Scoring scoring, const V &a, const V &b) {
  const size_t m = a.size(), // rows
               n = b.size(); // columns

  if (m == 0 || n == 0) {
    return 0;
  }

  std::vector<double> matrix(m * n); // row-major

  fill_matrix(scoring, a, b, matrix);

  return *(std::max_element(matrix.begin(), matrix.end()));
}

#endif // WATERMAN_EGGERT_H
