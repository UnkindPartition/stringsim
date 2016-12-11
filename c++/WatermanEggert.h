#ifndef WATERMAN_EGGERT_H
#define WATERMAN_EGGERT_H

#include <vector>
#include <algorithm>

struct Scoring {
  double match_value,
         mismatch_value,
         space_value;
};

template<typename V> double similarity(const Scoring &scoring, const V &a, const V &b) {
  const int m = a.size(), // rows
            n = b.size(); // columns

  if (m == 0 || n == 0) {
    return 0;
  }

  std::vector<double> matrix(m * n); // row-major

  // fill upper-left corner
  matrix[0] = a[0] == b[0] ? scoring.match_value : 0.0; // assuming mismatch_value <= 0 and match_value >= 0

  // fill upper row
  for (int j = 1; j < n; j++) {
    matrix[j] = std::max(matrix[j-1] + scoring.space_value, 0.0);
  }
  // fill leftmost column
  for (int i = 1; i < m; i++) {
    matrix[i * n] = std::max(matrix[(i-1) * n] + scoring.space_value, 0.0);
  }
  // fill the rest of the matrix
  for (int i = 1; i < m; i++) {
  for (int j = 1; j < n; j++) {
    matrix[i * n + j] = std::max({
      0.0,
      matrix[(i-1) * n + (j-1)] + (a[i] == b[i] ? scoring.match_value : scoring.mismatch_value),
      matrix[(i-1) * n + j],
      matrix[i * n + (j - 1)]
      });
  }}

  return *(std::max_element(matrix.begin(), matrix.end()));
}

#endif // WATERMAN_EGGERT_H
