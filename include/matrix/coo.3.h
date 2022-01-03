#ifndef _COO_3_H_
#define _COO_3_H_

#include "typedefs.h"
#include "matrix/coo.h"

/*
 * Special COO structure for easier/faster conversion to blocked
 * representations.
 */
template <class T, class IT>
struct Coo3 {
  Element<T, IT> * elements;
  IT rows, columns, nnz;
  IT blockrows, blockcols, nnzblocks;
};

/*
 * Getters/Setters
 */
template <class T, class IT>
inline IT get_row_index(Coo3<T, IT> * A, IT index){ return A->elements[index].row; }

template <class T, class IT>
inline IT get_column_index(Coo3<T, IT> * A, IT index){ return A->elements[index].col; }

template <class T, class IT>
inline T get_value(Coo3<T, IT> * A, IT index){ return A->elements[index].val; }

template <class T, class IT>
inline void set_row_index(Coo3<T, IT> * A, IT index, IT row)
{
    A->elements[index].row = row;
}

template <class T, class IT>
inline void set_column_index(Coo3<T, IT> * A, IT index, IT column)
{
    A->elements[index].col = column;
}

template <class T, class IT>
inline void set_value(Coo3<T, IT> * A, IT index, T val)
{
    A->elements[index].val = val;
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
inline void spmv (
        const Coo3<T,IT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->elements, A->nnz, x, y);
}

/*
 * Memory allocation/deallocation
 */
template <class T, class IT>
void allocate(Coo3<T, IT> * A, IT nnz)
{
    A->nnz = nnz;
    A->elements = new Element<T,IT>[nnz];
}

template <class T, class IT>
void release(Coo3<T,IT> A)
{
    delete [] A.elements;
}

#endif
