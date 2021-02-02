#ifndef _DENSE_H_
#define _DENSE_H_

#include <assert.h>

#include "typedefs.h"
#include "matrix/coo.2.h"
#include "matrix/coo.3.h"
#include "spmv/dense.h"
#include "spmv/omp/dense.h"

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized. In this case A, is not really a
 * sparse matrix. The name is still spmv for consistency and readabillity.
 */
template <class T, class IT>
inline void spmv(Dense<T,IT> const * const A,
                 T const * const __restrict x, T * const __restrict y)
{
    spmv_dense(A->val, A->rows, A->columns, x, y);
}

template <class T, class IT>
inline void spmv_serial(Dense<T, IT> const * const A,
                        T const * const __restrict x, T * const __restrict y)
{
    spmv_dense_serial(A->val, A->rows, A->columns, x, y);
}

/*
 * Template specializations which use Intel's MKL library
 */
#ifdef MKL_DENSE
#include <mkl.h>

template <>
inline void spmv_dense(float const * const val, int const M, int const N,
                       float const * const __restrict x, float * const __restrict y)
{
    DEBUG_USE_MKL( fprintf(stderr, "MKL Dense in use\n") );
    cblas_sgemv(CblasRowMajor, CblasNoTrans, M, N, 
            1.0f, val, N, 
            x, 1, 1.0f,
            y, 1);
}

template <>
inline void spmv_dense_serial(float const * const val, int const M, int const N,
                              float const * const __restrict x, float * const __restrict y)
{
    // No point in having serial/parallel version. That is determined at link-time.
    spmv_dense(val, M, N, x, y);
}

#endif


/* ------------------ Constructors begin ------------------ */

/* 
 * Construct a Dense matrix from an array of type NONZERO. Type NONZERO must
 * have a 'row', 'col' and 'val' field.
 */
template <class NONZERO, class T, class IT>
void Coo_to_Dense(Dense<T, IT> * A, NONZERO * nonzeros, IT rows, IT columns, IT nnz)
{
    IT pos, N;

    // Watch out for integer overflow
    N = rows * columns;
    assert(N >= rows);

    allocate(A, rows, columns);
    
    for(IT i=0; i<nnz; i++){
        pos = nonzeros[i].row * columns + nonzeros[i].col;
        A->val[pos] = nonzeros[i].val;
    }
}

template <class T, class IT>
void Coo_to_Dense(Dense<T, IT> * A, Coo2<T, IT> * B)
{
    Coo_to_Dense(A, B->triplets, B->rows, B->columns, B->nnz);
}

template <class T, class IT>
void Coo_to_Dense(Dense<T, IT> * A, Coo3<T, IT> * B)
{
    Coo_to_Dense(A, B->elements, B->rows, B->columns, B->nnz);
}

template <class T, class IT>
void allocate(Dense<T,IT> * A, IT rows, IT columns)
{
    A->rows = rows;
    A->columns = columns;
    A->val = new T[rows*columns](); // Zero-initialize
}

/*
 * Free all the memory allocated for this matrix.
 */
template <class T, class IT>
void release(Dense<T, IT> A)
{
    delete [] A.val;
}

#endif
