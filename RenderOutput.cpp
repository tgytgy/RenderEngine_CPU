//
// Created by 田庚雨 on 2025/1/20.
//

#include "RenderOutput.h"

RenderOutput::RenderOutput(Camera& camera, Vec2i& resolution, Model* model) {
    this->camera = camera;
    this->resolution = resolution;
    this->model = model;
}

#include "RenderOutput.h"
