//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_XCL_TYPES_H
#define CLRAY_XCL_TYPES_H

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct {
    ushort u;
} half;

#if defined(X64)

typedef ulong size_t;
typedef long ptrdiff_t;
typedef ulong uintptr_t;
typedef long intptr_t;

#else

typedef uint size_t;
typedef int ptrdiff_t;
typedef uint uintptr_t;
typedef int intptr_t;

#endif

typedef struct _image_2d {
} *image_2d_t;

typedef struct _image_3d {
} *image_3d_t;

typedef struct _sampler {
} *sampler_t;

typedef struct _event {
} *event_t;

#ifndef MAXFLOAT
#define MAXFLOAT (3.402823e+38)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0)
#endif

#ifndef NAN
#define NAN (0.0/0.0)
#endif

#endif //CLRAY_XCL_TYPES_H
