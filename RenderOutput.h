//
// Created by 田庚雨 on 2025/1/20.
//

#ifndef RENDEROUTPUT_H
#define RENDEROUTPUT_H
#include "Camera.h"
#include "Light.h"
#include "model.h"

class RenderOutput {
private:
    Camera* camera;
    Model* model;
    Light* light;
    Matrix4x4 viewport_matrix;
    Matrix4x4 final_viewport;
    const Vec2i& resolution;
    int model_count;
    float* z_buffer;
    unsigned char* image_arr;
    void set_pixel(int x, int y, Color color);
    void triangle(Model* model, Vec3f& p0, Vec3f& p1, Vec3f& p2, Vec2f (&uvs)[3], Vec3f (&normals)[3]);
    void clear_buffer();
    void flip_image_vertically(unsigned char* image_arr, int width, int height);
public:
    RenderOutput(Camera* camera, const Vec2i& resolution, Model* model, Light* light, int model_count);
    void rasterize();
    void write_png(string png_name);
    ~RenderOutput();
};



#endif //RENDEROUTPUT_H
