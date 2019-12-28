CC=g++ -std=c++17
TESTAPP = vector-test
EXEC_TEST=./$(TESTAPP)

all: build_test run_test

run_test:
	$(EXEC_TEST)

build_test: test.o
	$(CC) -o $(TESTAPP) test.o

test.o: test.cpp
	$(CC) -c test.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)

