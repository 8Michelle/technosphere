#include "bigInt.h"
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
	makeTest("Small int init", testInit);
	makeTest("Small int assignment", testAssignment);
	makeTest("Small int equality", testEquality);
	makeTest("Small int sum", testSum);
	makeTest("Big int sum", testBigSum);
	makeTest("Big int equality", testEquality);
	std::cout << "DONE.\n";
	return 0;
}
