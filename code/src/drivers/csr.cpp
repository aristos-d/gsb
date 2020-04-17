#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/csr.2.h"
#include "io/input.h"
#include "common.h"
#include "test/utils.h"

#ifndef MATRIXTYPE
#define MATRIXTYPE Csr<VALTYPE,ITYPE> A;
#endif

int main (int argc, char * argv[])
{
  int ret;
  double t;
  VALTYPE *x, *y;
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

  vector_init(&x, B.columns);
  vector_init(&y, B.rows);

  BENCH_CSV( spmv(&A, x, y), ITERATIONS, nonzeros(A), "CSR");

  vector_release(x);
  vector_release(y);
  release(A);
  return 0;
}
