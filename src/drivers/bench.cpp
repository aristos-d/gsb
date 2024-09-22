#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "matrix/coo.1.h"
#include "matrix/csr.1.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "test/bench.h"

int main (int argc, char * argv[])
{
    int ret;
    double t;
    bool bin;
    INDEXTYPE beta;

    Coo3<VALTYPE,INDEXTYPE> coo;
    Csr<VALTYPE,INDEXTYPE> csr;
    Cgbr2<VALTYPE,INDEXTYPE,SINDEXTYPE> cgbr;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [matrix]\n", argv[0]);
        return 1;
    }

    printf("Threads   : %d\n", RT_WORKERS);

    // Reading matrix
    printf("Reading matrix from disk..."); fflush(stdout);
    tick();
    ret = read_COO(&coo, argv[1]);
    t = tock();

    if (ret<0)
    {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1;
    }
    printf(" done in %.4f sec\n", t);

    // Pick block size
    beta = pick_block_size(coo.rows, coo.columns, sizeof(VALTYPE), RT_WORKERS);
    printf("Beta = %u\n", beta);

    // CSR - start
    printf("Converting to CSR format..."); fflush(stdout);
    tick();
    Coo_to_Csr(&csr, &coo);
    t = tock();
    printf(" done in %.4f sec\n", t);

    benchmark_spmv(csr, ITERATIONS, "CSR");

    release(csr);
    // CSR - end

    // GSB - start
    printf("Converting to GSB format..."); fflush(stdout);
    tick();
    Coo_to_Cgbr(&cgbr, &coo, beta, beta);
    t = tock();
    printf(" done in %.4f sec\n", t);

    benchmark_spmv(cgbr, ITERATIONS, "GSB");

    release(cgbr);
    // GSB - end

    // Free memory
    release(coo);
    return 0;
}
