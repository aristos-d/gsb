#ifndef _CSWR_H_
#define _CSWR_H_

#include <assert.h>
#include <stdio.h>
#include <omp.h>

#include "typedefs.h"
#include "utils.h"
#include "matrix/coo.h"

template <class T, class IT, class SIT>
IT nonzeros(const Cswr<T,IT,SIT> * A){ return A->nnz; }

template <class T, class IT, class SIT>
IT nonzeros(const Cswr<T,IT,SIT> A){ return A.nnz; }

template <class T, class IT, class SIT>
void spmv(Cswr<T,IT,SIT> const * const A, T const * const __restrict x, T * const __restrict y)
{
    #pragma omp parallel for schedule(dynamic,1)
    for (IT i=0; i<A->nwrows; i++) {
        IT wrow_offset = A->width * i;
        IT wrow_start = A->wrow_ptr[i];
        IT nnz = A->wrow_ptr[i+1] - wrow_start;

        spmv_coo(A->val + wrow_start, A->row + wrow_start, A->col + wrow_start, 
                 A->width, A->columns, nnz,
                 x, y + wrow_offset);
    } 
}

template <class NONZERO, class T, class IT, class SIT>
void Coo_to_Cswr(Cswr<T,IT,SIT> * A, NONZERO * array, IT rows, IT columns, IT nnz, IT width)
{
    A->rows = rows;
    A->columns = columns;
    A->nnz = nnz;
    A->width = width;
    A->nwrows = (rows + width - 1) / width;
    A->val = new T[nnz];
    A->row = new SIT[nnz];
    A->col = new IT[nnz];
    A->wrow_ptr = new IT[A->nwrows + 1](); // Initialize to zero!
    
    IT wr;

    sort_triplets_widerows(array, nnz, width);

    // Copy matrix. We no longer need original data.
    for (IT i=0; i<nnz; i++) {
        wr = array[i].row / width;
        A->wrow_ptr[wr+1]++;

        A->row[i] = (SIT) (array[i].row % width);
        A->col[i] = array[i].col;
        A->val[i] = array[i].val;

        assert(A->row[i] < width);
    } 

    for (IT j=0; j<A->nwrows; j++) {
        A->wrow_ptr[j+1] = A->wrow_ptr[j+1] + A->wrow_ptr[j];
    }
    assert(A->wrow_ptr[A->nwrows] == nnz);
}

template <class T, class IT, class SIT>
void Coo_to_Cswr(Cswr<T,IT,SIT> * A, Coo2<T,IT> * B, IT width = DEFAULT_CSWR_WIDTH)
{
    Coo_to_Cswr(A, B->triplets, B->rows, B->columns, B->nnz, width);
}

template <class T, class IT, class SIT>
void Coo_to_Cswr(Cswr<T,IT,SIT> * A, Coo3<T,IT> * B, IT width = DEFAULT_CSWR_WIDTH)
{
    Coo_to_Cswr(A, B->elements, B->rows, B->columns, B->nnz, width);
}

/*
 * Print information about the matrix
 */
template <class T, class IT, class SIT>
void print_info(Cswr<T,IT,SIT> A)
{
  printf("Cswr matrix : ");
  printf("%lu rows in %lu groups of %lu, %lu non-zeros\n", 
          (unsigned long) A.rows,
          (unsigned long) A.nwrows,
          (unsigned long) A.width, 
          (unsigned long) A.nnz);
}

template <class T, class IT, class SIT>
void release(Cswr<T,IT,SIT> A)
{
    delete [] A.val;
    delete [] A.row;
    delete [] A.col;
    delete [] A.wrow_ptr;
}

#endif

