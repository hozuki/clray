cmake_minimum_required(VERSION 3.4.3)
project(clray C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(CLRAY_SOURCES src/main.cpp src/vec3.h src/vec3.c src/img.h src/img.c src/ray.c src/ray.h src/utils.h src/sphere.c src/sphere.h src/camera.c src/camera.h src/utils.c src/pbr_material.c src/pbr_material.h src/hit_record.h src/vec2.h src/vec2.c src/scene.c src/scene.h src/raytracer.cpp src/raytracer.h src/runner_compute.c src/runner_compute.h src/rt_options.h src/compat.h)
add_executable(clray ${CLRAY_SOURCES})

if (UNIX)
    # libm (math) for sqrt
    target_link_libraries(clray m)
endif (UNIX)

include(FindOpenCL)

if (OpenCL_FOUND)
    message("Found OpenCL lib at ${OpenCL_INCLUDE_DIR} ${OpenCL_LIBRARY}")
else ()
    message(FATAL_ERROR "Please install one of the OpenCL SDKs.")
endif ()

include(thirdparty/computecpp-sdk/cmake/Modules/FindComputeCpp.cmake)

if (ComputeCpp_FOUND)
    message("Found ComputeCpp lib at ${ComputeCpp_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Please install ComputeCpp.")
endif ()

include(thirdparty/computecpp-sdk/cmake/Modules/ComputeCppCompilerChecks.cmake)

target_include_directories(clray PUBLIC ${OpenCL_INCLUDE_DIR} ${ComputeCpp_INCLUDE_DIRS})
target_link_libraries(clray ${OpenCL_LIBRARY})

add_sycl_to_target(TARGET clray SOURCES ${CLRAY_SOURCES})
