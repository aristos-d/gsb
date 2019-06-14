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
#define FNBUFL 128

/*
 * This executable reads a matrix in COO format (as Coo3) and an array of block
 * sizes. It sorts the non-zeros according to row and column. Then, it stores
 * the matrix on disk. Matrix  must be zero-indexed.
 */
int main(int argc, char * argv[])
{
  Coo2<VALUETYPE, INDEXTYPE> A;
  char buf[FNBUFL];
  int ret;
  double t;
  bool bin = false, mtx = false;

  if(argc != 6){
    fprintf(stderr, "Usage: %s [rows] [columns] [values] [output] bin/mtx/all\n", argv[0]);
    return 1;
  }

  if (strncmp(argv[5], "bin", 4) == 0) {
    bin = true;
  } else if (strncmp(argv[5], "mtx", 7) == 0) {
    mtx = true;
  } else if (strncmp(argv[5], "all", 4) == 0) {
    bin = true;
    mtx = true;
  } else {
    fprintf(stderr, "Unrecognized option. Choose between \"bin\", \"mtx\" and \"all\".\n");
    return 1;
  }

  printf("Reading matrix from disk..."); fflush(stdout);
  tick();
  ret = read_bin_COO(&A, argv[1], argv[2], argv[3]);
  t = tock();
  if (ret<0) {
    fprintf(stderr, "Error while reading matrix.\n");
    return 1;
  }

  printf(" done in %.3f sec\n", t);
  printf("Matrix info : %ldx%ld matrix with %ld non-zeros\n", (long) A.rows, (long) A.columns, (long) A.nnz);

  printf("Sorting..."); fflush(stdout);
  tick();
  sort_triplets(A.triplets, A.nnz, false);  // Always sort column-major
  t = tock();
  printf(" done in %.3f sec\n", t);

  // Create a matrix market file
  if (mtx) {
    strncpy(buf, argv[4], FNBUFL-10);
    strcat(buf, ".mtx");
    printf("Writing to file %s ...", buf); fflush(stdout);
    tick();
    ret = write_mm_COO(&A, buf);
    t = tock();
    if(ret<0){
      fprintf(stderr, "Error while writing matrix to disk.\n");
    }else{
      printf(" done in %.3f sec\n", t);
    }
  }
  
  // Create a binary triplet file
  if (bin) {
    strncpy(buf, argv[4], FNBUFL-10);
    strcat(buf, ".bin");
    printf("Writing to file %s ...", buf); fflush(stdout);
    tick();
    ret = write_bin_COO(A, buf);
    t = tock();
    if (ret<0) {
      fprintf(stderr, "Error while writing matrix to disk.\n");
    } else {
      printf(" done in %.3f sec\n", t);
    }
  }

  release(A);
  return 0;
}
