CC=g++ -std=c++14
APP = taskulator
TESTAPP = taskulator-test
EXEC_RUN=./$(APP)
EXEC_TEST=./$(TESTAPP)

all: build_app build_test

run:
	$(EXEC_RUN)

test:
	$(EXEC_TEST)

build_app: main.o task.o
	$(CC) -o $(APP) main.o task.o

build_test: test.o task.o
	$(CC) -o $(TESTAPP) test.o task.o

main.o: main.cpp task.h
	$(CC) -c main.cpp

test.o: test.cpp task.h
	$(CC) -c test.cpp

task.o: task.cpp task.h
	$(CC) -c task.cpp

clean:
	rm -rf *.o $(APP) $(TESTAPP)
