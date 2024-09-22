#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "utils.h"
#include "permute.h"
#include "expand.h"
#include "common.h"
#include "io/input.h"
#include "io/output.h"

#define FNBUFL 256
#define VALUETYPE float
#define INDEXTYPE uint32_t
#define TMPROWS "/tmp/csbr_prepare/rows"
#define TMPCOLS "/tmp/csbr_prepare/cols"

int main(int argc, char * argv[])
{
  int i, ret;
  char buf[FNBUFL], ext[FNBUFL];
  double t;
  size_t perm_size, index_size, blocks;

  INDEXTYPE *indeces, *permutation_vec;
  INDEXTYPE *block_sizes, *block_offsets;
  INDEXTYPE new_block_size;
  Coo3<VALUETYPE, INDEXTYPE> A;

  if (argc != 7){
    fprintf(stderr, "Usage: %s [rows] [columns] [values] [permutation] [block-sizes] [output]\n", argv[0]);
    return 1;
  }

  // Set up temporary storage
  system("mkdir -p /tmp/csbr_prepare");

  // Read permutation vector
  ret = read_bin_array(argv[4], &permutation_vec, &perm_size );
  if(ret<0){
    fprintf(stderr, "Error while reading permutation vector. Error : %d\n", ret);
    exit(1);
  }
  printf("Loaded permutation vector of size %lu\n", perm_size);

  // Permute rows
  ret = read_bin_array(argv[1], &indeces, &index_size );
  if(ret<0){
    fprintf(stderr, "Error while reading row indeces. Error : %d\n", ret);
    free(permutation_vec);
    return 1;
  }

  permute_indeces(indeces, permutation_vec, (INDEXTYPE) index_size);

  // Write permuted row indeces to disk
  ret = write_bin_array(TMPROWS, indeces, index_size);
  if(ret<0){
    fprintf(stderr, "Error while writing result to disk. Error : %d\n", ret);
    free(indeces);
    free(permutation_vec);
    return 1;
  }

  printf("%lu row indeces permuted.\n", index_size);
  free(indeces);

  // Permute columns
  ret = read_bin_array(argv[2], &indeces, &index_size );
  if(ret<0){
    fprintf(stderr, "Error while reading column indeces. Error : %d\n", ret);
    return 1;
  }

  permute_indeces(indeces, permutation_vec, (INDEXTYPE) index_size);

  // Write permuted column indeces to disk
  ret = write_bin_array(TMPCOLS, indeces, index_size);
  if(ret<0){
    fprintf(stderr, "Error while writing result to disk. Error : %d\n", ret);
    free(indeces);
    free(permutation_vec);
    return 1;
  }

  printf("%lu column indeces permuted.\n", index_size);
  free(permutation_vec);
  free(indeces);

  // Read block sizes
  ret = read_bin_array(argv[5], &block_sizes, &blocks );
  if(ret<0){
    fprintf(stderr, "Error while reading block sizes.\n");
    return 1;
  }

  printf("%lu block sizes loaded.\n", blocks);
  printf("Block size info : %lu block-rows and %lu block-columns\n", blocks, blocks);
  A.blockrows = (INDEXTYPE) blocks;
  A.blockcols = (INDEXTYPE) blocks;

  puts("Calculating block offsets from block sizes...");
  block_offsets = (INDEXTYPE *) malloc( (blocks + 1) * sizeof(INDEXTYPE) );
  if(block_offsets==NULL){
    fprintf(stderr, "Error. Could not allocate memory for block offsets.\n");
    return 1;
  }
  size_to_offset(block_offsets, block_sizes, (INDEXTYPE) blocks);

  A.rows = block_offsets[A.blockrows];
  A.columns = block_offsets[A.blockcols];

  ret = read_bin_COO(&A, TMPROWS, TMPCOLS, argv[3]);
  if(ret<0){
    fprintf(stderr, "Error while reading matrix.\n");
    exit(1);
  }
  printf("Matrix loaded to memory.\n");
  printf("Matrix info : %lux%lu matrix with %lu non-zeros\n", (unsigned long) A.rows, (unsigned long) A.columns, (unsigned long) A.nnz);

  // Calculate block ids
  printf("Calculating block ids... "); fflush(stdout);
  tick();
  calculate_block_id(A.elements, A.nnz, block_offsets, A.blockrows, block_offsets, A.blockcols);
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Sorting rows
  printf("Sorting rows..."); fflush(stdout);
  tick();
  sort_triplets(A.elements, A.nnz, true);
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Write binary triples file with sorted rows
  strncpy(buf, argv[6], FNBUFL-10);
  strcat(buf, ".row");
  printf("Writing to file %s ...", buf); fflush(stdout);

  tick();
  ret = write_bin_COO(A, buf);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while writing matrix to disk.\n");
  }else{
    printf(" done in %.3f sec\n", t);
  }

  // Sort blocks
  printf("Sorting blocks..."); fflush(stdout);
  tick();
  sort_elements_blocks(A.elements, A.nnz);
  t = tock();
  printf(" done in %.3f sec\n", t);

  printf("Counting non-zero blocks..."); fflush(stdout);
  tick();
  A.nnzblocks = count_blocks(A.elements, A.nnz);
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Write file in binary blocked format
  strncpy(buf, argv[6], FNBUFL-10);
  strcat(buf, ".block");

  printf("Writing to file %s ...", buf); fflush(stdout);
  tick();
  ret = write_bin_blocked(A, buf);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while writing matrix to disk.\n");
  }else{
    printf(" done in %.3f sec\n", t);
  }

  // Sort columns
  printf("Sorting columns..."); fflush(stdout);
  tick();
  sort_triplets(A.elements, A.nnz, false);
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Write binary triples file with sorted columns
  strncpy(buf, argv[6], FNBUFL-10);
  strcat(buf, ".col");

  printf("Writing to file %s ...", buf); fflush(stdout);
  tick();
  ret = write_bin_COO(A, buf);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while writing matrix to disk.\n");
  }else{
    printf(" done in %.3f sec\n", t);
  }

  // Expand indeces (MATRIX DATA IS NOW MODIFIED)
  printf("Expanding indeces..."); fflush(stdout);
  tick();
  new_block_size = expand_indeces(A.elements, A.nnz, block_sizes, block_offsets, (INDEXTYPE) blocks);
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Write binary triples file with sorted columns and expanded indeces
  strncpy(buf, argv[6], FNBUFL-20);
  snprintf(ext, FNBUFL, ".col.exp%lu", (unsigned long) new_block_size);
  strcat(buf, ext);
  A.rows = new_block_size * A.blockrows;
  A.columns = new_block_size * A.blockcols;

  printf("Writing to file %s ...", buf); fflush(stdout);
  tick();
  ret = write_bin_COO(A, buf);
  t = tock();
  if(ret<0){
    fprintf(stderr, "Error while writing matrix to disk.\n");
  }else{
    printf(" done in %.3f sec\n", t);
  }

  // Memory clean up
  free(block_sizes);
  free(block_offsets);
  free(A.elements);

  // Clean up temporary storage
  system("rm -rf /tmp/csbr_prepare");

  return 0;
}
