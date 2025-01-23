cmake_minimum_required(VERSION 3.30)
project(RenderEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(RenderEngine main.cpp tgaimage.cpp tgaimage.h model.cpp model.h geometry.h
        Matrix4x4.cpp
        Matrix4x4.h
        Camera.cpp
        Camera.h
        MathUtils.cpp
        MathUtils.h
        RenderOutput.cpp
        RenderOutput.h
        stb_image_write.h
        stb_image.h
        Light.cpp
        Light.h
)
