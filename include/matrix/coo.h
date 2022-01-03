#ifndef _COO_H_
#define _COO_H_

#include "nonzeros.h"
#include "spmv/coo.h"

/*
 * Generic method to set a point on any COO matrix
 */
template <typename T, typename IT,
          typename COO>
inline void set_point (COO * A, IT i, IT row, IT col, T val)
{
  set_row_index(A, i, row);
  set_column_index(A, i, col);
  set_value(A, i, val);
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT, class RIT, class CIT>
inline void spmv_coo(
        T const * const v, RIT const * const ri, CIT const * const ci,
        IT const rows, IT const columns, IT const nnz,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_coo(v, ri, ci, nnz, x, y);
}

#ifdef MKL_SPARSE
#include <mkl.h>
#include <stdio.h>

#include "params.h"

template <>
inline void spmv_coo(
        float const * const v, int const * const ri, int const * const ci,
        int const rows, int const columns, int const nnz,
        float const * const __restrict x,
        float * const __restrict y)
{
    DEBUG_USE_MKL( fprintf(stderr, "MKL COO in use\n") );
    const float alpha = 1.0f;
    const float beta = 1.0f;
    mkl_scoomv ("N", &rows, &columns, &alpha, "G__C", v, ri, ci, &nnz, x, &beta, y);
}

#endif

/*
 * Heavily overloaded constructors for COO matrices
 */
template <typename T, typename IT,
          template <typename, typename> class COOSRC,
          typename COODST>
int Coo_to_Coo(COODST * A, COOSRC<T,IT> * B)
{
    A->rows = B->rows;
    A->columns = B->columns;

    allocate(A, nonzeros(B));

    for (IT i=0; i<nonzeros(B); i++)
        set_point(A, i, get_row_index(B,i), get_column_index(B,i), get_value(B, i));

    return 0;
}

template <class COOTYPE, class T, class IT>
int Coo_to_Coo(COOTYPE * A, T * val, IT * I, IT * J, IT rows, IT columns, IT nnz)
{
  A->rows = rows;
  A->columns = columns;

  allocate(A, nnz);

  for (IT i=0; i<nnz; i++)
      set_point(A, i, I[i], J[i], val[i]);

  return 0;
}

template <class COOTYPE, class NONZERO, class IT>
int Coo_to_Coo(COOTYPE * A, NONZERO * array, IT rows, IT columns, IT nnz)
{
  A->rows = rows;
  A->columns = columns;

  allocate(A, nnz);

  for(IT i=0; i<nnz; i++)
      set_point(A, i, array[i].row, array[i].col, array[i].val);

  return 0;
}

#endif
