CC=g++ -std=c++17
TESTAPP = printf-test
EXEC_TEST=./$(TESTAPP)

all: build_test test

test:
	$(EXEC_TEST)

build_test: test.o print_tests.o print.o
	$(CC) -o $(TESTAPP) test.o print_tests.o print.o

test.o: test.cpp print_tests.hpp print.hpp
	$(CC) -c test.cpp

print_tests.o: print_tests.cpp print_tests.hpp print.hpp
	$(CC) -c print_tests.cpp

print.o: print.cpp print.hpp
	$(CC) -c print.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)

