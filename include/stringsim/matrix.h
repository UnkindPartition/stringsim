#ifndef STRINGSIM_MATRIX_H
#define STRINGSIM_MATRIX_H

#include <vector>
#include <cstddef>
#include <utility>
#include <memory>
#include <cassert>

namespace stringsim {

class Matrix {
  private:
    std::shared_ptr<std::vector<long>> matrix; // row-major
    size_t rows, cols;
  public:
    size_t row_begin, row_end, col_begin, col_end;
    Matrix(size_t rows, size_t cols)
      : matrix(std::make_shared<std::vector<long>>(rows * cols)),
        rows (rows), cols (cols), row_begin (0), row_end (rows), col_begin (0), col_end (cols)
    {}
    Matrix(size_t rows, size_t cols, long x)
      : matrix (std::make_shared<std::vector<long>>(rows * cols, x)),
        rows (rows), cols (cols), row_begin (0), row_end (rows), col_begin (0), col_end (cols)
    {}
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end)
      : matrix (std::make_shared<std::vector<long>>(rows * cols)),
        rows (rows), cols (cols), row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    Matrix(size_t rows, size_t cols, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end, long x)
      : matrix (std::make_shared<std::vector<long>>(rows * cols, x)),
        rows (rows), cols (cols), row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    // Create a submatrix that refers to a different part of the same underlying matrix.
    Matrix(const Matrix &mx, size_t row_begin, size_t row_end, size_t col_begin, size_t col_end)
      : matrix (mx.matrix), rows (mx.rows), cols (mx.cols),
        row_begin (row_begin), row_end (row_end), col_begin (col_begin), col_end (col_end)
    {}
    ~Matrix() = default;

    Matrix duplicate() const {
      Matrix copy(*this);
      // copy the payload
      copy.matrix = std::make_shared<std::vector<long>>(*matrix);
      return copy;
    }

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
      assert(i >= row_begin);
      assert(j >= col_begin);
      assert(i < row_end);
      assert(j < col_end);
      return (*matrix)[i * cols + j];
    }
    long& operator()(size_t i, size_t j) {
      return (*matrix)[i * cols + j];
    }
    bool operator==(const Matrix &other) const {
      return
        elements() ==  other.elements() &&
        rows       ==  other.rows       &&
        cols       ==  other.cols       &&
        row_begin  ==  other.row_begin  &&
        row_end    ==  other.row_end    &&
        col_begin  ==  other.col_begin  &&
        col_end    ==  other.col_end    ;
    }
    // Allocate and return a vector with elements of the matrix view.
    // Intended for testing only.
    std::vector<long> elements() const {
      std::vector<long> result;
      for (size_t i = row_begin; i < row_end; i++) {
      for (size_t j = col_begin; j < col_end; j++) {
        result.push_back(operator()(i,j));
      }}
      return result;
    }
    // Return *all* elements (not just in the view).
    // Intended for testing only.
    const std::vector<long>& all_elements() const {
      return *matrix;
    }
    // Returns the coordinates of the maximum element within the submatrix.
    // Relies on the fact that the matrix is non-negative.
    std::pair<size_t,size_t> max_element() const {
      assert(!empty());
      size_t max_i = row_begin, max_j = col_begin;
      long max_val = 0;
      for (size_t i = row_begin; i < row_end; i++) {
      for (size_t j = col_begin; j < col_end; j++) {
        if (operator()(i,j) > max_val) {
          max_i = i;
          max_j = j;
          max_val = operator()(i,j);
        }
      }}
      assert(max_i >= row_begin);
      assert(max_j >= col_begin);
      assert(max_i < row_end);
      assert(max_j < col_end);
      return std::pair<size_t,size_t>(max_i,max_j);
    }
    long max_value() const {
      long max_val = 0;
      for (size_t i = row_begin; i < row_end; i++) {
      for (size_t j = col_begin; j < col_end; j++) {
        if (operator()(i,j) > max_val) {
          max_val = operator()(i,j);
        }
      }}
      return max_val;
    }
};

} // namespace stringsim

#endif
