#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix/coo.h"
#include "matrix/csr.h"
#include "io/input.h"
#include "common.h"
#include "test/utils.h"

void serial(const IT * row_ptr, const IT * col_ind, const VT * val, const IT M, 
            const VT * x, VT * y);
    
void parallel(const IT * row_ptr, const IT * col_ind, const VT * val, const IT M,
              const VT * x, VT * y);

int main(int argc, char * argv[]){
  int ret;
  double t[ITERATIONS], el;
  bool binary;
  Csr<VT, IT> A;
  Coo2<VT, IT> B;
  VT *x, *y;

  if (argc < 2){
    fprintf(stderr, "Usage: %s [matrix] bin/mtx\n", argv[0]);
    return 1;
  } else if (argc < 3){
    printf("Assuming mtx file format.\n");
    binary = false;
  } else {
     if (strncmp(argv[2], "bin", 4) == 0) {
       binary = true;
     } else if (strncmp(argv[2], "mtx", 4) == 0) {
       binary = false;
     } else {
       fprintf(stderr, "Unrecognized file format option. Choose between \"bin\" and \"mtx\"\n");
       return 1;
     }
  }

  printf("Reading matrix from disk..."); fflush(stdout);
  
  tick();
  if(binary)
    ret = read_bin_COO<Coo2<VT, IT>, VT, IT>(&B, argv[1]);
  else
    ret = read_mm_COO<Coo2<VT, IT>, VT, IT>(&B, argv[1]);
  el = tock();

  if(ret!=0){
    fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
    return 1;
  }
  printf(" done in %f sec\n", el);

  // Conversion COO -> CSR. This requires sorting the COO matrix.
  printf("Converting to CSR..."); fflush(stdout);
  tick();
  Coo_to_Csr(&A, &B);
  el = tock();
  printf(" done in %f sec\n", el);    

  print_info(A);

  #define T VT
  INIT(x, A.columns, y, A.rows);
  #undef T

  BENCH_METH( serial(A.row_ptr, A.col_ind, A.val, A.rows, x, y), 
          ITERATIONS, nonzeros(A), "serial" );

  BENCH_METH( parallel(A.row_ptr, A.col_ind, A.val, A.rows, x, y), 
          ITERATIONS, nonzeros(A), "parallel" );

  release(A);
  return 0;
}
