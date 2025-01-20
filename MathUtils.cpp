//
// Created by 田庚雨 on 2025/1/20.
//

#include "MathUtils.h"
#include "Matrix4x4.h"

void MathUtils::set_rotate_matrix(const Vec3f& angles, Matrix4x4& matrix) {
    float sin_x = sinf(angles.x);
    float cos_x = cosf(angles.x);
    float sin_y = sinf(angles.y);
    float cos_y = cosf(angles.y);
    float sin_z = sinf(angles.z);
    float cos_z = cosf(angles.z);
    matrix.setValue(0, 0, cos_y * cos_z);
    matrix.setValue(1, 0, sin_x * sin_y * cos_z + cos_x * sin_z);
    matrix.setValue(2, 0, -1 * cos_x * sin_y * cos_z + sin_x * sin_z);
    matrix.setValue(0, 1, -1 * cos_y * sin_z);
    matrix.setValue(1, 1, -1 * sin_x * sin_y * sin_z + cos_x * cos_z);
    matrix.setValue(2, 1, cos_x * sin_y * sin_z + sin_x * cos_z);
    matrix.setValue(0, 2, sin_y);
    matrix.setValue(1, 2, -1 * sin_x * cos_y);
    matrix.setValue(2, 2, cos_x * cos_y);
    matrix.setValue(3, 3, 1);
}

void MathUtils::matrix_multiply(const Matrix4x4 &m, const Matrix4x4 &m1, Matrix4x4 &ret) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += m.getValue(i, k) * m1.getValue(k, j);
            }
            ret.setValue(i, j, sum);
        }
    }
}

void MathUtils::matrix_multiply(const Matrix4x4 &m, const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &ret) {
    for (int i = 0; i < 4; ++i) {          // 遍历结果矩阵的行
        for (int j = 0; j < 4; ++j) {      // 遍历结果矩阵的列
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {  // 遍历中间索引 k
                for (int l = 0; l < 4; ++l) {  // 遍历中间索引 l
                    sum += m.getValue(i, k) * m1.getValue(k, l) * m2.getValue(l, j);
                }
            }
            ret.setValue(i, j, sum);       // 设置结果矩阵的值
        }
    }
}

void MathUtils::matrix_multiply_v(const Matrix4x4 &m, const Vec3f &v, const float& w , Vec3f &ret) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    ret.x = m.getValue(0, 0) * x + m.getValue(0, 1) * y + m.getValue(0, 2) * z + m.getValue(0, 3) * w;
    ret.y = m.getValue(1, 0) * x + m.getValue(1, 1) * y + m.getValue(1, 2) * z + m.getValue(1, 3) * w;
    ret.z = m.getValue(2, 0) * x + m.getValue(2, 1) * y + m.getValue(2, 2) * z + m.getValue(2, 3) * w;
}

void MathUtils::matrix_multiply_vec(const Matrix4x4 &m, const Vec3f &v, Vec3f &ret) {
    matrix_multiply_v(m, v, 0.0f, ret);
}

void MathUtils::matrix_multiply_point(const Matrix4x4 &m, const Vec3f &v, Vec3f &ret) {
    matrix_multiply_v(m, v, 1.0f, ret);
}

void MathUtils::copy_vec(const Vec3f &v, Vec3f &ret) {
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
}