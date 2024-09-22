#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INDEXTYPE uint32_t
#define VALUETYPE float

/*
 * Tool to read binary files in some of the formats used in this project
 * WARNING : Always pipe results somewhere (eg. to 'less')
*/
int main (int argc, char * argv[])
{
    unsigned long i;
    int mode;
    INDEXTYPE n, r, c, nnz;
    VALUETYPE v;
    FILE * f;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [matrix-file] mode\n", argv[0]);
        fprintf(stderr, "\twhere 'mode' is either 'triples', 'blocks' or 'vector' (default is blocks)\n");
        return 1;
    }

    if (argc == 3 && strncmp(argv[2], "triples", 8) == 0) {
        mode = 0;
    } else if (argc == 3 && strncmp(argv[2], "blocks", 7) == 0) {
        mode = 1;
    } else if (argc == 3 && strncmp(argv[2], "vector", 7) == 0) {
        mode = 2;
    } else {
        printf("Assuming input is in blocked format.\n");
        mode = 1;
    }

    f = fopen(argv[1], "rb");
    if(f==NULL){
        fprintf(stderr, "Could not open file.\n");
    }

    switch(mode) {
        case 0:  // Binary triples file
        fread(&r, sizeof(INDEXTYPE), 1, f);
        fread(&c, sizeof(INDEXTYPE), 1, f);
        fread(&nnz, sizeof(INDEXTYPE), 1, f);
        printf("%lu rows x %lu columns, %lu non-zeros\n",
                (unsigned long) r, (unsigned long) c, (unsigned long) nnz);

        for (i=0; i<nnz; i++) {
            fread(&n, sizeof(INDEXTYPE), 1, f);
            printf("%12lu\n", (unsigned long) n);
        }
        printf("---------------------------------------\n");

        for (i=0; i<nnz; i++) {
            fread(&n, sizeof(INDEXTYPE), 1, f);
            printf("%12lu\n", (unsigned long) n);
        }
        printf("---------------------------------------\n");

        for (i=0; i<nnz; i++) {
            fread(&v, sizeof(VALUETYPE), 1, f);
            printf("%12f\n", (double) v);
        }
        break;

        case 1: // Binary file with block info (.csbr)
        fread(&r, sizeof(INDEXTYPE), 1, f);
        fread(&c, sizeof(INDEXTYPE), 1, f);
        fread(&nnz, sizeof(INDEXTYPE), 1, f);
        fread(&n, sizeof(INDEXTYPE), 1, f);
        fread(&n, sizeof(INDEXTYPE), 1, f);
        fread(&n, sizeof(INDEXTYPE), 1, f);

        for (i=0; i<nnz; i++) {
            fread(&n, sizeof(INDEXTYPE), 1, f);
            printf("%12u\t", n);
            fread(&n, sizeof(INDEXTYPE), 1, f);
            printf("%12u\t", n);
            fread(&n, sizeof(INDEXTYPE), 1, f);
            printf("%12u\t", n);
            fread(&v, sizeof(VALUETYPE), 1, f);
            printf("%12e\n", v);
        }
        break;

        case 2: // Binary vector file (INDEXTYPE)
        while(fread(&n, sizeof(INDEXTYPE), 1, f) > 0) printf("%12u\n", n);
        break;
    }

    fclose(f);
    return 0;
}
