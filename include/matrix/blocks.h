#ifndef _BLOCKS_H_
#define _BLOCKS_H_

#include "typedefs.h"
#include "utils.h"
#include "matrix/base.h"
#include "matrix/csr.h"
#include "matrix/dense.h"
#include "matrix/gsb.h"
#include "matrix/coo.1.h"

/* -------------------- Types of blocks -------------------- */

/*
 *  CSR block
 */
template <typename T, typename IT, typename SIT>
class BlockExtCsr : public BlockBase<T,IT> {
    private:
        IT * row_ptr;
        SIT * col_ind;
        T * val;
        IT M;

    public:
        BlockExtCsr(Element<T,IT> * array, IT rows, IT columns, IT nnz)
            : M(rows), BlockBase<T,IT>(nnz)
        {
            // Assumes input is sorted by row
            row_ptr = new IT[M + 1]();
            col_ind = new SIT[nnz];
            val = new T[nnz];

            for (IT i=0; i<nnz; i++) {
                col_ind[i] = array[i].col;
                val[i]  = array[i].val;
                row_ptr[array[i].row + 1]++;
            }

            for (IT i=0; i<M; i++) {
                row_ptr[i+1] += row_ptr[i];
            }

            assert(row_ptr[M] == nnz);
        }

        ~BlockExtCsr()
        {
            delete [] row_ptr;
            delete [] col_ind;
            delete [] val;
        }

        void spmv_block (T const * const x, T * const y) const
        {
            spmv_csr_serial (row_ptr, col_ind, val, M, x, y);
        }
};

/*
 *  CSR block with parallel SpMV
 */
template <typename T, typename IT, typename SIT>
class BlockExtCsrCilk : public BlockBase<T,IT> {
    private:
        IT * row_ptr;
        SIT * col_ind;
        T * val;
        IT M;

    public:
        BlockExtCsrCilk(Element<T,IT> * array, IT rows, IT columns, IT nnz)
            : M(rows), BlockBase<T,IT>(nnz)
        {
            // Assumes input is sorted by row
            row_ptr = new IT[M + 1]();
            col_ind = new SIT[nnz];
            val = new T[nnz];

            for (IT i=0; i<nnz; i++) {
                col_ind[i] = array[i].col;
                val[i]  = array[i].val;
                row_ptr[array[i].row + 1]++;
            }

            for (IT i=0; i<M; i++) {
                row_ptr[i+1] += row_ptr[i];
            }

            assert(row_ptr[M] == nnz);
        }

        ~BlockExtCsrCilk()
        {
            delete [] row_ptr;
            delete [] col_ind;
            delete [] val;
        }

        void spmv_block (T const * const x, T * const y) const
        {
            spmv_csr (row_ptr, col_ind, val, M, x, y);
        }
};

/* ----------------- Block Factory ----------------- */

template <typename T, typename IT>
class BlockFactory {
    public:
        virtual MatrixType create (
                BlockBase<T,IT> ** target,
                Element<T,IT> * array,
                IT rows, IT columns, IT nnz) = 0;
};

template <typename T, typename IT, typename SIT>
class BlockFactoryDefault : public BlockFactory<T,IT> {
    public:
        MatrixType create (BlockBase<T,IT> ** target,
                           Element<T,IT> * array, IT rows, IT columns, IT nnz)
        {
            float coo, csr, dense, density;

            coo = (float) nnz * (2 * sizeof(SIT) + sizeof(T));
            csr = (float) rows * sizeof(IT) + nnz * (sizeof(SIT) + sizeof(T));
            dense = ((float) rows) * columns * sizeof(T);
            density= ((float) nnz) / (rows * columns);
        
            if (coo < 1.0f * csr) { 
                // COO block
                *target =  new Coo<T,IT,SIT>(array, nnz);
                return BLOCK_COO;
            } else if (csr < dense) { 
                // CSR block
                if (density < 0.05f) {
                    *target = new BlockExtCsr<T,IT,SIT>(array, rows, columns, nnz);
                    return BLOCK_CSR;
                } else {
                    *target = new BlockExtCsrCilk<T,IT,SIT>(array, rows, columns, nnz);
                    return BLOCK_CSR_PAR;
                }
            } else {    
                // Dense block
                *target = new Dense<T,IT>(array, rows, columns, nnz);
                return BLOCK_DENSE;
            }
        }
};

#endif
