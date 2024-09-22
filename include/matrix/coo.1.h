#ifndef _COO_1_H_
#define _COO_1_H_

#include "typedefs.h"
#include "generic/coo.h"
#include "generic/base.h"

/*
 * COO matrix as three arrays.
 */
template <typename T, typename IT, typename SIT>
struct Coo : public BlockBase<T,IT>
{
    SIT *I;
    SIT *J;
    T *val;
    IT rows, columns;   // Not always used. Carefull

    Coo(Element<T,IT> * array, IT nnz) : BlockBase<T,IT>(nnz)
    {
        allocate(this, nnz);

        sort_triplets_morton(array, nnz);

        for (IT i=0; i<nnz; i++)
        {
            I[i] = array[i].row;
            J[i] = array[i].col;
            val[i] = array[i].val;
        }
    }

    Coo() {}    // We need this

    ~Coo()
    {
        delete [] I;
        delete [] J;
        delete [] val;
    }

    // Getters
    IT get_row_index (IT i) const { return I[i]; }
    IT get_column_index (IT i) const { return J[i]; }
    T get_value (IT i) const { return val[i]; }

    // Setters
    void set_row_index (IT i, IT row) { I[i] = row; }
    void set_column_index (IT i, IT column) { J[i] = column; }
    void set_value (IT i, T val_) { val[i] = val_; }

    /*
     * Sparse matrix - vector multiplication. Result is stored in y. Memory for
     * y should already be allocated and initialized.
     */
    void spmv (T const * const __restrict x, T * const __restrict y) const override
    {
        spmv_coo(val, I, J, this->nnz, x, y);
    }
};

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
void release(Coo<T,IT,SIT>& A)
{
    delete [] A.I;
    delete [] A.J;
    delete [] A.val;
}

#endif
