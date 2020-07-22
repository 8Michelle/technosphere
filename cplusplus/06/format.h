#include <string>
#include <sstream>
#include <vector>

template <class T>
std::string toString(const T& object) {
	std::stringstream ss;
	ss << object;
	std::string resultString = ss.str();
	return resultString;
}

template <class T>
void argParse(std::vector<std::string>& vectorArgs, T&& value) {
	vectorArgs.push_back(toString(value));
}

template <class T, class... Args>
void argParse(std::vector<std::string>& vectorArgs,
		T&& value, Args&&... args) {
	vectorArgs.push_back(toString(value));
	argParse(vectorArgs, args...);
}

std::string handler(const std::string& inputString,
					const std::vector<std::string>& vectorArgs);

template <class... Args>
std::string format(const std::string& inputString, Args&&... args) {
	std::vector<std::string> vectorArgs;
	argParse(vectorArgs, args...);
	return handler(inputString, vectorArgs);
}
