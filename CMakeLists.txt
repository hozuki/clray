cmake_minimum_required(VERSION 3.1)
project(clray C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_C_STANDARD 99)

add_executable(clray src/main.c src/vec3.h src/vec3.c src/img.h src/img.c src/ray.c src/ray.h src/utils.h src/sphere.c src/sphere.h src/camera.c src/camera.h src/utils.c src/pbr_material.c src/pbr_material.h src/hit_record.h src/vec2.h src/vec2.c src/scene.c src/scene.h src/raytracer.c src/raytracer.h src/runner_opencl.c src/runner_opencl.h src/kernel.cl src/opencl_compat.h src/runner_cpu.c src/runner_cpu.h)

if (UNIX)
    # libm (math) for sqrt
    target_link_libraries(clray m)
endif (UNIX)

include(FindOpenCL)

if (OpenCL_FOUND)
    message("Found OpenCL lib at ${OpenCL_INCLUDE_DIR} ${OpenCL_LIBRARY}")
else ()
    message(FATAL_ERROR "Please install one of the OpenCL SDKs.")
endif (OpenCL_FOUND)

target_include_directories(clray PUBLIC ${OpenCL_INCLUDE_DIR})
target_link_libraries(clray ${OpenCL_LIBRARY})
