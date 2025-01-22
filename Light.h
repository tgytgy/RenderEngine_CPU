//
// Created by 田庚雨 on 2025/1/22.
//

#ifndef LIGHT_H
#define LIGHT_H
#include "geometry.h"


class Light {
private:
    Vec3f _dir;
public:
    explicit Light(Vec3f dir);
    Vec3f get_dir() const;
};



#endif //LIGHT_H
