#ifndef _COO_2_H_
#define _COO_2_H_
#include <iostream>

#include "typedefs.h"
#include "test/utils.h"
#include "generic/coo.h"

/*
 * COO matrix as an array of triplets.
 */
template <class T, class IT>
struct Coo2
{
    Triplet<T, IT> * triplets;
    IT rows, columns, nnz;

    IT nonzeros() const { return nnz; }

    // Getters
    IT get_row_index (IT i) const { return triplets[i].row; }
    IT get_column_index (IT i) const { return triplets[i].col; }
    T get_value (IT i) const { return triplets[i].val; }

    // Setters
    void set_row_index (IT i, IT row) { triplets[i].row = row; }
    void set_column_index (IT i, IT column) { triplets[i].col = column; }
    void set_value (IT i, T val) { triplets[i].val = val; }

    /*
     * Sparse matrix - vector multiplication. Result is stored in y. Memory for
     * y should already be allocated and initialized.
     */
    void spmv (const T * __restrict x, T * __restrict y) const
    {
        spmv_coo(triplets, nnz, x, y);
    }
};

/*
 * Memory allocation/deallocation
 */
template <class T, class IT>
void allocate(Coo2<T, IT> *A, IT nnz)
{
    A->nnz = nnz;
    A->triplets = new Triplet<T,IT>[nnz];
}

template <class T, class IT>
void release(Coo2<T,IT> A)
{
    delete [] A.triplets;
}

#endif
