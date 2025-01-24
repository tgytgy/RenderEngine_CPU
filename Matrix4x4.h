#ifndef MATRIX_H
#define MATRIX_H

class Matrix4x4 {
private:
    float matrix[4][4]{};
public:
    Matrix4x4();
    float getValue(int row, int col) const;
    void setValue(int row, int col, float value);
};

#endif // MATRIX_H
