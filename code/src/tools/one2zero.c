#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SUFFIX ".zero"
#define FNBUFL 128

#define INDEXTYPE uint32_t

int main(int argc, char * argv[])
{
  unsigned long size;
  char buf[FNBUFL];
  INDEXTYPE *indeces, n;
  FILE *f_in, *f_out;
  
  if (argc < 2){
    fprintf(stderr, "Usage: %s [file-1] [file-2] ... [file-n]\n", argv[0]);
    exit(1);
  }

  for(int i=1; i<argc; i++){

    // Create new filename for output
    strncpy(buf, argv[i], FNBUFL-7);
    strcat(buf, SUFFIX);
    printf("Output file : %s\n", buf);

    // Read whole array into memory
    f_in = fopen(argv[i], "rb");
    if(f_in == NULL){
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }

    fseek(f_in, 0, SEEK_END);
    size = ftell(f_in);
    n = size / sizeof(INDEXTYPE);
    indeces = (INDEXTYPE *) malloc(n * sizeof(INDEXTYPE));

    fseek(f_in, 0, SEEK_SET);
    fread(indeces, sizeof(INDEXTYPE), n, f_in);
    fclose(f_in);

    // Convert and check
    for(INDEXTYPE j=0; j<n; j++){
        if(indeces[j] == 0){
            fprintf(stderr, "This file is not one-indexed! Aborting.\n");
            free(indeces);
            exit(1);
        }else{
            indeces[j] -= 1;
        }
    }

    // Write result to disk
    f_out = fopen(buf, "wb");
    if(f_out == NULL){
      fprintf(stderr, "Cannot open output file. Aborting.\n");
      exit(1);
    }

    fwrite(indeces, sizeof(INDEXTYPE), n, f_out);
    fclose(f_out);
    
    printf("Conversion complete : %lu values\n", n);
    free(indeces);
  }
  
  return 0;
}
