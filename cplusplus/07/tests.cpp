#include "vector.h"
#include "tests.h"
#include <cassert>
#include <string>
#include <stdexcept>

int testInit() {
	Vector<int> vector0;
	assert(vector0.size() == 0);
	assert(vector0.empty());
	Vector<int> vector1(10);
	assert(vector1.size() == 10);
	Vector<int> vector2(10, 31415);
	assert(vector2.size() == 10);
	for (int i = 0; i < 10; ++i) {
		assert(vector2[i] == 31415);
	}
	Vector<int> vector3 = {2, 4, 6, 8, 10};
	assert(vector3.size() == 5);
	for (int i = 1; i <= 5; ++i) {
		assert(vector3[i - 1] == 2 * i);
	}
	Vector<std::string> vector_c = {"home", "sweet", "home"};
	assert(vector_c[0] == "home");
	assert(vector_c[1] == "sweet");
	assert(vector_c[2] == "home");
	return 0;
}

int testPush() {
	Vector<int> vector(10);
	assert(vector.size() == 10);
	assert(vector.capacity() == 10);
	vector.push_back(27183);
	assert(vector[10] == 27183);
	assert(vector.size() == 11);
	assert(vector.capacity() == 20);
	for (int i = 0; i < 29; ++i) {
		vector.push_back(1);
	}
	assert(vector.size() == 40);
	assert(vector.capacity() == 40);
	int next = 31415;
	vector.push_back(next);
	assert(vector[40] == next);
	assert(vector.size() == 41);
	assert(vector.capacity() == 80);
	return 0;
}

int testPop() {
	Vector<int> vector = {8, 9, 10};
	assert(vector.size() == 3);
	int item_1 = vector[2];
	int item_2 = vector.pop_back();
	assert(item_1 == item_2);
	assert(vector.size() == 2);
	try {
		vector[2];
	} catch (std::out_of_range) {
		return 0;
	}
}

int testSize() {
	Vector<int> vector = {1, 2, 3, 4, 5};
	vector.resize(3);
	assert(vector.size() == 3);
	for (int i = 0; i < vector.size(); ++i) {
		assert(vector[i] == i + 1);
	}
	try {
		vector[3];
	} catch (std::out_of_range) {}
	vector.resize(4);
	assert(vector.size() == 4);
	for (int i = 0; i < vector.size() - 1; ++i) {
		assert(vector[i] == i + 1);
	}
	vector.resize(5, 31415);
	assert(vector.size() == 5);
	assert(vector[4] == 31415);
	Vector<int> vector2 = {1, 2};
	vector2.reserve(3);
	assert(vector2.size() == 2);
	assert(vector2.capacity() == 3);
	assert(vector2[0] == 1 && vector2[1] == 2);
	vector2.clear();
	assert(vector2.size() == 0);
	return 0;
}

int testIterator() {
	Vector<int> vector = {1, 2, 3, 4, 5};
	Iterator<int> begin = vector.begin();
	Iterator<int> end = vector.end();
	Iterator<int> current = begin;
	int i = 0;
	while (current != end) {
		assert(*current++ == vector[i++]);
	}
	ReverseIterator<int> rbegin = vector.rbegin();
	ReverseIterator<int> rend = vector.rend();
	ReverseIterator<int> rcurrent = rbegin;
	i = 0;
	while (rcurrent != rend) {
		assert(*rcurrent-- == vector[i++]);
	}
	return 0;
}
