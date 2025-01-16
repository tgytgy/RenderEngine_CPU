#include "Matrix.h"
#include <iostream>

// 构造函数，初始化矩阵的行数、列数和数据
Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("行数和列数必须大于0");
    }
    data.resize(rows, std::vector<float>(cols, 0.0f)); // 初始化矩阵，所有元素为0.0
}

// 获取矩阵的行数
int Matrix::getRows() const {
    return rows;
}

// 获取矩阵的列数
int Matrix::getCols() const {
    return cols;
}

// 设置矩阵元素的值
void Matrix::setValue(int row, int col, float value) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("行或列索引超出范围");
    }
    data[row][col] = value;
}

// 获取矩阵元素的值
float Matrix::getValue(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("行或列索引超出范围");
    }
    return data[row][col];
}

// 打印矩阵（可选功能）
void Matrix::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
