#ifndef _SPMV_CSBR_3_OMP_
#define _SPMV_CSBR_3_OMP_

#include <omp.h>

#include "rt.h"

/*
 * SpMV routine
 */
template <class T, class IT, class SIT>
void spmv_balanced (
        Csbr3<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    // For each block row
    #pragma omp parallel for schedule(dynamic,1)
    for (IT bi=0; bi<A->blockrows; bi++) {
        IT x_offset, y_offset;
        IT blockrow_start, blockrow_end;
        IT block_start, block_end;

        blockrow_start = A->blockrow_ptr[bi];
        blockrow_end = A->blockrow_ptr[bi+1];
        y_offset = bi * A->block_size;

        // For every block in block row
        for (IT k=blockrow_start; k<blockrow_end; k++) {
            x_offset = A->blockcol_ind[k] * A->block_size;
            block_start = A->block_ptr[k];
            block_end = A->block_ptr[k+1];

            spmv_coo(
                A->val + block_start,
                A->row_ind + block_start,
                A->col_ind + block_start,
                block_end - block_start,
                x + x_offset,
                y + y_offset);
        }
    }
}

/*
 *
 */
template <typename T, typename IT, typename SIT>
void spmv_chunk (
	    Csbr3<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y,
        BlockRowPartition<IT> const * const partition,
        IT const first, IT const last)
{
    if (last - first == 1) {

        IT x_offset;
        IT block_start, block_end;
        IT chunk_start, chunk_end;

        chunk_start = partition->chunks[first];
        chunk_end = partition->chunks[last];

        for (IT k=chunk_start; k<chunk_end; k++) {
            x_offset = A->blockcol_ind[k] * A->block_size;
            block_start = A->block_ptr[k];
            block_end = A->block_ptr[k + 1];

            spmv_coo(
                A->val + block_start,
                A->row_ind + block_start,
                A->col_ind + block_start,
                block_end - block_start,
                x + x_offset, y);
        }

    } else {
        IT middle = (first+last) / 2;

        #pragma omp task
        spmv_chunk(A, x, y, partition, first, middle);

        if (RT_SYNCHED) {
            spmv_chunk(A, x, y, partition, middle, last);
        } else {

            // We need C++ style allocation to ensure proper initialization
            T * temp = new T[A->block_size]();

            spmv_chunk(A, x, temp, partition, middle, last);
            #pragma omp taskwait

            #pragma omp simd
            for (IT i=0; i<A->block_size; i++)
                y[i] += temp[i];

            delete [] temp;
        }
    }
}

/*
 * High-level SpMV routine
 */
template <typename T, typename IT, typename SIT>
void spmv (
        Csbr3<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    if (A->balanced) {

        spmv_balanced(A, x, y);

    } else {

        // For each block row
        #pragma omp parallel for schedule(dynamic,1)
        for (IT bi=0; bi<A->blockrows; bi++) {

            IT nchunks = A->partition[bi].nchunks;
            IT y_start = get_block_row_offset(A, bi);
            IT y_end = get_block_row_offset(A, bi+1);

            spmv_chunk(A, x, y + y_start,
                       A->partition + bi,
                       (IT) 0,
                       nchunks);
        }
    }
}

#endif
