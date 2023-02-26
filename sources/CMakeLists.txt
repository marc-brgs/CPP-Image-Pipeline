cmake_minimum_required(VERSION 3.0)
project(image_pipeline_app)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE-DIRECTORIES})
add_executable(image_pipeline_app main.cpp operations.cpp operations.h pipeline.cpp pipeline.h)

target_link_libraries(image_pipeline_app ${OpenCV_LIBS})