#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INVPERM
#define _NO_CSB_ALIGNED_MALLOC_
#include "common.h"
#include "permute.h"
#include "matrix/coo.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "common.h"
#include "test/utils.h"

int main(int argc, char * argv[])
{
    int ret;
    double t;
    bool bin;
    size_t perm_size;
    INDEXTYPE * permutation, * inv_permutation;
    INDEXTYPE beta;
    VALTYPE * x, * y;
  
    Coo<VALTYPE,INDEXTYPE> triplets;
    Coo3<VALTYPE,INDEXTYPE> coo;
    Csr<VALTYPE,INDEXTYPE> csr;   // Don't use MKL for this one!
    Cgbr2<VALTYPE,INDEXTYPE,SINDEXTYPE> cgbr;
  
    if (argc != 3){
        fprintf(stderr, "Usage: %s [matrix] [permutation]\n", argv[0]);
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

    // Read permutation vector
    ret = read_bin_array(argv[2], &permutation, &perm_size );
    if (ret<0) {
        fprintf(stderr, "Error while reading permutation vector. Error : %d\n", ret);
        return 1;
    }
    printf("Read permutation vector. Size : %lu\n", perm_size);
  
    #ifdef INVPERM
    inv_permutation = invert_permutation(permutation, (INDEXTYPE) perm_size);
    if (inv_permutation == NULL) {
        fprintf(stderr, "Could not invert permutation vector\n");
        return 1;
    }
    printf("Inverted permutation vector\n");
    free(permutation);
    permutation = inv_permutation;
    #endif

    // Apply permutation
    printf("Applying permutation..."); fflush(stdout);
    tick();
    permute_indeces(coo.elements, coo.nnz, permutation);
    t = tock();
    printf(" done in %.4f sec\n", t);
    delete [] permutation;
    
    // Intitialize y and x
    #define T VALTYPE
    INIT(x, coo.columns, y, coo.rows);
    #undef T

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

    print_info(cgbr);
    
    BENCH_CSV( spmv(&cgbr, x, y), ITERATIONS, nonzeros(cgbr), "GSB");
    
    release(cgbr);
    // GSB - end
  
    // Free memory
    aligned_free(x);
    aligned_free(y);
    release(coo);
    return 0;
}
