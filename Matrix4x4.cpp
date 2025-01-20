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

Matrix4x4 Matrix4x4::operator *(Matrix4x4& rhs) const {
    Matrix4x4 result; // 用于存储结果的矩阵
    for (int i = 0; i < 4; ++i) { // 遍历结果矩阵的行
        for (int j = 0; j < 4; ++j) { // 遍历结果矩阵的列
            for (int k = 0; k < 4; ++k) { // 遍历左矩阵的列和右矩阵的行
                float& v = result.getValue(i, j);
                v += this->matrix[i][k] * rhs.getValue(k, j); // 累加乘积
            }
        }
    }
    return result;
}

Vec3f Matrix4x4::multiply(Vec3f& vec, bool isPoint) const {
    Vec3f result;
    float x = vec.x;
    float y = vec.y;
    float z = vec.z;
    float w = isPoint ? 1.0f : 0.0f; // 补充 1 或 0
    result.x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3] * w;
    result.y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3] * w;
    result.z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z + matrix[2][3] * w;
    return result;
}

void Matrix4x4::multiply(Vec3f &rawVec, Vec3f &vec, bool isPoint) const {
    float x = rawVec.x;
    float y = rawVec.y;
    float z = rawVec.z;
    float w = isPoint ? 1.0f : 0.0f; // 补充 1 或 0
    vec.x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3] * w;
    vec.y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3] * w;
    vec.z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z + matrix[2][3] * w;
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