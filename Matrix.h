#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept> // 用于异常处理

class Matrix {
private:
    int rows; // 矩阵的行数
    int cols; // 矩阵的列数
    std::vector<std::vector<float>> data; // 存储矩阵数据的二维向量

public:
    // 构造函数，传入行数和列数，元素默认值为0.0
    Matrix(int rows, int cols);

    // 获取矩阵的行数
    int getRows() const;

    // 获取矩阵的列数
    int getCols() const;

    // 根据行和列设置矩阵元素的值
    void setValue(int row, int col, float value);

    // 根据行和列获取矩阵元素的值
    float getValue(int row, int col) const;

    // 打印矩阵（可选功能）
    void print() const;
};

#endif // MATRIX_H
