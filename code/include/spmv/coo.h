#ifndef _SPMV_COO_H_
#define _SPMV_COO_H_

template <class T, class IT, class RIT, class CIT>
inline void spmv_coo(T const * const v, RIT const * const ri, CIT const * const ci, IT const nnz,
                     T const * const __restrict x, T * const __restrict y)
{
    for (IT i=0; i<nnz; i++) {
        y[ri[i]] += v[i] * x[ci[i]];
    }
}

template <class NONZERO, class T, class IT>
inline void spmv_coo(NONZERO const * const nonzeros, IT nnz, T const * const __restrict x, T * const __restrict y)
{
    for (IT i=0; i<nnz; i++) {
        y[nonzeros[i].row] += nonzeros[i].val * x[nonzeros[i].col];
    }
}

#endif
