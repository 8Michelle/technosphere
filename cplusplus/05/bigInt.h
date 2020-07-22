#include <limits>
#include <ostream>

using digit = unsigned;

class BigInt {
public:

	BigInt(int value = 0);
	~BigInt();

	BigInt(const BigInt& value);
	BigInt(BigInt&& value);

	BigInt& operator=(int value);
	BigInt& operator=(const BigInt& value);
	BigInt& operator=(BigInt&& value);

	BigInt operator+(int value) const;
	BigInt operator+(const BigInt& value) const;

	BigInt operator-(int value) const;
	BigInt operator-(const BigInt& value) const;

	BigInt& operator+=(int value);
	BigInt& operator+=(const BigInt& value);

	BigInt& operator-=(int value);
	BigInt& operator-=(const BigInt& value);

	BigInt operator-() const;

	bool operator==(int value) const;
	bool operator==(const BigInt& value) const;
	bool operator!=(int value) const;
	bool operator!=(const BigInt& value) const;
	bool operator<(int value) const;
	bool operator<(const BigInt& value) const;
	bool operator>(int value) const;
	bool operator>(const BigInt& value) const;
	bool operator<=(int value) const;
	bool operator<=(const BigInt& value) const;
	bool operator>=(int value) const;
	bool operator>=(const BigInt& value) const;

	BigInt abs();

	friend std::ostream& operator<<(std::ostream& out, const BigInt& value);

private:
	digit* data;
	size_t size;
	size_t limit = 1000000000;
	int string_size = 9;
	bool negative;
	void alloc();
};
