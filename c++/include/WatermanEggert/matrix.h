#ifndef WE_MATRIX_H
#define WE_MATRIX_H

#include <vector>
#include <cstddef>
#include <utility>
#include <memory>

class Matrix {
  private:
    std::shared_ptr<std::vector<long>> matrix; // row-major
    const size_t rows, cols;
  public:
    const size_t row_begin, row_end, col_begin, col_end;
    Matrix(size_t rows, size_t cols) : rows (rows), cols (cols), row_begin (0), row_end (rows), col_begin (0), col_end (cols) {
      matrix = std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols));
    }
    Matrix(size_t rows, size_t cols, long x)
      : matrix (std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols, x))),
        rows (rows), cols (cols), row_begin (0), row_end (rows), col_begin (0), col_end (cols)
    {}
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end)
      : matrix (std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols))),
        rows (rows), cols (cols), row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end, long x)
      : matrix (std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols, x))),
        rows (rows), cols (cols), row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    // Create a submatrix that refers to a different part of the same underlying matrix.
    Matrix(const Matrix &mx, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end)
      : matrix (mx.matrix), rows (mx.rows), cols (mx.cols),
        row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    // Is the view empty?
    bool empty() const {
      return (row_begin >= row_end || col_begin >= col_end);
    }
    // Are all elements in the view <= than a given number?
    bool all_le(long x) const {
      for (size_t i = row_begin; i < row_end; i++) {
      for (size_t j = col_begin; j < col_end; j++) {
        if (operator()(i,j) > x) {
          return false;
        }
      }}
      return true;
    }
    long operator()(size_t i, size_t j) const {
      return (*matrix)[i * cols + j];
    }
    long& operator()(size_t i, size_t j) {
      return (*matrix)[i * cols + j];
    }
    const std::vector<long>& elements() const {
      return *matrix;
    }
    // Returns the coordinates of the maximum element within the submatrix.
    // Relies on the fact that the matrix is non-negative.
    std::pair<size_t,size_t> max_element() const {
      size_t max_i = 0, max_j = 0;
      long max_val = 0;
      for (size_t i = row_begin; i < row_end; i++) {
      for (size_t j = col_begin; j < col_end; j++) {
        if (operator()(i,j) > max_val) {
          max_i = i;
          max_j = j;
        }
      }}
      return std::pair<size_t,size_t>(max_i,max_j);
    }
};

#endif