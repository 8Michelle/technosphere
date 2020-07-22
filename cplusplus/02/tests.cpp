#include "parser.h"
#include "tests.h"
#include <exception>

// Тесты регистрации callback'ов
int registerOnBeginTest(onBegin func) {
	try {
		registerOnBeginCallback(func);
	} catch (std::exception) {
		return 1;
	}
	return 0;
}

int registerOnNumberTest(onNumber func) {
	try {
		registerOnNumberCallback(func);
	} catch (std::exception) {
		return 1;
	}
	return 0;
}

int registerOnStringTest(onString func) {
	try {
		registerOnStringCallback(func);
	} catch (std::exception) {
		return 1;
	}
	return 0;
}

int registerOnEndTest(onEnd func) {
	try {
		registerOnEndCallback(func);
	} catch (std::exception) {
		return 1;
	}
	return 0;
}

// Тесты выполнения зарегистрированных callback'ов
int testParseOnBegin(onBegin func, const char* text) {
	try {
		registerOnBeginCallback(func);
		try {
			parse(text);
			return 0;
		} catch (std::exception) {
			return 2;
		}
	} catch (std::exception) {
		return 1;
	}
}

int testParseOnString(onString func, const char* text) {
	try {
		registerOnStringCallback(func);
		try {
			parse(text);
			return 0;
		} catch (std::exception) {
			return 2;
		}
	} catch (std::exception) {
		return 1;
	}
}

int testParseOnNumber(onNumber func, const char* text) {
	try {
		registerOnNumberCallback(func);
		try {
			parse(text);
			return 0;
		} catch (std::exception) {
				return 2;
		}
	} catch (std::exception) {
		return 1;
	}
}

int testParseOnEnd(onEnd func, const char* text) {
	try {
		registerOnEndCallback(func);
		try {
			parse(text);
			return 0;
		} catch (std::exception) {
			return 2;
		}
	} catch (std::exception) {
		return 1;
	}
}

int testParse(const char* text) {
	try {
		parse(text);
		return 0;
	} catch (InvalidTokenException& e) {
		return 3;
	} catch (std::exception) {
		return 2;
	}
}