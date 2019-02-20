//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_XCL_VECTORS_H
#define CLRAY_XCL_VECTORS_H

#include "xcl_types.h"

template<typename T>
struct _vector2 {
    union {
        struct {
            T x, y;
        };
        T v[2];
    }
};

template<typename T>
struct _vector3 {
    union {
        struct {
            T x, y, z;
        };
        T v[3];
    }
};

template<typename T>
struct _vector4 {
    union {
        struct {
            T x, y, z, w;
        };
        T v[4];
    }
};

template<typename T>
struct _vector8 {
    T v[8];
};

typedef _vector2<char> char2;
typedef _vector3<char> char3;
typedef _vector4<char> char4;
typedef _vector8<char> char8;

typedef _vector2<uchar> uchar2;
typedef _vector3<uchar> uchar3;
typedef _vector4<uchar> uchar4;
typedef _vector8<uchar> uchar8;

typedef _vector2<short> short2;
typedef _vector3<short> short3;
typedef _vector4<short> short4;
typedef _vector8<short> short8;

typedef _vector2<ushort> ushort2;
typedef _vector3<ushort> ushort3;
typedef _vector4<ushort> ushort4;
typedef _vector8<ushort> ushort8;

typedef _vector2<int> int2;
typedef _vector3<int> int3;
typedef _vector4<int> int4;
typedef _vector8<int> int8;

typedef _vector2<uint> uint2;
typedef _vector3<uint> uint3;
typedef _vector4<uint> uint4;
typedef _vector8<uint> uint8;

typedef _vector2<long> long2;
typedef _vector3<long> long3;
typedef _vector4<long> long4;
typedef _vector8<long> long8;

typedef _vector2<ulong> ulong2;
typedef _vector3<ulong> ulong3;
typedef _vector4<ulong> ulong4;
typedef _vector8<ulong> ulong8;

typedef _vector2<float> float2;
typedef _vector3<float> float3;
typedef _vector4<float> float4;
typedef _vector8<float> float8;

#endif //CLRAY_XCL_VECTORS_H
