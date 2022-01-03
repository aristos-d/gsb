#ifndef _CSBR_2_H_
#define _CSBR_2_H_
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <limits>
#include <omp.h>

#include "utils.h"
#include "generic/csr.h"
#include "matrix/coo.3.h"

/*
 * CSR matrix containing CSR blocks of variable size.
 */
template <class T, class IT>
struct Csbr2 {
  IT * blockrow_ptr;         // Indices for row_ptr array

  // Block information
  IT * blockrow_offset;
  IT * blockcol_offset;
  IT blockrows;
  IT blockcols;
  IT nnzblocks;

  // Original size
  IT rows;
  IT columns;
  IT nnz;

  // Pointers to the large arrays
  IT ** row_ptr;
  IT * col_ind;
  T * val;
};

/*
 * Return the offset of block column "i"
 */
template <class T, class IT>
inline IT get_block_column_offset(const Csbr2<T,IT> * A, IT i)
{
    return A->blockcol_offset[i];
}

template <class T, class IT>
inline IT get_block_row_offset(const Csbr2<T,IT> * A, IT i)
{
    return A->blockrow_offset[i];
}


/*
 * SpMV routine for matrices in CSBR2 format. y vector MUST be already initialized.
 */
template <class T, class IT>
void spmv(const Csbr2<T, IT> * const A, const T * __restrict x, T * __restrict y)
{
  // For each block row
  #pragma omp parallel for schedule(dynamic,1)
  for (IT bi=0; bi<A->blockrows; bi++) {
    IT rows;
    IT blockrow_start, blockrow_end;
    IT y_offset;

    y_offset = A->blockrow_offset[bi];
    rows = A->blockrow_offset[bi+1] - y_offset;
    blockrow_start = A->blockrow_ptr[bi];
    blockrow_end = A->blockrow_ptr[bi+1];

    // For every block in block row
    for (IT k=blockrow_start; k<blockrow_end; k++) {
      spmv_csr_serial ( A->row_ptr[k], A->col_ind, A->val, rows, x, y + y_offset);
    }
  }
}

/*
Construct a CSBR2 matrix from a Coo3 matrix.
*/
template <class T, class IT>
void Coo_to_Csbr(Csbr2<T,IT> * A, Coo3<T,IT> * B,
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT block_index;
  IT b, prev_b;

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
  A->blockrow_ptr = (IT *) calloc(A->blockrows + 1, sizeof(IT));
  A->row_ptr = (IT **) malloc(A->nnzblocks * sizeof(IT *));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));

  block_index = std::numeric_limits<IT>::max();
  prev_b = std::numeric_limits<IT>::max();

  for (IT i=0; i<A->nnz; i++) {
    b = B->elements[i].block;

    // Check if this is the begining of a new block
    if (b != prev_b) {
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;
      bc_size = blockcol_offset[bc + 1] - bc_offset;

      block_index++;
      prev_b = b;
      
      // CSR block initialize and block meta data
      A->blockrow_ptr[br+1]++;
      A->row_ptr[block_index] = (IT *) calloc(br_size + 1, sizeof(IT));      
    }

    assert(B->elements[i].row >= br_offset);
    assert(B->elements[i].row <  br_offset + br_size);
    assert(B->elements[i].col >= bc_offset);
    assert(B->elements[i].col <  bc_offset + bc_size);

    A->row_ptr[block_index][B->elements[i].row - br_offset + 1]++;
    A->col_ind[i] = B->elements[i].col;
    A->val[i] = B->elements[i].val;
  }

  for (br=0; br<A->blockrows; br++) {
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }

  // Get row pointers by accumulating row non-zero counts

  IT nnz_count = 0;
  for (IT br=0; br<blockrows; br++) {
    IT block_size;
    IT blockrow_start, blockrow_end;

    block_size = A->blockrow_offset[br+1] - A->blockrow_offset[br];
    blockrow_start = A->blockrow_ptr[br];
    blockrow_end = A->blockrow_ptr[br+1];

    // For every block in block row
    for (IT k=blockrow_start; k<blockrow_end; k++) {
      A->row_ptr[k][0] = nnz_count;
      for (IT i=0; i<block_size; i++) {
        A->row_ptr[k][i+1] = A->row_ptr[k][i+1] + A->row_ptr[k][i];
      }
      nnz_count = A->row_ptr[k][block_size];
    }
  }

  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);
}

/*
 * Additional constructor to create a CSBR2 matrix with a constant block size.
 * Final blocks in each dimension will be a little larger when block size is
 * not a divisor of total number of rows/columns.
 */
template <class T, class IT, template<typename, typename> class COO>
void Coo_to_Csbr(Csbr2<T, IT> * A, COO<T,IT> * B, IT br_size, IT bc_size)
{
  Coo_to_Blocked(A, B, br_size, bc_size);
}

template <class T, class IT, template<typename, typename> class COO>
void Coo_to_Blocked(Csbr2<T,IT> * A, COO<T,IT> * B,
                    IT * blockrow_offset, IT blockrows, 
                    IT * blockcol_offset, IT blockcols)
{
    Coo_to_Csbr(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

/*
 * Print information about the matrix
 */
template <class T, class IT>
void print_info(Csbr2<T, IT> A)
{
  printf("Csbr2 matrix : \n");
  printf("\t%ld rows x %ld columns\n", (long) A.rows, (long) A.columns);
  printf("\t%ld block-rows x %ld block-columns\n", (long) A.blockrows, (long) A.blockcols);
  printf("\t%ld non-zeros in %ld blocks\n", (long) A.nnz, (long) A.nnzblocks);
  printf("\t%.4f%% non-zeros\n", 100 * (float) A.nnz / ( (float) A.rows * (float) A.columns));
  printf("\t%.4f%% non-zero blocks\n", 100 * (float) A.nnzblocks / ( (float) A.blockrows * (float) A.blockcols));
}


/*
 * Release SOME memory allocated for the matrix. This function only releases memory
 * allocated by the Coo_to_Csbr function.
 */
template <class T, class IT>
void release(Csbr2<T, IT> A)
{
  for (IT i=0; i<A.nnzblocks; i++) {
    free(A.row_ptr[i]);
  }
  // Block-row and block-column offsets may point to the same array
  if (A.blockcol_offset == A.blockrow_offset) {
    free(A.blockrow_offset);
  } else {
    free(A.blockcol_offset);
    free(A.blockrow_offset);
  }
  free(A.blockrow_ptr);
  free(A.row_ptr);
  free(A.col_ind);
  free(A.val);
}

#endif
