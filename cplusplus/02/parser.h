#include <exception>
#include <string>

using onBegin = void (*)();
using onEnd = void (*)();
using onNumber = void (*)(const int number);
using onString = void (*)(const std::string string);

void registerOnNumberCallback(onNumber callback);
void registerOnStringCallback(onString callback);
void registerOnBeginCallback(onBegin callback);
void registerOnEndCallback(onEnd callback);

void parse(const char* text);

class InvalidTokenException: public std::exception {
	const char* what() const throw();
};