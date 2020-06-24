TARGET_CPPS := run_tests.cpp
CPP_FILES := $(filter-out $(TARGET_CPPS),$(wildcard *.cpp))
OBJ_FILES := $(CPP_FILES:.cpp=.o)
H_FILES   := $(wildcard *.h)

CPP_FLAGS = -std=c++17 -Wall -Werror -Wpedantic -O0 -g

all: run-tests

run-tests: $(OBJ_FILES) run_tests.o
	g++ -o $@ $^

%.o: %.cpp $(H_FILES) Makefile
	g++ -c $(CPP_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o run-tests explore *~
