run_test: test
	./test
test: test.cpp test_utils.cpp test_utils.h WatermanEggert.h
	g++ -o test --std=c++11 test_utils.cpp test.cpp -lgtest -lgtest_main

.PHONY: run_test
