#define NOMINMAX
#define _NO_CSBR_TIMER_
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <cilk/cilk_api.h>
#include <cilk/cilk.h>

#include "csb/cilk_util.h"
#include "csb/utility.h"
#include "csb/triple.h"
#include "csb/csc.h"
#include "csb/bicsb.h"
#include "csb/bmcsb.h"
#include "csb/spvec.h"
#include "csb/Semirings.h"

#include "params.h"
#include "common.h"
#include "io/input.h"

template<class T, class ITYPE>
int benchmark_SpMV(BmCsb<T, ITYPE, 8> * A, int iterations);

int main(int argc, char * argv[]){
  int ret1, ret2, ret3;
  double ts, te;
  FILE * f;
  INDEXTYPE *rows, *cols;
  VALUETYPE *vals;
  INDEXTYPE nr, nc, nnz, beta;
  Csc<VALUETYPE, INDEXTYPE> *A;
  BmCsb<VALUETYPE, INDEXTYPE, 8> *B; // Third template parameter ???

  if (argc < 4){
    fprintf(stderr, "Usage: %s [rows] [columns] [values]\n", argv[0]);
    exit(1);
  }

  timer_init();

  f = fopen(argv[1], "rb");
  fread(&nr, sizeof(INDEXTYPE), 1, f);
  fread(&nc, sizeof(INDEXTYPE), 1, f);
  fread(&nnz, sizeof(INDEXTYPE), 1, f);
  rows = new INDEXTYPE[nnz];
  cols = new INDEXTYPE[nnz];
  vals = new VALUETYPE[nnz];

  printf("Reading matrix from disk..."); fflush(stdout);
  ts = timer_seconds_since_init();
  // WARNING : Data must be sorted column-majjor
  ret1 = fread(rows, sizeof(INDEXTYPE), nnz, f);  
  ret2 = fread(cols, sizeof(INDEXTYPE), nnz, f);
  ret3 = fread(vals, sizeof(VALUETYPE), nnz, f);
  te = timer_seconds_since_init();
  printf(" done in %f sec\n", te-ts);

  printf("Matrix info : %lu rows x %lu columns, %lu non-zeros\n", nr, nc, nnz);

  if(ret1<nnz || ret2<nnz || ret3<nnz){
    fprintf(stderr, "Something went wrong while reading data. Aborting.\n");
    exit(1);
  }

  printf("Converting to CSC..."); fflush(stdout);
  ts = timer_seconds_since_init();
  A = new Csc<VALUETYPE, INDEXTYPE>(rows, cols, vals, nnz, nr, nc, false);
  te = timer_seconds_since_init();
  printf(" done in %f sec\n", te-ts);

  // Free some space
  delete [] rows;
  delete [] cols;
  delete [] vals;

  printf("Converting to bitmasked CSB..."); fflush(stdout);
  ts = timer_seconds_since_init();
  B = new BmCsb<VALUETYPE, INDEXTYPE, 8>(*A, __cilkrts_get_nworkers());  
  te = timer_seconds_since_init();
  printf(" done in %f sec\n", te-ts);

  delete A;

  printf("Matrix info : %ldx%ld matrix\n", (long) B->rowsize(), (long) B->colsize());

  // Benchmarking happens here
  benchmark_SpMV<VALUETYPE, INDEXTYPE>( B, ITERATIONS);
  
  delete B;
  return 0;
}


template<class T, class ITYPE>
int benchmark_SpMV(BmCsb<T, ITYPE, 8> * A, int iterations){
  typedef PTSR<VALUETYPE,VALUETYPE> PTDD;
  T * x, * y;
  double ts, te, time_avg;
  char * worker_number[] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};

  // Allocate memory
  x = new T[A->colsize()];
  y = new T[A->rowsize()];

  // Initialize x and y
  srand(0);
  for(size_t i=0; i<A->colsize(); i++) x[i] = ((T) rand())/RAND_MAX;
  for(size_t i=0; i<A->rowsize(); i++) y[i] = 0.0f;
  printf("Vectors initialized.\n");

  printf("--------------- Benchmarking ---------------\n");
  printf("Cilk workers, Mean time (sec)\n");

  for(int w=0; w<9; w++){
    // Set up Cilk RTS
    __cilkrts_end_cilk();
    __cilkrts_set_param("nworkers", worker_number[w]);
    __cilkrts_init();
    printf("%3d, ", __cilkrts_get_nworkers());

    // Run SpMV multiple times and measure execution time
    bmcsb_gespmv(*A, x, y);
    ts = timer_seconds_since_init();
    for(int i=0; i<iterations; i++){
      bmcsb_gespmv(*A, x, y);
    }
    te = timer_seconds_since_init();

    // Calculate mean
    time_avg = (te - ts) / iterations;
    printf("%.6f\n", time_avg);
  }

  // Free memory
  delete [] y;
  delete [] x;

  printf("------------ End of benchmarking -----------\n");

  return 0;
}