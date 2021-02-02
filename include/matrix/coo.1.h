#ifndef _COO_1_H_
#define _COO_1_H_

#include "matrix/coo.h"

/*
 * COO matrix as three arrays.
 */
template <class T, class IT>
struct Coo {
  T * val;
  IT * I;
  IT * J;
  IT rows, columns, nnz;
};

/*
 * Getters/Setters
 */
template <class T, class IT>
inline IT nonzeros (const Coo<T,IT> * const A){ return A->nnz; }

template <class T, class IT>
inline IT nonzeros (const Coo<T,IT> A){ return A.nnz; }

template <class T, class IT>
inline IT get_row_index (Coo<T,IT> * A, IT i){ return A->I[i]; }

template <class T, class IT>
inline IT get_column_index (Coo<T,IT> * A, IT i){ return A->I[i]; }

template <class T, class IT>
inline T get_value (Coo<T,IT> * A, IT i){ return A->val[i]; }

template <class T, class IT>
inline void set_row_index (Coo<T,IT> * A, IT i, IT row)
{
    A->I[i] = row;
}

template <class T, class IT>
inline void set_column_index (Coo<T,IT> * A, IT i, IT column)
{
    A->J[i] = column;
}

template <class T, class IT>
inline void set_value (Coo<T,IT> * A, IT i, T val)
{
    A->val[i] = val;
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
inline void spmv (
        Coo<T,IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_coo(A->val, A->I, A->J, A->nnz, x, y);
}

/*
 * Memory allocation/deallocation
 */
template <class T, class IT>
void allocate(Coo<T, IT> *A, IT nnz)
{
    A->nnz = nnz;
    A->I = new IT[nnz];
    A->J = new IT[nnz];
    A->val = new T[nnz];
}

template <class T, class IT>
void release(Coo<T,IT> A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

#endif
