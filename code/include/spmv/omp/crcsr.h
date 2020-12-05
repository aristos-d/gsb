#ifndef _SPMV_OMP_CRCSR_H
#define _SPMV_OMP_CRCSR_H

#include <omp.h>

template <class T, class IT>
void spmv (
        CompRowCsr<T,IT> const * const A,
	    T const * const __restrict x,
        T * const __restrict y)
{
    #pragma omp parallel for schedule(dynamic,1)
    for (IT i=0; i<A->nnz_rows_num; i++) {
        IT ri, row_start, row_end;
        T y_temp;
        row_start = A->row_ptr[i];
        row_end = A->row_ptr[i+1];
        ri = A->nnz_rows[i];
        y_temp = y[ri];
        #pragma unroll
        for (IT k=row_start; k < row_end; k++) {
            y_temp += A->val[k] * x[A->col_ind[k]];
        }
        y[ri] = y_temp;
    }
}

#endif
