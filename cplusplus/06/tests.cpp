#include "format.h"
#include "tests.h"
#include <cassert>
#include <ostream>

int testInit() {
	assert(format("{0}, {1}, {2}", 'a', 'b', 'c') == "a, b, c");
	assert(format("{2}, {1}, {0}", 10, 11, 12) == "12, 11, 10");
	assert(format("{0}, {0}, {0}", 11) == "11, 11, 11");
	assert(format("{0}", 10, 11, 12) == "10");
	return 0;
}

int testDiffTypes() {
	assert(format("{0} -- {1} -- {2}{3}{3}", 100, "one hundred", '1', '0') == "100 -- one hundred -- 100");
	std::vector<int> v;
	for (int i = 1; i < 4; ++i) {
		v.push_back(i);
	}
	assert(format("testType {0}", TestType(v)) == "testType 1 2 3 ");
	return 0;
}

int testExceptions() {
	try {
		std::string s = format("{1}", "one arg");
	} catch (std::runtime_error) {}
	try {
		std::string s = format("invalid}{string", "arg1");
	} catch (std::runtime_error) {}
	try {
		std::string s = format("invalid{_}string", "arg1");
	} catch (std::runtime_error) {}
	return 0;
}

TestType::TestType(std::vector<int> testFields):
_testFields(testFields) {
}

std::ostream& operator<<(std::ostream& out,
		const TestType& testObject) {
	for (int i : testObject._testFields) {
		out << i << " ";
	}
	return out;
}
