#ifndef WE_TEST_UTILS_H
#define WE_TEST_UTILS_H
#include "stringsim/stringsim.h"
#include <string>
extern const stringsim::Scoring scoring;
stringsim::Matrix alloc_fill_matrix(std::string a, std::string b);
stringsim::Matrix alloc_fill_matrix(std::string a, std::string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end);
#endif
