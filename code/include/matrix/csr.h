#ifndef _CSR_H_
#define _CSR_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "typedefs.h"
#include "utils.h"
#include "matrix/coo.h"
#include "spmv/csr.h"
#include "spmv/omp/csr.h"

/*
 * Returns the number of non-zero elements of the matrix.
 */
template <class T, class IT>
inline IT nonzeros(Csr<T,IT> const * const A){ return A->row_ptr[A->rows]; }

template <class T, class IT>
inline IT nonzeros(Csr<T,IT> const A){ return A.row_ptr[A.rows]; }

template <class T, class IT, class SIT>
inline IT nonzeros(BlockCsr<T,IT,SIT> const * const A){ return A->row_ptr[A->rows]; }

template <class T, class IT, class SIT>
inline IT nonzeros(BlockCsr<T,IT,SIT> const A){ return A.row_ptr[A.rows]; }

template <class T, class IT>
inline void spmv (Csr<T,IT> const * const A,
                  T const * const __restrict x,
                  T * const __restrict y)
{
    spmv_csr(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

template <class T, class IT>
inline void spmv_serial (Csr<T,IT> const * const A,
                         T const * const __restrict x,
                         T * const __restrict y)
{
    spmv_csr_serial(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

template <class T, class IT, class SIT>
inline void spmv (BlockCsr<T,IT,SIT> const * const A,
                  T const * const __restrict x,
                  T * const __restrict y)
{
    spmv_csr(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

template <class T, class IT, class SIT>
inline void spmv_serial (BlockCsr<T,IT,SIT> const * const A,
                         T const * const __restrict x,
                         T * const __restrict y)
{
    spmv_csr_serial(A->row_ptr, A->col_ind, A->val, A->rows, x, y);
}

/* ------------------ Constructors begin ------------------ */

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


template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Csr(BlockCsr<T,IT,SIT> * A, NONZERO * nonzeros,
               IT rows, IT columns, IT nnz, bool isSorted)
{
  A->rows = rows;
  A->columns = columns;

  if(!isSorted) sort_triplets(nonzeros, nnz);

  // Allocate memory 
  A->val = new T[nnz];
  A->col_ind = new SIT[nnz];
  A->row_ptr = new IT[rows+1]();

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

template <class NONZERO, class T, class IT>
int Coo_to_Csr(Csr<T, IT> *A, NONZERO *nonzeros, IT rows, IT columns, IT nnz)
{
    return Coo_to_Csr(A, nonzeros, rows, columns, nnz, true, true);
}

template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Csr(BlockCsr<T,IT,SIT> *A, NONZERO *nonzeros, IT rows, IT columns, IT nnz)
{
    return Coo_to_Csr(A, nonzeros, rows, columns, nnz, true);
}

template <class T, class IT>
int Coo_to_Csr(Csr<T, IT> *A, Coo<T, IT> * B)
{
    return Coo_to_Csr(A, B->val, B->I, B->J, B->rows, B->columns, B->nnz);
}

template <class T, class IT>
int Coo_to_Csr(Csr<T, IT> *A, Coo2<T, IT> * B)
{
    return Coo_to_Csr(A, B->triplets, B->rows, B->columns, B->nnz, false, true);
}

template <class T, class IT>
int Coo_to_Csr(Csr<T, IT> *A, Coo3<T, IT> * B)
{
    return Coo_to_Csr(A, B->elements, B->rows, B->columns, B->nnz, false, true);
}

/*
 * Print information about the matrix
 */
template <class T, class IT>
void print_info(Csr<T, IT> A)
{
    printf("Csr matrix : ");
    printf("%lu rows, %lu non-zeros\n", (unsigned long) A.rows, (unsigned long) A.row_ptr[A.rows]);
}

/*
 * Free all the memory allocated for this matrix.
 */
template <class T, class IT>
void release(Csr<T, IT> A)
{
    delete [] A.val;
    delete [] A.row_ptr;
    delete [] A.col_ind;
}

template <class T, class IT, class SIT>
void release(BlockCsr<T,IT,SIT> A)
{
    delete [] A.val;
    delete [] A.row_ptr;
    delete [] A.col_ind;
}

#endif
