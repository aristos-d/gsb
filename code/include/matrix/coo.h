#ifndef _COO_H_
#define _COO_H_

#include <stdlib.h>
#include <assert.h>

#include "typedefs.h"
#include "utils.h"
#include "spmv/coo.h"

/*
 * Getters/Setters for Coo
 */
template <class T, class IT>
inline IT nonzeros (const Coo<T,IT> * const A){ return A->nnz; }

template <class T, class IT>
inline IT nonzeros (const Coo<T,IT> A){ return A.nnz; }

template <class T, class IT>
inline IT get_row_index (Coo<T,IT> * A, IT i){ return A->I[i]; }

template <class T, class IT>
inline IT get_column_index (Coo<T,IT> * A, IT i){ return A->I[i]; }

template <class T, class IT>
inline T get_value (Coo<T,IT> * A, IT i){ return A->val[i]; }

template <class T, class IT>
inline void set_row_index (Coo<T,IT> * A, IT i, IT row)
{
    A->I[i] = row;
}

template <class T, class IT>
inline void set_column_index (Coo<T,IT> * A, IT i, IT column)
{
    A->J[i] = column;
}

template <class T, class IT>
inline void set_value (Coo<T,IT> * A, IT i, T val)
{
    A->val[i] = val;
}

/*
 * Getters/Setters for Coo2
 */
template <class T, class IT>
inline IT nonzeros(const Coo2<T, IT> * const A){ return A->nnz; }

template <class T, class IT>
inline IT nonzeros(const Coo2<T, IT> A){ return A.nnz; }

template <class T, class IT>
inline IT get_row_index(Coo2<T, IT> * A, IT index){ return A->triplets[index].row; }

template <class T, class IT>
inline IT get_column_index(Coo2<T, IT> * A, IT index){ return A->triplets[index].col; }

template <class T, class IT>
inline T get_value(Coo2<T, IT> * A, IT index){ return A->triplets[index].val; }

template <class T, class IT>
inline void set_row_index(Coo2<T, IT> * A, IT index, IT row)
{
    A->triplets[index].row = row;
}

template <class T, class IT>
inline void set_column_index(Coo2<T, IT> * A, IT index, IT column)
{
    A->triplets[index].col = column;
}

template <class T, class IT>
inline void set_value(Coo2<T, IT> * A, IT index, T val)
{
    A->triplets[index].val = val;
}

/*
 * Getters/Setters for Coo3
 */
template <class T, class IT>
inline IT nonzeros(const Coo3<T, IT> * const A){ return A->nnz; }

template <class T, class IT>
inline IT nonzeros(const Coo3<T, IT> A){ return A.nnz; }

template <class T, class IT>
inline IT get_row_index(Coo3<T, IT> * A, IT index){ return A->elements[index].row; }

template <class T, class IT>
inline IT get_column_index(Coo3<T, IT> * A, IT index){ return A->elements[index].col; }

template <class T, class IT>
inline T get_value(Coo3<T, IT> * A, IT index){ return A->elements[index].val; }

template <class T, class IT>
inline void set_row_index(Coo3<T, IT> * A, IT index, IT row)
{
    A->elements[index].row = row;
}

template <class T, class IT>
inline void set_column_index(Coo3<T, IT> * A, IT index, IT column)
{
    A->elements[index].col = column;
}

template <class T, class IT>
inline void set_value(Coo3<T, IT> * A, IT index, T val)
{
    A->elements[index].val = val;
}

/*
 * Getters/Setters for BlockCoo
 */
template <class T, class IT, class SIT>
inline IT nonzeros(const BlockCoo<T,IT,SIT> * const A){ return A->nnz; }

template <class T, class IT, class SIT>
inline IT nonzeros(const BlockCoo<T,IT,SIT> A){ return A.nnz; }

template <class T, class IT, class SIT>
inline IT get_row_index(BlockCoo<T,IT,SIT> * A, IT index){ return A->I[index]; }

template <class T, class IT, class SIT>
inline IT get_column_index(BlockCoo<T,IT,SIT> * A, IT index){ return A->I[index]; }

template <class T, class IT, class SIT>
inline T get_value(BlockCoo<T,IT,SIT> * A, IT index){ return A->val[index]; }

template <class T, class IT, class SIT>
inline void set_row_index(BlockCoo<T,IT,SIT> * A, IT index, IT row)
{
    A->I[index] = row;
}

template <class T, class IT, class SIT>
inline void set_column_index(BlockCoo<T,IT,SIT> * A, IT index, IT column)
{
    A->J[index] = column;
}

template <class T, class IT, class SIT>
inline void set_value(BlockCoo<T,IT,SIT> * A, IT index, T val)
{
    A->val[index] = val;
}

