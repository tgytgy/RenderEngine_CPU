//
// Created by 田庚雨 on 2025/1/20.
//

#include "RenderOutput.h"

#include <iostream>

#include "MathUtils.h"

RenderOutput::RenderOutput(Camera* camera, const Vec2i& resolution, Model* model, Light* light, int model_count): image_arr(
        new unsigned char [resolution.x * resolution.y * 4]), light(light), resolution(resolution),
    z_buffer(new float[resolution.x * resolution.y]) {
    this->camera = camera;
    this->model = model;
    this->model_count = model_count;
    float half_width = static_cast<float>(resolution.x) / 2.f;
    float half_height = static_cast<float>(resolution.y) / 2.f;
    viewport_matrix.setValue(0, 0, half_width);
    viewport_matrix.setValue(0, 3, half_width);
    viewport_matrix.setValue(1, 1, half_height);
    viewport_matrix.setValue(1, 3, half_height);
    viewport_matrix.setValue(2, 2, 1);
    viewport_matrix.setValue(3, 3, 1);
    for (int i = 0; i < resolution.x * resolution.y; i++) {
        int j = i * 4;
        *(z_buffer + i) = -std::numeric_limits<float>::max();
        image_arr[j] = 0;
        image_arr[j+1] = 0;
        image_arr[j+2] = 0;
        image_arr[j+3] = 255;
    }
}

void RenderOutput::set_pixel(const int x, const int y, Color color) {
    int index = 4*(x + y*resolution.x);
    image_arr[index] = color.r;       // red
    image_arr[index+1] = color.g;     // green
    image_arr[index+2] = color.b;     // blue
    image_arr[index+3] = color.a;     // alpha
}

void RenderOutput::triangle(Vec3f& p0, Vec3f& p1, Vec3f& p2, Color color) {
    vector<int> xVec = {static_cast<int>(p0.x), static_cast<int>(p1.x), static_cast<int>(p2.x)};
    vector<int> yVec = {static_cast<int>(p0.y), static_cast<int>(p1.y), static_cast<int>(p2.y)};
    Vec2i bboxmin(max(0, *ranges::min_element(xVec)), max(0, *ranges::min_element(yVec)));
    Vec2i bboxmax(min(resolution.x, *ranges::max_element(xVec)), min(resolution.y, *ranges::max_element(yVec)));
    Vec2i P;
    for (P.x = bboxmin.x; P.x < bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y < bboxmax.y; P.y++) {
            Vec3f bc_screen = MathUtils::barycentric(Vec2i{xVec[0], yVec[0]}, Vec2i{xVec[1], yVec[1]}, Vec2i{xVec[2], yVec[2]}, P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            float z = 0;
            z += bc_screen.x*p0.z;
            z += bc_screen.y*p1.z;
            z += bc_screen.z*p2.z;
            if (z > z_buffer[P.x+P.y*resolution.x]) {
                *(z_buffer+P.x+P.y*resolution.x)=z;
                set_pixel(P.x, P.y, color);
            }
        }
    }
}

void RenderOutput::rasterize() {
    Model* crt_model = nullptr;
    for (int k = 0; k < model_count; k++) {
        crt_model = model + k;
        cout << "count: " <<crt_model->nfaces() << endl;
        for (int i = 0; i < crt_model->nfaces(); i++) {
            vector<int> face = crt_model->face(i);
            Vec3f viewport_pos[3];
            Vec3f world_pos[3];
            for (int j = 0; j < 3; j++) {
                Vec3f w_pos = crt_model->vert(face[j]);
                world_pos[j] = w_pos;
                MathUtils::matrix_multiply_point(crt_model->get_transform_matrix(), w_pos, viewport_pos[j]);
                MathUtils::matrix_multiply_point(camera->get_view_matrix(), viewport_pos[j], viewport_pos[j]);
                MathUtils::matrix_multiply_point(*camera->get_projection_matrix(), viewport_pos[j], viewport_pos[j]);
                MathUtils::matrix_multiply_point(viewport_matrix, viewport_pos[j], viewport_pos[j]);
            }
            Vec3f reverseNormal = (world_pos[2] - world_pos[0]) ^ (world_pos[1] - world_pos[0]);
            reverseNormal = reverseNormal.normalize();
            float intensity = light->get_dir() * reverseNormal;
            cout << camera->get_view_dir() * reverseNormal << endl;
            if (camera->get_view_dir() * reverseNormal > 0.0f) {
                int color = static_cast<int>(intensity * 255);
                triangle(viewport_pos[0], viewport_pos[1], viewport_pos[2], Color(color, color, color, 255));
            }
        }
    }
    stbi_write_png("output.png", resolution.x, resolution.y, 4, image_arr, resolution.x * 4);
}

RenderOutput::~RenderOutput() {
    delete[] image_arr;
    delete[] z_buffer;
}

#include "RenderOutput.h"
