cmake_minimum_required(VERSION 3.30)
project(RenderEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(RenderEngine main.cpp tgaimage.cpp tgaimage.h model.cpp model.h geometry.h)
