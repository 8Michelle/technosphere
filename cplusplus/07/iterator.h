#include <iterator>

template <class T>
class Iterator : public std::iterator<
		std::random_access_iterator_tag, T> {
public:
	Iterator(T* ptr):
	ptr_(ptr) {
	}
	typename Iterator::reference operator*() const {
		return *ptr_;
	}
	Iterator& operator++() {
		++ptr_;
		return *this;
	}
	Iterator operator++(int) {
		Iterator<T> tmp = *this;
		++ptr_;
		return tmp;
	}
	bool operator==(const Iterator<T>& other) const {
		return (ptr_ == other.ptr_);
	}
	bool operator!=(const Iterator<T>& other) const {
		return !(*this == other);
	}
	Iterator& operator--() {
		--ptr_;
		return *this;
	}
	Iterator operator--(int) {
		Iterator<T> tmp = *this;
		--ptr_;
		return tmp;
	}
	Iterator operator+(typename Iterator<T>::difference_type value) const {
		Iterator<T> tmp = *this;
		tmp += value;
		return tmp;
	}
	Iterator& operator+=(typename Iterator<T>::difference_type value) {
		ptr_ += value;
		return *this;
	}
	Iterator operator-(typename Iterator<T>::difference_type value) const {
		return (*this + (-value));
	}
	Iterator& operator-=(typename Iterator<T>::difference_type value) {
		return (*this += (-value));
	}
	typename Iterator::difference_type operator-(const Iterator<T>& other) const {
		return (ptr_ - other.ptr_);
	}
	bool operator<(const Iterator<T>& other) const {
		return (*this - other < 0);
	}
	bool operator>(const Iterator<T>& other) const {
		return (other < *this);
	}
	bool operator>=(const Iterator<T>& other) const {
		return (*this > other || *this == other);
	}
	bool operator<=(const Iterator<T>& other) const {
		return (other >= *this);
	}
	typename Iterator::value_type operator[](int index) const {
		return *(*this + index);
	}
protected:
	typename Iterator::pointer ptr_;
};

template <class T>
class ReverseIterator : public Iterator<T> {
public:
	ReverseIterator(T* ptr):
	Iterator<T>(ptr) {
	}
	ReverseIterator& operator++() {
		--ReverseIterator::ptr_;
		return *this;
	}
	ReverseIterator operator++(int) {
		ReverseIterator<T> tmp = *this;
		--ReverseIterator<T>::ptr_;
		return tmp;
	}
	ReverseIterator& operator--() {
		++ReverseIterator<T>::ptr_;
		return *this;
	}
	ReverseIterator operator--(int) {
		ReverseIterator<T> tmp = *this;
		++ReverseIterator<T>::ptr_;
		return tmp;
	}
	ReverseIterator operator+(typename ReverseIterator<T>::difference_type value) const {
		ReverseIterator<T> tmp = *this;
		tmp += value;
		return tmp;
	}
	ReverseIterator& operator+=(typename ReverseIterator<T>::difference_type value) {
		ReverseIterator::ptr_ -= value;
		return *this;
	}
	ReverseIterator operator-(typename ReverseIterator<T>::difference_type value) const {
		return (*this + (-value));
	}
	ReverseIterator& operator-=(typename ReverseIterator<T>::difference_type value) {
		return (*this += (-value));
	}
	typename ReverseIterator::difference_type operator-(const ReverseIterator<T>& other) const {
		return (other.ptr_ - ReverseIterator::ptr_);
	}
	bool operator<(const ReverseIterator<T>& other) const {
		return (*this - other < 0);
	}
	bool operator>(const ReverseIterator<T>& other) const {
		return (other < *this);
	}
	bool operator>=(const ReverseIterator<T>& other) const {
		return (*this > other || *this == other);
	}
	bool operator<=(const ReverseIterator<T>& other) const {
		return (other >= *this);
	}
	typename ReverseIterator::value_type operator[](int index) const {
		return *(*this + index);
	}
};
