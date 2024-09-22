#ifndef _CRCSR_H_
#define _CRCSR_H_
#include <cstdio>
#include <cassert>

#include "utils.h"
#include "matrix/coo.2.h"
#include "matrix/coo.3.h"

/*
 * CSR matrix with compressed row pointers. Only pointers for non-empty rows
 * are stored. Non-empty row indexes are stored in additional array.
 */
template <class T, class IT>
struct CompRowCsr {
  T * val;
  IT * col_ind;
  IT * row_ptr;
  IT * nnz_rows;
  IT nnz_rows_num;

  IT nonzeros() const
  {
    return row_ptr[nnz_rows_num];
  }
};

#include "spmv/crcsr.h"

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

    for (IT i=0; i<nnz; i++)
    {
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

    for (IT r=0; r<nnzrows; r++)
    {
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
void Coo_to_CrCsr (CompRowCsr<T, IT> * A, NONZERO * nonzeros, IT nnz)
{
    IT nnzrows = count_nnz_rows(nonzeros, nnz);
    Coo_to_CrCsr(A, nonzeros, nnz, nnzrows);
}

template <class T, class IT>
void Coo_to_CrCsr (CompRowCsr<T, IT> * A, Coo2<T, IT> * B)
{
    Coo_to_CrCsr(A, B->triplets, B->nnz);
}

template <class T, class IT>
void Coo_to_CrCsr (CompRowCsr<T, IT> * A, Coo3<T, IT> * B)
{
    Coo_to_CrCsr(A, B->elements, B->nnz);
}

/*
 * Memory deallocation function
 */
template <class T, class IT>
void release (CompRowCsr<T, IT>& A)
{
    delete [] A.nnz_rows;
    delete [] A.row_ptr;
    delete [] A.col_ind;
    delete [] A.val;
}

#endif
