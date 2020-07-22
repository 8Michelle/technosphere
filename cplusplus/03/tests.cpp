#include "matrix.h"
#include "tests.h"
#include <stdexcept>


int checkEqual(int x, int y) {
	if (x == y) {
		return 0;
	} else {
		return 1;
	}
}

int check(bool condition) {
	if (condition) {
		return 0;
	} else {
		return 2;
	}
}

int checkOutOfRange(const Matrix& matrix, int i, int j) {
	try {
		matrix[i][j];
	} catch (std::out_of_range) {
		return 3;
	}
	return 0;
}
