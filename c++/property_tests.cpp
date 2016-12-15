#include <rapidcheck.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "WatermanEggert.h"
#include "test_utils.h"

using namespace std;

inline vector<char> aString() { return *rc::gen::container<std::vector<char>>(rc::gen::inRange('a', 'e')); }

int main() {
  rc::check("fill_matrix is properly restricted",
    []() {
      const auto s1 = aString(), s2 = aString();


    });

  return 0;
}