/*
 * Set a point in a COO matrix. Implemented for Coo, Coo2 and Coo3
 */
template <class T, class IT>
inline void set_point (Coo<T, IT> * A, IT i, IT row, IT col, T val)
{
  A->I[i] = row;
  A->J[i] = col;
  A->val[i] = val;
}

template <class T, class IT>
inline void set_point (Coo2<T, IT> * A, IT i, IT row, IT col, T val)
{
  A->triplets[i].row = row;
  A->triplets[i].col = col;
  A->triplets[i].val = val;
}

template <class T, class IT>
inline void set_point (Coo3<T, IT> * A, IT i, IT row, IT col, T val)
{
  A->elements[i].row = row;
  A->elements[i].col = col;
  A->elements[i].val = val;
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


template <class T, class IT>
inline void spmv (
        Coo<T,IT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_coo(A->val, A->I, A->J, A->nnz, x, y);
}

template <class T, class IT>
inline void spmv (
        const Coo2<T,IT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->triplets, A->nnz, x, y);
}

template <class T, class IT>
inline void spmv (
        const Coo3<T,IT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->elements, A->nnz, x, y);
}

template <class T, class IT, class SIT>
inline void spmv (
        const BlockCoo<T,IT,SIT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->val, A->I, A->J, A->nnz, x, y);
}

template <class T, class IT, class SIT>
inline void spmv (
        const BlockCoo2<T,IT,SIT> * const A,
        const T * __restrict x,
        T * __restrict y)
{
    spmv_coo(A->triplets, A->nnz, x, y);
}

#ifdef MKL_SPARSE
#include <mkl.h>
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
    mkl_scoomv ("N", &rows, &columns, &alpha, "G__C", v, ri, ci, &nnz, x, &beta, y );
}

#endif

/*
 * Heavily overloaded constructors for COO matrices
 */
template <typename T, typename IT,
          template <typename, typename> class COOSRC,
          template <typename, typename> class COODST>
int Coo_to_Coo(COODST<T,IT> * A, COOSRC<T,IT> * B)
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

/*
 * Contructor for a BlockCoo matrix. It is different from the others because it
 * also sorts the non-zeros in Morton order.
 * WARNING : Sorts the input array in-place.
 */
template <class NONZERO, class T, class IT, class SIT>
int Coo_to_Coo(BlockCoo<T,IT,SIT> * A, NONZERO * array, IT nnz)
{
  allocate(A, nnz);

  if (sizeof(IT) > sizeof(SIT)) {
    IT max_index = 1 << (8 * sizeof(SIT));
    for (IT i=0; i<nnz; i++) {
        assert(array[i].row < max_index && array[i].col < max_index);
    }
  }

  sort_triplets_morton(array, nnz);

  for (IT i=0; i<nnz; i++) {
    A->I[i] = (SIT) array[i].row;
    A->J[i] = (SIT) array[i].col;
    A->val[i] = array[i].val;
  }

  return 0;
}

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
 * Allocate memory for a COO matrix. Implemented for Coo, Coo2 and Coo3
 */
template <class T, class IT>
void allocate(Coo<T, IT> *A, IT nnz)
{
    A->nnz = nnz;
    A->I = new IT[nnz];
    A->J = new IT[nnz];
    A->val = new T[nnz];
}

template <class T, class IT>
void allocate(Coo2<T, IT> *A, IT nnz)
{
    A->nnz = nnz;
    A->triplets = new Triplet<T,IT>[nnz];
}

template <class T, class IT>
void allocate(Coo3<T, IT> * A, IT nnz)
{
    A->nnz = nnz;
    A->elements = new Element<T,IT>[nnz];
}

template <class T, class IT, class SIT>
void allocate(BlockCoo<T,IT,SIT> * A, IT nnz)
{
    A->nnz = nnz;
    A->val = new T[nnz];
    A->I = new SIT[nnz];
    A->J = new SIT[nnz];
}

template <class T, class IT, class SIT>
void allocate(BlockCoo2<T, IT, SIT> *A, IT nnz)
{
    A->nnz = nnz;
    A->triplets = new Triplet<T,SIT>[nnz];
}

/*
 * Release all memory allocated for a COO matrix. Implemented for Coo, Coo2 and Coo3
 */
template <class T, class IT>
void release(Coo<T,IT> A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

template <class T, class IT>
void release(Coo2<T,IT> A)
{
    delete [] A.triplets;
}

template <class T, class IT>
void release(Coo3<T,IT> A)
{
    delete [] A.elements;
}

template <class T, class IT, class SIT>
void release(BlockCoo<T,IT,SIT> A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

template <class T, class IT, class SIT>
void release(BlockCoo2<T,IT,SIT> A)
{
    delete [] A.triplets;
}

#endif
