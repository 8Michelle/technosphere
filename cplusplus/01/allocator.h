#include <stddef.h>

//extern char* ptr_start;
//extern char* ptr_end;
//extern char* ptr_current;

void makeAllocator(size_t maxSize);
char* alloc(size_t size);
void reset();
