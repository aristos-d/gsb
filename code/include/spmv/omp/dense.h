#ifndef _SPMV_DENSE_OMP_H_
#define _SPMV_DENSE_OMP_H_
#include <omp.h>

template <class T, class IT>
inline void spmv_dense(T const * const val, IT const M, IT const N,
                       T const * const __restrict x, T * const __restrict y)
{
    #pragma omp parallel for schedule(static,32)
    for (IT i=0; i<M; i++) {
        T temp = y[i];
        for (IT j=0; j<N; j++) {
            temp += val[i*N + j] * x[j];
        }
        y[i] = temp;
    }
}

#endif
