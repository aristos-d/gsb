#ifndef _CSR_1_H_
#define _CSR_1_H_
#include <iostream>
#include <cassert>

#include "generic/csr.h"

/*
 * CSR matrix represented as an array of row pointers and two arrays for
 * non-zero values and column indexes.
 */
template <class T, class IT>
struct Csr {
  T * val;
  IT * col_ind;
  IT * row_ptr;
  IT rows, columns;
  // In 64-bit machine, with 32-bit indices, adding column information does not
  // increase struct size due to padding. It is not technically necessary.
};

/*
 * Get the number of non-zero elements of the matrix.
 */
template <class T, class IT>
inline IT nonzeros (Csr<T,IT> const * const A) { return A->row_ptr[A->rows]; }

template <class T, class IT>
inline IT nonzeros (Csr<T,IT> const A) { return A.row_ptr[A.rows]; }

/*
 * SpMV routine wrappers
 */
template <class T, class IT>
inline void spmv (
        Csr<T,IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_csr(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

template <class T, class IT>
inline void spmv_serial (
        Csr<T,IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_csr_serial(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

/*
 * Construct a CSR matrix by processing three coordinate arrays. The arrays are
 * also sorted IN-PLACE based on row index. Data is not copied to save storage.
 */
template <class T, class IT>
int Coo_to_Csr(Csr<T, IT> *A, T *val, IT *I, IT *J, IT rows, IT columns, IT nnz)
{
  A->val = val;
  A->col_ind = J;
  A->row_ptr = new IT[rows + 1];
  A->rows = rows;
  A->columns = columns;

  sort_triplets(I, J, val, nnz);

  // Count non zero values in each row
  for (IT i=0; i < nnz; i++) {
    (A->row_ptr[I[i] + 1])++;
  }

  // Calculate row pointers as the cumulative sum of row non-nonzeros
  for (IT i=0; i<A->rows; i++) {
    A->row_ptr[i+1] = A->row_ptr[i+1] + A->row_ptr[i];
  }

  assert(A->row_ptr[A->rows] == nnz);
  return 0;
}

/*
 * Construct a CSR matrix by processing a matrix in COO format. Triplets of
 * original matrix get sorted but then data is copied. Return 0 on success.
 * WARNING : This function requires that the original COO and the produced CSR
 *           matrices can both fit in memory at the same time.
 */
template <class NONZERO, class T, class IT>
int Coo_to_Csr(Csr<T, IT> * A, NONZERO *nonzeros,
               IT rows, IT columns, IT nnz, bool isSorted, bool allocate)
{
  A->rows = rows;
  A->columns = columns;

  if(!isSorted) sort_triplets(nonzeros, nnz);

  // Allocate memory and check
  if (allocate) {
    A->val = new T[nnz];
    A->col_ind = new IT[nnz];
    A->row_ptr = new IT[rows+1]();
  } else {
    A->row_ptr = new IT[rows+1]();
  }

  for (IT i=0; i<nnz; i++) {
    A->col_ind[i] = nonzeros[i].col;
    A->val[i] = nonzeros[i].val;
    (A->row_ptr[nonzeros[i].row + 1])++;
  }

  // Calculate row pointers as the cumulative sum of row non-nonzeros
  for (IT i=0; i<A->rows; i++) {
    A->row_ptr[i+1] = A->row_ptr[i+1] + A->row_ptr[i];
  }

  assert(A->row_ptr[A->rows] == nnz);
  return 0;
}

/*
 * Constructor wrappers
 */
//template <class NONZERO, class T, class IT>
//int Coo_to_Csr(Csr<T, IT> *A, NONZERO *nonzeros, IT rows, IT columns, IT nnz)
//{
//    return Coo_to_Csr(A, nonzeros, rows, columns, nnz, true, true);
//}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <class T, class IT>
void show(Csr<T, IT> A)
{
    if (A.rows > 20) {
        std::cout
            << "Printing a matrix with more than 20 rows is a bad idea.\n";
    } else {
        for (IT i = 0; i < A.rows; i++) {
            std::cout << i << ": ";
            IT row_start, next_row;
            row_start = A.row_ptr[i];
            next_row = A.row_ptr[i + 1];
            for (IT k = row_start; k < next_row; k++) {
                std::cout << '(' << A.col_ind[k] << ',' << A.val[k] << ')'
                          << '\t';
            }
            std::cout << '\n';
        }
    }
}

/*
 * Memory deallocation
 */
template <class T, class IT>
void release(Csr<T, IT> A)
{
    delete [] A.val;
    delete [] A.row_ptr;
    delete [] A.col_ind;
}

#endif
