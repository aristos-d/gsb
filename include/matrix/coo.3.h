#ifndef _COO_3_H_
#define _COO_3_H_

#include "typedefs.h"
#include "generic/coo.h"

/*
 * Special COO structure for easier/faster conversion to blocked
 * representations.
 */
template <class T, class IT>
struct Coo3
{
    Element<T, IT> * elements;
    IT rows, columns, nnz;
    IT blockrows, blockcols, nnzblocks;

    IT nonzeros() { return nnz; }

    /*
     * Getters
     */
    IT get_row_index (IT i) { return elements[i].row; }
    IT get_column_index (IT i) { return elements[i].col; }
    T get_value (IT i) { return elements[i].val; }

    /*
     * Setters
     */
    void set_row_index (IT i, IT row) { elements[i].row = row; }
    void set_column_index (IT i, IT column) { elements[i].col = column; }
    void set_value (IT i, T val) { elements[i].val = val; }
};

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
