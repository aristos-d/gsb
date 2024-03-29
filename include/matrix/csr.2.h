#ifndef _CSR_2_H_
#define _CSR_2_H_
#include <cassert>

#include "typedefs.h"
#include "utils.h"
#include "generic/coo.h"
#include "spmv/csr.h"
#include "spmv/omp/csr.h"

/*
 * CSR matrix represented as an array of row pointers an array of value -
 * column index pairs.
 */
template <class T, class IT>
struct Csr2 {
  NonZero<T, IT> * nonzeros;
  IT * row_ptr;
  IT rows, columns;
};

/*
 * Get the number of non-zero elements of the matrix.
 */
template <class T, class IT>
inline IT nonzeros (const Csr2<T, IT> * A) { return A->row_ptr[A->rows]; }

template <class T, class IT>
inline IT nonzeros (const Csr2<T, IT> A) { return A.row_ptr[A.rows]; }

/*
 * SpMV routine wrappers
 */
template <class T, class IT>
void spmv (
        Csr2<T, IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_csr2(A->row_ptr, A->nonzeros, A->rows, x, y);
}

template <class T, class IT>
void spmv_serial (
        Csr2<T, IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_csr2_serial(A->row_ptr, A->nonzeros, A->rows, x, y);
}

/*
 * Construct a CSR2 matrix by processing a matrix in COO format. Triplets of
 * original matrix get sorted but then data is copied. Return 0 on success.
 * WARNING : This function requires that the original COO and the produced CSR2
 *           matrices can both fit in memory at the same time if allocate is
 *           true.
 */
template <class NONZERO, class T, class IT>
int Coo_to_Csr(Csr2<T, IT> * A, NONZERO * nonzeros,
               IT rows, IT columns, IT nnz,
               bool isSorted, bool allocate)
{
  A->rows = rows;
  A->columns = columns;

  if (!isSorted) sort_triplets(nonzeros, nnz);

  // Allocate memory and check
  if (allocate) {
    A->nonzeros = new NonZero<T,IT>[nnz];
  }

  A->row_ptr = new IT[rows + 1]();

  for (IT i=0; i<nnz; i++) {
    A->nonzeros[i].index = nonzeros[i].col;
    A->nonzeros[i].val = nonzeros[i].val;
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
 * Free all the memory allocated for this matrix.
 */
template <class T, class IT>
void release(Csr2<T, IT> A)
{
    delete [] A.row_ptr;
    delete [] A.nonzeros;
}

#endif
