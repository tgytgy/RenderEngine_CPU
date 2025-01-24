#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "Matrix4x4.h"
#include "tgaimage.h"


struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
    Color operator*(const float f) const {
        return {static_cast<unsigned char>(static_cast<float>(r) * f),
            static_cast<unsigned char>(static_cast<float>(g) * f),
            static_cast<unsigned char>(static_cast<float>(b) * f), a};
    }
};
class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    Vec3f pos = Vec3f(0, 0, 0);
    Vec3f rotate = Vec3f(0, 0, 0);
    Vec3f scale = Vec3f(1, 1, 1);
    Matrix4x4 t_matrix;
    Matrix4x4 r_matrix;
    Matrix4x4 s_matrix;
    Matrix4x4 model_transform_matrix;
    TGAImage diffusemap_;
    unsigned char* tex_data;
    int tex_width, tex_height, tex_channels;
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename, const char* tex_name);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    Vec3f normal(int iface, int nvert);
    Vec3f get_world_normal(int iface, int nvert, Vec3f &world_normal);
    Vec2f uv(int iface, int nvert);
    TGAColor diffuse(Vec2i uv);
    std::vector<int> face(int idx);
    void set_transform(const Vec3f& pos, const Vec3f& rotate, const Vec3f& scale);
    void update_transform_matrix();
    const Matrix4x4& get_transform_matrix();
    void get_world_pos(const Vec3f& raw_pos, Vec3f &world_pos);
    Color get_color(Vec2f uv);
};

#endif //__MODEL_H__