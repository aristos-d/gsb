#ifndef _SPMV_CSR_H_
#define _SPMV_CSR_H_

template <class T, class RIT, class CIT>
inline void spmv_csr_serial (
        RIT const * const row_ptr,
        CIT const * const col_ind,
        T const * const val,
        RIT const M,
        T const * const __restrict x,
        T * const __restrict y)
{
    RIT row_start, row_end;

    row_end = row_ptr[0];

    for (RIT i=0; i<M; i++) {
        row_start = row_end;
        row_end = row_ptr[i+1];
        for (RIT j=row_start; j < row_end; j++) {
            y[i] += val[j] * x[col_ind[j]];
        }
    }
}

template <class NONZERO, class T, class IT>
inline void spmv_csr2_serial (
        IT const * const row_ptr,
        NONZERO const * const nonzeros,
        IT const M,
        T const * const __restrict x,
        T * const __restrict y)
{
    IT row_start, row_end;
    T res;

    row_end = row_ptr[0];

    for (IT i=0; i<M; i++) {
        row_start = row_end;
        row_end = row_ptr[i+1];
        res = y[i];
        for (IT j=row_start; j < row_end; j++) {
            res += nonzeros[j].val * x[nonzeros[j].index];
        }
        y[i] = res;
    }
}

#endif
