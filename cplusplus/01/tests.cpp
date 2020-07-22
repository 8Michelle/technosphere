#include <iostream>
#include <string>
#include "allocator.h"
#include "tests.h"

int makeAllocatorTest(size_t maxSize) {
	try {
		makeAllocator(maxSize);
	} catch (std::bad_alloc& e) {
		return 1; // Allocation error
	}
	return 0; // OK
}

int allocTest(size_t size) {
	char* ptr = alloc(size);
	if (ptr != nullptr) {
		return 0; // ОК
	}
	return 2; // Не удалось выделить память нужного размера
}

int resetTest() {
	makeAllocator(10);
	char* ptr1 = alloc(10);
	reset();
	char* ptr2 = alloc(10);
	if (ptr2 != nullptr) {
		return 0; // ОК
	} else {
		return 3; // Не удалось сделать сброс
	}
}
