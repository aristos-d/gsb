#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "typedefs.h"
#include "utils.h"
#include "io/input.h"
#include "io/output.h"

#define SUFFIX ".sorted"
#define FNBUFL 128

int main(int argc, char * argv[]){
  Coo2<double, uint32_t> A;
  int i, ret1=0, ret2=0;
  size_t size;
  uint32_t * block_sizes;
  char buf_row[FNBUFL], buf_col[FNBUFL], buf_val[FNBUFL], buf_mtx[FNBUFL];

  // Read files
  switch(argc){
    case 5 :
      ret2 = read_bin_array(argv[4], &block_sizes, &size );
    case 4 :
      ret1 = read_bin_COO<double, uint32_t>
                            (&A, argv[1], argv[2], argv[3]);
      break;
    case 3 :
      ret2 = read_bin_array(argv[4], &block_sizes, &size );
    case 2 :
      ret1 = read_mm_COO(&A, argv[1]);
      break;
    default:
      fprintf(stderr, "Usage: %s [row-filename] [col-filename] [val-filename] \n", argv[0]);
      fprintf(stderr, "OR     %s [row-filename] [col-filename] [val-filename] [block-sizes-filename]\n", argv[0]);
      fprintf(stderr, "OR     %s [mtx-filename] \n", argv[0]);
      fprintf(stderr, "OR     %s [mtx-filename] [block-sizes-filename]\n", argv[0]);
      exit(1);
  }

  // Check read is complete
  if(ret1<0 || ret2 < 0){
    fprintf(stderr, "Error while reading matrix.\n");
    exit(1);
  }

  // Sort matrix
  switch(argc){
    case 5 :
    case 3 :
      // Sort according to blocks
      sort_triplets_blocks(A.triplets, A.nnz,
            block_sizes, (uint32_t) size, block_sizes, (uint32_t) size);
      break;
    case 4 :
    case 2 :
      // Sort according to row
      sort_triplets(A.triplets, A.nnz);
      break;
  }

  // Write result to new files
  switch(argc){
    case 5 :
    case 4 :
      // Create new filenames for output
      strncpy(buf_row, argv[1], FNBUFL-10);
      strcat(buf_row, SUFFIX);
      fprintf(stdout, "Output file for rows : %s\n", buf_row);

      strncpy(buf_col, argv[2], FNBUFL-10);
      strcat(buf_col, SUFFIX);
      fprintf(stdout, "Output file for columns : %s\n", buf_col);

      strncpy(buf_val, argv[3], FNBUFL-10);
      strcat(buf_val, SUFFIX);
      fprintf(stdout, "Output file for values : %s\n", buf_val);

      // Write result to disk
      ret1 = write_bin_COO(&A, buf_row, buf_col, buf_val);
      break;
    case 3 :
    case 2 :
      // Create filename for output
      strncpy(buf_mtx, argv[1], FNBUFL-10);
      strcat(buf_mtx, SUFFIX);
      fprintf(stdout, "Output file for rows : %s\n", buf_mtx);

      // Write result to disk
      ret1 = write_mm_COO(&A, buf_mtx);
      break;
  }

  // Check write
  if(ret1<0){
    fprintf(stderr, "Error while writing resulting matrix to disk.\n");
  }

  release(A);
  return 0;
}
