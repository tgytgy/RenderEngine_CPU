#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include "geometry.h"

class Matrix4x4 {
private:
    float matrix[4][4]{};
public:
    Matrix4x4();
    Matrix4x4 operator*(Matrix4x4 &rhs) const;
    Vec3f multiply(Vec3f &vec, bool isPoint) const;
    void multiply(Vec3f &rawVec, Vec3f &vec, bool isPoint) const;
    float getValue(int row, int col) const;
    void setValue(int row, int col, float value);
};

#endif // MATRIX_H
