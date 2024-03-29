#include <cstdio>

#include "matrix/coo.2.h"
#include "matrix/csr.1.h"
#include "matrix/csr.2.h"
#include "io/input.h"
#include "common.h"
#include "test/bench.h"

#ifndef MATRIXTYPE
#define MATRIXTYPE Csr<VALTYPE,ITYPE> A;
#endif

int main (int argc, char * argv[])
{
  int ret;
  double t;
  MATRIXTYPE A;
  Coo2<VALTYPE,ITYPE> B;

  if (argc < 2){
    fprintf(stderr, "Usage: %s [matrix]\n", argv[0]);
    return 1;
  }

  printf("Reading matrix from disk..."); fflush(stdout);

  tick();
  ret = read_COO(&B, argv[1]);
  t = tock();

  if (ret!=0) {
    fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
    return 1;
  }
  printf(" done in %f sec\n", t);

  // Conversion COO -> CSR. This requires sorting the COO matrix.
  printf("Converting to CSR..."); fflush(stdout);
  tick();
  Coo_to_Csr(&A, &B);
  t = tock();
  printf(" done in %f sec\n", t);

  print_info(A);

  benchmark_spmv(A, ITERATIONS, "CSR");

  release(A);
  return 0;
}
