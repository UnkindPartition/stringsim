#include "stringsim/stringsim.h"
#include "test_utils.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

TEST(fill_matrix, case_one_mismatch) {
  ASSERT_THAT(alloc_fill_matrix("a", "b").elements(), ElementsAre(0));
}
TEST(fill_matrix, case_one_match) {
  ASSERT_THAT(alloc_fill_matrix("a", "a").elements(), ElementsAre(3));
}
TEST(fill_matrix, case_one_empty) {
  ASSERT_THAT(alloc_fill_matrix("abc", "").elements(), ElementsAre());
}
TEST(fill_matrix, case_two_match) {
  ASSERT_THAT(alloc_fill_matrix("ab", "abc").elements(), ElementsAre(3,0,0,0,6,1));
}
TEST(fill_matrix, case_mismatch_in_long_match) {
  ASSERT_THAT(alloc_fill_matrix("abcde", "abdde").elements(), ElementsAreArray({
      3, 0, 0, 0, 0,
      0, 6, 1, 0, 0,
      0, 1, 4, 0, 0,
      0, 0, 4, 7, 2,
      0, 0, 0, 2, 10
      }));
}
TEST(fill_matrix, case_restricted_1) {
  ASSERT_THAT(alloc_fill_matrix("ac", "ab", 0, 2, 1, 2).all_elements(), ElementsAreArray({
      0, 0,
      0, 0,
      }));
}
TEST(fill_matrix, case_restricted_2) {
  ASSERT_THAT(alloc_fill_matrix("ac", "abc", 0, 2, 1, 3).all_elements(), ElementsAreArray({
      0, 0, 0,
      0, 0, 3,
      }));
}
TEST(fill_matrix, case_restricted_3) {
  ASSERT_THAT(alloc_fill_matrix("abcde", "abdde", 2, 5, 1, 4).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 3, 3, 0,
      0, 0, 0, 1, 0
      }));
}
TEST(fill_matrix, case_restricted_edge_cases) {
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 0, 0, 0).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 1, 0, 0).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 1, 0, 1).all_elements(), ElementsAreArray({
      3, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 2, 0, 1).all_elements(), ElementsAreArray({
      3, 0, 0, 0, 0,
      3, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 0, 3, 0, 2).all_elements(), ElementsAreArray({
      3, 3, 0, 0, 0,
      3, 6, 0, 0, 0,
      3, 6, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 1, 4, 0, 2).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      3, 3, 0, 0, 0,
      3, 6, 0, 0, 0,
      3, 6, 0, 0, 0,
      0, 0, 0, 0, 0
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 4, 5, 4, 5).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 3
      }));
  ASSERT_THAT(alloc_fill_matrix("aaaaa", "aaaaa", 4, 5, 3, 5).all_elements(), ElementsAreArray({
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 3, 3
      }));
}
TEST(find_alignment_and_update, case1) {
  const string a = "abbcccddb",
               b = "acccbb";
  auto matrix = alloc_fill_matrix(a, b);
  ASSERT_THAT(matrix.elements(), ElementsAreArray({
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

  Matrix alignment = find_alignment(scoring, a, b, matrix);
  ASSERT_EQ(Matrix(matrix, 3, 6, 1, 4), alignment);

  vector<Matrix> affected, unaffected;
  tie(unaffected, affected) = remove_alignment({matrix}, alignment);
  ASSERT_THAT(unaffected, UnorderedElementsAreArray({
    Matrix(matrix, 0, 3, 0, 1)
    }));
  ASSERT_THAT(affected, UnorderedElementsAreArray({
    Matrix(matrix, 6, 9, 0, 1),
    Matrix(matrix, 0, 3, 4, 6),
    Matrix(matrix, 6, 9, 4, 6),
    }));

  vector<Matrix> matrices = affected;
  matrices.insert(matrices.end(), unaffected.begin(), unaffected.end());
  for (Matrix &mx : matrices) {
    update_matrix(scoring, a, b, mx);
  }
  ASSERT_THAT(matrix.elements(), ElementsAreArray({
  //  a  c  c  c  b  b
      3, 0, 0, 0, 0, 0, // a
      0, 1, 0, 0, 3, 3, // b
      0, 0, 0, 0, 3, 6, // b
      0, 3, 3, 3, 0, 1, // c
      0, 3, 6, 6, 1, 0, // c
      0, 3, 6, 9, 4, 0, // c
      0, 0, 1, 4, 0, 0, // d
      0, 0, 0, 0, 0, 0, // d
      0, 0, 0, 0, 3, 3, // b
      }));

  Matrix alignment2 = choose_alignment(scoring, a, b, matrices);
  ASSERT_EQ(Matrix(matrix, 1, 3, 4, 6), alignment2);

  vector<Matrix> affected2, unaffected2;
  tie(unaffected2, affected2) = remove_alignment(matrices, alignment2);
  ASSERT_THAT(unaffected2, UnorderedElementsAreArray({
    Matrix(matrix, 6, 9, 0, 1)
    }));
  ASSERT_THAT(affected2, UnorderedElementsAreArray({
    Matrix(matrix, 0, 1, 0, 1)
    }));
}
TEST(similarity, case_simple) {
  ASSERT_EQ(0, similarity(scoring, string("foo"), string("faa")));
  ASSERT_EQ(2, similarity(scoring, string("fao"), string("faa")));
  ASSERT_EQ(0, similarity(scoring, string("aoa"), string("aia")));
  ASSERT_EQ(3, similarity(scoring, string("aoaa"), string("aiaa")));
  ASSERT_EQ(26, similarity(scoring, string("KingGeorge"), string("KingGeorge")));
}
TEST(similarity, case_twopart) {
  ASSERT_EQ(22, similarity(scoring, string("KingGeorge"), string("GeorgeKing")));
  ASSERT_EQ(22, similarity(scoring, string("King1George"), string("George2King")));
  ASSERT_EQ(22, similarity(scoring, string("King1George"), string("GeorgeKing")));
}
TEST(similarity, case_threepart) {
  ASSERT_EQ(33-12, similarity(scoring, string("OneTwoThree"), string("ThreeTwoOne")));
}
TEST(similarity, regr1) {
  ASSERT_EQ(1, similarity(Scoring({4L,-2L,-3L,-3L}), string("Ve"), string("Vi")));
}
