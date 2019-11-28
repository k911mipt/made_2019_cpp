CC=g++ -std=c++17
TESTAPP = bigint-test
EXEC_TEST=./$(TESTAPP)

all: build_test test

test:
	$(EXEC_TEST)

build_test: test.o
	$(CC) -o $(TESTAPP) test.o

test.o: test.cpp long_arithmetic.hpp
	$(CC) -c test.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)

