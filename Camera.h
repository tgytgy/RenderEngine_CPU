//
// Created by 田庚雨 on 2025/1/16.
//

#ifndef RENDERENGINE_CAMERA_H
#define RENDERENGINE_CAMERA_H
#include <vector>

#include "geometry.h"
#include "Matrix4x4.h"
using namespace std;

enum CameraType {
    Orthogonal,
    Perspective
};

class Camera {
public:
    Camera(const Vec3f& worldPos, const Vec3f& rotate, const Vec2i& resolution, float field_of_view, float near_clip, float far_clip, float o_size, CameraType camera_type);
private:
    Vec3f worldPos; //相机世界坐标
    Vec3f rotate;   //相机旋转
    float field_of_view;    //垂直视角
    float near_clip;        //近裁剪平面
    float far_clip;         //远裁剪平面
    float o_size;           //正交相机size
    int projection_tp;      //相机类型 0 透视 1 正交
    Vec3f min_pos;          //xyz最小点
    Vec3f max_pos;          //xyz最大点
    Vec3f raw_view_dir = {0, 0, -1};                 //观测方向
    Vec3f raw_up_dir = {0, 1, 0};                   //相机向上方向
    Vec3f view_dir = {0, 0, -1};                 //观测方向
    Vec3f up_dir = {0, 1, 0};                   //相机向上方向
    Matrix4x4 r_matrix;      //旋转矩阵
    Matrix4x4 t_matrix;      //位移矩阵
    Matrix4x4 v_matrix;      //最终的view transform矩阵
    Matrix4x4 raw_to_crt_matrix;    //标准坐标系到目标状态的旋转
    Matrix4x4 o_s_matrix;           //正交投影缩放矩阵
    Matrix4x4 o_t_matrix;           //正交投影位移矩阵
    Matrix4x4 o_matrix;             //正交投影矩阵
    Matrix4x4 p_matrix;             //透视投影矩阵
    Matrix4x4 p2o_matrix;           //透视转正交矩阵

    void cal_o_matrix();
};


#endif //RENDERENGINE_CAMERA_H
