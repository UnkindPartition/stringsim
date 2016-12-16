#include "WatermanEggert/WatermanEggert.h"
#include "test_utils.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

TEST(fill_matrix, case_one_mismatch) {
  ASSERT_THAT(alloc_fill_matrix("a", "b"), ElementsAre(0));
}
TEST(fill_matrix, case_one_match) {
  ASSERT_THAT(alloc_fill_matrix("a", "a"), ElementsAre(3));
}
TEST(fill_matrix, case_one_empty) {
  ASSERT_THAT(alloc_fill_matrix("abc", ""), ElementsAre());
}
TEST(fill_matrix, case_two_match) {
  ASSERT_THAT(alloc_fill_matrix("ab", "abc"), ElementsAre(3,0,0,0,6,1));
}
TEST(fill_matrix, case_mismatch_in_long_match) {
  ASSERT_THAT(alloc_fill_matrix("abcde", "abdde"), ElementsAreArray({
      3, 0, 0, 0, 0,
      0, 6, 1, 0, 0,
      0, 1, 4, 0, 0,
      0, 0, 4, 7, 2,
      0, 0, 0, 2, 10
      }));
}
TEST(fill_matrix, case_restricted_1) {
  ASSERT_THAT(alloc_fill_matrix("ac", "ab", 0, 2, 1, 2), ElementsAreArray({
      0, 0,
      0, 0,
      }));
}
TEST(fill_matrix, case_restricted_2) {
  ASSERT_THAT(alloc_fill_matrix("ac", "abc", 0, 2, 1, 3), ElementsAreArray({
      0, 0, 0,
      0, 0, 3,
      }));
}
TEST(fill_matrix, case_restricted_3) {
  ASSERT_THAT(alloc_fill_matrix("abcde", "abdde", 2, 5, 1, 4), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 3, 3, 0,
      0, 0, 0, 1, 0
      }));
}
TEST(fill_matrix, case_restricted_edge_cases) {
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 0, 0, 0), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 1, 0, 0), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 1, 0, 1), ElementsAreArray({
      3, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 2, 0, 1), ElementsAreArray({
      3, 0, 0, 0, 0,
      3, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 3, 0, 2), ElementsAreArray({
      3, 3, 0, 0, 0,
      3, 6, 0, 0, 0,
      3, 6, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 1, 4, 0, 2), ElementsAreArray({
      0, 0, 0, 0, 0,
      3, 3, 0, 0, 0,
      3, 6, 0, 0, 0,
      3, 6, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 4, 5, 4, 5), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 3
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 4, 5, 3, 5), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 3, 3
      }));
}
TEST(clear_alignment, case_middle) {
  const int m = 5, n = 4;
  vector<long> matrix(m*n, 1);
  clear_alignment(m, n, 2, 4, 1, 3, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
      1, 0, 0, 1,
      1, 0, 0, 1,
      0, 0, 0, 0,
      0, 0, 0, 0,
      1, 0, 0, 1,
      }));
}
TEST(clear_alignment, case_top_left) {
  const int m = 5, n = 4;
  vector<long> matrix(m*n, 1);
  clear_alignment(m, n, 0, 2, 0, 1, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 1, 1, 1,
      0, 1, 1, 1,
      0, 1, 1, 1,
      }));
}
TEST(clear_alignment, case_bottom_right) {
  const int m = 5, n = 4;
  vector<long> matrix(m*n, 1);
  clear_alignment(m, n, 2, 5, 2, 4, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
      1, 1, 0, 0,
      1, 1, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      }));
}
TEST(clear_alignment, case_all) {
  const int m = 5, n = 4;
  vector<long> matrix(m*n, 1);
  clear_alignment(m, n, 0, 5, 0, 4, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      }));
}
TEST(clear_alignment_and_update, case1) {
  const string a = "abbcccddb",
               b = "acccbb";
  auto matrix = alloc_fill_matrix(a, b);
  size_t a_begin, a_end, b_begin, b_end;
  ASSERT_THAT(matrix, ElementsAreArray({
  //  a  c  c  c  b  b
      3, 0, 0, 0, 0, 0, // a
      0, 1, 0, 0, 3, 3, // b
      0, 0, 0, 0, 3, 6, // b
      0, 3, 3, 3, 0, 1, // c
      0, 3, 6, 6, 1, 0, // c
      0, 3, 6, 9, 4, 0, // c
      0, 0, 1, 4, 7, 2, // d
      0, 0, 0, 0, 2, 5, // d
      0, 0, 0, 0, 3, 5, // b
      }));

  find_alignment(scoring, a, b, matrix, a_begin, a_end, b_begin, b_end);
  ASSERT_EQ(3, a_begin); // eTwenty
  ASSERT_EQ(6, a_end);
  ASSERT_EQ(1, b_begin);
  ASSERT_EQ(4, b_end);
  clear_alignment_and_update(scoring, a, b, a_begin, a_end, b_begin, b_end, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
  //  a  c  c  c  b  b
      3, 0, 0, 0, 0, 0, // a
      0, 0, 0, 0, 3, 3, // b
      0, 0, 0, 0, 3, 6, // b
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // d
      0, 0, 0, 0, 0, 0, // d
      0, 0, 0, 0, 3, 3, // b
      }));

  find_alignment(scoring, a, b, matrix, a_begin, a_end, b_begin, b_end);
  ASSERT_EQ(1, a_begin); // eTwenty
  ASSERT_EQ(3, a_end);
  ASSERT_EQ(4, b_begin);
  ASSERT_EQ(6, b_end);
  clear_alignment_and_update(scoring, a, b, a_begin, a_end, b_begin, b_end, matrix);
  ASSERT_THAT(matrix, ElementsAreArray({
  //  a  c  c  c  b  b
      3, 0, 0, 0, 0, 0, // a
      0, 0, 0, 0, 0, 0, // b
      0, 0, 0, 0, 0, 0, // b
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // c
      0, 0, 0, 0, 0, 0, // d
      0, 0, 0, 0, 0, 0, // d
      0, 0, 0, 0, 0, 0, // b
      }));
}
