#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _NO_CSB_ALIGNED_MALLOC_
#include "common.h"
#include "matrix/coo.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "test/utils.h"

int main(int argc, char * argv[])
{
    int ret;
    double t;
    bool bin;
    INDEXTYPE beta;
    VALTYPE * x, * y;
  
    Coo3<VALTYPE,INDEXTYPE> coo;
    Csr<VALTYPE,INDEXTYPE> csr;   // Don't use MKL for this one!
    Cgbr2<VALTYPE,INDEXTYPE,SINDEXTYPE> cgbr; 
  
    if (argc != 2){
        fprintf(stderr, "Usage: %s [matrix]\n", argv[0]);
        return 1;
    }

    printf("Threads   : %d\n", RT_WORKERS);
    
    // Reading matrix
    printf("Reading matrix from disk..."); fflush(stdout);
    tick();
    ret = read_COO(&coo, argv[1]);
    t = tock();

    if (ret<0) {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1;
    } 
    printf(" done in %.4f sec\n", t);

    // Pick block size
    beta = pick_block_size(coo.rows, coo.columns, sizeof(VALTYPE), RT_WORKERS);
    printf("Beta = %u\n", beta);

    // Intitialize y and x
    vector_init(&x, coo.columns);
    vector_init(&y, coo.rows);
    
    // CSR - start
    printf("Converting to CSR format..."); fflush(stdout);
    tick();
    Coo_to_Csr(&csr, &coo);
    t = tock();
    printf(" done in %.4f sec\n", t);
    
    BENCH_CSV( spmv(&csr, x, y), ITERATIONS, nonzeros(csr), "CSR");
    
    release(csr);
    // CSR - end

    // GSB - start
    printf("Converting to GSB format..."); fflush(stdout);
    tick();
    Coo_to_Cgbr(&cgbr, &coo, beta, beta);
    t = tock();
    printf(" done in %.4f sec\n", t);
    
    BENCH_CSV( spmv(&cgbr, x, y), ITERATIONS, nonzeros(cgbr), "GSB");
    
    release(cgbr);
    // GSB - end
  
    // Free memory
    vector_release(x);
    vector_release(y);
    release(coo);
    return 0;
}
