std::string toString(const BigInt& value);
bool checkEqual(const BigInt& value, const char* expected);
using Test = int (*)();

int testInit();
int testAssignment();
int testEquality();
int testSum();
int testBigSum();
int testBigEquality();
