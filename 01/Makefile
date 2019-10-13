CC=g++
APP = calculator
TESTAPP = calculator-test
EXEC_RUN=./$(APP)
EXEC_TEST=./$(TESTAPP)

all: build_app build_test

run:
	$(EXEC_RUN)

test:
	$(EXEC_TEST)

build_app: main.o calc.o
	$(CC) -o $(APP) main.o calc.o

build_test: test.o calc.o
	$(CC) -o $(TESTAPP) test.o calc.o

main.o: main.cpp calc.h
	$(CC) -c main.cpp

test.o: test.cpp calc.h
	$(CC) -c test.cpp

calc.o: calc.cpp calc.h
	$(CC) -c calc.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)
