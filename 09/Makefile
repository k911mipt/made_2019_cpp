CC=g++ -std=c++17
TESTAPP = thread-test
EXEC_TEST=./$(TESTAPP)
FLAGS = -pthread

all: build_test run_test

run_test:
	$(EXEC_TEST)

build_test: test.o thread_pool_tests.o thread_pool.o
	$(CC) $(FLAGS) -o $(TESTAPP) test.o thread_pool_tests.o thread_pool.o

test.o: test.cpp thread_pool_tests.hpp
	$(CC) -c test.cpp

thread_pool.o: thread_pool.cpp thread_pool.hpp
	$(CC) -c thread_pool.cpp

thread_pool_tests.o: thread_pool_tests.cpp thread_pool_tests.hpp
	$(CC) -c thread_pool_tests.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)

