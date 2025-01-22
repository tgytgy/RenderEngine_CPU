//
// Created by 田庚雨 on 2025/1/22.
//

#include "Light.h"

Light::Light(const Vec3f dir):_dir(dir) {
}

Vec3f Light::get_dir() const {
    return _dir;
}
