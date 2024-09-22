#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "typedefs.h"
#include "utils.h"
#include "expand.h"
#include "io/input.h"
#include "io/output.h"

#define SUFFIX ".exp%lu"
#define FNBUFL 256
#define VALUETYPE float
#define INDEXTYPE uint32_t

int main(int argc, char * argv[]){
  Coo3<VALUETYPE, INDEXTYPE> A;
  INDEXTYPE new_block_size;
  INDEXTYPE *block_size, *block_offset;
  int i, ret;
  size_t blocks;
  char buf[FNBUFL], buf2[FNBUFL];
  double t;

  if (argc < 6){
    fprintf(stderr, "Usage: %s [rows] [columns] [values] [block-sizes] [outputfile]\n", argv[0]);
    return 1;
  }

  ret = read_bin_array(argv[4], &block_size, &blocks );
  if(ret<0){
    fprintf(stderr, "Error while reading block sizes. Error : %d\n", ret);
    return 1;
  }
  printf("Reading complete : block sizes for %lu blocks\n", blocks);

  block_offset = (INDEXTYPE *) malloc((blocks+1) * sizeof(INDEXTYPE));
  size_to_offset(block_offset, block_size, (INDEXTYPE) blocks);
  A.rows = block_offset[blocks];
  A.columns = block_offset[blocks];
  A.blockrows = (INDEXTYPE) blocks;
  A.blockcols = (INDEXTYPE) blocks;

  printf("Matrix info :\n\t%lu rows x %lu columns\n\t%lu blocks\n",
          (unsigned long) A.rows, (unsigned long) A.columns,
          (unsigned long) A.blockrows);

  printf("Reading matrix from disk..."); fflush(stdout);
  read_bin_COO(&A, argv[1], argv[2], argv[3]);
  printf(" done\n");

  printf("Calculating block ids... "); fflush(stdout);
  tick();
  calculate_block_id(A.elements, A.nnz, block_offset, A.blockrows, block_offset, A.blockcols);
  t = tock();
  printf(" done in %.3f sec\n", t);

  printf("Sorting columns..."); fflush(stdout);
  tick();
  sort_triplets(A.elements, A.nnz, false);
  t = tock();
  printf(" done in %.3f sec\n", t);

  printf("Expanding blocks..."); fflush(stdout);
  tick();
  new_block_size = expand_indeces(A.elements, A.nnz, block_size, block_offset, A.blockrows);
  t = tock();
  printf(" done in %.3f sec\n", t);

  A.rows = new_block_size * A.blockrows;
  A.columns = new_block_size * A.blockcols;

  // Create new filename for output
  strncpy(buf, argv[5], FNBUFL-10);
  sprintf(buf2, SUFFIX, (unsigned long) new_block_size);
  strcat(buf, buf2);
  printf("Output file : %s\n", buf);

  // Write result to disk
  ret = write_bin_COO(A, buf);
  if(ret<0){
    fprintf(stderr, "Error while writing result to disk.\n");
  }else{
    printf("Writing complete.\n");
  }

  // Clean up
  free(block_size);
  free(block_offset);
  free(A.elements);
  return 0;
}
