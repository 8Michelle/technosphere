#include <iostream>
#include "matrix.h"
#include "tests.h"
#include <stddef.h>

void makeTest(std::string title, int result) {
	static int num = 1;
	std::cout << "Test " << num << ". " << title << ".\n";
	switch (result) {
		case 0:
			std::cout << "OK.\n";
			break;
		case 1:
			std::cout << "Values unequal.\n";
			break;
		case 2:
			std::cout << "Condition is wrong.\n";
			break;
		case 3:
			std::cout << "Matrix index out of range.\n";
			break;
	}
	std::cout << std::endl;
	num++;
}

int main() {
	Matrix matrix1(2, 2);

	makeTest("Correct rows value", checkEqual(matrix1.getRows(), 2));
	makeTest("Correct columns value", checkEqual(matrix1.getCols(), 2));
	matrix1[0][0] = 1;
	matrix1[0][1] = 2;
	matrix1[1][0] = 3;
	matrix1[1][1] = 4;

	makeTest("Correct 00 value", checkEqual(matrix1[0][0], 1));
	makeTest("Correct 01 value", checkEqual(matrix1[0][1], 2));
	makeTest("Correct 10 value", checkEqual(matrix1[1][0], 3));
	makeTest("Correct 11 value", checkEqual(matrix1[1][1], 4));

	std::cout << "Multiply the matrix by 10.\n";
	matrix1 *= 10;

	makeTest("Correct 00 value", checkEqual(matrix1[0][0], 1 * 10));
	makeTest("Correct 01 value", checkEqual(matrix1[0][1], 2 * 10));
	makeTest("Correct 10 value", checkEqual(matrix1[1][0], 3 * 10));
	makeTest("Correct 11 value", checkEqual(matrix1[1][1], 4 * 10));

	Matrix matrix2(2, 2);
	matrix2[0][0] = 10;
	matrix2[0][1] = 20;
	matrix2[1][0] = 30;
	matrix2[1][1] = 40;

	makeTest("Check equal matrices", check(matrix2 == matrix1));
	Matrix matrix4(0, 0);
	makeTest("Check unequal matrices", check(matrix4 != matrix1));

	makeTest("Check access out of range", checkOutOfRange(matrix1, 2, 1));
	makeTest("Check access in range", checkOutOfRange(matrix1, 1, 1));

	makeTest("Check null matrix", checkOutOfRange(matrix4, 0, 0));

	std::cout << "DONE\n";
	return 0;
}
