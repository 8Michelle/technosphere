#include <ostream>
#include <istream>
#include <cstdint>
#pragma once

enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out);

	template <class T>
	Error save(T&& object) {
		return object.serialize(*this);
	}

	template <class... Args>
	Error operator()(Args... args) {
		return process(args...);
	}

private:
	template <class T>
	Error process(T value) {
		return serialValue(value);
	}

	template <class T, class... Args>
	Error process(T value, Args... args) {
		serialValue(value);
		return process(std::forward<Args>(args)...);
	}

	Error serialValue(bool value);
	Error serialValue(uint64_t value);

	std::ostream& out_;
};

class Deserializer {
	static constexpr char Separator = ' ';
public:
	explicit Deserializer(std::istream& in);

	template <class T>
	Error load(T&& object) {
		return object.serialize(*this);
	}

	template <class... Args>
	Error operator()(Args&... args) {
		return process(args...);
	}

private:
	template <class T>
	Error process(T& value) {
		return deserialValue(value);
	}

	template <class T, class... Args>
	Error process(T& value, Args&... args) {
		deserialValue(value);
		return process(std::forward<Args&>(args)...);
	}

	Error deserialValue(bool& value);
	Error deserialValue(uint64_t& value);

	std::istream& in_;
};