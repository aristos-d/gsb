#ifndef _GSB_H_
#define _GSB_H_

#include "rt.h"

/*
 * Returns the number of non-zero elements of the matrix.
 */
template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT nonzeros (const GSB<T,IT,SIT> * A) { return A->nnz; }

template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT nonzeros (const GSB<T,IT,SIT> A) { return A.nnz; }

template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT block_nonzeros (const GSB<T,IT,SIT> A, IT block) { return nonzeros(A.blocks[block]); }

template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
inline IT block_nonzeros (const GSB<T,IT,SIT> * A, IT block) { return nonzeros(A->blocks[block]); }

// ------------------------- SpMV -------------------------

template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
void spmv_chunk (
	    GSB<T,IT,SIT> const * const A,
        T const * const __restrict x, T * const __restrict y,
        BlockRowPartition<IT> const * const partition,
        IT const first, IT const last, IT const bsize)
{
    if (last - first == 1) {

        IT x_offset;
        IT bstart = partition->chunks[first];
        IT bend = partition->chunks[last];

        for (IT k=bstart; k<bend; k++) {
            x_offset = A->blockcol_offset[A->blockcol_ind[k]];
            spmv(A->blocks[k], x + x_offset, y);
        }

    } else {

        IT middle = (first+last) / 2;

        #pragma omp task
        spmv_chunk(A, x, y, partition, first, middle, bsize);

        if (RT_SYNCHED) {
            spmv_chunk(A, x, y, partition, middle, last, bsize);
        } else {
            
            // We need C++ style allocation to ensure proper initialization
            T * temp = new T[bsize]();

            spmv_chunk(A, x, temp, partition, middle, last, bsize);
            #pragma omp taskwait

            #pragma omp simd
            for (IT i=0; i<bsize; i++)
                y[i] += temp[i];

            delete [] temp;
        }
    }
}

/*
 * SpMV routine for matrices in GSB format. y vector MUST be already initialized.
 */
template <typename T, typename IT, typename SIT,
          template <typename, typename, typename> class GSB>
void spmv(GSB<T,IT,SIT> const * const A,
          T const * const __restrict x, T * const __restrict y)
{
    // For each block row
    #pragma omp parallel for schedule(dynamic,1)
    for (IT bi=0; bi<A->blockrows; bi++) {

        IT nchunks = A->partition[bi].nchunks;
        IT y_start = A->blockrow_offset[bi];
        IT y_end = A->blockrow_offset[bi+1];

        spmv_chunk(A, x, y + y_start, A->partition + bi, (IT) 0, nchunks, y_end - y_start);
    }
}

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
