#include "bigInt.h"
#include "tests.h"
#include <cassert>
#include <sstream>

std::string toString(const BigInt& value) {
	std::stringstream buf;
	buf << value;
	return buf.str();
}

bool checkEqual(const BigInt& value, const char* expected) {
	return (toString(value) == expected);
}

int testInit() {
	assert(checkEqual(BigInt(8314), "8314"));
	assert(checkEqual(BigInt(-6123), "-6123"));
	assert(checkEqual(BigInt(0), "0"));
	assert(checkEqual(BigInt(), "0"));
	BigInt bigInt1 = 602;
	assert(checkEqual(bigInt1, "602"));
	BigInt bigInt2 = bigInt1;
	assert(checkEqual(bigInt2, "602"));
	BigInt bigInt3 = std::move(bigInt1);
	assert(checkEqual(bigInt3, "602"));
	return 0;
}

int testAssignment() {
	BigInt bigInt1 = 543;
	BigInt bigInt2;
	bigInt2 = 100;
	assert(checkEqual(bigInt2, "100"));
	bigInt2 = bigInt1;
	assert(checkEqual(bigInt2, "543"));
	BigInt bigInt3;
	bigInt3 = std::move(bigInt1);
	assert(checkEqual(bigInt3, "543"));
	return 0;
}

int testEquality() {
	assert(BigInt(100) == BigInt(100));
	assert(BigInt(100) != BigInt(50));
	assert(BigInt(100) != BigInt(-100));
	assert(BigInt(100) > BigInt(50));
	assert(BigInt(100) >= BigInt(50));
	assert(BigInt(100) >= BigInt(100));
	assert(BigInt(100) > BigInt(-50));
	assert(BigInt(-100) < BigInt(-50));
	return 0;
}

int testSum() {
	BigInt bigInt1 = 10;
	assert(checkEqual((bigInt1 + 10), "20"));
	assert(checkEqual((bigInt1 - 5), "5"));
	assert(checkEqual((bigInt1 - 20), "-10"));
	assert(checkEqual((bigInt1 -10), "0"));
	bigInt1 += 10;
	assert(checkEqual(bigInt1, "20"));
	assert(BigInt(100) + BigInt(200) == BigInt(300));
	assert(BigInt(100) - BigInt(200) == BigInt(-100));
	return 0;
}

int testBigSum() {
	BigInt bigInt1;
	for (int i = 0; i < 100; ++i) {
		bigInt1 += 1000000000;
	}
	assert(checkEqual(bigInt1, "100000000000"));
	BigInt bigInt2;
	for (int i = 0; i < 100; ++i) {
		bigInt2 += 2000000000;
	}
	assert(checkEqual(bigInt1 + bigInt2, "300000000000"));
	assert(checkEqual(bigInt1 - bigInt2, "-100000000000"));
	return 0;
}

int testBigEquality() {
	BigInt bigInt1;
	for (int i = 0; i < 100; ++i) {
		bigInt1 += 1000000000;
	}
	BigInt bigInt2 = bigInt1;
	assert(bigInt1 == bigInt2);
	assert(checkEqual(bigInt1, "100000000000"));
	BigInt bigInt3;
	for (int i = 0; i < 100; ++i) {
		bigInt3 += 2000000000;
	}
	assert(bigInt1 < bigInt3);
	bigInt3 = -bigInt3;
	assert(bigInt1 > bigInt3);
	assert(bigInt1 >= bigInt3);
	assert(bigInt1 >= bigInt2);
	return 0;
}