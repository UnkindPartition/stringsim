#ifndef WE_MATRIX_H
#define WE_MATRIX_H

#include <vector>
#include <cstddef>
#include <iterator>
#include <memory>

class Matrix {
  private:
    std::shared_ptr<std::vector<long>> matrix; // row-major
  public:
    const size_t rows, cols;
    const size_t row_begin, row_end, col_begin, col_end;
    Matrix(size_t rows, size_t cols) : rows (rows), cols (cols), row_begin (0), col_begin (0), row_end (rows), col_end (cols) {
      matrix = std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols));
    }
    Matrix(size_t rows, size_t cols, long x)
      : rows (rows), cols (cols), row_begin (0), col_begin (0), row_end (rows), col_end (cols) {
      matrix = std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols, x));
    }
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end)
      : rows (rows), cols (cols), row_begin (row_begin), col_begin (col_begin), row_end (row_end), col_end (col_end) {
      matrix = std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols));
    }
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end, long x)
      : rows (rows), cols (cols), row_begin (row_begin), col_begin (col_begin), row_end (row_end), col_end (col_end) {
      matrix = std::shared_ptr<std::vector<long>>(new std::vector<long>(rows * cols, x));
    }
    long operator()(size_t i, size_t j) const {
      return (*matrix)[i * cols + j];
    }
    long& operator()(size_t i, size_t j) {
      return (*matrix)[i * cols + j];
    }
    const std::vector<long>& elements() {
      return *matrix;
    }
};

#endif
