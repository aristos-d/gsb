#ifndef _BLOCK_COO_2_H
#define _BLOCK_COO_2_H
#include <cassert>

#include "typedefs.h"
#include "generic/coo.h"

/*
 * Minimalistic version of COO (Coo2) to serve as a block of a larger matrix.
 */
template <class T, class IT, class SIT>
struct BlockCoo2
{
  Triplet<T,SIT> * triplets;
  IT nnz;

  IT nonzeros() const { return nnz; }
};

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <class T, class IT, class SIT>
inline void spmv (
        const BlockCoo2<T,IT,SIT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->triplets, A->nnz, x, y);
}

/*
 * Contructor for a BlockCoo matrix. It is different from the others because it
 * also sorts the non-zeros in Morton order.
 */
template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Coo(BlockCoo2<T,IT,SIT> * A, NONZERO * array, IT nnz)
{
  allocate(A, nnz);

  if (sizeof(IT) > sizeof(SIT)) {
    IT max_index = 1 << (8 * sizeof(SIT));
    for (IT i=0; i<nnz; i++) {
        assert(array[i].row < max_index && array[i].col < max_index);
    }
  }

  for (IT i=0; i<nnz; i++) {
    A->triplets[i].row = (SIT) array[i].row;
    A->triplets[i].col = (SIT) array[i].col;
    A->triplets[i].val = array[i].val;
  }

  sort_triplets_morton(A->triplets, A->nnz);

  return 0;
}


/*
 * Memory allocation/deallocation
 */
template <class T, class IT, class SIT>
void allocate(BlockCoo2<T, IT, SIT> *A, IT nnz)
{
    A->nnz = nnz;
    A->triplets = new Triplet<T,SIT>[nnz];
}

template <class T, class IT, class SIT>
void release(BlockCoo2<T,IT,SIT> A)
{
    delete [] A.triplets;
}

#endif
