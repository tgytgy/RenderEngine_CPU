#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"
#include "MathUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(const char *filename, const char* tex_name) : verts_(), faces_(), norms_(), uv_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i=0;i<3;i++) iss >> n.raw[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) iss >> uv.raw[i];
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp.raw[0] >> trash >> tmp.raw[1] >> trash >> tmp.raw[2]) {
                for (int i=0; i<3; i++) tmp.raw[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    tex_data = stbi_load(tex_name, &tex_width, &tex_height, &tex_channels, 0);
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    //load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
    stbi_image_free(tex_data);
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i=0; i<(int)faces_[idx].size(); i++) face.push_back(faces_[idx][i].raw[0]);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

void Model::set_transform(const Vec3f& pos, const Vec3f& rotate, const Vec3f& scale) {
    MathUtils::copy_vec(pos, this->pos);
    MathUtils::copy_vec(rotate, this->rotate);
    MathUtils::copy_vec(scale, this->scale);
    update_transform_matrix();
}

void Model::update_transform_matrix() {
    t_matrix.setValue(0, 0, 1);
    t_matrix.setValue(1, 1, 1);
    t_matrix.setValue(2, 2, 1);
    t_matrix.setValue(3, 3, 1);
    t_matrix.setValue(0, 3, pos.x);
    t_matrix.setValue(1, 3, pos.y);
    t_matrix.setValue(2, 3, pos.z);
    s_matrix.setValue(0, 0, scale.x);
    s_matrix.setValue(1, 1, scale.y);
    s_matrix.setValue(2, 2, scale.z);
    s_matrix.setValue(3, 3, 1);
    MathUtils::set_rotate_matrix(rotate, r_matrix);
    MathUtils::matrix_multiply(t_matrix, r_matrix, s_matrix, model_transform_matrix);
}

const Matrix4x4 & Model::get_transform_matrix() {
    return model_transform_matrix;
}

void Model::get_world_pos(const Vec3f &raw_pos, Vec3f &world_pos) {
    MathUtils::matrix_multiply_point(get_transform_matrix(), raw_pos, world_pos);
}


Color Model::get_color(const Vec2f uv) {
    // 钳制 UV 坐标
    float clamped_u = std::clamp(uv.x, 0.0f, 1.0f);
    float clamped_v = 1.0f - std::clamp(uv.y, 0.0f, 1.0f);

    // 计算纹理坐标
    const int u = static_cast<int>(std::round(static_cast<float>(tex_width - 1) * clamped_u));
    const int v = static_cast<int>(std::round(static_cast<float>(tex_height - 1) * clamped_v));

    // 边界检查
    const int safe_u = std::clamp(u, 0, tex_width - 1);
    const int safe_v = std::clamp(v, 0, tex_height - 1);

    // 采样颜色
    const int index = 4 * (safe_u + safe_v * tex_width);
    return Color(tex_data[index], tex_data[index + 1], tex_data[index + 2], tex_data[index + 3]);
}

Vec2f Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert].raw[1];
    return {uv_[idx].x, uv_[idx].y};
}

Vec3f Model::normal(int iface, int nvert) {
    int idx = faces_[iface][nvert].raw[1];
    return {norms_[idx].x, norms_[idx].y, norms_[idx].z};
}

Vec3f Model::get_world_normal(int iface, int nvert, Vec3f &world_normal) {
    MathUtils::matrix_multiply_vec(get_transform_matrix(), normal(iface, nvert), world_normal);
}

/*void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2i uv) {
    return diffusemap_.get(uv.x, uv.y);
}

Vec2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert].raw[1];
    return Vec2i(uv_[idx].x*diffusemap_.get_width(), uv_[idx].y*diffusemap_.get_height());
}*/

