#include "parser.h"
#include "tests.h"
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <string>

std::stringstream ss;

void testOnBeginFound() {}
void testOnNumberFound(const int number) {}
void testOnStringFound(const std::string string) {}
void testOnEndFound() {}

void parseOnBegin() {
	ss << "Begin\n";
}

void parseOnNumber(const int number) {
	ss << "Number token: " << number << std::endl;
}

void parseOnString(const std::string string) {
	ss << "String token: " << string << std::endl;
}

void parseOnEnd() {
	ss << "End\n";
}

void makeTest(std::string title, int result) {
	static int num = 1;
	std::cout << "Test " << num << ". " << title << ".\n";
	std::cout << ss.str();
	ss.str("");
	switch (result) {
		case 0:
			std::cout << "OK.\n";
			break;
		case 1:
			std::cout << "Registration error.\n";
			break;
		case 2:
			std::cout << "Parser error.\n";
			break;
		case 3:
			std::cout << "Invalid token error.\n";
			break;
	}
	std::cout << std::endl;
	num++;
}

int main() {
	makeTest("Register onBegin",
			 registerOnBeginTest(testOnBeginFound));
	makeTest("Register onNumber",
			 registerOnNumberTest(testOnNumberFound));
	makeTest("Register onString",
			 registerOnStringTest(testOnStringFound));
	makeTest("Register onEnd",
			 registerOnEndTest(testOnEndFound));
	makeTest("Parsing with onBegin",
			 testParseOnBegin(parseOnBegin, "123 abc"));
	makeTest("Parsing with onNumber",
			 testParseOnNumber(parseOnNumber, "123 abc"));
	makeTest("Parsing with onString",
			 testParseOnString(parseOnString, "123 abc"));
	makeTest("Parsing with onEnd",
			 testParseOnEnd(parseOnEnd, "123 abc"));
	makeTest("Parsing with different separators",
			 testParse("123\tabc\n456 def"));
	makeTest("Parsing string with invalid token",
			 testParse("12a"));
	makeTest("Parsing string with invalid token",
			 testParse("a12"));
	makeTest("Parsing string with invalid token",
			 testParse("123 12a"));
	return 0;
}
