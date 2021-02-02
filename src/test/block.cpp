#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

#define VALUETYPE float
#define INDEXTYPE int
#define SMALLINDEXTYPE uint16_t

#include "typedefs.h"
#include "common.h"
#include "matrix/coo.1.h"
#include "matrix/coo.2.h"
#include "matrix/block_coo.1.h"
#include "matrix/csr.h"
#include "matrix/csr.2.h"
#include "matrix/dense.h"
#include "io/input.h"
#include "test/utils.h"

int main(int argc, char* argv[])
{
  int ret;
  double t_avg;
  float nnzratio;
  unsigned long N;
  float alpha = 1.0f, beta = 1.0f;
  VALUETYPE *x, *y;

  Coo<VALUETYPE,INDEXTYPE> coo;
  Coo2<VALUETYPE,INDEXTYPE> coo2;
  BlockCoo<VALUETYPE,INDEXTYPE,SMALLINDEXTYPE> coo_si;
  Csr <VALUETYPE,INDEXTYPE> csr;
  Csr2<VALUETYPE,INDEXTYPE> csr2;
  BlockCsr<VALUETYPE,INDEXTYPE,SMALLINDEXTYPE> csr_si;
  Dense<VALUETYPE,INDEXTYPE> dense;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [binary matrix filename(s)]\n", argv[0]);
    return 1;
  }

  srand(0);

  printf("   N,    NNZ, NZ-RATIO,      COO,      COO2,   COO_SI,"
    "     CSR,     CSR2,   CSR_SI,    DENSE\n");

  // Iterate over every filename provided as argument
  for (int f=1; f<argc; f++) {

    // Load matrix to memory
    ret = read_COO(&coo2, argv[f]);
    if (ret!=0) {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1; 
    }

    sscanf(basename(argv[f]), "size%luratio%f%*s", &N, &nnzratio);
    printf("%lu, %6lu, %f, ", (unsigned long) N, (unsigned long) coo2.nnz, nnzratio);
    fflush(stdout);

    // Initialize x and y vectors
    vector_zero(&x, coo2.columns);
    vector_random(&y, coo2.rows);

    /* ----------------------- Coo ----------------------- */
    Coo_to_Coo(&coo, &coo2);

    BENCH_AVG( spmv(&coo, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    /* ----------------------- Coo2 ----------------------- */
    
    BENCH_AVG( spmv(&coo2, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    /* --------------------- Coo (SI) --------------------- */

    Coo_to_Coo(&coo_si, coo2.triplets, coo2.nnz);
    
    BENCH_AVG( spmv(&coo_si, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    release(coo_si);


    /* ----------------------- Csr ------------------------ */
    Coo_to_Csr(&csr, &coo2);

    BENCH_AVG( spmv_serial(&csr, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    /* ----------------------- Csr2 ----------------------- */
    Coo_to_Csr(&csr2, &coo2);

    BENCH_AVG( spmv_serial(&csr2, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    release(csr2);

    /* ----------------------- Csr (SI) ------------------- */
    Coo_to_Csr(&csr_si, coo2.triplets, coo2.rows, coo2.columns, coo2.nnz);

    BENCH_AVG( spmv_serial(&csr_si, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    release(csr_si);
    

    /* ----------------------- Dense ---------------------- */
    Coo_to_Dense(&dense, &coo2);

    BENCH_AVG( spmv_serial(&dense, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    // Clean up
    vector_release(y);
    vector_release(x);
    release(coo2);
  }
  
  return 0;
}
