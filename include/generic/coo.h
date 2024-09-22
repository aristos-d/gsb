#ifndef _COO_H_
#define _COO_H_

#include "spmv/coo.h"

#include <cstddef>
#include <iostream>

/*
 * Generic method to set a point on any COO matrix
 */
template <typename T, typename IT,
          typename COO>
inline void set_point (COO * A, IT i, IT row, IT col, T val)
{
  A->set_row_index(i, row);
  A->set_column_index(i, col);
  A->set_value(i, val);
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

    allocate(A, B->nonzeros());

    for (IT i=0; i<B->nonzeros(); i++)
        set_point(A, i, B->get_row_index(i), B->get_column_index(i), B->get_value(i));

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

  for (IT i=0; i<nnz; i++)
      set_point(A, i, array[i].row, array[i].col, array[i].val);

  return 0;
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <typename COOTYPE>
void show (COOTYPE const& A)
{
    if (A.nnz > 20)
    {
        std::cout << "Printing a matrix with more than 20 non-zeros is a bad idea.\n";
        return;
    }

    for (size_t i = 0; i < A.nonzeros(); i++)
    {
        std::cout << '(' << A.get_row_index(i) << ", " <<
                            A.get_column_index(i) << ", " <<
                            A.get_value(i) <<
                     ')' << '\n';
    }
    std::cout << '\n';
}

#endif
