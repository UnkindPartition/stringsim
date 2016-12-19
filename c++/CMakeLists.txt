cmake_minimum_required (VERSION 2.6)
project (WatermanEggert)
enable_testing()

option(BUILD_TESTS "Build the tests" ON)

add_library(WatermanEggert INTERFACE)
include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -Wno-long-long -pedantic")

if (BUILD_TESTS)
  add_library(test_utils test/test_utils.cpp)

  # Unit tests
  add_subdirectory("googletest")
  add_executable(unit_tests test/unit_tests.cpp)
  target_link_libraries (unit_tests gmock gtest gtest_main test_utils)
  add_test(unit_tests ./unit_tests)
  add_test(property_tests ./property_tests)

  # Property tests
  add_subdirectory("rapidcheck")
  add_executable(property_tests test/property_tests.cpp)
  target_link_libraries(property_tests rapidcheck test_utils)

endif (BUILD_TESTS)
