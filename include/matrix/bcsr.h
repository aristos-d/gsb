#ifndef BCSR_H
#define BCSR_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <limits>

#include "partition.h"
#include "utils.h"
#include "matrix/coo.3.h"
#include "matrix/csr.1.h"
#include "spmv/omp/gsb.h"

/*
 * Dense matrix of CSR blocks of variable size or Blocked Compressed Sparse
 * Rows
 */
template <class T, class IT>
struct Bcsr
{
    // Block information
    IT * blockrow_offset;
    IT * blockcol_offset;
    IT blockrows;
    IT blockcols;

    // Partitioning information
    BlockRowPartition<IT> * partition;
    bool balanced {true}; // TODO: Find a heuristic for this.

    // Original size
    IT rows;
    IT columns;
    IT nnz;

    // Actual block array
    Csr<T,IT> * blocks;
    IT nnzblocks;

    // Pointers to the large arrays
    IT * col_ind;
    T * val;

    IT nonzeros() const { return nnz; }

    // Returns the number of non-zero elements in a block of the matrix.
    IT block_nonzeros(IT i) const
    {
        return blocks[i].row_ptr[blocks[i].rows];
    }

    // Return the offset of block column "i"
    IT get_block_column_offset(IT i) const
    {
        return blockcol_offset[i];
    }

    IT get_block_row_offset(IT i) const
    {
        return blockrow_offset[i];
    }

    /*
     * spmv routine for matrices in BCSR format. y vector MUST be already
     * initialized. Parallel spmv for blocks
     */
    void spmv(const T * __restrict x, T * __restrict y) const
    {
        spmv_blocked<Bcsr<T,IT>,T,IT>(this, x, y);
    }
};

/* ------------------ Constructors begin ------------------ */

/*
 * Construct a BCSR matrix from a Coo3 matrix.
 */
template <class T, class IT>
void Coo_to_Bcsr(Bcsr<T, IT> * A, Coo3<T, IT> * B,
                 IT * blockrow_offset, IT blockrows,
                 IT * blockcol_offset, IT blockcols)
{
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT b, prev_b;
  IT row_index, col_index;

  DEBUG(printf("Constructing BCSR with %lux%lu blocks\n", (unsigned long) blockrows, (unsigned long) blockcols));
  DEBUG(printf("Original COO matrix has %lu non-zeros\n", (unsigned long) B->nnz));

  // Sort triplets according to block-row, block-column, row, column
  calculate_block_id(B->elements, B->nnz, blockrow_offset, blockrows, blockcol_offset, blockcols);
  sort_elements_blocks(B->elements, B->nnz);
  DEBUG(puts("Sorting complete"));

  A->rows = B->rows;
  A->columns = B->columns;
  A->nnz = B->nnz;
  A->blockrows = blockrows;
  A->blockcols = blockcols;
  A->blockrow_offset = blockrow_offset;
  A->blockcol_offset = blockcol_offset;
  A->nnzblocks = count_blocks(B->elements, B->nnz);

  // Sanity checks on block sizes
  assert(blockrows <= B->rows);
  assert(blockcols <= B->columns);
  assert(A->blockrow_offset[blockrows] == B->rows);
  assert(A->blockcol_offset[blockcols] == B->columns);

  // Memory allocations
  A->blocks = (Csr<T,IT> *) malloc(blockrows * blockcols * sizeof(Csr<T,IT>));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));

  DEBUG(puts("Memory allocation done"));

  // Mark all blocks as having 0 rows to filter out empty ones
  for (IT i=0; i<blockrows * blockcols; i++) {
    A->blocks[b].rows = 0;
  }

  prev_b = std::numeric_limits<IT>::max();

  for (IT i=0; i<A->nnz; i++) {
    b = B->elements[i].block;

    // Check if this is the begining of a new block
    if (b != prev_b)
    {
      prev_b = b;
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;
      bc_size = blockcol_offset[bc + 1] - bc_offset;

      // CSR block initialize
      A->blocks[b].rows = br_size;
      A->blocks[b].val = A->val + i;
      A->blocks[b].col_ind = A->col_ind + i;
      A->blocks[b].row_ptr = (IT *) calloc(br_size + 1, sizeof(IT));
    }

    assert(B->elements[i].row >= br_offset);
    assert(B->elements[i].row <  br_offset + br_size);
    assert(B->elements[i].col >= bc_offset);
    assert(B->elements[i].col <  bc_offset + bc_size);

    row_index = B->elements[i].row - br_offset;
    col_index = B->elements[i].col - bc_offset;

    A->blocks[b].row_ptr[row_index + 1]++;
    A->col_ind[i] = col_index;
    A->val[i] = B->elements[i].val;
  }

  DEBUG(puts("Data copied"));

  // Get row pointers from row non-zero counters
  for (IT bi=0; bi<A->nnzblocks; bi++)
  {
    for (IT i=0; i<A->blocks[bi].rows; i++)
    {
      A->blocks[bi].row_ptr[i+1] = A->blocks[bi].row_ptr[i+1] + A->blocks[bi].row_ptr[i];
    }
  }
}

/*
 * Constructor wrapper
 */
template <class T, class IT>
void Coo_to_Blocked(Bcsr<T, IT> * A, Coo3<T, IT> * B,
                    IT * blockrow_offset, IT blockrows,
                    IT * blockcol_offset, IT blockcols)
{
  Coo_to_Bcsr(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

/*
 * Print information about the matrix
 */
template <class T, class IT>
void print_info(Bcsr<T, IT> A)
{
  printf("Bcsr matrix : \n");
  printf("\t%ld rows x %ld columns\n", (long) A.rows, (long) A.columns);
  printf("\t%ld block-rows x %ld block-columns\n", (long) A.blockrows, (long) A.blockcols);
  printf("\t%ld non-zeros in %ld blocks\n", (long) A.nnz, (long) A.nnzblocks);
  printf("\t%.4f%% non-zeros\n", 100 * (float) A.nnz / ( (float) A.rows * (float) A.columns));
  printf("\t%.4f%% non-zero blocks\n", 100 * (float) A.nnzblocks / ( (float) A.blockrows * (float) A.blockcols));
}

/*
 * Release SOME memory allocated for the matrix. This function only releases memory
 * allocated by the Coo_to_Bcsr function.
 */
template <class T, class IT>
void release(Bcsr<T, IT> A)
{
  for (IT i=0; i<A.blockrows * A.blockcols; i++)
  {
    if (A.blocks[i].rows > 0) free(A.blocks[i].row_ptr);
  }
  // Block-row and block-column offsets may point to the same array
  if (A.blockcol_offset == A.blockrow_offset){
    free(A.blockrow_offset);
  } else {
    free(A.blockcol_offset);
    free(A.blockrow_offset);
  }
  free(A.blocks);
  free(A.col_ind);
  free(A.val);
}

#endif /* BCSR_H */
