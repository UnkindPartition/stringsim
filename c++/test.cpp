#include "WatermanEggert.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

vector<long> alloc_fill_matrix(Scoring scoring, string a, string b) {
  vector<long> matrix(a.size() * b.size());
  fill_matrix(scoring, a, b, matrix);
  return matrix;
}
vector<long> alloc_fill_matrix(Scoring scoring, string a, string b,
  size_t a_begin, size_t a_end, size_t b_begin, size_t b_end) {
  vector<long> matrix(a.size() * b.size(), 0);
  fill_matrix(scoring, a, b, a_begin, a_end, b_begin, b_end, matrix);
  return matrix;
}

const Scoring scoring = {
  .match_value = 3,
  .mismatch_value = -2,
  .space_value = -5
};

TEST(fill_matrix, case_one_mismatch) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "a", "b"), ElementsAre(0));
}
TEST(fill_matrix, case_one_match) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "a", "a"), ElementsAre(3));
}
TEST(fill_matrix, case_one_empty) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "abc", ""), ElementsAre());
}
TEST(fill_matrix, case_two_match) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "ab", "abc"), ElementsAre(3,0,0,0,6,1));
}
TEST(fill_matrix, case_mismatch_in_long_match) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "abcde", "abdde"), ElementsAreArray({
      3, 0, 0, 0, 0,
      0, 6, 1, 0, 0,
      0, 1, 4, 0, 0,
      0, 0, 4, 7, 2,
      0, 0, 0, 2, 10
      }));
}
TEST(fill_matrix, case_restricted_1) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "ac", "ab", 0, 2, 1, 2), ElementsAreArray({
      0, 0,
      0, 0,
      }));
}
TEST(fill_matrix, case_restricted_2) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "ac", "abc", 0, 2, 1, 3), ElementsAreArray({
      0, 0, 0,
      0, 0, 3,
      }));
}
TEST(fill_matrix, case_restricted_3) {
  ASSERT_THAT(alloc_fill_matrix(scoring, "abcde", "abdde", 2, 5, 1, 4), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 3, 3, 0,
      0, 0, 0, 1, 0
      }));
}
