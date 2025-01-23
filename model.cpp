#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"
#include "MathUtils.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_() {
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
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    //load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
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

