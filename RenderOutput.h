//
// Created by 田庚雨 on 2025/1/20.
//

#ifndef RENDEROUTPUT_H
#define RENDEROUTPUT_H
#include "Camera.h"
#include "model.h"


class RenderOutput {
private:
    Camera camera;
    Vec2i resolution;
    Model* model;
    Matrix4x4 viewport_matrix;
public:
    RenderOutput(Camera& camera, Vec2i& resolution, Model* model);
    void render();
};



#endif //RENDEROUTPUT_H
