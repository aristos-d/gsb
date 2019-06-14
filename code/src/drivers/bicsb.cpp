#define NOMINMAX
#define _NO_CSBR_UTILS_
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
#include "csb/spvec.h"
#include "csb/Semirings.h"

#include "params.h"
#include "io/input.h"
#include "test/csb.h"

int main(int argc, char * argv[]){
  int ret;
  double ts, te;
  FILE * f;
  ITYPE beta;
  Coo<VALTYPE, ITYPE> coo;
  Csc<VALTYPE, ITYPE> *A;
  BiCsb<VALTYPE, ITYPE> *B;

  if (argc != 2 && argc != 3){
    fprintf(stderr, "Usage: %s [matrix] logbeta (optional)\n", argv[0]);
    return 1;
  }

  timer_init();
  if (argc == 3) beta = (ITYPE) atoi(argv[2]);
  else beta = 0;

  printf("Reading matrix from disk..."); fflush(stdout);

  ts = timer_seconds_since_init();
  ret = read_COO(&coo, argv[1]);
  te = timer_seconds_since_init();
  
  if (ret!=0) {
    fprintf(stderr, "Something went wrong while reading data. Aborting.\n");
    return 1;
  }
  printf(" done in %f sec\n", te-ts);

  printf("Converting to CSC..."); fflush(stdout);
  ts = timer_seconds_since_init();
  A = new Csc<VALTYPE, ITYPE>(coo.I, coo.J, coo.val, coo.nnz, coo.rows, coo.columns);
  te = timer_seconds_since_init();
  printf(" done in %f sec\n", te-ts);

  // Free some space
  release(coo);

  printf("Converting to CSB..."); fflush(stdout);
  ts = timer_seconds_since_init();
  B = new BiCsb<VALTYPE, ITYPE>(*A, __cilkrts_get_nworkers(), beta);  
  te = timer_seconds_since_init();
  printf(" done in %f sec\n", te-ts);

  delete A;

  printf("Matrix info : %ldx%ld matrix\n", (long) B->rowsize(), (long) B->colsize());

  // Benchmarking happens here
  benchmark_spmv<VALTYPE, ITYPE>(*B, ITERATIONS);
  
  delete B;
  return 0;
}


