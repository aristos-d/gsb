#include <cstdlib>
#include <cstdio>

#include "matrix/coo.3.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "matrix/csbr.3.h"
#include "matrix/bcsr.h"
#include "matrix/cgbr.h"
#include "matrix/cgbr.2.h"
#include "io/input.h"
#include "io/blocked.h"
#include "common.h"
#include "test/bench.h"

int main(int argc, char * argv[])
{
    int ret;
    double t;
    ITYPE *block_sizes, *block_offsets_row;
    ITYPE *block_offsets_col;
    size_t num_blocks_row, num_blocks_col;
    Coo3<VALTYPE, ITYPE> B;
    MATRIXTYPE A;

    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage: %s [matrix] ([block-row-sizes] [block-col-size])\n"
                      , argv[0]);
        return 1;
    }

    // Reading matrix
    printf("Reading matrix from disk..."); fflush(stdout);
    tick();
    ret = read_COO(&B, argv[1]);
    t = tock();

    if (ret<0) {
        fprintf(stderr, "Something went wrong while reading the matrix. Aborting.\n");
        return 1;
    }
    printf(" done in %.4f sec\n", t);

    if (argc == 5) {

        // Reading block sizes for rows
        printf("Reading block-row sizes from disk..."); fflush(stdout);
        tick();
        ret = read_bin_array(argv[2], &block_sizes, &num_blocks_row );
        t = tock();
        if (ret<0) {
            fprintf(stderr, "Error while reading block-row sizes.\n");
            return 1;
        }
        printf(" done in %.4f sec.\n", t);

        printf("Calculating block-row offsets from block sizes..."); fflush(stdout);
        block_offsets_row = new ITYPE[num_blocks_row + 1];
        size_to_offset(block_offsets_row, block_sizes, (ITYPE) num_blocks_row);
        printf(" done\n");

        free(block_sizes);  // Block sizes not needed anymore

        // Reading block size for columns
        printf("Reading block-coloumn sizes from disk..."); fflush(stdout);
        tick();
        ret = read_bin_array(argv[3], &block_sizes, &num_blocks_col );
        t = tock();
        if (ret<0) {
            fprintf(stderr, "Error while reading block-coloumn sizes.\n");
            return 1;
        }
        printf(" done in %.4f sec.\n", t);

        printf("Calculating block-coloumn offsets from block sizes..."); fflush(stdout);
        block_offsets_col = new ITYPE[num_blocks_col + 1];
        size_to_offset(block_offsets_col, block_sizes, (ITYPE) num_blocks_col);
        printf(" done\n");

        free(block_sizes);  // Block sizes not needed anymore

        printf("Converting to blocked format..."); fflush(stdout);
        tick();
        Coo_to_Blocked(&A, &B,
                       block_offsets_row, (ITYPE) num_blocks_row,
                       block_offsets_col, (ITYPE) num_blocks_col);
        t = tock();
        printf(" done in %.4f sec\n", t);

    } else {  // Block sizes not provided. Pick constant block size.

        ITYPE beta = pick_block_size(B.rows, B.columns, sizeof(VALTYPE), RT_WORKERS);

        printf("Converting to blocked format..."); fflush(stdout);
        tick();
        Coo_to_Blocked(&A, &B, beta, beta);
        t = tock();
        printf(" done in %.4f sec\n", t);

    }

    release(B);

    print_info(A);

    #ifdef PARTITION_DEBUG
    printf("\n\n");
    partition_dump(A.partition, A.blockrows);

    #else
    // Benchmarking happens here
    benchmark_spmv(A, ITERATIONS, "GSB");
    #endif

    // Free memory
    release(A);
    return 0;
}
