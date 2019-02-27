//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_OPENCL_COMPAT_H
#define CLRAY_OPENCL_COMPAT_H

#if defined(__IN_OPENCL__)

typedef int int32_t;
typedef uint uint32_t;
typedef ulong uint64_t;
typedef long int64_t;
typedef float cl_float;
typedef int cl_int;
typedef uint cl_uint;
typedef ulong cl_ulong;

#ifndef NULL

#define NULL (0)

#endif

typedef struct {} *FILE;

#define __global __global
#define __constant __constant
#define __local __local

#else

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#ifdef __APPLE__

#include <OpenCL/opencl.h>

#else

#include <CL/cl.h>

#endif

#define __global
#define __constant
#define __local

#endif

#endif //CLRAY_OPENCL_COMPAT_H
