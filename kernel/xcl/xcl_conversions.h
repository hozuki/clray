//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_XCL_CONVERSIONS_H
#define CLRAY_XCL_CONVERSIONS_H

#include "xcl_types.h"

template<TA, TB>
TA _convert(TB &b) {
    return *(TA *)(&b);
}

#define _DEFINE_CONVERT(type) \
    \
template<TB> \
convert_##type##(TB &b) { \
    return _convert<##type##, TB>(b); \
} \
 \
template<TB> \
as_##type##(TB &b) { \
    return convert_##type##<TB>(b); \
} \

template<TB> \
convert_##type##_rte(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_rtz(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_rtp(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_rtn(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_sat_rte(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_sat_rtz(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_sat_rtp(TB &b) { \
    return convert_##type##<TB>(b); \
} \
 \
template<TB> \
convert_##type##_sat_rtn(TB &b) { \
    return convert_##type##<TB>(b); \
}

#define _DEFINE_CONVERT_V(type) \
_DEFINE_CONVERT(type##2) \
_DEFINE_CONVERT(type##3) \
_DEFINE_CONVERT(type##4) \
_DEFINE_CONVERT(type##8)

_DEFINE_CONVERT(char)
_DEFINE_CONVERT(uchar)
_DEFINE_CONVERT(short)
_DEFINE_CONVERT(ushort)
_DEFINE_CONVERT(int)
_DEFINE_CONVERT(uint)
_DEFINE_CONVERT(long)
_DEFINE_CONVERT(ulong)
_DEFINE_CONVERT(float)

_DEFINE_CONVERT_V(char)
_DEFINE_CONVERT_V(uchar)
_DEFINE_CONVERT_V(short)
_DEFINE_CONVERT_V(ushort)
_DEFINE_CONVERT_V(int)
_DEFINE_CONVERT_V(uint)
_DEFINE_CONVERT_V(long)
_DEFINE_CONVERT_V(ulong)
_DEFINE_CONVERT_V(float)

#endif //CLRAY_XCL_CONVERSION_H
