#include <stdlib.h>
#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "matrix/coo.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "matrix/cgbr.h"
#include "common.h"

template<class T, class IT>
int benchmark_SpMV(Csbr<T, IT> A, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;
  const char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate and initialize x and y
  srand(0);
  x = (T *) malloc(A.columns * sizeof(T));
  y = (T *) malloc(A.rows * sizeof(T));
  for(uint32_t i=0; i<A.columns; i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rows; i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, SpMV, SpMV_sb, SpMV_p, SpMV_p_sb\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    SpMV(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run SpMV multiple times and measure execution time
    SpMV_serial_block(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_serial_block(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run SpMV multiple times and measure execution time
    SpMV_partition(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_partition(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run experimental SpMV multiple times and measure excution time
    SpMV_partition_serial_block(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_partition_serial_block(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");
  
  // Free memory
  free(y);
  free(x);
  return 0;
}

template<class T, class IT>
int benchmark_SpMV(Csbr2<T, IT> A, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;
  const char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate and initialize x and y
  srand(0);
  x = (T *) malloc(A.columns * sizeof(T));
  y = (T *) malloc(A.rows * sizeof(T));
  for(uint32_t i=0; i<A.columns; i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rows; i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, SpMV, SpMV_sb\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    SpMV(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run SpMV multiple times and measure execution time
    SpMV_serial_block(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_serial_block(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  free(y);
  free(x);
  return 0;
}

template<class T, class IT>
int benchmark_SpMV(Bcsr<T, IT> A, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;
  const char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate and initialize x and y
  srand(0);
  x = (T *) malloc(A.columns * sizeof(T));
  y = (T *) malloc(A.rows * sizeof(T));
  for(uint32_t i=0; i<A.columns; i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rows; i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, SpMV, SpMV_sb\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    SpMV(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run SpMV multiple times and measure execution time
    SpMV_serial_block(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_serial_block(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  free(y);
  free(x);
  return 0;
}


template<class T, class IT>
int benchmark_SpMV(Cgbr<T, IT> A, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;
  const char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate and initialize x and y
  srand(0);
  x = (T *) malloc(A.columns * sizeof(T));
  y = (T *) malloc(A.rows * sizeof(T));
  for(uint32_t i=0; i<A.columns; i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rows; i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, SpMV, SpMV_sb\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    SpMV(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f, ", time_avg);

    // Run SpMV multiple times and measure execution time
    SpMV_serial_block(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV_serial_block(&A, x, y);
    te = timer_seconds_since_init();
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  free(y);
  free(x);
  return 0;
}

template<class CSRMATRIX, class T, class IT>
int benchmark_SpMV(CSRMATRIX A, IT columns, int iterations)
{
  T * x, * y;
  double ts, te, time_avg;
  const char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate and initialize x and y
  srand(0);
  x = (T *) malloc(A.columns * sizeof(T));
  y = (T *) malloc(A.rows * sizeof(T));
  for(uint32_t i=0; i<A.columns; i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rows; i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, SpMV\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    SpMV(&A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++) SpMV(&A, x, y);
    te = timer_seconds_since_init();

    // Calculate mean
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  free(y);
  free(x);
  return 0;
}
