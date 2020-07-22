#include "threadPool.h"
#include <cassert>
#include <iostream>

int testFunction(int i) {
	return i;
}

struct TestStruct {
public:
	TestStruct(int i):
	data(i) {
	}
	int data;
};

int testStructFunction(TestStruct testStruct) {
	return testStruct.data;
}

int main() {
	ThreadPool pool(4);
	for (int i = 0; i < 10; ++i) {
		assert(pool.exec(testFunction, i).get() == i);
	}
	std::cout << "Simple function test: OK\n";
	for (int i = 10; i < 15; ++i) {
		assert(pool.exec(testStructFunction, TestStruct(i)).get() == i);
	}
	std::cout << "Struct function test: OK\n";
	std::cout << "DONE\n";
	return 0;
}
