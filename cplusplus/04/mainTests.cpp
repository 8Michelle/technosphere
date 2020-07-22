#include "serializer.h"
#include "tests.h"
#include <iostream>
#include <cstdint>
#include <string>

void makeTest(std::string title, int result) {
	static int num = 1;
	std::cout << "Test " << num << ". " << title << ".\n";
	switch (result) {
		case 0:
			std::cout << "OK.\n";
			break;
		case 1:
			std::cout << "Incorrect serialization.\n";
			break;
		case 2:
			std::cout << "Corrupted archive.\n";
			break;
		case 3:
			std::cout << "Unknown error.\n";
	}
	std::cout << std::endl;
	num++;
}

int main() {
	Data data {31415, true, 27183};
	makeTest("Serialize", testSerializeDeserialize(data));
	makeTest("Deserialize", testDeserializeString("31415 true 27183", data));
	makeTest("Deserialize invalid string", testDeserializeInvalidString("111", data));
	makeTest("Deserialize empty string", testDeserializeInvalidString("", data));
	std::cout << "DONE\n";
	return 0;
}