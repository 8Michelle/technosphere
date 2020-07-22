#include "serializer.h"
#include "tests.h"
#include <sstream>
#include <cstdint>
#include <iostream>

bool Data::operator==(Data data) {
	return (a == data.a && b == data.b && c == data.c);
}

int testSerializeDeserialize(Data data) {
	std::stringstream ss;
	Serializer serializer(ss);
	Error error = serializer.save(data);
	if (error == Error::NoError) {
		Deserializer deserializer(ss);
		Data new_data {0, false, 0};
		deserializer.load(new_data);
		if (new_data == data) {
			return 0;
		}
		return 1;
	} else if (error == Error::CorruptedArchive) {
		return 2;
	}
	return 3;
}

int testDeserializeString(std::string string, Data data) {
	std::stringstream ss;
	Data new_data {0, false, 0};
	ss << string;
	Deserializer deserializer(ss);
	Error error = deserializer.load(new_data);
	if (error == Error::NoError) {
		if (new_data == data) {
			return 0;
		}
		return 1;
	} else if (error == Error::CorruptedArchive) {
		return 2;
	}
	return 3;
}

int testDeserializeInvalidString(std::string string, Data data) {
	std::stringstream ss;
	Data new_data {0, false, 0};
	ss << string;
	Deserializer deserializer(ss);
	Error error = deserializer.load(new_data);
	//std::cout << new_data.b << std::endl;
 	if (error == Error::CorruptedArchive) {
		return 0;
	}
	return 1;
}