//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_XCL_H
#define CLRAY_XCL_H

#if defined(__IN_OPENCL__)

#define kernel
#define __kernel

#define global
#define __global
#define local
#define __local
#define constant
#define __constant

#define __read_only
#define __write_only

#define __OPENCL_VERSION__ ("")
#define __ENDIAN_LITTLE__ (1)
#define __ROUNDING_MODE__ ("rte")
#define __kernel_exec(X, typeN)
#define __IMAGE_SUPPORT__ (1)
#define __FAST_RELAXED_MATH__ (1)

#include "xcl/xcl_types.h"
#include "xcl/xcl_vectors.h"
#include "xcl/xcl_conversions.h"

#include "xcl/xcl_workitem_1_0.h"

#endif

#endif //CLRAY_XCL_H
