#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SUFFIX ".float"
#define FNBUFL 128

#define OLDTYPE double
#define NEWTYPE float

int main(int argc, char * argv[])
{
  unsigned long size;
  char buf[FNBUFL];
  OLDTYPE v;
  NEWTYPE v_new;
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

    size = 0;
    f_in = fopen(argv[i], "rb");
    f_out = fopen(buf, "wb");

    if(f_in == NULL || f_out == NULL){
      fprintf(stderr, "Cannot open files. Aborting.\n");
      exit(1);
    }

    while(fread(&v, sizeof(OLDTYPE), 1, f_in) > 0){
      size++;
      v_new = (NEWTYPE) v;
      fwrite(&v_new, sizeof(NEWTYPE), 1, f_out);
    }

    fclose(f_in);
    fclose(f_out);
    
    printf("Conversion complete : %lu values\n", size);
  }
  
  return 0;
}
