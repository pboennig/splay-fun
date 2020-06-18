TARGET_CPPS := insert_test.cpp 
CPP_FILES := $(filter-out $(TARGET_CPPS),$(wildcard *.cpp))
OBJ_FILES := $(CPP_FILES:.cpp=.o)
H_FILES   := $(wildcard *.h)

CC=/usr/local/opt/llvm/bin/clang++
CPP_FLAGS = -std=c++17 -Wall -Werror -Wpedantic -O0 -g -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib
all: run-tests

run-tests: $(OBJ_FILES) run_tests.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(H_FILES) Makefile
	$(CC) -c $(CPP_FLAGS) -o $@ $< 

.PHONY: clean

clean: 
	rm -f *.o run-tests 
