#ifndef _GSB_H_
#define _GSB_H_
#include <cstdio>

#include "spmv/omp/gsb.h"

/*
 * Returns the number of non-zero elements of a block
 */
template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT block_nonzeros (const GSB<T,IT,SIT> A, IT i) { return block(&A, i)->nonzeros(); }

template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT block_nonzeros (const GSB<T,IT,SIT> * A, IT i) { return block(A, i)->nonzeros(); }


// ------------------------- Constructors -------------------------

/*
 * Additional constructor to create a GSB matrix with a constant block size.
 * Final blocks in each dimension will be a little larger when block size is
 * not a divisor of total number of rows/columns.
 */
template <class T, class IT, class MATRIX,
          template <typename, typename> class COO>
void Coo_to_Blocked(MATRIX * A, COO<T,IT> * B, IT br_size, IT bc_size)
{
    IT blockrows = B->rows / br_size;
    IT blockcols = B->columns / bc_size;
    if (blockrows == 0) blockrows = 1;
    if (blockcols == 0) blockcols = 1;
    IT * blockrow_offset = new IT[blockrows + 1];
    IT * blockcol_offset = new IT[blockcols + 1];

    // Calculate block-row offsets
    blockrow_offset[0] = 0;
    for (IT i=0; i<(blockrows - 1); i++) {
        blockrow_offset[i+1] = blockrow_offset[i] + br_size;
    }
    blockrow_offset[blockrows] = B->rows;

    // Calculate block-column offsets
    blockcol_offset[0] = 0;
    for (IT i=0; i<(blockcols - 1); i++) {
        blockcol_offset[i+1] = blockcol_offset[i] + bc_size;
    }
    blockcol_offset[blockcols] = B->columns;

    Coo_to_Blocked(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

// ------------------------- Utilities -------------------------

/*
 * Print information about the matrix
 */
template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
void print_info(GSB<T,IT,SIT> A)
{
    printf("Matrix info : \n");
    printf("\t%lu rows x %lu columns\n",
            (unsigned long) A.rows,
            (unsigned long) A.columns);
    printf("\t%lu block-rows x %lu block-columns\n",
            (unsigned long) A.blockrows,
            (unsigned long) A.blockcols);
    printf("\t%lu non-zeros in %lu blocks\n",
            (unsigned long) A.nnz,
            (unsigned long) A.nnzblocks);
    printf("\t%.4f%% non-zeros\n",
            100 * (float) A.nnz / ( (float) A.rows * (float) A.columns));
    printf("\t%.4f%% non-zero blocks\n",
            100 * (float) A.nnzblocks / ( (float) A.blockrows * (float) A.blockcols));
    if (A.balanced) printf("\tBalanced block-rows\n");
    else            printf("\tUnbalanced block-rows\n");
    printf("\tBlocks : %lu COO, ", (unsigned long) A.type_block_count[BLOCK_COO]);
    printf("%lu CSR, ", (unsigned long) A.type_block_count[BLOCK_CSR]);
    printf("%lu CSR (parallel), ", (unsigned long) A.type_block_count[BLOCK_CSR_PAR]);
    printf("%lu dense\n", (unsigned long) A.type_block_count[BLOCK_DENSE]);
    printf("\tNon-Zeros : %lu COO, ", (unsigned long) A.type_nnz_count[BLOCK_COO]);
    printf("%lu CSR, ", (unsigned long) A.type_nnz_count[BLOCK_CSR]);
    printf("%lu CSR (parallel), ", (unsigned long) A.type_nnz_count[BLOCK_CSR_PAR]);
    printf("%lu dense\n", (unsigned long) A.type_nnz_count[BLOCK_DENSE]);
}

#endif
