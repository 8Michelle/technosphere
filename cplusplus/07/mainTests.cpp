#include "vector.h"
#include "tests.h"
#include <iostream>

void makeTest(std::string title, Test test) {
	static int num = 1;
	std::cout << "Test " << num << ". " << title << ":\t";
	if (test() == 0) {
		std::cout << "OK.\n";
	}
	num++;
}

int main() {
	makeTest("Init vector", testInit);
	makeTest("Push item", testPush);
	makeTest("Pop item", testPop);
	makeTest("Size operations", testSize);
	makeTest("Iterator", testIterator);
	return 0;
}
