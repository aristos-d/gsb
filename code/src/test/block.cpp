#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

#define VALUETYPE float
#define INDEXTYPE int
#define SMALLINDEXTYPE uint16_t

#include "typedefs.h"
#include "common.h"
#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/csr.2.h"
#include "matrix/dense.h"
#include "io/input.h"
#include "test/utils.h"
#include "mkl.h"

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

  if (argc < 2){
    fprintf(stderr, "Usage: %s [binary matrix filename(s)]\n", argv[0]);
    return 1;
  }

  srand(0);

  printf("   N,    NNZ, NZ-RATIO,      COO,      COO2,   COO_SI,  COO_MKL,"
    "     CSR,     CSR2,   CSR_SI,  CSR_MKL,    DENSE, DENSE_MKL\n");

  // Iterate over every filename provided as argument
  for (int f=1; f<argc; f++) {

    // Load matrix to memory
    ret = read_bin_COO(&coo2, argv[f]);
    if (ret!=0) {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1; 
    }

    sscanf(basename(argv[f]), "size%luratio%f%*s", &N, &nnzratio);
    printf("%lu, %6lu, %f, ", N, coo2.nnz, nnzratio); fflush(stdout);

    // Allocate memory for x, y and initialize x
    x = (VALUETYPE *) alligned_malloc(coo2.columns * sizeof(VALUETYPE));
    y = (VALUETYPE *) alligned_malloc(coo2.rows * sizeof(VALUETYPE));
    for(INDEXTYPE i=0; i<coo2.rows; i++) y[i] = (VALUETYPE) 0.0f;
    for(INDEXTYPE i=0; i<coo2.columns; i++) x[i] = ((VALUETYPE) rand())/RAND_MAX;

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

    /* ------------------ Coo (MKL-deprecated) ----------- */

//    void mkl_scoomv (const char *transa , const MKL_INT *m , const MKL_INT *k ,
//      const float*alpha , const char *matdescra , 
//      const float *val , const MKL_INT *rowind , constMKL_INT *colind , const MKL_INT *nnz , 
//      const float *x , const float *beta , float *y );

    BENCH_AVG(
        mkl_scoomv ("N", &(coo.rows), &(coo.columns),
            &alpha, "G__C", 
            coo.val, coo.I, coo.J, &(coo.nnz), 
            x, &beta, y ), 
        ITERATIONS,
        t_avg );
    printf("%e, ", t_avg); fflush(stdout);
    
    release(coo);

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
    
    /* ----------------------- Csr (MKL) ------------------ */
    
    BENCH_AVG(
        mkl_scsrmv("N", &(csr.rows), &(csr.columns),
            &alpha, "G__C",
            csr.val, csr.col_ind,
            csr.row_ptr, csr.row_ptr + 1,
            x, &beta, y),
        ITERATIONS,
        t_avg );
    printf("%e, ", t_avg); fflush(stdout);
    
    release(csr);

    /* ----------------------- Dense ---------------------- */
    Coo_to_Dense(&dense, &coo2);

    BENCH_AVG( spmv_serial(&dense, x, y), ITERATIONS, t_avg );
    printf("%e, ", t_avg); fflush(stdout);

    /* ----------------------- Dense (MKL) ---------------- */
    BENCH_AVG(
        cblas_sgemv(CblasRowMajor, CblasNoTrans, 
            dense.rows, dense.columns, alpha, dense.val, dense.columns,
            x, 1, beta, y, 1),
        ITERATIONS,
        t_avg );
    printf("%e\n", t_avg); fflush(stdout);

    release(dense);

    // Clean up
    aligned_free(y);
    aligned_free(x);
    release(coo2);
  }
  
  return 0;
}
