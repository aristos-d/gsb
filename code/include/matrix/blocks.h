#ifndef _BLOCKS_H_
#define _BLOCKS_H_

#include "typedefs.h"
#include "utils.h"
#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/dense.h"
#include "matrix/gsb.h"

/*
 * Abstract block
 */
template <typename T, typename IT>
class BlockBase {
    protected:
        BlockBase() {}
        BlockBase(IT _nnz) : nnz(_nnz) {}
    public:
        virtual ~BlockBase() {}
        virtual void spmv_block (T const * const x, T * const y) const = 0;
        IT nnz;
};

template <typename T, typename IT>
inline IT nonzeros(BlockBase<T,IT> const * const block)
{
    return block->nnz;
}

template <typename T, typename IT>
inline IT nonzeros(BlockBase<T,IT> const block)
{
    return block.nnz;
}

template <typename T, typename IT>
inline void spmv(BlockBase<T,IT> const * const block, T const * const x, T * const y)
{
    block->spmv_block(x, y);
}

/*
 * Block Factory
 */ 
template <typename T, typename IT>
class BlockFactory {
    public:
        virtual MatrixType create (BlockBase<T,IT> ** target, Element<T,IT> * array,
                                   IT rows, IT columns, IT nnz) = 0;
};

/*
 * Version 2 of CGBR using abstract blocks 
 */
template <typename T, typename IT, typename SIT>
class Cgbr2 : public BlockBase<T,IT> {
    public:
        BlockBase<T,IT> ** blocks;
        IT * blockrow_ptr;         // Indexes for blocks array
        IT * blockcol_ind;
      
        // Report statistics
        IT type_block_count[MATRIX_TYPE_NUM];
        IT type_nnz_count[MATRIX_TYPE_NUM];
      
        // Partitioning information
        BlockRowPartition<IT> * partition;
        bool balanced;
      
        // Block size information
        IT * blockrow_offset;
        IT * blockcol_offset;
        IT blockrows;
        IT blockcols;
        IT nnzblocks;
      
        // Original size
        IT rows;
        IT columns;
        
        Cgbr2(){}
        Cgbr2(Element<T,IT> * array, IT _rows, IT _columns, IT nnz,
              IT * _blockrow_offset, IT _blockrows,
              IT * _blockcol_offset, IT _blockcols,
              BlockFactory<T,IT> & factory);
        
        ~Cgbr2() {}

        void spmv_block (T const * const __restrict x, T * const __restrict y) const
	{
            spmv(this, x, y);
	}
};

/* -------------------- Types of blocks -------------------- */

/*
 *  COO block
 */
template <typename T, typename IT, typename SIT>
class BlockExtCoo : public BlockBase<T,IT> {
    private:
        SIT * ri;
        SIT * ci;
        T * v;

    public:
        BlockExtCoo(Element<T,IT> * array, IT nnz) : BlockBase<T,IT>(nnz)
        {
            ri = new SIT[nnz];
            ci = new SIT[nnz];
            v = new T[nnz];

            sort_triplets_morton(array, nnz);

            for (IT i=0; i<nnz; i++) {
                ri[i] = array[i].row;
                ci[i] = array[i].col;
                v[i]  = array[i].val;
            }
        }

        ~BlockExtCoo()
        {
            delete [] ri;
            delete [] ci;
            delete [] v;
        }

        void spmv_block (T const * const x, T * const y) const
        {
            spmv_coo (v, ri, ci, this->nnz, x, y);
        }
};

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

/*
 *  Dense block
 */
template <typename T, typename IT>
class BlockExtDense : public BlockBase<T,IT> {
    private:
        T * val;
        IT M, N;

    public:
        BlockExtDense(Element<T,IT> * array, IT rows, IT columns, IT nnz)
            : M(rows), N(columns), BlockBase<T,IT>(nnz)
        {
            IT pos;

            val = new T[M*N]();

            for (IT i=0; i<nnz; i++) {
                pos = array[i].row * N + array[i].col;
                val[pos] = array[i].val;
            }
        }

        ~BlockExtDense()
        {
            delete [] val;
        }

        void spmv_block (T const * const x, T * const y) const
        {
            spmv_dense (val, M, N, x, y);
        }
};

/* ----------------- Default Block Factory ----------------- */

template <typename T, typename IT, typename SIT>
class BlockFactoryDefault : public BlockFactory<T,IT> {
    public:
        MatrixType create (BlockBase<T,IT> ** target,
                           Element<T,IT> * array, IT rows, IT columns, IT nnz)
        {
            float coo, csr, dense, density;
            int workers = RT_WORKERS;

            coo = (float) nnz * (2 * sizeof(SIT) + sizeof(T));
            csr = (float) rows * sizeof(IT) + nnz * (sizeof(SIT) + sizeof(T));
            dense = ((float) rows) * columns * sizeof(T);
            density= ((float) nnz) / (rows * columns);
        
            if (coo < 1.0f * csr) { 
                // COO block
                *target =  new BlockExtCoo<T,IT,SIT>(array, nnz);
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
                *target = new BlockExtDense<T,IT>(array, rows, columns, nnz);
                return BLOCK_DENSE;
            }
        }
};

#endif
