#include "format.h"
#include <string>
#include <stdexcept>

std::string handler(const std::string& inputString,
					const std::vector<std::string>& vectorArgs) {
	std::string newString = "";
	std::string subString = "";
	bool closed = true;
	for (int i = 0; i < inputString.size(); ++i) {
		if (closed) {
			if (inputString[i] == '}') {
				throw std::runtime_error("");
			} else if (inputString[i] == '{') {
				closed = false;
			} else {
				newString += inputString[i];
			}
		} else {
			if (inputString[i] == '}') {
				closed = true;
				int index = std::stoi(subString);
				if (index >= vectorArgs.size()) {
					throw std::runtime_error("");
				}
				newString += vectorArgs[index];
				subString = "";
			} else if (isdigit(inputString[i])) {
				subString += inputString[i];
			} else {
				throw std::runtime_error("");
			}
		}
	}
	if (!closed) {
		throw std::runtime_error("");
	}
	return newString;
}
