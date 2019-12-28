CC=g++ -std=c++14
TESTAPP = tokenizer-test
EXEC_TEST=./$(TESTAPP)

all: build_test test

test:
	$(EXEC_TEST)

build_test: test.o
	$(CC) -o $(TESTAPP) test.o

test.o: test.cpp tokenizer.h
	$(CC) -c test.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)
