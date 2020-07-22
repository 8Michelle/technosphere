#include "parser.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <sstream>

const char* InvalidTokenException::what() const throw() {
	return "Invalid token";
}

onBegin onBeginCallback;
onNumber onNumberCallback;
onString onStringCallback;
onEnd onEndCallback;

void registerOnNumberCallback(onNumber callback) {
	onNumberCallback = callback;
}

void registerOnStringCallback(onString callback) {
	onStringCallback = callback;
}

void registerOnBeginCallback(onBegin callback) {
	onBeginCallback = callback;
}

void registerOnEndCallback(onEnd callback) {
	onEndCallback = callback;
}

bool isNumber(const char* c) {
	while (!isspace(*c) & (*c != '\0')) {
		if (!isdigit(*c++)) {
			return false;
		}
	}
	return true;
}

bool isString(const char* c) {
	while (!isspace(*c) & (*c != '\0')) {
		if (isdigit(*c++)) {
			return false;
		}
	}
	return true;
}

std::string getToken(const char* token) {
	std::string string = "";
	char start_token = *token;
	while (!std::isspace(*token) & (*token != '\0')) {
		string += *token++;
	}
	return string;
}

void parse(const char* text) {
	if (onBeginCallback) {
		onBeginCallback();
	}
	const char* firstChar = text;
	while (*text++) {
		if (isspace(*text) | (*text == '\0')) {
			std::string string = getToken(firstChar);
			if (isNumber(firstChar)) {
				if (onNumberCallback) {
					int number;
					std::istringstream(string) >> number;
					onNumberCallback(number);
				}
			} else if (isString(firstChar)) {
				if (onStringCallback) {
					onStringCallback(string);
				}
			} else {
				throw InvalidTokenException();
			}
			if (*text != '\0') {
				firstChar = ++text;
			} else {
				break;
			}
		}
	}
	if (onEndCallback) {
		onEndCallback();
	}
}
