#ifndef _COO_2_H_
#define _COO_2_H_
#include <iostream>

#include "typedefs.h"
#include "test/utils.h"
#include "matrix/coo.h"

/*
 * COO matrix as an array of triplets.
 */
template <class T, class IT>
struct Coo2 {
  Triplet<T, IT> * triplets;
  IT rows, columns, nnz;
};

/*
 * Getters/Setters
 */
template <class T, class IT>
inline IT nonzeros(const Coo2<T, IT> * const A){ return A->nnz; }

template <class T, class IT>
inline IT nonzeros(const Coo2<T, IT> A){ return A.nnz; }

template <class T, class IT>
inline IT get_row_index(Coo2<T, IT> * A, IT index){ return A->triplets[index].row; }

template <class T, class IT>
inline IT get_column_index(Coo2<T, IT> * A, IT index){ return A->triplets[index].col; }

template <class T, class IT>
inline T get_value(Coo2<T, IT> * A, IT index){ return A->triplets[index].val; }

template <class T, class IT>
inline void set_row_index(Coo2<T, IT> * A, IT index, IT row)
{
    A->triplets[index].row = row;
}

template <class T, class IT>
inline void set_column_index(Coo2<T, IT> * A, IT index, IT column)
{
    A->triplets[index].col = column;
}

template <class T, class IT>
inline void set_value(Coo2<T, IT> * A, IT index, T val)
{
    A->triplets[index].val = val;
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
inline void spmv (
        const Coo2<T, IT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->triplets, A->nnz, x, y);
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <class T, class IT>
void show (Coo2<T, IT> A)
{
    if (A.nnz > 20) {
      std::cout << "Printing a matrix with more than 20 non-zeros is a bad idea.\n";
    } else {
      for (IT i=0; i<A.nnz; i++) show(A.triplets[i]);
      std::cout << '\n';
  }
}

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
