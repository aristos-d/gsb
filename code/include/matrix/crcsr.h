#ifndef _CRCSR_H_
#define _CRCSR_H_

#include <stdlib.h>
#include <assert.h>

#include "typedefs.h"
#include "utils.h"
#include "matrix/coo.h"

/*
 * Returns the number of non-zero elements of the matrix.
 */
template <class T, class IT>
inline IT nonzeros(const CompRowCsr<T,IT> * A){ return A->row_ptr[A->nnz_rows_num]; }

template <class T, class IT>
inline IT nonzeros(const CompRowCsr<T,IT> A){ return A.row_ptr[A.nnz_rows_num]; }

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
void spmv(CompRowCsr<T,IT> const * const A,
	  T const * const __restrict x, T * const __restrict y)
{
    // For every non-zero row
    #pragma omp parallel for schedule(dynamic,1)
    for (IT i=0; i<A->nnz_rows_num; i++) {
      IT ri, row_start, row_end;
      T y_temp;
      row_start = A->row_ptr[i];
      row_end = A->row_ptr[i+1];
      ri = A->nnz_rows[i];
      y_temp = y[ri];
      #pragma unroll
      for(IT k=row_start; k < row_end; k++){
          y_temp += A->val[k] * x[A->col_ind[k]];
      }
      y[ri] = y_temp;
    }
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT>
void spmv_serial(CompRowCsr<T, IT> const * const A,
		 T const * const __restrict x, T * const __restrict y)
{
    IT ri, row_start, row_end;
    T y_temp;

    // For every non-zero row
    for(IT i=0; i<A->nnz_rows_num; i++){
      row_start = A->row_ptr[i];
      row_end = A->row_ptr[i+1];
      ri = A->nnz_rows[i];
      y_temp = y[ri];
      #pragma unroll
      for(IT k=row_start; k<row_end; k++){
          y_temp += A->val[k] * x[A->col_ind[k]];
      }
      y[ri] = y_temp;
    }
}

/* ------------------ Constructors begin ------------------ */

/* 
 * Construct a CSRCSR matrix from an array of type NONZERO. Type NONZERO must
 * have a 'row', 'col' and 'val' field.
 */
template <class T, class IT, class NONZERO>
void Coo_to_CrCsr(CompRowCsr<T, IT> * A, NONZERO * nonzeros, IT nnz, IT nnzrows)
{
    IT cur_row_index, cur_row;

    A->nnz_rows_num = nnzrows;
    A->nnz_rows = new IT[nnzrows];
    A->row_ptr = new IT[nnzrows + 1];
    A->col_ind = new IT[nnz];
    A->val = new T[nnz];

    sort_triplets(nonzeros, nnz);

    A->nnz_rows[0] = nonzeros[0].row;
    cur_row_index = 0;

    for (IT i=0; i<nnz; i++) {
        
        A->col_ind[i] = nonzeros[i].col;
        A->val[i] = nonzeros[i].val;
        
        cur_row = nonzeros[i].row;
        if (A->nnz_rows[cur_row_index] != cur_row) {
            cur_row_index++;
            A->nnz_rows[cur_row_index] = cur_row;
        }  
        A->row_ptr[cur_row_index+1]++;
    }

    assert(cur_row_index == nnzrows - 1);

    for (IT r=0; r<nnzrows; r++) {
       A->row_ptr[r+1] = A->row_ptr[r+1] + A->row_ptr[r];
    }

    assert(A->row_ptr[nnzrows] == nnz);

    DEBUG(printf("CRCSR constructed\n"));
}

/* 
 * Construct a CSRCSR matrix from an array of type NONZERO. Type NONZERO must
 * have a 'row', 'col' and 'val' field. This is just a wrapper around the
 * previous function for cases where we do not know the number of non-zero
 * rows.
 */
template <class T, class IT, class NONZERO>
void Coo_to_CrCsr(CompRowCsr<T, IT> * A, NONZERO * nonzeros, IT nnz)
{
    IT nnzrows = count_nnz_rows(nonzeros, nnz);
    Coo_to_CrCsr(A, nonzeros, nnz, nnzrows);
}

template <class T, class IT>
void Coo_to_CrCsr(CompRowCsr<T, IT> * A, Coo2<T, IT> * B)
{
    Coo_to_CrCsr(A, B->triplets, B->nnz);
}

template <class T, class IT>
void Coo_to_CrCsr(CompRowCsr<T, IT> * A, Coo3<T, IT> * B)
{
    Coo_to_CrCsr(A, B->elements, B->nnz);
}

/*
 * Memory deallocation function
 */
template <class T, class IT>
void release(CompRowCsr<T, IT> A)
{
    delete [] A.nnz_rows;
    delete [] A.row_ptr;
    delete [] A.col_ind;
    delete [] A.val;
}

#endif
