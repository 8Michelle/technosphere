#include "serializer.h"
#include <ostream>
#include <istream>
#include <cstdint>
#include <string>
#include <iostream>

Serializer::Serializer(std::ostream& out):
out_(out) {
}

Error Serializer::serialValue(bool value) {
	std::string text = value ? "true" : "false";
	out_ << text << Separator;
	return Error::NoError;
}

Error Serializer::serialValue(uint64_t value) {
	std::string text = std::to_string(value);
	//out_ << text;
	out_ << text << Separator;
	return Error::NoError;
}

//void Serializer::addSeparator() {
//	out_ << Separator;
//}

Deserializer::Deserializer(std::istream& in):
in_(in){
}

Error Deserializer::deserialValue(bool& value) {
	std::string text;
	in_ >> text;
	if (text == "true") {
		value = true;
	} else if (text == "false") {
		value = false;
	} else {
		return Error::CorruptedArchive;
	}
	return Error::NoError;
}

Error Deserializer::deserialValue(uint64_t& value) {
	std::string text;
	in_ >> text;
	try {
		value = std::stoul(text);
		return Error::NoError;
	} catch (std::invalid_argument) {
		return Error::CorruptedArchive;
	}
}
