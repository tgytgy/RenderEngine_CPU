#include "Matrix4x4.h"
#include <iostream>

#include "geometry.h"

Matrix4x4::Matrix4x4(){
    for (auto & i : matrix) {
        for (float & j : i) {
            j = 0;
        }
    }
}

// 获取矩阵元素的值
float Matrix4x4::getValue(const int row, const int col) const{
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        throw std::out_of_range("行或列索引超出范围");
    }
    return matrix[row][col];
}

void Matrix4x4::setValue(const int row, const int col, const float value) {
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        throw std::out_of_range("行或列索引超出范围");
    }
    matrix[row][col] = value;
}