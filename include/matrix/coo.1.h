#ifndef _COO_1_H_
#define _COO_1_H_
#include <iostream>

#include "typedefs.h"
#include "generic/coo.h"
#include "generic/base.h"

/*
 * COO matrix as three arrays.
 */
template <typename T, typename IT, typename SIT>
class Coo : public BlockBase<T,IT> {
    public:
        SIT *I;
        SIT *J;
        T *val;
        IT rows, columns;   // Not always used. Carefull

        Coo(Element<T,IT> * array, IT nnz) : BlockBase<T,IT>(nnz)
        {
            allocate(this, nnz);

            sort_triplets_morton(array, nnz);

            for (IT i=0; i<nnz; i++) {
                I[i] = array[i].row;
                J[i] = array[i].col;
                val[i]  = array[i].val;
            }
        }

        Coo() {}    // We need this

        ~Coo()
        {
            delete [] I;
            delete [] J;
            delete [] val;
        }

        void spmv_block (T const * const x, T * const y) const
        {
            spmv_coo (val, I, J, this->nnz, x, y);
        }
};

/*
 * Getters/Setters
 */
template <typename T, typename IT, typename SIT>
inline IT get_row_index (Coo<T,IT,SIT> * A, IT i){ return A->I[i]; }

template <typename T, typename IT, typename SIT>
inline IT get_column_index (Coo<T,IT,SIT> * A, IT i){ return A->I[i]; }

template <typename T, typename IT, typename SIT>
inline T get_value (Coo<T,IT,SIT> * A, IT i){ return A->val[i]; }

template <typename T, typename IT, typename SIT>
inline void set_row_index (Coo<T,IT,SIT> * A, IT i, IT row)
{
    A->I[i] = row;
}

template <typename T, typename IT, typename SIT>
inline void set_column_index (Coo<T,IT,SIT> * A, IT i, IT column)
{
    A->J[i] = column;
}

template <typename T, typename IT, typename SIT>
inline void set_value (Coo<T,IT,SIT> * A, IT i, T val)
{
    A->val[i] = val;
}

/*
 * Sparse matrix - vector multiplication. Result is stored in y. Memory for y
 * should already be allocated and initialized.
 */
template <typename T, typename IT, typename SIT>
inline void spmv (
        Coo<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    spmv_coo(A->val, A->I, A->J, A->nnz, x, y);
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <typename T, typename IT, typename SIT>
void show (Coo<T,IT,SIT> A)
{
    if (A.nnz > 20) {
        std::cout << "Printing a matrix with more than 20 non-zeros is a bad idea.\n";
    } else {
        for (IT i=0; i<A.nnz; i++)
            std::cout << '(' << A.I[i] << ", " << A.J[i] << ", " << A.val[i] << ')' << '\n';
        std::cout << '\n';
    }
}

/*
 * Memory allocation/deallocation
 */
template <typename T, typename IT, typename SIT>
void allocate(Coo<T,IT,SIT> *A, IT nnz)
{
    A->nnz = nnz;
    A->I = new SIT[nnz];
    A->J = new SIT[nnz];
    A->val = new T[nnz];
}

template <typename T, typename IT, typename SIT>
void release(Coo<T,IT,SIT> A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

#endif
