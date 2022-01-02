#include <stdlib.h>
#include <stdio.h>

#include "matrix/coo.3.h"
#include "matrix/csbr.3.h"
#include "spmv/omp/csbr.3.h"
#include "io/input.h"
#include "common.h"
#include "test/bench.h"

int main (int argc, char ** argv)
{
    int ret;
    double t;
    Coo3<VALTYPE, ITYPE> B;
    MATRIXTYPE A;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [matrix]\n", argv[0]);
        return 1;
    }

    // Reading matrix
    printf("Reading matrix from disk..."); fflush(stdout);
    tick();
    ret = read_COO(&B, argv[1]);
    t = tock();

    if (ret<0) {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1;
    }
    printf(" done in %.4f sec\n", t);

    ITYPE beta = 256;//pick_block_size(B.rows, B.columns, sizeof(VALTYPE), RT_WORKERS);

    printf("Converting to blocked format..."); fflush(stdout);
    tick();
    Coo_to_Blocked(&A, &B, beta, beta);
    t = tock();
    printf(" done in %.4f sec\n", t);

    release(B);

    print_info(A);

    #ifdef PARTITION_DEBUG
    printf("\n\n");
    partition_dump(A.partition, A.blockrows);

    #else
    // Benchmarking happens here
    benchmark_spmv(A, ITERATIONS, "CSBR3");
    #endif

    // Free memory
    release(A);
    return 0;
}
