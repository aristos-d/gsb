#ifndef _CSR_2_H_
#define _CSR_2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cilk/cilk.h>

#include "typedefs.h"
#include "utils.h"
#include "matrix/coo.h"

/*
 * Returns the number of non-zero elements of the matrix.
 */
template <class T, class IT>
inline IT nonzeros(const Csr2<T, IT> * A){ return A->row_ptr[A->rows]; }

template <class T, class IT>
inline IT nonzeros(const Csr2<T, IT> A){ return A.row_ptr[A.rows]; }

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
inline void spmv_csr2(IT const * const row_ptr, NonZero<T,IT> const * const nonzeros,
                      IT const M, T const * const __restrict x, T * const __restrict y)
{
    // For every row, in parallel
    cilk_for (IT i=0; i<M; i++) {
      IT row_start = row_ptr[i];
      IT row_end = row_ptr[i+1];
      T res = y[i];
      for (IT j=row_start; j<row_end; j++) {
          res += nonzeros[j].val * x[nonzeros[j].index];
      }
      y[i] = res;
    }
}

template <class T, class IT>
inline void spmv_csr2_serial(IT const * const row_ptr, NonZero<T,IT> const * const nonzeros,
                             IT const M, T const * const __restrict x, T * const __restrict y)
{
  IT row_start, row_end;
  T res;
   
  row_end = row_ptr[0];

  // For every row
  for (IT i=0; i<M; i++) {
    row_start = row_end;
    row_end = row_ptr[i+1];
    res = y[i];
    for (IT j=row_start; j < row_end; j++) {
        res += nonzeros[j].val * x[nonzeros[j].index];
    }
    y[i] = res;
  }
}

template <class T, class IT>
void spmv(Csr2<T, IT> const * const A,
          T const * const __restrict x,
          T * const __restrict y)
{
    spmv_csr2(A->row_ptr, A->nonzeros, A->rows, x, y);
}

template <class T, class IT>
void spmv_serial(Csr2<T, IT> const * const A,
                 T const * const __restrict x,
                 T * const __restrict y)
{
    spmv_csr2_serial(A->row_ptr, A->nonzeros, A->rows, x, y);
}


/* ------------------ Constructors begin ------------------ */

/*
 * Construct a CSR2 matrix by processing a matrix in COO format. Triplets of
 * original matrix get sorted but then data is copied. Return 0 on success.
 * WARNING : This function requires that the original COO and the produced CSR2
 * matrices can both fit in memory at the same time if allocate is true.
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

template <class T, class IT>
int Coo_to_Csr(Csr2<T, IT> * A, Coo2<T, IT> * B)
{
  return Coo_to_Csr(A, B->triplets, B->rows, B->columns, B->nnz, false, true);
}

template <class T, class IT>
int Coo_to_Csr(Csr2<T, IT> * A, Coo3<T, IT> * B)
{
  return Coo_to_Csr(A, B->elements, B->rows, B->columns, B->nnz, false, true);
}

/*
 * Print information about the matrix
 */
template <class T, class IT>
void print_info(Csr2<T, IT> A)
{
    printf("Csr2 matrix : ");
    printf("%lu rows, %lu non-zeros\n", (unsigned long) A.rows, (unsigned long) A.row_ptr[A.rows]);
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
