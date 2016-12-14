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
