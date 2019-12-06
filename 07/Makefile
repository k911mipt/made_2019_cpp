CC=g++ -std=c++17
TESTAPP = printf-test
EXEC_TEST=./$(TESTAPP)

all: build_test test

test:
	$(EXEC_TEST)

build_test: test.o
	$(CC) -o $(TESTAPP) test.o

test.o: test.cpp print.hpp print_tests.hpp
	$(CC) -c test.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)

