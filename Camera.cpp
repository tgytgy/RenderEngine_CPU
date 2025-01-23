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
    this->camera_type = camera_type;
    MathUtils::set_rotate_matrix(rotate, raw_to_crt_matrix);
    MathUtils::set_rotate_matrix({-rotate.x, -rotate.y, -rotate.z}, r_matrix);
    MathUtils::matrix_multiply_vec(raw_to_crt_matrix, raw_view_dir, view_dir);
    MathUtils::matrix_multiply_vec(raw_to_crt_matrix, raw_up_dir, up_dir);
    t_matrix.setValue(0, 0, 1);
    t_matrix.setValue(1, 1, 1);
    t_matrix.setValue(2, 2, 1);
    t_matrix.setValue(0, 3, -1 * worldPos.x);
    t_matrix.setValue(1, 3, -1 * worldPos.y);
    t_matrix.setValue(2, 3, -1 * worldPos.z);
    t_matrix.setValue(3, 3, 1);
    MathUtils::matrix_multiply(r_matrix, t_matrix, v_matrix);
    const float n = -1 * this->near_clip; //近裁剪平面z坐标
    const float f = -1 * this->far_clip; //远裁剪平面z坐标
    switch (camera_type) {
        case Perspective:
            p2o_matrix.setValue(0, 0, n);
            p2o_matrix.setValue(1, 1, n);
            p2o_matrix.setValue(2, 2, n + f);
            p2o_matrix.setValue(2, 3, -1 * n * f);
            p2o_matrix.setValue(3, 2, 1);
            min_pos.y = -1 * near_clip * tanf(MathUtils::get_radians(field_of_view / 2));
            min_pos.x = min_pos.y * static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
            min_pos.z = f;
            max_pos.x = -1 * min_pos.x;
            max_pos.y = -1 * min_pos.y;
            max_pos.z = n;
            cal_o_matrix();
            MathUtils::matrix_multiply(o_matrix, p2o_matrix, p_matrix);
            projection_matrix = &p_matrix;
            break;
        case Orthogonal:
            max_pos.x = this->o_size * static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
            max_pos.y = this->o_size;
            max_pos.z = n;
            min_pos.x = -1 * max_pos.x;
            min_pos.y = -1 * max_pos.y;
            min_pos.z = f;
            cal_o_matrix();
            projection_matrix = &o_matrix;
            break;
    }
}

void Camera::cal_o_matrix() {
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

const Matrix4x4& Camera::get_view_matrix() {
    return v_matrix;
}

const Matrix4x4 * Camera::get_projection_matrix() {
    return projection_matrix;
}

const Vec3f & Camera::get_view_dir() {
    return view_dir;
}
