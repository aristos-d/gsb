#include <stdlib.h>
#include <stdio.h>

#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/csr.2.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "matrix/cgbr.h"
#include "matrix/cgbr.2.h"
#include "matrix/cswr.h"

#include "test/utils.h" // The INIT, BENCH, BENCH_METH macros are defined here
#include "common.h"

template<class T, class IT>
void benchmark_spmv(const Csbr<T, IT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, nonzeros(A) )

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT>
void benchmark_spmv(const Csbr2<T, IT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, nonzeros(A) )

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT>
void benchmark_spmv(const Bcsr<T, IT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, nonzeros(A) )

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT, class SIT>
void benchmark_spmv(const Cgbr<T,IT,SIT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, nonzeros(A));

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT, class SIT>
void benchmark_spmv(const Cgbr2<T,IT,SIT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, A.nnz);

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT>
void benchmark_spmv(const Csr<T, IT> A, int iterations)
{
  T * x, * y;

  INIT(x, A.columns, y, A.rows);

  printf("Threads   : %d\n", RT_WORKERS);

  BENCH( spmv(&A, x, y), iterations, nonzeros(A) );

  // Free memory
  aligned_free(y);
  aligned_free(x);
}

template<class T, class IT>
void benchmark_spmv(const Csr2<T, IT> A, int iterations)
{
    T * x, * y;
    INIT(x, A.columns, y, A.rows);

    printf("Threads   : %d\n", RT_WORKERS);

    BENCH( spmv(&A, x, y), iterations, nonzeros(A) );

    // Free memory
    aligned_free(y);
    aligned_free(x);
}

template<class T, class IT, class SIT>
void benchmark_spmv(const Cswr<T, IT, SIT> A, int iterations)
{
    T * x, * y;
    INIT(x, A.columns, y, A.rows);

    printf("Threads   : %d\n", RT_WORKERS);

    BENCH( spmv(&A, x, y), iterations, nonzeros(A));

    // Free memory
    aligned_free(y);
    aligned_free(x);
}
