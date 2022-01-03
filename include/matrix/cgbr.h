#ifndef _CGBR_H_
#define _CGBR_H_
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include "typedefs.h"
#include "utils.h"
#include "partition.h"
#include "generic/gsb.h"
#include "generic/nonzeros.h"
#include "matrix/coo.3.h"
#include "matrix/block_coo.1.h"
#include "matrix/block_csr.h"
#include "matrix/dense.h"

/*
 * Union of all matrix types used for blocks.
 */
template <class T, class IT, class SIT>
union Matrix{
  BlockDense<T,IT> dense;
  BlockCsr<T,IT,SIT> csr;
  BlockCoo<T,IT,SIT> coo;
};

/*
 * Generic matrix representing a block of a larger sparse matrix.
 */
template <class T, class IT, class SIT>
struct MatrixBlock{
  Matrix<T,IT,SIT> matrix;
  MatrixType type;
};

/*
 * CSR matrix containing blocks of variable size and type or Compressed
 * Generalized Block Rows
 */
template <class T, class IT, class SIT>
struct Cgbr {
  MatrixBlock<T,IT,SIT> * blocks;
  IT * blockrow_ptr;         // Indexes for blocks array
  IT * blockcol_ind;
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
  IT nnz;
};

/*
 * Get pointer to block i
 */
template <class T, class IT, class SIT>
inline MatrixBlock<T,IT,SIT> * block (Cgbr<T,IT,SIT> const * const A, IT i)
{
    return A->blocks + i;
}

/*
 * Return the offset of block column "i"
 */
template <class T, class IT, class SIT>
inline IT get_block_column_offset(const Cgbr<T,IT,SIT> * A, IT i)
{
    return A->blockcol_offset[i];
}

template <class T, class IT, class SIT>
inline IT get_block_row_offset(const Cgbr<T,IT,SIT> * A, IT i)
{
    return A->blockrow_offset[i];
}

/*
 * Returns the number of non-zeros in the `i`th block
 */
template <class T, class IT, class SIT>
inline IT nonzeros (MatrixBlock<T,IT,SIT> const * const block)
{
    switch (block->type)
    {
        case BLOCK_COO :
            return nonzeros(block->matrix.coo);
        case BLOCK_CSR :
            return nonzeros(block->matrix.csr);
        case BLOCK_DENSE :
            return block->matrix.dense.rows * block->matrix.dense.columns;
        default:
            fprintf(stderr, "Invalid block found!\n");
            return 0;
    }
}

/*
 * Are the non-zeros distributed (almost) equally among rows?
 */
template <class T, class IT, class SIT>
bool is_balanced (Cgbr<T,IT,SIT> const * const A)
{
    float mean_nnz = A->nnz / (float) A->blockrows;
    float br_nnz;
    IT blockrow_start, blockrow_end;
    IT col_index;

    for (IT i=0; i<A->blockrows; i++) {

        blockrow_start = A->blockrow_ptr[i];
        blockrow_end = A->blockrow_ptr[i+1];
        br_nnz = 0.0f;

        // Count the non-zeros of each block-row
        for (IT k=blockrow_start; k<blockrow_end; k++) {
            col_index = A->blockcol_ind[k];
            br_nnz += block_nonzeros(A, k);
        }

        if (br_nnz > 2.0f * mean_nnz) return false;
    }

    return true;
}

/*
 * SpMV routine for a block of a CSBR matrix. It only calls the appropriate spmv
 * implementation based on the block matrix type.
 */
template <class T, class IT, class SIT>
inline void spmv (
        MatrixBlock<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    switch (A->type)
    {
        case BLOCK_COO :
            spmv(&(A->matrix.coo), x, y);
            break;
        case BLOCK_CSR :
            spmv(&(A->matrix.csr), x, y);
            break;
        case BLOCK_DENSE :
            spmv(&(A->matrix.dense), x, y);
            break;
        default:
            DEBUG(fprintf(stderr, "Invalid block found!\n"));
    }
}

/*
 * SpMV routine for a block of a CGBR matrix. It only calls the appropriate spmv
 * (serial) implementation based on the block matrix type.
 */
template <class T, class IT, class SIT>
inline void spmv_serial (
        MatrixBlock<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    switch (A->type)
    {
        case BLOCK_COO :
            spmv(&(A->matrix.coo), x, y);
            break;
        case BLOCK_CSR :
            spmv_serial(&(A->matrix.csr), x, y);
            break;
        case BLOCK_DENSE :
            spmv_serial(&(A->matrix.dense), x, y);
            break;
        default:
            DEBUG(fprintf(stderr, "Invalid block found!\n"));
    }
}

/*
 * SpMV routine for matrices in CGBR format. y vector MUST be already initialized.
 */
template <class T, class IT, class SIT>
void spmv_serial (
        Cgbr<T,IT,SIT> const * const A,
        T const * const __restrict x,
        T * const __restrict y)
{
    // For each block row
    for (IT bi=0; bi<A->blockrows; bi++) {
        IT x_offset, y_offset;
        IT blockrow_start, blockrow_end;
        IT col_index;

        blockrow_start = A->blockrow_ptr[bi];
        blockrow_end = A->blockrow_ptr[bi+1];
        y_offset = A->blockrow_offset[bi];

        // For every block in block row
        for (IT k=blockrow_start; k<blockrow_end; k++) {
            col_index = A->blockcol_ind[k];
            x_offset = A->blockcol_offset[col_index];
            spmv_serial(A->blocks + k, x + x_offset, y + y_offset);
        }
    }
}

/* ------------------ Constructors begin ------------------ */

