#ifndef _CSBR_3_H_
#define _CSBR_3_H_

#include <limits>
#include <cassert>
#include <cstdio>

#include "partition.h"
#include "matrix/coo.3.h"

/*
 * CSR matrix containing COO blocks of fixed size.
 */
template <class T, class IT, class SIT>
struct Csbr3
{
    IT * block_ptr;     // Indices for actual data arrays
    IT * blockcol_ind;
    IT * blockrow_ptr;  // Indices for block_ptr

    // Block information
    IT blockrows;
    IT blockcols;
    IT nnzblocks;
    IT block_size;

    // Original size
    IT rows;
    IT columns;
    IT nnz;

    // Pointers to actual data
    SIT * row_ind;
    SIT * col_ind;
    T * val;

    // Partitioning data
    bool balanced;
    BlockRowPartition<IT> * partition;

    IT nonzeros() const { return nnz; }

    // Return the offset of block column "i"
    IT get_block_column_offset(IT i) const
    {
        return block_size * i;
    }

    IT get_block_row_offset(IT i) const
    {
        return block_size * i;
    }
};

/*
 * Construct a CSBR3 matrix from a Coo3 matrix.
 */
template <class T, class IT, class SIT>
void Coo_to_Csbr (Csbr3<T,IT,SIT> * A, Coo3<T,IT> * B, IT block_size)
{
    IT br, br_offset;
    IT bc, bc_offset;
    IT block_index;
    IT b, prev_b;

    A->rows = B->rows;
    A->columns = B->columns;
    A->nnz = B->nnz;
    A->blockrows = (B->rows + block_size - 1) / block_size;
    A->blockcols = (B->columns + block_size - 1) / block_size;
    A->block_size = block_size;

    // Sort triplets according to block-row, block-column, row, column
    calculate_block_id(B->elements, B->nnz, block_size, block_size, A->blockcols);
    sort_elements_blocks(B->elements, B->nnz);
    DEBUG(puts("Sorting complete"));

    A->nnzblocks = count_blocks(B->elements, B->nnz);

    // Memory allocations
    A->blockrow_ptr = new IT[A->blockrows + 1]();
    A->block_ptr = new IT[A->nnzblocks + 1];
    A->blockcol_ind = new IT[A->nnzblocks];
    A->row_ind = new SIT[A->nnz];
    A->col_ind = new SIT[A->nnz];
    A->val = new T[A->nnz];

    A->block_ptr[A->nnzblocks] = A->nnz;

    block_index = std::numeric_limits<IT>::max();
    prev_b = std::numeric_limits<IT>::max();

    for (IT i = 0; i < A->nnz; i++) {
        b = B->elements[i].block;

        // Check if this is the begining of a new block
        if (b != prev_b) {
            br = b / A->blockcols;
            bc = b % A->blockcols;
            br_offset = br * block_size;
            bc_offset = bc * block_size;

            block_index++;
            prev_b = b;

            // CSR block initialize and block meta data
            A->block_ptr[block_index] = i;
            A->blockcol_ind[block_index] = bc;
            A->blockrow_ptr[br + 1]++;
        }

        assert(B->elements[i].row >= br_offset);
        assert(B->elements[i].row < br_offset + block_size);
        assert(B->elements[i].col >= bc_offset);
        assert(B->elements[i].col < bc_offset + block_size);

        // TODO: Assert type casts are safe
        A->row_ind[i] = B->elements[i].row - br_offset;
        A->col_ind[i] = B->elements[i].col - bc_offset;
        A->val[i] = B->elements[i].val;
    }

    // Calculate block-row pointers and check for sanity
    for (br = 0; br < A->blockrows; br++)
        A->blockrow_ptr[br + 1] = A->blockrow_ptr[br + 1] + A->blockrow_ptr[br];

    assert(A->blockrow_ptr[A->blockrows] == A->nnzblocks);
}

template <class T, class IT, class SIT>
void Coo_to_Blocked (Csbr3<T,IT,SIT> * A, Coo3<T,IT> * B,
                     IT blockrow_size, IT blockcol_size)
{
    assert(blockrow_size == blockcol_size);
    Coo_to_Csbr(A, B, blockrow_size);
}

template <class T, class IT, class SIT>
void print_info(Csbr3<T,IT,SIT> const& A)
{
    printf("Matrix info : \n");
    printf("\t%lu rows x %lu columns\n",
            (unsigned long) A.rows,
            (unsigned long) A.columns);
    printf("\t%lu x %lu blocks of size %lu\n",
            (unsigned long) A.blockrows,
            (unsigned long) A.blockcols,
            (unsigned long) A.block_size);
    printf("\t%lu non-zeros in %lu blocks\n",
            (unsigned long) A.nnz,
            (unsigned long) A.nnzblocks);
    printf("\t%.4f%% non-zeros\n",
            100 * (float) A.nnz / ( (float) A.rows * (float) A.columns));
    printf("\t%.4f%% non-zero blocks\n",
            100 * (float) A.nnzblocks / ( (float) A.blockrows * (float) A.blockcols));
}

/*
 * Memory deallocation
 */
template <class T, class IT, class SIT>
void release (Csbr3<T,IT,SIT>& A)
{
    delete [] A.blockrow_ptr;
    delete [] A.block_ptr;
    delete [] A.blockcol_ind;
    delete [] A.row_ind;
    delete [] A.col_ind;
    delete [] A.val;
}

#endif
