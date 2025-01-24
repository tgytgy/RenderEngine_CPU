//
// Created by 田庚雨 on 2025/1/20.
//

#include "RenderOutput.h"
#include <iostream>
#include "MathUtils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

RenderOutput::RenderOutput(Camera *camera, const Vec2i &resolution, Model *model, Light *light,int model_count): image_arr(new unsigned char [resolution.x * resolution.y * 4]), light(light),resolution(resolution),z_buffer(new float[resolution.x * resolution.y]) {
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
}

void RenderOutput::set_pixel(const int x, const int y, Color color) {
    int index = 4*(x + y*resolution.x);
    image_arr[index] = color.r;       // red
    image_arr[index+1] = color.g;     // green
    image_arr[index+2] = color.b;     // blue
    image_arr[index+3] = color.a;     // alpha
}

void RenderOutput::triangle(Model* model, Vec3f& p0, Vec3f& p1, Vec3f& p2, Vec2f (&uvs)[3], Vec3f (&normals)[3]) {
    vector<int> xVec = {static_cast<int>(p0.x), static_cast<int>(p1.x), static_cast<int>(p2.x)};
    vector<int> yVec = {static_cast<int>(p0.y), static_cast<int>(p1.y), static_cast<int>(p2.y)};
    Vec2i bboxmin(max(0, *ranges::min_element(xVec)), max(0, *ranges::min_element(yVec)));
    Vec2i bboxmax(min(resolution.x, *ranges::max_element(xVec)), min(resolution.y, *ranges::max_element(yVec)));
    Vec2i P;
    Vec2f uv;
    Vec3f normal;
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
                uv.x=0;
                uv.y=0;
                uv.x+=bc_screen.x*uvs[0].x+bc_screen.y*uvs[1].x+bc_screen.z*uvs[2].x;
                uv.y+=bc_screen.x*uvs[0].y+bc_screen.y*uvs[1].y+bc_screen.z*uvs[2].y;
                
                normal.x = 0;
                normal.y = 0;
                normal.z = 0;
                normal.x += bc_screen.x*normals[0].x+bc_screen.y*normals[1].x+bc_screen.z*normals[2].x;
                normal.y += bc_screen.x*normals[0].y+bc_screen.y*normals[1].y+bc_screen.z*normals[2].y;
                normal.z += bc_screen.x*normals[0].z+bc_screen.y*normals[1].z+bc_screen.z*normals[2].z;
                MathUtils::reverse_vec(normal);
                set_pixel(P.x, P.y, Color(255, 255, 255, 255) * std::clamp(MathUtils::vec_cos(light->get_dir(), normal), 0.0f, 1.0f));
                //set_pixel(P.x, P.y, model->get_color(uv) * MathUtils::vec_cos(light->get_dir(), normal));
            }
        }
    }
}

void RenderOutput::rasterize() {
    clear_buffer();
    Model* crt_model = nullptr;
    for (int k = 0; k < model_count; k++) {
        crt_model = model + k;
        for (int i = 0; i < crt_model->nfaces(); i++) {
            vector<int> face = crt_model->face(i);
            Vec3f viewport_pos[3];
            Vec3f world_pos[3];
            Vec2f uv[3];
            Vec3f vert_normal[3];
            for (int j = 0; j < 3; j++) {
                Vec3f w_pos = crt_model->vert(face[j]);
                world_pos[j] = w_pos;
                crt_model->get_world_pos(w_pos, world_pos[j]);
                MathUtils::matrix_multiply_point(camera->get_view_matrix(), world_pos[j], viewport_pos[j]);
                MathUtils::matrix_multiply_point(*camera->get_projection_matrix(), viewport_pos[j], viewport_pos[j]);
                MathUtils::matrix_multiply_point(viewport_matrix, viewport_pos[j], viewport_pos[j]);
                uv[j] = crt_model->uv(i, j);
                crt_model->get_world_normal(i, j, vert_normal[j]);
            }
            Vec3f reverseNormal = (world_pos[2] - world_pos[0]) ^ (world_pos[1] - world_pos[0]);
            reverseNormal = reverseNormal.normalize();
            float intensity = light->get_dir() * reverseNormal;
            float cos_view_face = camera->get_view_dir() * reverseNormal;
            if (cos_view_face > 0.0f) {
                int color = static_cast<int>(intensity * 255);
                triangle(crt_model, viewport_pos[0], viewport_pos[1], viewport_pos[2], uv, vert_normal);
            }
        }
    }
}

void RenderOutput::write_png(string png_name) {
    png_name+=".png";
    flip_image_vertically(image_arr, resolution.x, resolution.y);
    stbi_write_png(png_name.data(), resolution.x, resolution.y, 4, image_arr, resolution.x * 4);
}

void RenderOutput::clear_buffer() {
    for (int i = 0; i < resolution.x * resolution.y; i++) {
        int j = i * 4;
        *(z_buffer + i) = -std::numeric_limits<float>::max();
        image_arr[j] = 0;
        image_arr[j+1] = 0;
        image_arr[j+2] = 0;
        image_arr[j+3] = 255;
    }
}

void RenderOutput::flip_image_vertically(unsigned char* image_arr, int width, int height) {
    int rowSize = width * 4; // 每行的字节数 (4 channels: RGBA)
    unsigned char* tempRow = new unsigned char[rowSize]; // 临时存储一行数据

    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = image_arr + y * rowSize; // 当前行
        unsigned char* row2 = image_arr + (height - 1 - y) * rowSize; // 对应的镜像行

        // 交换两行数据
        std::swap_ranges(row1, row1 + rowSize, row2);
    }

    delete[] tempRow;
}

RenderOutput::~RenderOutput() {
    delete[] image_arr;
    delete[] z_buffer;
}