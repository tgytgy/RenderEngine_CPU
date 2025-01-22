#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "Matrix4x4.h"
#include "tgaimage.h"

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
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    Vec2i uv(int iface, int nvert);
    TGAColor diffuse(Vec2i uv);
    std::vector<int> face(int idx);
    void set_transform(const Vec3f& pos, const Vec3f& rotate, const Vec3f& scale);
    void update_transform_matrix();
    const Matrix4x4& get_transform_matrix();
};

#endif //__MODEL_H__