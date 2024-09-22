#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "typedefs.h"
#include "utils.h"
#include "common.h"
#include "io/input.h"
#include "io/output.h"

#define VALUETYPE float
#define INDEXTYPE uint32_t

/*
This executable reads a matrix in COO format (as Coo3) and an array of block
sizes. It then sorts the non-zeros according to block ID, row and column. Then,
it stores the matrix (with the block IDs) to a file on disk.
Matrix  must be zero-indexed
*/
int main(int argc, char * argv[]){
  Coo3<VALUETYPE, INDEXTYPE> A;
  int i, ret;
  double t;
  size_t size;
  INDEXTYPE *block_sizes, *block_offsets;

  if(argc != 6){
    fprintf(stderr, "Usage: %s [rows] [columns] [values] [block-sizes] [output]\n", argv[0]);
    exit(1);
  }

  printf("Reading block sizes from disk...");
  tick();
  ret = read_bin_array(argv[4], &block_sizes, &size );
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while reading block sizes.\n");
    exit(1);
  }else{
    printf(" done in %.3f sec\nBlock size info : %lu block-rows and %lu block-columns\n", t, size, size);
  }

  A.blockrows = (INDEXTYPE) size;
  A.blockcols = (INDEXTYPE) size;

  puts("Calculating block offsets from block sizes...");
  block_offsets = (INDEXTYPE *) malloc( (size + 1) * sizeof(INDEXTYPE) );
  if(block_offsets==NULL){
    fprintf(stderr, "Error. Could not allocate memory for block offsets.\n");
    exit(1);
  }
  size_to_offset(block_offsets, block_sizes, A.blockrows);

  A.rows = block_offsets[A.blockrows];
  A.columns = block_offsets[A.blockcols];
  free(block_sizes);  // Block sizes not needed anymore

  printf("Reading matrix from disk..."); fflush(stdout);
  tick();
  ret = read_bin_COO(&A, argv[1], argv[2], argv[3]);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while reading matrix.\n");
    exit(1);
  }
  printf(" done in %.3f sec\n", t);

  printf("Matrix info : %lux%lu matrix with %lu non-zeros\n", (unsigned long) A.rows, (unsigned long) A.columns, (unsigned long) A.nnz);

  printf("Calculating block ids..."); fflush(stdout);
  tick();
  calculate_block_id(A.elements, A.nnz, block_offsets, A.blockrows, block_offsets, A.blockcols);
  t = tock();
  printf(" done in %.3f sec\n", t);

  free(block_offsets);  // Block offsets not needed anymore

  printf("Sorting..."); fflush(stdout);
  tick();
  sort_elements_blocks(A.elements, A.nnz);
  t = tock();
  printf(" done in %.3f sec\n", t);

  printf("Counting non-zero blocks..."); fflush(stdout);
  tick();
  A.nnzblocks = count_blocks(A.elements, A.nnz);
  t = tock();
  printf(" done in %.3f sec\n", t);

  printf("Writing to disk..."); fflush(stdout);
  tick();
  ret = write_bin_blocked(A, argv[5]);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while writing resulting matrix to disk.\n");
  }else{
    printf(" done in %.3f sec\n", t);
  }

  free(A.elements);
  return 0;
}
