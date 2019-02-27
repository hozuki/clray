#include <stdbool.h>
#include <stdio.h>

#include "runner_cpu.h"
#include "runner_opencl.h"
#include "utils.h"

static bool _test_opencl = true;

int main() {
    uint64_t startTime = time_ms();

    if (_test_opencl) {
        run_opencl_test();
    } else {
        run_cpu_test();
    }

    uint64_t endTime = time_ms();

    double timeSecs = (endTime - startTime) / 1000.0;
    fprintf(stdout, "Time used: %f (%f min)\n", timeSecs, timeSecs / 60);

    return 0;
}