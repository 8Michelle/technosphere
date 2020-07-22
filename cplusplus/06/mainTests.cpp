#include "format.h"
#include "tests.h"
#include <iostream>
#include <string>

void makeTest(std::string title, Test test) {
	static int num = 1;
	std::cout << "Test " << num << ". " << title << ":\t";
	if (test() == 0) {
		std::cout << "OK.\n";
	}
	num++;
}

int main() {
	makeTest("Simple expressions test", testInit);
	makeTest("Different types test", testDiffTypes);
	makeTest("Runtime exceptions test", testExceptions);
	std::cout << "DONE\n";
	return 0;
}
