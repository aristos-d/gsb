#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define float float
#define int int

#include "typedefs.h"
#include "common.h"
#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/cgbr.h"
#include "matrix/cswr.h"
#include "io/input.h"
#include "test/utils.h"

int main(int argc, char* argv[])
{
  int ret;
  double t;
  float *x, *y1, *y2;

  Coo<float,int> coo;
  Coo<float,int> coo_mkl; 
  Coo3<float,int> coo3;
  Csr<float,int> csr;
  Csr<float,int> csr_mkl;
  Cgbr<float,int,int> cgbr;
  Cswr<float,int,int> cswr;

  if (argc < 2){
    fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
    exit(1);
  }

  printf("Reading matrix from file...\n");
  ret = read_mm_COO(&coo3, argv[1]);
  if(ret!=0){
    fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
    exit(1);
  }

  int b_size = (int) BLOCK_SIZE(coo3.rows, coo3.columns);
  if(b_size > coo3.rows) b_size = coo3.rows;
  if(b_size > coo3.columns) b_size = coo3.columns;
  printf("Block size : %d\n", b_size);

  // Allocate memory for x, y and initialize x
  x = (float *) alligned_malloc(coo3.columns * sizeof(float));
  y1 = (float *) alligned_malloc(coo3.rows * sizeof(float));
  y2 = (float *) alligned_malloc(coo3.rows * sizeof(float));
  srand(0);
  for(int i=0; i<coo3.rows; i++){
    y1[i] = (float) 0.0f;
    y2[i] = (float) 0.0f;
  }
  for(int i=0; i<coo3.columns; i++){
    x[i] = ((float) rand())/RAND_MAX;
  }
  printf("Random vector initialized.\n");

  /* ----------------------- Csr ----------------------- */
  puts("----------------------- CSR -----------------------");
  printf("Converting to CSR format... "); fflush(stdout);
  tick();
  Coo_to_Csr(&csr, &coo3);
  t = tock();
  printf("%f sec\n", t);

  print_info(csr);

  // Time spmv with CSR matrix
  tick();
  spmv_serial(&csr, x, y1);
  t = tock();
  printf("spmv : %f sec\n", t);

  /* ----------------------- Csr (MKL) ----------------------- */
  puts("----------------------- CSR (MKL) -----------------------");
  printf("Converting to CSR (MKL)... "); fflush(stdout);
  tick();
  Coo_to_Csr(&csr_mkl, &coo3);
  t = tock();
  printf("%f sec\n", t);

  // Time spmv with CSR matrix
  tick();
  spmv(&csr_mkl, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(csr);
  release(csr_mkl);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(int i=0; i<coo3.rows; i++) y2[i] = 0.0f;

  /* ----------------------- Coo (MKL) ----------------------- */
  puts("----------------------- COO (MKL) -----------------------");
  printf("Converting to COO (MKL)... "); fflush(stdout);
  tick();
  Coo_to_Coo(&coo_mkl, coo3.elements, coo3.rows, coo3.columns, coo3.nnz);
  t = tock();
  printf("%f sec\n", t);

  // Time spmv with COO matrix
  tick();
  spmv(&coo_mkl, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(coo_mkl);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(int i=0; i<coo3.rows; i++) y2[i] = 0.0f;

  /* ----------------------- Cswr ----------------------- */
  puts("----------------------- CSWR -----------------------");
  printf("Converting to CSWR..."); fflush(stdout);
  tick();
  Coo_to_Cswr(&cswr, &coo3, 32);
  t = tock();
  printf("%f sec\n", t);

  // Time spmv with COO matrix
  tick();
  spmv(&cswr, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(cswr);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(int i=0; i<coo3.rows; i++) y2[i] = 0.0f;

  /* ----------------------- Cgbr ----------------------- */
  puts("----------------------- CGBR -----------------------");
  printf("Converting... "); fflush(stdout);
  tick();
  Coo_to_Cgbr(&cgbr, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t); 
  
  print_info(cgbr);

  // Time spmv with CGBR matrix
  tick();
  spmv_serial(&cgbr, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(coo3);
  release(cgbr);

  // Check if results match
  check_results(y1, y2, coo3.rows);

  // Free memory
  aligned_free(y1);
  aligned_free(y2);
  aligned_free(x);
  return 0;
}
