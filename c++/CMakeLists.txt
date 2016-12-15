cmake_minimum_required (VERSION 2.6)
project (WatermanEggert)
enable_testing()

option(BUILD_TESTS "Build the tests" ON)

add_library(WatermanEggert INTERFACE)

if (BUILD_TESTS)
  add_library(test_utils test_utils.cpp)

  find_package(GTest REQUIRED)
  add_executable(unit_tests unit_tests.cpp)
  include_directories(${GTest_INCLUDE_DIRS})
  target_link_libraries (unit_tests ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} test_utils)
  add_test(unit_tests ./unit_tests)
endif (BUILD_TESTS)
