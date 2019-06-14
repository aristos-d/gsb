#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INVPERM
#define _NO_CSB_ALIGNED_MALLOC_
#include "common.h"
#include "permute.h"
#include "csb/cilk_util.h"
#include "csb/utility.h"
#include "csb/triple.h"
#include "csb/csc.h"
#include "csb/bicsb.h"
#include "matrix/coo.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "common.h"
#include "test/utils.h"
#include "test/csb.h"

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
    Csc<VALTYPE,INDEXTYPE> * csc;
    BiCsb<VALTYPE,INDEXTYPE> * csb;
    Csr<VALTYPE,INDEXTYPE> csr;   // Don't use MKL for this one!
    Cgbr2<VALTYPE,INDEXTYPE,SINDEXTYPE> cgbr;
  
    if (argc != 3){
        fprintf(stderr, "Usage: %s [matrix] [permutation]\n", argv[0]);
        return 1;
    }

    printf("Cilk threads   : %d\n", __cilkrts_get_nworkers());
    
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

    beta = pick_block_size<VALTYPE,INDEXTYPE>(coo.rows, coo.columns, __cilkrts_get_nworkers());
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

//    // Create Coo copy
//    printf("Converting to COO format..."); fflush(stdout);
//    tick();
//    sort_triplets(coo.elements, coo.nnz, false); // Sort column-major
//    Coo_to_Coo(&triplets, &coo);
//    t = tock();
//    printf(" done in %.4f sec\n", t);
//    
//    // Create CSC copy
//    printf("Converting to CSC format..."); fflush(stdout);
//    tick();
//    csc = new Csc<VALTYPE,INDEXTYPE>(triplets.I, triplets.J, triplets.val,
//                         triplets.nnz, triplets.rows, triplets.columns);
//    t = tock();
//    printf(" done in %.4f sec\n", t);
//  
//    release(triplets);
//
//    // CSB - start
//    printf("Converting to CSB..."); fflush(stdout);
//    tick();
//    csb = new BiCsb<VALTYPE,INDEXTYPE>(*csc, __cilkrts_get_nworkers());
//    t = tock();
//    printf(" done in %.4f sec\n", t);
//    
//    beta = csb->beta();
//    printf("Beta = %u\n", beta);
//
//    typedef PTSR<VALTYPE,VALTYPE> PTDD;
//    BENCH_CSV( bicsb_gespmv<PTDD>(*csb, x, y), ITERATIONS, csb->numnonzeros(), "CSB");
//    
//    delete csc;
//    delete csb;
//    // CSB - end
//    
//    // CSR - start
//    printf("Converting to CSR format..."); fflush(stdout);
//    tick();
//    Coo_to_Csr(&csr, &coo);
//    t = tock();
//    printf(" done in %.4f sec\n", t);
//    
//    BENCH_CSV( spmv(&csr, x, y), ITERATIONS, nonzeros(csr), "CSR");
//    
//    release(csr);
//    // CSR - end

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
