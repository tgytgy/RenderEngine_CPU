//
// Created by 田庚雨 on 2025/1/16.
//

#include "Camera.h"
#include "MathUtils.h"

Camera::Camera(const Vec3f& worldPos, const Vec3f& rotate, const Vec2i& resolution, float field_of_view, float near_clip, float far_clip, float o_size, CameraType camera_type) {
    this->field_of_view = field_of_view;
    this->near_clip = near_clip;
    this->far_clip = far_clip;
    this->o_size = o_size;
    this->projection_tp = projection_tp;
    max_pos.x = this->o_size * static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
    max_pos.y = this->o_size;
    max_pos.z = -1 * this->near_clip;
    min_pos.x = -1 * max_pos.x;
    min_pos.y = -1 * max_pos.y;
    min_pos.z = -1 * this->far_clip;
    MathUtils::set_rotate_matrix(rotate, raw_to_crt_matrix);
    MathUtils::matrix_multiply_vec(raw_to_crt_matrix, raw_view_dir, view_dir);
    MathUtils::matrix_multiply_vec(raw_to_crt_matrix, raw_up_dir, up_dir);
    Vec3f view_cross_up = view_dir ^ up_dir;
    r_matrix.setValue(0, 0, view_cross_up.x);
    r_matrix.setValue(0, 1, view_cross_up.y);
    r_matrix.setValue(0, 2, view_cross_up.z);
    r_matrix.setValue(1, 0, up_dir.x);
    r_matrix.setValue(1, 1, up_dir.y);
    r_matrix.setValue(1, 2, up_dir.z);
    r_matrix.setValue(2, 0, -1 * view_dir.x);
    r_matrix.setValue(2, 1, -1 * view_dir.y);
    r_matrix.setValue(2, 2, -1 * view_dir.z);
    t_matrix.setValue(0, 2, -1 * worldPos.x);
    t_matrix.setValue(1, 2, -1 * worldPos.y);
    t_matrix.setValue(2, 2, -1 * worldPos.z);
    MathUtils::matrix_multiply(r_matrix, t_matrix, v_matrix);
    if (camera_type == Perspective) {
        const float n = -1 * near_clip;   //近裁剪平面z坐标
        const float f = -1 * far_clip;    //远裁剪平面z坐标
        p2o_matrix.setValue(0, 0, n);
        p2o_matrix.setValue(1, 1, n);
        p2o_matrix.setValue(2, 2, n + f);
        p2o_matrix.setValue(2, 3, -1 * n * f);
        p2o_matrix.setValue(3, 3, n);
        MathUtils::matrix_multiply(o_matrix, p2o_matrix, p_matrix);
    }else {
        o_s_matrix.setValue(0, 0, 2/(max_pos.x-min_pos.x));
        o_s_matrix.setValue(1, 1, 2/(max_pos.y-min_pos.y));
        o_s_matrix.setValue(2, 2, 2/(max_pos.z-min_pos.z));
        o_s_matrix.setValue(3, 3, 1);
        o_t_matrix.setValue(0, 0, 1);
        o_t_matrix.setValue(1, 1, 1);
        o_t_matrix.setValue(2, 2, 1);
        o_t_matrix.setValue(3, 3, 1);
        o_t_matrix.setValue(0, 3, -1*(max_pos.x+min_pos.x)/2);
        o_t_matrix.setValue(1, 3, -1*(max_pos.y+min_pos.y)/2);
        o_t_matrix.setValue(2, 3, -1*(max_pos.z+min_pos.z)/2);
        MathUtils::matrix_multiply(o_s_matrix, o_t_matrix, o_matrix);
    }
}
