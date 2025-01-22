//
// Created by 田庚雨 on 2025/1/20.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H
#include "geometry.h"
#include "Matrix4x4.h"


class MathUtils {
public:
    static void set_rotate_matrix(const Vec3f& angles, Matrix4x4& matrix);
    static void matrix_multiply(const Matrix4x4& m, const Matrix4x4& m1, Matrix4x4& ret);
    static void matrix_multiply(const Matrix4x4& m, const Matrix4x4& m1, const Matrix4x4& m2, Matrix4x4& ret);
    static void matrix_multiply_vec(const Matrix4x4& m, Vec3f v, Vec3f& ret);
    static void matrix_multiply_point(const Matrix4x4& m, Vec3f v, Vec3f& ret);
    static void copy_vec(const Vec3f &v, Vec3f &ret);
    static Vec3f barycentric(const Vec2i &p0, const Vec2i &p1, const Vec2i &p2, const Vec2i &P);
private:
    static void matrix_multiply_v(const Matrix4x4 &m, const Vec3f &v, const float& w , Vec3f &ret);
};

#endif //MATHUTILS_H
