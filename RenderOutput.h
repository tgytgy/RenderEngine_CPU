//
// Created by 田庚雨 on 2025/1/20.
//

#ifndef RENDEROUTPUT_H
#define RENDEROUTPUT_H
#include "Camera.h"
#include "Light.h"
#include "model.h"
#include "stb_image_write.h"


struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
};
class RenderOutput {
private:
    Camera* camera;
    Model* model;
    Light* light;
    Matrix4x4 viewport_matrix;
    const Vec2i& resolution;
    int model_count;
    float* z_buffer;
    unsigned char* image_arr;
    void set_pixel(int x, int y, Color color);
    void triangle(Vec3f& p0, Vec3f& p1, Vec3f& p2, Color color);
public:
    RenderOutput(Camera* camera, const Vec2i& resolution, Model* model, Light* light, int model_count);
    void rasterize();
    ~RenderOutput();
};



#endif //RENDEROUTPUT_H
