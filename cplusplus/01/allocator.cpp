#include "allocator.h"
#include <iostream>

char* ptr_start = nullptr;
char* ptr_end = nullptr;
char* ptr_current = nullptr;

void makeAllocator(size_t maxSize) {
    ptr_start = (char*) malloc(maxSize);
	ptr_current = ptr_start;
	if (ptr_start != nullptr) {
		ptr_end = ptr_current + maxSize;  /*так как char занимает 1 байт, нет необходимости
                                     * делить maxSize на sizeof(char)*/
    } else {
		throw std::bad_alloc();
	}
}

char* alloc(size_t size) {
    if (ptr_end - ptr_current < size ||
			(int) size < 0 ||
			ptr_start == nullptr) {
        return nullptr;
    }
    char* ptr = ptr_current;
    ptr_current += size;
    return ptr;
}

void reset() {
    ptr_current = ptr_start;
}
