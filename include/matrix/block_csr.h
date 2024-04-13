#ifndef _BLOCK_CSR_H_
#define _BLOCK_CSR_H_

/*
 * CSR matrix represented as an array of row pointers and two arrays for
 * non-zero values and column indexes.
 */
template <class T, class IT, class SIT>
struct BlockCsr
{
    T * val;
    SIT * col_ind;
    IT * row_ptr;
    IT rows, columns;
    // In 64-bit machine, with 32-bit indices, adding column information does not
    // increase struct size due to padding. It is not technically necessary.

    IT nonzeros() const { return row_ptr[rows]; }

    /*
     * SpMV routine wrappers
     */
    void spmv(T const * const __restrict x, T * const __restrict y) const
    {
        spmv_csr(row_ptr, col_ind, val, rows, x, y);
    }

    void spmv_serial(T const * const __restrict x, T * const __restrict y) const
    {
        spmv_csr_serial(row_ptr, col_ind, val, rows, x, y);
    }
};

/*
 * Constructor from array of non-zeros
 */
template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Csr(BlockCsr<T,IT,SIT> * A, NONZERO * nonzeros,
               IT rows, IT columns, IT nnz, bool isSorted)
{
  A->rows = rows;
  A->columns = columns;

  if (!isSorted) sort_triplets(nonzeros, nnz);

  // Allocate memory
  A->val = new T[nnz];
  A->col_ind = new SIT[nnz];
  A->row_ptr = new IT[rows+1]();

  for (IT i=0; i<nnz; i++)
  {
    A->col_ind[i] = nonzeros[i].col;
    A->val[i] = nonzeros[i].val;
    (A->row_ptr[nonzeros[i].row + 1])++;
  }

  // Calculate row pointers as the cumulative sum of row non-nonzeros
  for (IT i=0; i<A->rows; i++)
  {
    A->row_ptr[i+1] = A->row_ptr[i+1] + A->row_ptr[i];
  }

  assert(A->row_ptr[A->rows] == nnz);
  return 0;
}

template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Csr(BlockCsr<T,IT,SIT> *A, NONZERO *nonzeros, IT rows, IT columns, IT nnz)
{
    return Coo_to_Csr(A, nonzeros, rows, columns, nnz, true);
}

/*
 * Memory deallocation
 */
template <class T, class IT, class SIT>
void release(BlockCsr<T,IT,SIT> A)
{
    delete [] A.val;
    delete [] A.row_ptr;
    delete [] A.col_ind;
}

#endif
