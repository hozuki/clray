cmake_minimum_required(VERSION 3.0)
project(clray C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_C_STANDARD 99)

add_executable(clray src/main.c src/vec3.h src/vec3.c src/img.h src/img.c src/ray.c src/ray.h src/utils.h src/sphere.c src/sphere.h src/camera.c src/camera.h src/utils.c src/pbr_material.c src/pbr_material.h src/hit_record.h src/vec2.h src/vec2.c src/scene.c src/scene.h src/raytracer.c src/raytracer.h)

if (UNIX)
    # libm (math) for sqrt
    target_link_libraries(clray m)
endif (UNIX)
