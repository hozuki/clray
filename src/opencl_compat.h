//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_OPENCL_COMPAT_H
#define CLRAY_OPENCL_COMPAT_H

#if defined(__IN_OPENCL__)

typedef int int32_t;
typedef ulong int64_t;
typedef long uint64_t;

#ifndef NULL

#define NULL (0)

#endif

typedef struct {} *FILE;

#define __global __global
#define __constant __constant
#define __local __local

#else

#define __global
#define __constant
#define __local

#endif

#endif //CLRAY_OPENCL_COMPAT_H
