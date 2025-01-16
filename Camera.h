//
// Created by 田庚雨 on 2025/1/16.
//

#ifndef RENDERENGINE_CAMERA_H
#define RENDERENGINE_CAMERA_H
#include <vector>
#include "Matrix.h"
#include "MathUtil.h"
using namespace std;

class Camera {
public:
    Camera(vector<float> worldPos, vector<float> rotate, float field_of_view, float near_clip, float far_clip, int projection_tp);
    Matrix& set_transform_matrix();
private:
    vector<float> worldPos; //相机世界坐标
    vector<float> rotate;   //相机旋转
    float field_of_view;    //垂直视角
    float near_clip;        //近裁剪平面
    float far_clip;         //远裁剪平面
    int projection_tp;      //相机类型 0 透视 1 正交
    Matrix view_transform = Matrix(4, 4);   //view transform矩阵
    Matrix r_transform = Matrix(4, 4);      //旋转变换矩阵
    Matrix t_transform = Matrix(4, 4);      //位移矩阵
};


#endif //RENDERENGINE_CAMERA_H
