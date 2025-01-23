//
// Created by 田庚雨 on 2025/1/20.
//

#include "MathUtils.h"

#include <iostream>

#include "Matrix4x4.h"

void MathUtils::set_rotate_matrix(const Vec3f& angles, Matrix4x4& matrix) {
    float sin_x = sinf(get_radians(angles.x));
    float cos_x = cosf(get_radians(angles.x));
    float sin_y = sinf(get_radians(angles.y));
    float cos_y = cosf(get_radians(angles.y));
    float sin_z = sinf(get_radians(angles.z));
    float cos_z = cosf(get_radians(angles.z));
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

float MathUtils::Pi = 3.14159265358979323846264338327950288f;

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

void MathUtils::matrix_multiply_v(const Matrix4x4 &m, const Vec3f &v, const float &w, Vec3f &ret) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    ret.x = m.getValue(0, 0) * x + m.getValue(0, 1) * y + m.getValue(0, 2) * z + m.getValue(0, 3) * w;
    ret.y = m.getValue(1, 0) * x + m.getValue(1, 1) * y + m.getValue(1, 2) * z + m.getValue(1, 3) * w;
    ret.z = m.getValue(2, 0) * x + m.getValue(2, 1) * y + m.getValue(2, 2) * z + m.getValue(2, 3) * w;
}

void MathUtils::matrix_multiply_vec(const Matrix4x4 &m, Vec3f v, Vec3f &ret) {
    matrix_multiply_v(m, v, 0.0f, ret);
}

void MathUtils::matrix_multiply_point(const Matrix4x4 &m, Vec3f v, Vec3f &ret) {
    const float _w = m.getValue(3, 0) * v.x + m.getValue(3, 1) * v.y + m.getValue(3, 2) * v.z + m.getValue(3, 3);
    matrix_multiply_v(m, v, 1.0f, ret);
    ret.x/=_w;
    ret.y/=_w;
    ret.z/=_w;
}

void MathUtils::clear_matrix(Matrix4x4 &m) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.setValue(i, j, 0.0f);
        }
    }
}

void MathUtils::copy_vec(const Vec3f &v, Vec3f &ret) {
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
}

Vec3f MathUtils::barycentric(const Vec2i &p0, const Vec2i &p1, const Vec2i &p2, const Vec2i &P) {
    Vec3i u = Vec3i(p1.x - p0.x, p2.x - p0.x, p0.x - P.x) ^ Vec3i(p1.y - p0.y, p2.y - p0.y, p0.y - P.y);
    if (std::abs(u.z) < 1)return {-1, 1, 1};
    return {
        1.f - static_cast<float>(u.x + u.y) / static_cast<float>(u.z),
        static_cast<float>(u.x) / static_cast<float>(u.z), static_cast<float>(u.y) / static_cast<float>(u.z)
    };
}

float MathUtils::get_radians(const float degrees) {
    return degrees * (Pi/180.f);
}
