#include "tests.h"
#include "allocator.h"
#include <iostream>

void makeTest(int num, std::string title, int result) {
	std::cout << "Test " << num << ". " << title << ".\n";
	switch (result) {
		case 0:
			std::cout << "OK.\n";
			break;
		case 1:
			std::cout << "Allocator error.\n";
			break;
		case 2:
			std::cout << "Allocation error.\n";
			break;
		case 3:
			std::cout << "Memory reset error.\n";
			break;
	}
	std::cout << std::endl;
}

int main() {
	int result = 0;
	makeTest(1, "Making allocator for 100 b", makeAllocatorTest(100));
	makeTest(2, "Allocating 10 b", allocTest(10));
	makeTest(3, "Resetting", resetTest());
	makeTest(4, "Making allocator for 100 b", makeAllocatorTest(100));
	for (int i = 0; i < 5; i++) {
		makeTest(i + 5, "Allocating 20 b", allocTest(20));
	}
	makeTest(10, "Resetting", resetTest());
	makeTest(11, "Making allocator for 100 b", makeAllocatorTest(100));
	makeTest(12, "Allocating 0 b", allocTest(0));
	makeTest(13, "Allocating -10 b", allocTest(-10));
	makeTest(14, "Allocating 110 b", allocTest(110));
	makeTest(15, "Making allocator for 0 b", makeAllocatorTest(0));
	makeTest(16, "Allocating 10 b", allocTest(10));
	makeTest(17, "Allocating -10 b", allocTest(-10));
	makeTest(18, "Allocating 0 b", allocTest(0));
	makeTest(19, "Making allocator for 9223372036854775807 b", makeAllocatorTest(9223372036854775807));
	return 0;
}