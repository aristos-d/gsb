#ifndef _SPMV_CSR_OMP_H_
#define _SPMV_CSR_OMP_H_
#include <omp.h>

template <class T, class RIT, class CIT>
inline void spmv_csr (RIT const * const row_ptr, CIT const * const col_ind, T const * const val, RIT const M,
                      T const * const __restrict x, T * const __restrict y)
{
    #pragma omp parallel for schedule(dynamic,16)
    for (RIT i=0; i<M; i++) {
        RIT row_start = row_ptr[i];
        RIT row_end = row_ptr[i+1];
        for (RIT k=row_start; k<row_end; k++) {
            y[i] += val[k] * x[col_ind[k]];
        }
    }
}

template <class NONZERO, class T, class IT>
inline void spmv_csr2(IT const * const row_ptr, NONZERO const * const nonzeros,
                      IT const M, T const * const __restrict x, T * const __restrict y)
{
    #pragma omp parallel for schedule(dynamic,16)
    for (IT i=0; i<M; i++) {
      IT row_start = row_ptr[i];
      IT row_end = row_ptr[i+1];
      T res = y[i];
      for (IT j=row_start; j<row_end; j++) {
          res += nonzeros[j].val * x[nonzeros[j].index];
      }
      y[i] = res;
    }
}
#endif
