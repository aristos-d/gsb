#include "test/utils.h"

#ifndef BENCH_THREADS

template<class T, class IT>
int benchmark_spmv(BiCsb<T, IT> &A, int iterations)
{
  typedef PTSR<T,T> PTDD;
  T * x, * y;
  double *t = new double[iterations];

  INIT(x, A.colsize(), y, A.rowsize());

  printf("Cilk threads   : %3d\n", __cilkrts_get_nworkers());

  BENCH( bicsb_gespmv<PTDD>(A, x, y), iterations, A.numnonzeros());

  // Free memory
  aligned_free( (unsigned char *) y);
  aligned_free( (unsigned char *) x);
  delete[] t;
  return 0;
}

#else

template<class T, class IT>
int benchmark_spmv(BiCsb<T, IT> &A, int iterations)
{
  typedef PTSR<T,T> PTDD;
  T * x, * y;
  double ts, te, time_avg;
  char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate memory
  srand(0);
  x = (T *) aligned_malloc(A.colsize() * sizeof(T));
  y = (T *) aligned_malloc(A.rowsize() * sizeof(T));
  for(uint32_t i=0; i<A.colsize(); i++) x[i] = ((T) rand())/RAND_MAX;
  for(uint32_t i=0; i<A.rowsize(); i++) y[i] = 0.0f;
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
    bicsb_gespmv<PTDD>(A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++){
      bicsb_gespmv<PTDD>(A, x, y);
    }
    te = timer_seconds_since_init();

    // Calculate mean
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  printf("------------ End of benchmarking -----------\n");

  // Free memory
  aligned_free( (unsigned char *) y);
  aligned_free( (unsigned char *) x);
  return 0;
}

#endif
