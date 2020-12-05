#ifndef _SPMV_CRCSR_H_
#define _SPMV_CRCSR_H_

#include "spmv/omp/crcsr.h"

template <class T, class IT>
void spmv_serial (
        CompRowCsr<T,IT> const * const A,
		T const * const __restrict x,
        T * const __restrict y)
{
    IT ri, row_start, row_end;
    T y_temp;

    for (IT i=0; i<A->nnz_rows_num; i++) {
        row_start = A->row_ptr[i];
        row_end = A->row_ptr[i+1];
        ri = A->nnz_rows[i];
        y_temp = y[ri];
        #pragma unroll
        for (IT k=row_start; k<row_end; k++) {
            y_temp += A->val[k] * x[A->col_ind[k]];
        }
        y[ri] = y_temp;
    }
}

#endif
