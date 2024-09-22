#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "permute.h"
#include "io/input.h"
#include "io/output.h"

#define SUFFIX ".permuted"
#define FNBUFL 128
#define INDEXTYPE uint32_t

int main(int argc, char * argv[])
{
  INDEXTYPE *I, *permutation, *inv_permutation;
  int i, ret;
  size_t perm_size, size;
  char buf[FNBUFL];

  if (argc != 4) {
    fprintf(stderr, "Usage: %s [rows] [columns] [permutation-vector]\n", argv[0]);
    return 1;
  }

  ret = read_bin_array(argv[3], &permutation, &perm_size );
  if (ret<0) {
    fprintf(stderr, "Error while reading permutation vector. Error : %d\n", ret);
    return 1;
  }
  printf("Reading complete : permutation vector of size %lu\n", perm_size);

  #ifdef INVPERM
  inv_permutation = invert_permutation(permutation, (INDEXTYPE) perm_size);
  if (inv_permutation == NULL) {
    fprintf(stderr, "Could not invert permutation vector\n");
    return 1;
  }
  printf("Inverted permutation vector\n");
  free(permutation);
  permutation = inv_permutation;
  #endif

  // Permute columns first
  ret = read_bin_array(argv[2], &I, &size );
  if (ret<0) {
    fprintf(stderr, "Error while reading column indeces. Error : %d\n", ret);
    return 1;
  }
  printf("Reading complete : %lu column indeces\n", size);

  permute_indeces(I, permutation, (INDEXTYPE) size);

  // Create new filename for output
  strncpy(buf, argv[2], FNBUFL-10);
  strcat(buf, SUFFIX);
  fprintf(stdout, "Output file : %s\n", buf);

  // Write result to disk
  ret = write_bin_array(buf, I, size);
  if (ret<0) {
    fprintf(stderr, "Error while writing result to disk.\n");
  }else{
    printf("Writing complete.\n");
  }

  free(I);

  // Now permute rows
  ret = read_bin_array(argv[1], &I, &size );
  if(ret<0){
    fprintf(stderr, "Error while reading row indeces. Error : %d\n", ret);
    return 1;
  }
  printf("Reading complete : %lu row indeces\n", size);

  permute_indeces(I, permutation, (INDEXTYPE) size);

  // Create new filename for output
  strncpy(buf, argv[1], FNBUFL-10);
  strcat(buf, SUFFIX);
  fprintf(stdout, "Output file : %s\n", buf);

  // Write result to disk
  ret = write_bin_array(buf, I, size);
  if(ret<0){
    fprintf(stderr, "Error while writing result to disk.\n");
  }else{
    printf("Writing complete.\n");
  }

  free(I);

  // Clean up
  free(permutation);
  return 0;
}
