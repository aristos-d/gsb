#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "matrix/coo.3.h"
#include "matrix/csr.1.h"
#include "matrix/csr.2.h"
#include "matrix/cswr.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "matrix/cgbr.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "rt.h"
#include "test/utils.h"

int main(int argc, char* argv[])
{
  int ret;
  double t;
  VALTYPE *x, *y1, *y2;

  Coo3<VALTYPE,INDEXTYPE> coo3;
  Csr<VALTYPE,INDEXTYPE> csr;
  Csr2<VALTYPE,INDEXTYPE> csr2;
  Cswr<VALTYPE,INDEXTYPE, uint8_t> cswr;
  Csbr<VALTYPE,INDEXTYPE> csbr;
  Csbr2<VALTYPE,INDEXTYPE> csbr2;
  Cgbr<VALTYPE,INDEXTYPE,INDEXTYPE> cgbr;
  Cgbr<VALTYPE,INDEXTYPE,SMALLINDEXTYPE> cgbr_si;
  Cgbr2<VALTYPE,INDEXTYPE,SMALLINDEXTYPE> cgbr2;
  Element<VALTYPE,INDEXTYPE> *elements_bk;

  if (argc < 2){
    fprintf(stderr, "Usage: %s [martix-filename]\n", argv[0]);
    exit(1);
  }

  printf("Reading matrix from file...\n");
  ret = read_COO(&coo3, argv[1]);
  if(ret!=0){
    fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
    exit(1);
  }

  INDEXTYPE b_size = pick_block_size(coo3.rows, coo3.columns, sizeof(VALTYPE), RT_WORKERS);
  if(b_size > coo3.rows) b_size = coo3.rows;
  if(b_size > coo3.columns) b_size = coo3.columns;
  printf("Block size : %lu\n", (unsigned long) b_size);

  // Allocate memory for x, y and initialize x
  x = new VALTYPE[coo3.columns];
  y1 = new VALTYPE[coo3.rows];
  y2 = new VALTYPE[coo3.rows];

  srand(0);
  for (INDEXTYPE i=0; i<coo3.columns; i++) {
    x[i] = ((VALTYPE) rand())/ (VALTYPE) RAND_MAX;
  }
  printf("Random vector initialized.\n");

  /* ----------------------- Csr ----------------------- */
  printf("Converting to CSR format... "); fflush(stdout);
  tick();
  Coo_to_Csr(&csr, &coo3);
  t = tock();
  printf("%f sec\n", t);

  print_info(csr);

  // Time spmv with CSR matrix
  tick();
  spmv(&csr, x, y1);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(csr);

  /* ----------------------- Csr2 ----------------------- */
  puts("----------------------- CSR(2) -----------------------");
  printf("Converting... "); fflush(stdout);
  tick();
  Coo_to_Csr(&csr2, &coo3);
  t = tock();
  printf("%f sec\n", t);

  // Time spmv with CSR matrix
  tick();
  spmv(&csr2, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(csr2);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  /* ----------------------- Cswr ----------------------- */
  puts("----------------------- CSWR -----------------------");
  printf("Converting... "); fflush(stdout);
  tick();
  Coo_to_Cswr(&cswr, &coo3);
  t = tock();
  printf("%f sec\n", t);

  print_info(cswr);

  // Time spmv with CSWR matrix
  tick();
  spmv(&cswr, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(cswr);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  /* ----------------------- Csbr ----------------------- */
  puts("----------------------- CSBR -----------------------");
  printf("Converting... "); fflush(stdout);
  tick();
  Coo_to_Csbr(&csbr, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t);

  print_info(csbr);

  // Time spmv with CSBR matrix
  tick();
  spmv(&csbr, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  release(csbr);

  /* ----------------------- Csbr2 ----------------------- */
  puts("----------------------- CSBR(2) -----------------------");
  printf("Converting... "); fflush(stdout);
  tick();
  Coo_to_Csbr(&csbr2, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t);

  print_info(csbr2);

  // Time spmv with CSBR(2) matrix
  tick();
  spmv(&csbr2, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(csbr2);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  // Before creating the CGBR matrix we have to back up the data in coo3
  // because CGBR constructor will modify them but we still need original for
  // second CGBR matrix construction.
  elements_bk = new Element<VALTYPE,INDEXTYPE>[coo3.nnz];
  memcpy(elements_bk, coo3.elements, coo3.nnz * sizeof(Element<VALTYPE, INDEXTYPE>));

  /* ----------------------- Cgbr ----------------------- */
  puts("----------------------- CGBR -----------------------");
  printf("Converting with block size %u... ", b_size); fflush(stdout);
  tick();
  Coo_to_Cgbr(&cgbr, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t);

  print_info(cgbr);

  // Time spmv with CGBR matrix
  tick();
  spmv(&cgbr, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  release(cgbr);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  // Copy data back from back up.
  memcpy(coo3.elements, elements_bk, coo3.nnz * sizeof(Element<VALTYPE, INDEXTYPE>));

  /* -------------- Cgbr (with smaller index) ------------ */
  puts("--------------------- CGBR (SI) --------------------");
  printf("Converting with block size %u... ", b_size); fflush(stdout);
  tick();
  Coo_to_Cgbr(&cgbr_si, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t);

  print_info(cgbr_si);

  // Time spmv with CGBR matrix
  tick();
  spmv(&cgbr_si, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  release(cgbr_si);

  // Copy data back from back up again.
  memcpy(coo3.elements, elements_bk, coo3.nnz * sizeof(Element<VALTYPE, INDEXTYPE>));

  /* -------------- Cgbr (2) ------------ */
  puts("--------------------- CGBR(2) --------------------");
  printf("Converting with block size %u... ", b_size); fflush(stdout);
  tick();
  Coo_to_Cgbr(&cgbr2, &coo3, b_size, b_size);
  t = tock();
  printf("%f sec\n", t);

  print_info(cgbr2);

  // Time spmv with CGBR matrix
  tick();
  spmv(&cgbr2, x, y2);
  t = tock();
  printf("spmv : %f sec\n", t);

  // Check if results match
  check_results(y1, y2, coo3.rows);
  for(INDEXTYPE i=0; i<coo3.rows; i++) y2[i] = (VALTYPE) 0.0f;

  release(cgbr2);

  // Clean up
  release(coo3);
  delete [] elements_bk;
  delete [] y1;
  delete [] y2;
  delete [] x;
  return 0;
}
