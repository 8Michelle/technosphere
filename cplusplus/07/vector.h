#include "allocator.h"
#include "iterator.h"
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <memory>

template <class T,
		class Alloc = Allocator<T>,
		class Iter = Iterator<T>,
		class Riter = ReverseIterator<T> >
class Vector {
public:
	using size_type = size_t;
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using allocator_type = Alloc;
	using iterator = Iter;
	using reverse_iterator = Riter;

	Vector(size_type count = 0):
			size_(count),
			capacity_(count) {
		data_ = allocator.allocate(count);
		value_type defaultValue;
		value_type* current = data_;
		const value_type* end = data_ + size_;
		while(current != end) {
			std::allocator_traits<allocator_type>::construct(allocator, current++);
		}
	}
	Vector(size_t count, const T& defaultValue):
	Vector<T>(count) {
		value_type* current = data_;
		const value_type* end = data_ + size_;
		while(current != end) {
			std::allocator_traits<allocator_type>::construct(allocator, current++, defaultValue);
		}
	}

	Vector(std::initializer_list<T> init):
	Vector<T>(init.size()) {
		size_type i = 0;
		auto current = init.begin();
		const auto end = init.end();
		while(current != end) {
			data_[i++] = *current++;
		}
	}

	~Vector() {
		delete_data_();
		allocator.deallocate(data_, capacity_);
	}

	iterator begin() const noexcept {
		return Iterator<T>(data_);
	}
	reverse_iterator rbegin() const noexcept {
		return ReverseIterator<T>(data_);
	}
	iterator end() const noexcept {
		return Iterator<T>(data_ + size_);
	}
	reverse_iterator rend() const noexcept {
		return ReverseIterator<T>(data_ + size_);
	}

	void push_back(value_type&& value) {
		if (size_ == capacity_) {
			reserve(capacity_ * 2);
		}
		data_[size_] = std::move(value);
		++size_;
	}

	void push_back(const value_type& value) {
		if (size_ == capacity_) {
			reserve(capacity_ * 2);
		}
		data_[size_] = value;
		++size_;
	}

	value_type pop_back() {
		value_type tmp = data_[size_ - 1];
		--size_;
		return tmp;
	}

	reference operator[](size_type index) {
		if (index < 0 || index >= size_) {
			throw std::out_of_range("");
		}
		reference ref_tmp = data_[index];
		return ref_tmp;
	}

	void resize(size_type newSize) {
		if (newSize == size_) {
			return;
		} else if (newSize > size_) {
			reserve(newSize);
		}
		size_ = newSize;
	}

	void resize(size_type newSize, const value_type& defaultValue) {
		if (newSize == size_) {
			return;
		} else if (newSize > size_) {
			reserve(newSize);
			for (size_type i = size_; i < newSize; ++i) {
				data_[i] = defaultValue;
			}
		}
		size_ = newSize;
	}

	void reserve(size_type count) {
		if (count <= capacity_) {
			return;
		}
		value_type* tmp_data_ = allocator.allocate(count);
		std::copy(data_, data_ + size_, tmp_data_);
		delete_data_();
		allocator.deallocate(data_, capacity_);
		data_ = tmp_data_;
		capacity_ = count;
	}

	bool empty() const {
		return (size_ == 0);
	}

	size_type size() const {
		return size_;
	}

	void clear() {
		size_ = 0;
	}

	size_type capacity() const {
		return capacity_;
	}

private:
	allocator_type allocator;
	size_type size_;
	size_type capacity_;
	value_type* data_;
	void delete_data_() {
		value_type* current = data_;
		const value_type* end = data_ + size_;
		while (current != end) {
			std::allocator_traits<allocator_type>::destroy(allocator, current++);
		}
	}
};
