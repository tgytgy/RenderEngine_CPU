#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

using namespace std;

const auto white = TGAColor(255, 255, 255, 255);
const auto red = TGAColor(255, 0, 0, 255);
Model *model = nullptr;
const int width = 800;
const int height = 800;

Vec3f barycentric(const Vec2i &p0, const Vec2i &p1, const Vec2i &p2, const Vec2i &P) {
    Vec3i u = Vec3i(p1.x - p0.x, p2.x - p0.x, p0.x - P.x) ^ Vec3i(p1.y - p0.y, p2.y - p0.y, p0.y - P.y);
    if (std::abs(u.z) < 1)return {-1, 1, 1};
    return {
        1.f - static_cast<float>(u.x + u.y) / static_cast<float>(u.z),
        static_cast<float>(u.x) / static_cast<float>(u.z), static_cast<float>(u.y) / static_cast<float>(u.z)
    };
}

void triangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2, float *zbuffer, TGAImage &image, const TGAColor &color) {
    vector<int> xVec = {static_cast<int>(p0.x), static_cast<int>(p1.x), static_cast<int>(p2.x)};
    vector<int> yVec = {static_cast<int>(p0.y), static_cast<int>(p1.y), static_cast<int>(p2.y)};
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1); //边界检测
    Vec2i bboxmin(max(0, *ranges::min_element(xVec)), max(0, *ranges::min_element(yVec)));
    Vec2i bboxmax(min(clamp.x, *ranges::max_element(xVec)), min(clamp.y, *ranges::max_element(yVec)));
    Vec2i P;
    float zVal = 0;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            Vec3f bc_screen  = barycentric(Vec2i{xVec[0], yVec[0]}, Vec2i{xVec[1], yVec[1]}, Vec2i{xVec[2], yVec[2]}, P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            zVal = 0;
            zVal += p0.z * bc_screen.x;
            zVal += p1.z * bc_screen.y;
            zVal += p2.z * bc_screen.z;
            if (zbuffer[P.x + P.y * width] < zVal) {
                zbuffer[P.x + P.y * width] = zVal;
                image.set(P.x, P.y, color);
            }
        } 
    } 
}

/// 绘制线段方法
/// @param x0 线段起点x坐标
/// @param y0 线段起点y坐标
/// @param x1 线段终点x坐标
/// @param y1 线段终点y坐标
/// @param image 被绘制的图像
/// @param color 线段颜色
void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color) {
    bool steep = false;
    if (abs(y1 - y0) > abs(x1 - x0)) {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int dError = abs(dy) * 2;
    int error = 0;
    int y = y0;
    const int unitY = y1 - y0 > 0 ? 1 : -1;
    if (steep) {
        //把if_else拿到循环体外以提升性能，这样就不用每次循环判断一次，现代编译器可能会识别并优化这点，还没研究哪些版本可以优化所以这里手动优化
        for (int x = x0; x < x1; x++) {
            image.set(y, x, color);
            error += dError;
            if (error > dx) {
                y += unitY;
                error -= dx * 2;
            }
        }
    } else {
        for (int x = x0; x < x1; x++) {
            image.set(x, y, color);
            error += dError;
            if (error > dx) {
                y += unitY;
                error -= dx * 2;
            }
        }
    }
}

void line(const Vec2i v0, const Vec2i v1, TGAImage &image, const TGAColor &color) {
    line(v0.x, v0.y, v1.x, v1.y, image, color);
}

Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
}

int main(int argc, char **argv) {
    std::random_device rd;  // 获取一个随机种子
    std::mt19937 gen(rd()); // 初始化随机数生成器
    std::uniform_int_distribution<> int_dis(0, 255);
    TGAImage image(width, height, TGAImage::RGB);
    const Vec3f light_dir = Vec3f(0, 0, -1);
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }
    auto start = chrono::high_resolution_clock::now();
    float *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    //核心代码开始
    for (int i = 0; i < model->nfaces(); i++) {
        vector<int> face = model->face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = world2screen(v);
            world_coords[j] = v;
        }
        Vec3f reverseNormal = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        reverseNormal = reverseNormal.normalize();
        float intensity = light_dir * reverseNormal;
        if (intensity > 0) {
            int color = static_cast<int>(intensity * 255);
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuffer, image,
                    TGAColor(color, color, color, 255));
        }
    }
    //核心代码结束
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Function execution time: " << duration.count() << " seconds" << std::endl;
    image.flip_vertically(); // want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
