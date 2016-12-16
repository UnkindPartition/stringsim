#ifndef WE_TEST_UTILS_H
#define WE_TEST_UTILS_H
#include "WatermanEggert/WatermanEggert.h"
#include <string>
extern const Scoring scoring;
Matrix alloc_fill_matrix(std::string a, std::string b);
Matrix alloc_fill_matrix(std::string a, std::string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end);
#endif