template <class T, class IT, class SIT>
MatrixType Coo_to_MatrixBlock (
        MatrixBlock<T, IT, SIT> * block,
        IT rows, IT columns,
        Element<T, IT> * source, IT nnz)
{
    float coo, csr, nnzratio;

    coo = ((float) nnz) * (2 * sizeof(SIT) + sizeof(T));
    csr = (((float) rows) * sizeof(IT)) + (nnz * (sizeof(SIT) + sizeof(T)));
    nnzratio = nnz / ((float) rows * columns);

    if (coo < 1.5f * csr) {
        // COO block
        Coo_to_Coo(&(block->matrix.coo), source, nnz);
        block->type = BLOCK_COO;
    } else if (nnzratio < 0.3f) {
        // CSR block
        Coo_to_Csr(&(block->matrix.csr), source, rows, columns, nnz);
        block->type = BLOCK_CSR;
    } else {
        // Dense block
        Coo_to_Dense(&(block->matrix.dense), source, rows, columns, nnz);
        block->type = BLOCK_DENSE;
    }
    return block->type;
}

/*
 * Construct a CGBR matrix from a COO matrix.
 */
template <class T, class IT, class SIT>
void Coo_to_Cgbr(
        Cgbr<T,IT,SIT> * A,
        Coo3<T,IT> * B,
        IT *blockrow_offset, IT blockrows,
        IT *blockcol_offset, IT blockcols)
{
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT block_index;
  IT b;
  MatrixType block_type;

  DEBUG(fprintf(stderr, "Small index size %u, maximum value %u\n",
              sizeof(SIT), 1 << (8 * sizeof(SIT))));

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

  for(IT i=0; i<MATRIX_TYPE_NUM; i++){
    A->type_block_count[i] = 0;
    A->type_nnz_count[i] = 0;
  }

  // Sanity checks on block sizes
  assert(blockrows <= B->rows);
  assert(blockcols <= B->columns);
  assert(A->blockrow_offset[blockrows] == B->rows);
  assert(A->blockcol_offset[blockcols] == B->columns);

  // Memory allocations
  A->blocks = new MatrixBlock<T,IT,SIT>[A->nnzblocks];
  A->blockcol_ind = new IT[A->nnzblocks];
  A->blockrow_ptr = new IT[A->blockrows + 1]();

  block_index = 0;
  IT i=0, j=0, k;

  while (i<A->nnz) {
    b = B->elements[i].block;
    j = i + 1;

    // Scan forward to find the end of the current block
    while (j<A->nnz && B->elements[j].block == b) j++;

    br = b / blockcols;
    bc = b % blockcols;
    br_offset = blockrow_offset[br];
    bc_offset = blockcol_offset[bc];
    br_size = blockrow_offset[br + 1] - br_offset;
    bc_size = blockcol_offset[bc + 1] - bc_offset;

    // Block meta data
    A->blockrow_ptr[br+1]++;
    A->blockcol_ind[block_index] = bc;

    // Adjust indeces inside block
    for (k=i; k<j; k++) {
      assert(B->elements[k].row >= br_offset);
      assert(B->elements[k].row < br_offset + br_size);
      assert(B->elements[k].col >= bc_offset);
      assert(B->elements[k].col < bc_offset + bc_size);
      B->elements[k].row -= br_offset;
      B->elements[k].col -= bc_offset;
    }

    // Initialize new block
    block_type = Coo_to_MatrixBlock(A->blocks + block_index, br_size, bc_size, B->elements + i, j - i);
    A->type_block_count[block_type]++;
    A->type_nnz_count[block_type] += (j-i);

    // Prepare for next block
    block_index++;
    i = j;
  }

  for (br=0; br<A->blockrows; br++) {
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }

  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);

  A->balanced = is_balanced(A);

  if (!A->balanced) {
      // Unbalanced block-rows. We need within-block-row parallelism.
      partition_init(A);
  }
}

/*
 * Constructor wrapper
 */
template <typename T, typename IT, typename SIT>
void Coo_to_Cgbr (
        Cgbr<T,IT,SIT> * A,
        Coo3<T,IT> * B,
        IT br_size, IT bc_size)
{
    Coo_to_Blocked(A, B, br_size, bc_size);
}

template <typename T, typename IT, typename SIT>
void Coo_to_Blocked (
        Cgbr<T,IT,SIT> * A,
        Coo3<T,IT> * B,
        IT *blockrow_offset, IT blockrows,
        IT *blockcol_offset, IT blockcols)
{
    Coo_to_Cgbr(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

/*
 * Release memory allocated for a generalized matrix block
 */
template <class T, class IT, class SIT>
void release (MatrixBlock<T,IT,SIT> *A)
{
    switch (A->type)
    {
        case BLOCK_COO :
            release(A->matrix.coo);
            break;
        case BLOCK_CSR :          // Block is a matrix in classic CSR format
            release(A->matrix.csr);
            break;
        case BLOCK_DENSE :        // Block is a dense matrix
            release(A->matrix.dense);
            break;
        default:            // Error!
            fprintf(stderr, "Invalid block found!\n");
    }
}

/*
 * Release memory allocated for the CGBR matrix
 */
template <class T, class IT, class SIT>
void release(Cgbr<T,IT,SIT> A)
{
    if (!A.balanced)
        partition_destroy(&A);

    // Release memory for each block
    for (IT i=0; i<A.nnzblocks; i++) {
        release(A.blocks + i);
    }

    delete [] A.blocks;
    delete [] A.blockrow_ptr;
    delete [] A.blockcol_ind;
}

#endif
