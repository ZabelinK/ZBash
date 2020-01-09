all: Executor
	c++ -std=c++11 -o ./build/main ./src/main.cpp ./build/Executor.o -I"./include/"

test: Executor
	c++ -std=c++11 -o ./build/test/main ./test/main.cpp ./build/Executor.o -I"./include/"

Executor:
	c++ -std=c++11 -c -o ./build/Executor.o ./src/Executor.cpp -I"./include/"
