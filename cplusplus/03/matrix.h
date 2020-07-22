#include <stddef.h>
#pragma once

class Matrix {
private:
	class Row {
	public:
		Row(size_t cols);
		~Row();
		int& operator[](int j);
		const int& operator[](int j) const;
	private:
		int* body;
		size_t cols;
	};
	size_t rows;
	size_t cols;
	Row** body;
public:
	Matrix(size_t rows, size_t cols);
	~Matrix();
	size_t getRows() const;
	size_t getCols() const;
	Matrix& operator*=(const int value);
	bool operator==(const Matrix& matrix) const;
	bool operator!=(const Matrix& matrix) const;
	Row& operator[](int i);
	const Row& operator[](int i) const;
};
