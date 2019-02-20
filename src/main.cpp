#include <stdio.h>

#include "compat.h"
#include "runner_compute.h"
#include "utils.h"

EXTERN_C int main() {
    uint64_t startTime = time_ms();

    run_compute_test();

    uint64_t endTime = time_ms();

    fprintf(stdout, "Execution time: %f s\n", (endTime - startTime) / 1000.0);

    return 0;
}