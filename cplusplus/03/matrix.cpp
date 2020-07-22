#include "matrix.h"
#include <stddef.h>
#include <stdexcept>

Matrix::Matrix(size_t rows, size_t cols):
rows(rows),
cols(cols){
	body = new Row*[rows];
	for (int i = 0; i < rows; ++i) {
		body[i] = new Row(cols);
	}
}

Matrix::~Matrix() {
	for (int i = 0; i < rows; ++i) {
		delete body[i];
	}
	delete[] body;
}

Matrix::Row::Row(size_t cols):
		cols(cols) {
	body = new int[cols];
}

Matrix::Row::~Row() {
	delete[] body;
}

size_t Matrix::getCols() const {
	return cols;
}

size_t Matrix::getRows() const {
	return rows;
}

int& Matrix::Row::operator[](int j) {
	if (j >= cols) {
		throw std::out_of_range("");
	}
	return body[j];
}

const int& Matrix::Row::operator[](int j) const {
	if (j >= cols) {
		throw std::out_of_range("");
	}
	return body[j];
}

Matrix::Row& Matrix::operator[](int i) {
	if (i >= rows) {
		throw std::out_of_range("");
	}
	return (*body[i]);
}

const Matrix::Row& Matrix::operator[](int i) const {
	if (i >= rows) {
		throw std::out_of_range("");
	}
	return (*body[i]);
}

Matrix& Matrix::operator*=(const int value) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			(*body[i])[j] *= value;
		}
	}
	return *this;
}

bool Matrix::operator==(const Matrix& matrix) const {
	if (rows != matrix.getRows() || cols != matrix.getCols()) {
		return false;
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if ((*body[i])[j] != matrix[i][j]) {
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator!=(const Matrix& matrix) const {
	return !(*this == matrix);
}
