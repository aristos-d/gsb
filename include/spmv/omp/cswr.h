#ifndef _SPMV_OMP_CSWR_H
#define _SPMV_OMP_CSWR_H

#include <omp.h>

template <class T, class IT, class SIT>
void spmv(
        Cswr<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    #pragma omp parallel for schedule(dynamic,1)
    for (IT i=0; i<A->nwrows; i++) {
        IT wrow_offset = A->width * i;
        IT wrow_start = A->wrow_ptr[i];
        IT nnz = A->wrow_ptr[i+1] - wrow_start;

        spmv_coo(A->val + wrow_start, A->row + wrow_start, A->col + wrow_start,
                 A->width, A->columns, nnz,
                 x, y + wrow_offset);
    }
}

#endif
