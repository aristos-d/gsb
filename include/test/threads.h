#include <stdlib.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "matrix/coo.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "matrix/cgbr.h"
#include "common.h"

static const char * worker_number[] = {
    "1", "2", "4", "8", "16", "32", "64", "128", "256"
};

template <class T, class IT,
          template <typename, typename> class MATRIX>
void benchmark_spmv_threads (MATRIX<T,IT> const A, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;

  // Allocate and initialize x and y
  srand(0);
  vector_random(&x, A.columns);
  vector_zero(&y, A.rows);
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Threads, SpMV\n");

  for (int w=0; w<9; w++) {
    // Set up the parallel runtime
    RT_SET_WORKERS(worker_number[w]);
    printf("%3d, ", RT_WORKERS);

    // Run SpMV multiple times and measure execution time
    spmv(&A, x, y);
    ts = timer_seconds_since_init();
    for (int i=0; i<iterations; i++) spmv(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  vector_release(y);
  vector_release(x);
  return 0;
}

