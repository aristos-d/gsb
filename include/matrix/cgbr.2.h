#ifndef _CGBR_2_H_
#define _CGBR_2_H_

#include "typedefs.h"
#include "blocks.h"
#include "utils.h"
#include "partition.h"
#include "generic/gsb.h"
#include "matrix/blocks.h"

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

        Cgbr2() {}
        Cgbr2(Element<T,IT> * array, IT _rows, IT _columns, IT nnz,
              IT * _blockrow_offset, IT _blockrows,
              IT * _blockcol_offset, IT _blockcols,
              BlockFactory<T,IT> & factory);
        int create(Element<T,IT> * array, IT _rows, IT _columns, IT nnz,
                   IT * _blockrow_offset, IT _blockrows,
                   IT * _blockcol_offset, IT _blockcols,
                   BlockFactory<T,IT> & factory);
        ~Cgbr2() {}

        void spmv_block (T const * const __restrict x, T * const __restrict y) const
	    {
            spmv(this, x, y);
	    }
};

/*
 * Return the offset of block column "i"
 */
template <class T, class IT, class SIT>
inline IT get_block_column_offset(const Cgbr2<T,IT,SIT> * A, IT i)
{
    return A->blockcol_offset[i];
}

template <class T, class IT, class SIT>
inline IT get_block_row_offset(const Cgbr2<T,IT,SIT> * A, IT i)
{
    return A->blockrow_offset[i];
}

/*
 * Get pointer to block i
 */
template <class T, class IT, class SIT>
inline BlockBase<T,IT> * block (Cgbr2<T,IT,SIT> const * const A, IT i)
{
    return A->blocks[i];
}

// --------------------- Constructor ---------------------

template <class T, class IT, class SIT>
int Cgbr2<T,IT,SIT>::create (
        Element<T,IT> * array, IT _rows, IT _columns, IT nnz,
        IT * _blockrow_offset, IT _blockrows,
        IT * _blockcol_offset, IT _blockcols,
        BlockFactory<T,IT> & factory)
{
    this->nnz = nnz;
    rows = _rows;
    columns = _columns;
    blockrows =_blockrows;
    blockcols =_blockcols;
    blockrow_offset = _blockrow_offset;
    blockcol_offset = _blockcol_offset;

    IT br, br_offset, br_size;
    IT bc, bc_offset, bc_size;
    IT b;
    MatrixType block_type;

    // Sanity checks on block sizes
    assert(blockrows <= rows);
    assert(blockcols <= columns);
    assert(blockrow_offset[blockrows] == rows);
    assert(blockcol_offset[blockcols] == columns);

    // Sort non-zeros according to block-row, block-column, row, column
    calculate_block_id(array, nnz, blockrow_offset, blockrows, blockcol_offset, blockcols);
    sort_elements_blocks(array, nnz);

    nnzblocks = count_blocks(array, nnz);

    // Memory allocations
    blocks = new BlockBase<T,IT>*[nnzblocks];
    blockcol_ind = new IT[nnzblocks];
    blockrow_ptr = new IT[blockrows + 1]();

    IT block_index = 0;
    IT i=0, j=0, k;

    while (i<nnz) {
        b = array[i].block;
        j = i + 1;

        // Scan forward to find the end of the current block
        while (j<nnz && array[j].block == b) j++;

        br = b / blockcols;
        bc = b % blockcols;
        br_offset = blockrow_offset[br];
        bc_offset = blockcol_offset[bc];
        br_size = blockrow_offset[br + 1] - br_offset;
        bc_size = blockcol_offset[bc + 1] - bc_offset;

        // Block meta data
        blockrow_ptr[br+1]++;
        blockcol_ind[block_index] = bc;

        // Adjust indeces inside block
        for (k=i; k<j; k++) {
            assert(array[k].row >= br_offset);
            assert(array[k].row < br_offset + br_size);
            assert(array[k].col >= bc_offset);
            assert(array[k].col < bc_offset + bc_size);
            array[k].row -= br_offset;
            array[k].col -= bc_offset;
        }

        // Initialize new block
        block_type = factory.create(blocks + block_index, array + i,
                                    br_size, bc_size, j-i);
        type_block_count[block_type]++;
        type_nnz_count[block_type] += (j-i);

        // Prepare for next block
        block_index++;
        i = j;
    }

    for (br=0; br<blockrows; br++) {
        blockrow_ptr[br+1] = blockrow_ptr[br+1] + blockrow_ptr[br];
    }

    assert(blockrow_ptr[blockrows] == nnzblocks);

    // Now create block-row partitioning
    partition_init(this);
    return 0;
}

template <class T, class IT, class SIT>
Cgbr2<T,IT,SIT>::Cgbr2 (Element<T,IT> * array, IT rows, IT columns, IT nnz,
        IT * blockrow_offset, IT blockrows,
        IT * blockcol_offset, IT blockcols,
        BlockFactory<T,IT> & factory)
{
    this->create(array, rows, columns, nnz,
            blockrow_offset, blockrows, blockcol_offset, blockcols, factory);
}

template <typename T, typename IT, typename SIT>
int Coo_to_Cgbr (Cgbr2<T,IT,SIT> * A, Coo3<T,IT> * B,
                  IT * blockrow_offset, IT blockrows,
                  IT * blockcol_offset, IT blockcols,
                  BlockFactory<T,IT> & factory )
{
  return A->create(B->elements, B->rows, B->columns, B->nnz,
            blockrow_offset, blockrows, blockcol_offset, blockcols, factory);
}

/*
 * Overload constructor to provide a default block factory
 */
template <typename T, typename IT, typename SIT>
void Coo_to_Cgbr (Cgbr2<T,IT,SIT> * A, Coo3<T,IT> * B,
                  IT * blockrow_offsets, IT blockrows,
                  IT * blockcol_offsets, IT blockcols)
{
    BlockFactoryDefault<T,IT,SIT> factory;
    Coo_to_Cgbr (A, B, blockrow_offsets, blockrows, blockcol_offsets, blockcols, factory);
}

template <typename T, typename IT, typename SIT>
void Coo_to_Cgbr(Cgbr2<T,IT,SIT> * A, Coo3<T,IT> * B, IT br_size, IT bc_size)
{
    Coo_to_Blocked(A, B, br_size, bc_size);
}

template <typename T, typename IT, typename SIT>
void Coo_to_Blocked (Cgbr2<T,IT,SIT> * A, Coo3<T,IT> * B,
                  IT * blockrow_offsets, IT blockrows,
                  IT * blockcol_offsets, IT blockcols)
{
    Coo_to_Cgbr(A, B, blockrow_offsets, blockrows, blockcol_offsets, blockcols);
}

/* ------------------- Utillities ------------------- */

/*
 * Release memory allocated for the CGBR matrix
 */
template <class T, class IT, class SIT>
void release(Cgbr2<T,IT,SIT> A)
{
    partition_destroy(&A);

    // Release memory for each block
    for (IT i=0; i<A.nnzblocks; i++) {
        delete A.blocks[i];
    }

    delete [] A.blocks;
    delete [] A.blockrow_ptr;
    delete [] A.blockcol_ind;
}

#endif
