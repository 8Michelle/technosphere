#include "bigInt.h"

BigInt::BigInt(int value):
size(1),
data(new digit[1]),
negative(value < 0) {
	data[0] = std::abs(value) % limit;
	if (data[0] < std::abs(value)) {
		alloc();
		data[1] = std::abs(value) / limit;
	}
}

BigInt::~BigInt() {
	delete[] data;
}

BigInt::BigInt(const BigInt& value):
size(value.size),
negative(value.negative),
data(new digit[value.size]) {
	std::copy(value.data, value.data + size, data);
}

BigInt::BigInt(BigInt &&value):
size(value.size),
negative(value.negative),
data(value.data) {
	value.data = nullptr;
	value.size = 0;
	value.negative = false;
}

BigInt& BigInt::operator=(int value) {
	BigInt tmp = value;
	*this = std::move(tmp);
	return *this;
}

BigInt& BigInt::operator=(const BigInt& value) {
	if (*this == value) {
		return *this;
	}
	digit* tmp = new digit[value.size];
	delete[] data;
	data = tmp;
	size = value.size;
	negative = value.negative;
	std::copy(value.data, value.data + size, data);
	return *this;
}

BigInt& BigInt::operator=(BigInt&& value) {
	if (*this == value) {
		return *this;
	}
	delete[] data;
	data = value.data;
	size = value.size;
	negative = value.negative;
	value.data = nullptr;
	value.size = 0;
	value.negative = false;
	return *this;
}

BigInt BigInt::operator+(int value) const {
	return (*this + BigInt(value));
}

BigInt BigInt::operator+(const BigInt &value) const {
	BigInt tmp_a = *this;
	BigInt tmp_b = value;
	if (size < value.size) {
		std::swap(tmp_a, tmp_b);
	}
	if (tmp_a.negative == tmp_b.negative) {
		int i = 0;
		digit other = 0;
		while (true) {
			digit a = (tmp_a.size > i) ? tmp_a.data[i] : 0;
			digit b = (tmp_b.size > i) ? tmp_b.data[i] : 0;
			digit tmp = (a + b + other) % limit;
			other = (tmp < tmp_a.data[i]) ? 1 : 0;
			tmp_a.data[i] = tmp;
			++i;
			if (i >= tmp_a.size) {
				if (other != 0) {
					tmp_a.alloc();
				} else {
					break;
				}
			}
		}
	} else {
		if (tmp_a.abs() < tmp_b.abs()) {
			std::swap(tmp_a, tmp_b);
		}
		int i = 0;
		digit other = 0;
		while (true) {
			digit b = (tmp_b.size > i) ? tmp_b.data[i] : 0;
			digit tmp = ((tmp_a.data[i] - b - other) + limit) % limit;
			other = (tmp > tmp_a.data[i]) ? 1 : 0;
			tmp_a.data[i] = tmp;
			++i;
			if (i == tmp_a.size) {
				break;
			}
		}
	}
	return tmp_a;
}

BigInt BigInt::operator-(int value) const {
	return (*this - BigInt(value));
}

BigInt BigInt::operator-(const BigInt& value) const {
	return (*this + (-value));
}

BigInt& BigInt::operator+=(int value) {
	return (*this += BigInt(value));
}

BigInt& BigInt::operator+=(const BigInt& value) {
	*this = std::move(*this + value);
	return *this;
}

BigInt& BigInt::operator-=(int value) {
	return (*this -= BigInt(value));
}

BigInt& BigInt::operator-=(const BigInt& value) {
	return (*this += (-value));
}

BigInt BigInt::operator-() const {
	BigInt tmp = *this;
	tmp.negative = !tmp.negative;
	return tmp;
}

bool BigInt::operator==(int value) const {
	return (*this == BigInt(value));
}

bool BigInt::operator==(const BigInt &value) const {
	if (size == value.size && negative == value.negative) {
		for (int i = 0; i < size; ++i) {
			if (data[i] != value.data[i]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool BigInt::operator!=(int value) const {
	return (*this != BigInt(value));
}

bool BigInt::operator!=(const BigInt& value) const {
	return !(*this == value);
}

bool BigInt::operator<(int value) const {
	return (*this < BigInt(value));
}

bool BigInt::operator<(const BigInt &value) const {
	if (negative && !value.negative) {
		return true;
	} else if (!negative && value.negative) {
		return false;
	} else {
		for (int i = std::max(size, value.size) - 1; i >= 0; --i) {
			digit a = (size > i) ? data[i] : 0;
			digit b = (value.size > i) ? value.data[i] : 0;
			if (a != b) {
				return (negative ^ (a < b));
			}
		}
		return false;
	}
}

bool BigInt::operator<=(int value) const {
	return (*this <= BigInt(value));
}

bool BigInt::operator<=(const BigInt &value) const {
	return ((*this == value) || (*this < value));
}

bool BigInt::operator>(int value) const {
	return (*this > BigInt(value));
}

bool BigInt::operator>(const BigInt &value) const {
	return !(*this <= value);
}

bool BigInt::operator>=(int value) const {
	return (*this >= BigInt(value));
}

bool BigInt::operator>=(const BigInt &value) const {
	return !(*this < value);
}

void BigInt::alloc() {
	digit* tmp = new digit[size + 1];
	std::copy(data, data + size, tmp);
	delete[] data;
	data = tmp;
	++size;
}

BigInt BigInt::abs() {
	BigInt tmp = *this;
	tmp.negative = false;
	return tmp;
}

std::ostream& operator<<(std::ostream& out, const BigInt& value) {
	std::string string = (value.negative) ? "-" : "";
	std::string tmp = "";
	bool flag = true;
	for (int i = value.size - 1; i >= 0; --i) {
		int value_tmp = value.data[i];
		if (flag) {
			if (value_tmp == 0) {
				if (i == 0) {
					out << "0";
					return out;
				}
				tmp = "";
			} else {
				tmp = std::to_string(value_tmp);
				flag = false;
			}
		} else {
			tmp = std::to_string(value_tmp);
			int diff = value.string_size - tmp.size();
			for (int i = 0; i < diff; ++i) {
				tmp = "0" + tmp;
			}
		}
		string += tmp;
	}
	out << string;
	return out;
}
