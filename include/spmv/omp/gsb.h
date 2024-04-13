#ifndef _SPMV_OMP_GSB_H_
#define _SPMV_OMP_GSB_H_
#include <omp.h>

#include "rt.h"

/*
 * SpMV routine that handles a chunk of a blockrow. May recursively call
 * itself.
 */
template <class T, class IT, class SIT,
          template <typename, typename, typename> class GSB>
void spmv_chunk (
	    GSB<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y,
        BlockRowPartition<IT> const * const partition,
        IT const first, IT const last, IT const bsize)
{
    if (last - first == 1) {

        IT x_offset;
        IT bstart = partition->chunks[first];
        IT bend = partition->chunks[last];

        for (IT k=bstart; k<bend; k++)
        {
            x_offset = A->get_block_column_offset(A->blockcol_ind[k]);
            A->block(k)->spmv(x + x_offset, y);
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
 * SpMV routine for matrices in GSB format.
 */
template <typename T, typename IT, typename SIT,
          template <typename, typename, typename> class GSB>
void spmv_unbalanced (
        GSB<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    // For each block row
    #pragma omp parallel for schedule(dynamic,1)
    for (IT bi=0; bi<A->blockrows; bi++) {

        IT nchunks = A->partition[bi].nchunks;
        IT y_start = A->get_block_row_offset(bi);
        IT y_end = A->get_block_row_offset(bi+1);

        A->spmv_chunk(x, y + y_start, A->partition + bi, IT(0), nchunks, y_end - y_start);
    }
}

/*
 * SpMV routine for matrices in GSB format with "balanced" block-rows.
 * Parallelism within the block-row is unnecessary.
 */
template <typename T, typename IT, typename SIT,
          template <typename, typename, typename> class GSB>
void spmv_balanced (
        GSB<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    // For each block row
    #pragma omp parallel for schedule(dynamic,1)
    for (IT bi=0; bi<A->blockrows; bi++)
    {
        IT x_offset, y_offset;
        IT blockrow_start, blockrow_end;
        IT col_index;

        blockrow_start = A->blockrow_ptr[bi];
        blockrow_end = A->blockrow_ptr[bi+1];
        y_offset = A->get_block_row_offset(bi);

        // For every block in block row
        for (IT k=blockrow_start; k<blockrow_end; k++)
        {
            col_index = A->blockcol_ind[k];
            x_offset = A->get_block_column_offset(A->blockcol_ind[k]); //A->blockcol_offset[col_index];
            A->block(k)->spmv(x + x_offset, y + y_offset);
        }
    }
}

/*
 * High-level SpMV routine for blocked sparse matrices. Requires that the
 * blocked sparse matrix has:
 * - A BlockRowPartition member named "partition".
 * - A "get_block_row_offset" member method.
 * - A "get_block_column_offset" member method.
 * - A "block" method returning a pointer to an object that defines a
 *   "spmv" method.
 */
template <typename T, typename IT, typename SIT,
          template <typename, typename, typename> class GSB>
void spmv_blocked (
        GSB<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    if (A->balanced) {

        spmv_balanced(A, x, y);

    } else {

        // For each block row
        #pragma omp parallel for schedule(dynamic,1)
        for (IT bi=0; bi<A->blockrows; bi++)
        {
            IT nchunks = A->partition[bi].nchunks;
            IT y_start = A->get_block_row_offset(bi);
            IT y_end = A->get_block_row_offset(bi+1);

            spmv_chunk(A, x, y + y_start,
                       A->partition + bi,
                       IT(0),
                       nchunks,
                       y_end - y_start);
        }
    }
}

#endif
