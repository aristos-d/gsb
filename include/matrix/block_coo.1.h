#ifndef _BLOCK_COO_1_H
#define _BLOCK_COO_1_H

#include <cassert>

/*
 * Minimalistic version of COO to serve as a block of a larger matrix.
 */
template <class T, class IT, class SIT>
struct BlockCoo
{
    T * val;
    SIT * I;
    SIT * J;
    IT nnz;

    IT nonzeros() const { return nnz; }

    // Getters
    IT get_row_index(IT index) const { return I[index]; }
    IT get_column_index(IT index) const { return J[index]; }
    T get_value(IT index) const { return val[index]; }

    // Setters
    void set_row_index(IT index, IT row) { I[index] = row; }
    void set_column_index(IT index, IT column) { J[index] = column; }
    void set_value(IT index, T val) { val[index] = val; }

    /*
     * Sparse matrix - vector multiplication. Result is stored in y. Memory for
     * y should already be allocated and initialized.
     */
    void spmv(const T * __restrict x, T * __restrict y) const
    {
        spmv_coo(val, I, J, nnz, x, y);
    }
};

/*
 * Contructor for a BlockCoo matrix. It is different from the others because it
 * also sorts the non-zeros in Morton order.
 * WARNING : Sorts the input array in-place.
 */
template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Coo(BlockCoo<T,IT,SIT> * A, NONZERO * array, IT nnz)
{
  allocate(A, nnz);

  if (sizeof(IT) > sizeof(SIT))
  {
    IT max_index = 1 << (8 * sizeof(SIT));
    for (IT i=0; i<nnz; i++)
    {
        assert(array[i].row < max_index && array[i].col < max_index);
    }
  }

  sort_triplets_morton(array, nnz);

  for (IT i=0; i<nnz; i++)
  {
    A->I[i] = (SIT) array[i].row;
    A->J[i] = (SIT) array[i].col;
    A->val[i] = array[i].val;
  }

  return 0;
}

/*
 * Memory allocation/deallocation
 */
template <class T, class IT, class SIT>
void allocate(BlockCoo<T,IT,SIT> * A, IT nnz)
{
    A->nnz = nnz;
    A->val = new T[nnz];
    A->I = new SIT[nnz];
    A->J = new SIT[nnz];
}

template <class T, class IT, class SIT>
void release(BlockCoo<T,IT,SIT> A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

#endif
