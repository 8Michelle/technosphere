#include <cstddef>
#include <limits>
#include <cstdlib>
#include <new>

template <class T>
class Allocator {
public:
	using value_type = T;
	using pointer = T*;
	using size_type = size_t;
	pointer allocate(size_type count) const {
		if (count > max_size()) {
			throw std::bad_alloc();
		}
		pointer ptr = static_cast<pointer>(std::malloc(count * sizeof(value_type)));
		if (ptr) {
			return ptr;
		} else {
			throw std::bad_alloc();
		}
	}
	void deallocate(pointer ptr, size_type count) const {
		std::free(ptr);
	}
	size_type max_size() const {
		return std::numeric_limits<size_type>::max();
	}
};