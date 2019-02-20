//
// Created by MIC on 2019-02-18.
//

#ifndef CLRAY_CLU_H
#define CLRAY_CLU_H

#ifdef __APPLE__

#include <OpenCL/opencl.h>

#else

#include <CL/cl.h>

#endif

void run_opencl_test();

#endif //CLRAY_CLU_H
