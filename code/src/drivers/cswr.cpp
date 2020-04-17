#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix/coo.h"
#include "matrix/cswr.h"
#include "io/input.h"
#include "common.h"
#include "test/utils.h"

#ifndef MATRIXTYPE
#define MATRIXTYPE Cswr<VALTYPE,ITYPE,SITYPE>
#endif

int main(int argc, char * argv[])
{
  int ret;
  double t;
  bool binary;
  MATRIXTYPE A;
  VALTYPE *x, *y;
  Coo2<VALTYPE,ITYPE> B;
  ITYPE width;

  switch(argc){
    case 2:
      printf("Using default width : %u\n", DEFAULT_CSWR_WIDTH);
      width = DEFAULT_CSWR_WIDTH;
      break;
    case 3:
      width = atoi(argv[2]);
      break;
    default:
      fprintf(stderr, "Usage: %s [matrix] [width (optional)]\n", argv[0]);
      return 1;
  }

  printf("Reading matrix from disk..."); fflush(stdout);
  
  tick();
  ret = read_COO(&B, argv[1]);
  t = tock();

  if(ret!=0){
    fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
    return 1;
  }
  printf(" done in %f sec\n", t);

  // Conversion COO -> CSWR. This requires sorting the COO matrix.
  printf("Converting to CSWR..."); fflush(stdout);
  tick();
  Coo_to_Cswr(&A, &B, width);
  t = tock();
  printf(" done in %f sec\n", t);    

  print_info(A);

  vector_init(&x, B.columns);
  vector_init(&y, B.rows);

  BENCH_CSV( spmv(&A, x, y), ITERATIONS, nonzeros(A), "CSWR");

  vector_release(x);
  vector_release(y);

  release(A);
  release(B);
  return 0;
}
