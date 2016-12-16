#ifndef WE_MATRIX_H
#define WE_MATRIX_H

#include <vector>
#include <cstddef>
#include <iterator>

class Matrix {
  private:
    std::vector<long> matrix; // row-major
  public:
    size_t m, n; // rows, columns
    Matrix(size_t m, size_t n) :
      m (m), n (n), matrix(std::vector<long>(m*n)) {}
    Matrix(size_t m, size_t n, long x) :
      m (m), n (n), matrix(std::vector<long>(m*n, x)) {}
    long operator()(size_t i, size_t j) const {
      return matrix[i * n + j];
    }
    long& operator()(size_t i, size_t j) {
      return matrix[i * n + j];
    }
    const std::vector<long>& elements() {
      return matrix;
    }
};

class Cell : public Matrix {
  public:
    size_t a_begin, a_end, b_begin, b_end;

    Cell(size_t m, size_t n) :
      Matrix(m, n), a_begin (0), b_begin (0), a_end (m), b_end (n) {};

    Cell(Matrix mx) :
      Matrix(mx), a_begin (0), b_begin (0), a_end (mx.m), b_end (mx.n) {};

    Cell(Matrix mx, size_t a_begin, size_t a_end, size_t b_begin, size_t b_end) :
      Matrix(mx), a_begin(a_begin), a_end(a_end), b_begin(b_begin), b_end(b_end) {};
};

#endif
