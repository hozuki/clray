#include <stdbool.h>

#include "runner_cpu.h"
#include "runner_opencl.h"

static bool _test_opencl = true;

int main() {
    if (_test_opencl) {
        run_opencl_test();
    } else {
        run_cpu_test();
    }

    return 0;
}