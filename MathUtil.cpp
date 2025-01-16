//
// Created by 田庚雨 on 2025/1/16.
//

#include "MathUtil.h"

vector<float> MathUtil::crossProduct(const vector<float> &A, const vector<float> &B) {
    vector<float> result(3);
    result[0] = A[1] * B[2] - A[2] * B[1]; // Cx = Ay * Bz - Az * By
    result[1] = A[2] * B[0] - A[0] * B[2]; // Cy = Az * Bx - Ax * Bz
    result[2] = A[0] * B[1] - A[1] * B[0]; // Cz = Ax * By - Ay * Bx
    return result;
}
