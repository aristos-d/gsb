#ifndef _CSBR_H_
#define _CSBR_H_
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <limits>
#include <omp.h>

#include "typedefs.h"
#include "utils.h"
#include "partition.h"
#include "matrix/csr.1.h"
#include "matrix/coo.2.h"
#include "matrix/coo.3.h"

/*
 * CSR matrix containing CSR blocks of variable size or Compressed Sparse
 * Block-Rows.
 */
template <class T, class IT>
struct Csbr
{
  Csr<T,IT> * blocks;
  IT * blockrow_ptr;         // Indices for blocks array
  IT * blockcol_ind;

  // Block information
  IT * blockrow_offset;
  IT * blockcol_offset;
  IT blockrows;
  IT blockcols;
  IT nnzblocks;

  // Partitioning information
  BlockRowPartition<IT> * partition;

  // Original size
  IT rows;
  IT columns;
  IT nnz;

  // Pointers to the large arrays
  IT * col_ind;
  T * val;

  IT nonzeros () const { return nnz; }
};

/*
 * Returns the number of non-zero elements in a block of the matrix.
 */
template <class T, class IT>
inline IT block_nonzeros(const Csbr<T, IT> * A, IT i)
{
    return A->blocks[i].row_ptr[A->blocks[i].rows];
}

/*
 *
 */
template <class T, class IT>
void spmv_chunk(Csbr<T,IT> const * const A,
                T const * const __restrict x, T * const __restrict y,
                BlockRowPartition<IT> const * const partition,
                IT const first, IT const last, IT const bsize)
{
    if (last - first == 1) {

        IT x_offset;
        IT bstart = partition->chunks[first];
        IT bend = partition->chunks[last];

        for (IT k=bstart; k<bend; k++) {
            x_offset = A->blockcol_offset[A->blockcol_ind[k]];
            spmv_serial(A->blocks + k, x + x_offset, y);
        }

    } else {

        IT middle = (first+last) / 2;

        #pragma omp task
        spmv_chunk(A, x, y, partition, first, middle, bsize);

        if (RT_SYNCHED) {
            spmv_chunk(A, x, y, partition, middle, last, bsize);
        } else {

            // We need C++ style allocation to ensure proper initialization
            T * temp = new T[bsize]();

            spmv_chunk(A, x, temp, partition, middle, last, bsize);
            #pragma omp taskwait

            #pragma omp simd
            for (IT i=0; i<bsize; i++)
                y[i] += temp[i];

            delete [] temp;
        }
    }
}

/*
 * SpMV routine for matrices in CSBR format. y vector MUST be already initialized.
 * Parallel spmv for blocks
 */
template <class T, class IT>
void spmv(Csbr<T,IT> const * const A,
                    T const * const __restrict x, T * const __restrict y)
{
    IT const * const blockrow_ptr = A->blockrow_ptr;
    IT const * const blockrow_offset = A->blockrow_offset;

    // For each block row
    #pragma omp parallel for schedule(dynamic, 1)
    for(IT bi=0; bi<A->blockrows; bi++){

      IT nchunks = A->partition[bi].nchunks;
      IT y_start = blockrow_offset[bi];
      IT y_end = blockrow_offset[bi+1];

      spmv_chunk(A, x, y + y_start, A->partition + bi, (IT) 0, nchunks, y_end - y_start);

    }
}

/* ------------------ Constructors begin ------------------ */

/*
 * Construct a CSBR matrix from a Coo2 matrix.
 */
template <class T, class IT>
void Coo_to_Csbr(Csbr<T, IT> * A, Coo2<T, IT> * B,
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  A->rows = B->rows;
  A->columns = B->columns;
  A->nnz = B->nnz;
  A->blockrows = blockrows;
  A->blockcols = blockcols;
  A->blockrow_offset = blockrow_offset;
  A->blockcol_offset = blockcol_offset;

  // Sanity checks on block sizes
  assert(blockrows <= B->rows);
  assert(blockcols <= B->columns);
  assert(A->blockrow_offset[blockrows] == B->rows);
  assert(A->blockcol_offset[blockcols] == B->columns);

  IT nnz = A->nnz;
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT block_nnz, nnz_blocks;

  // Sort triplets according to block row and block column
  sort_triplets_blocks(B->triplets, nnz, blockrow_offset, blockrows, blockcol_offset, blockcols);

  DEBUG(puts("Sorting complete"));

  // Initialize non-zero counter to zero
  IT * counter = new IT[blockrows*blockcols]();

  // Count non-zeros of each block
  for(IT i=0; i<nnz; i++)
  {
    br = index_to_blockindex(blockrow_offset, blockrows, B->triplets[i].row);
    bc = index_to_blockindex(blockcol_offset, blockcols, B->triplets[i].col );
    counter[br*blockcols + bc]++;
  }

  // Memory allocation for block-row pointers
  A->blockrow_ptr = (IT *) malloc((blockrows+1) * sizeof(IT));
  A->col_ind = (IT *) malloc( nnz * sizeof(IT));
  A->val = (T *) malloc( nnz * sizeof(T));

  // Count non-zero blocks, calculate block row pointers
  nnz_blocks = 0;
  A->blockrow_ptr[0] = 0;
  for(IT i=0; i<blockrows; i++){
    for(IT j=0; j<blockcols; j++){
      if(counter[i*blockcols + j] > 0) nnz_blocks++ ;
    }
    A->blockrow_ptr[i+1] = nnz_blocks;
  }
  A->nnzblocks = nnz_blocks;

  A->blocks = (Csr<T,IT> *) malloc(nnz_blocks * sizeof(Csr<T,IT>));
  A->blockcol_ind = (IT *) malloc(nnz_blocks * sizeof(IT));

  // Initialize every CSR block
  IT block_index = 0;
  IT val_index = 0;

  for(IT i=0; i<blockrows; i++)
  {
    br_offset = blockrow_offset[i];
    br_size = blockrow_offset[i + 1] - br_offset;

    for(IT j=0; j<blockcols; j++)
    {
      block_nnz = counter[i*blockcols+j];
      if(block_nnz>0){
        bc_offset = blockcol_offset[j];
        bc_size = blockcol_offset[j+1] - bc_offset;
        A->blockcol_ind[block_index] = j;

        // Calculate new row and column indexes
        for(IT k=0; k<block_nnz; k++){
          B->triplets[val_index+k].row = B->triplets[val_index+k].row - br_offset;
          B->triplets[val_index+k].col = B->triplets[val_index+k].col - bc_offset;
        }

        // Initialize CSR block
        A->blocks[block_index].col_ind = A->col_ind + val_index;
        A->blocks[block_index].val = A->val + val_index;

        Coo_to_Csr(A->blocks + block_index, B->triplets + val_index,
                   br_size, bc_size, block_nnz,
                   false, false);

        val_index = val_index + block_nnz;
        block_index++;
      }
    }
  }

  // Release temporary memory
  delete [] counter;

  partition_init(A);
}

/*
 * Construct a CSBR matrix from a Coo3 matrix.
 */
template <class T, class IT>
void Coo_to_Csbr(Csbr<T, IT> * A, Coo3<T, IT> * B,
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT block_index;
  IT b, prev_b;
  IT row_index, col_index;

  DEBUG(printf("Constructing CSBR with %lux%lu blocks\n", (unsigned long) blockrows, (unsigned long) blockcols));
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
  A->blocks = (Csr<T,IT> *) malloc(A->nnzblocks * sizeof(Csr<T,IT>));
  A->blockcol_ind = (IT *) malloc(A->nnzblocks * sizeof(IT));
  A->blockrow_ptr = (IT *) calloc(A->blockrows + 1, sizeof(IT));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));

  DEBUG(puts("Memory allocation done"));

  block_index = std::numeric_limits<IT>::max();
  prev_b = std::numeric_limits<IT>::max();

  for (IT i=0; i<A->nnz; i++)
  {
    b = B->elements[i].block;

    // Check if this is the begining of a new block
    if (b != prev_b)
    {
      block_index++;
      prev_b = b;
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;
      bc_size = blockcol_offset[bc + 1] - bc_offset;

      // Block meta data
      A->blockrow_ptr[br+1]++;
      A->blockcol_ind[block_index] = bc;

      // CSR block initialize
      A->blocks[block_index].rows = br_size;
      A->blocks[block_index].val = A->val + i;
      A->blocks[block_index].col_ind = A->col_ind + i;
      A->blocks[block_index].row_ptr = (IT *) calloc(br_size + 1, sizeof(IT));
    }

    assert(B->elements[i].row >= br_offset);
    assert(B->elements[i].row <  br_offset + br_size);
    assert(B->elements[i].col >= bc_offset);
    assert(B->elements[i].col <  bc_offset + bc_size);

    row_index = B->elements[i].row - br_offset;
    col_index = B->elements[i].col - bc_offset;

    A->blocks[block_index].row_ptr[row_index + 1]++;
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

  for (br=0; br<A->blockrows; br++)
  {
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }

  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);

  partition_init(A);
}

/*
 * Additional constructor to create a CSBR matrix with a constant block size.
 * Final blocks in each dimension will be a little larger when block size is
 * not a divisor of total number of rows/columns.
 */
template <class T, class IT, class COOTYPE>
void Coo_to_Csbr(Csbr<T, IT> * A, COOTYPE * B, IT br_size, IT bc_size)
{
  IT blockrows = B->rows / br_size;
  IT blockcols = B->columns / bc_size;

  if(blockrows == 0) blockrows = 1;
  if(blockcols == 0) blockcols = 1;
  IT * blockrow_offset = (IT *) malloc( (blockrows + 1) * sizeof(IT));
  IT * blockcol_offset = (IT *) malloc( (blockcols + 1) * sizeof(IT));

  // Calculate block-row offsets
  blockrow_offset[0] = 0;
  for (IT i=0; i<(blockrows - 1); i++)
  {
    blockrow_offset[i+1] = blockrow_offset[i] + br_size;
  }
  blockrow_offset[blockrows] = B->rows;

  // Calculate block-column offsets
  blockcol_offset[0] = 0;
  for (IT i=0; i<(blockcols - 1); i++)
  {
    blockcol_offset[i+1] = blockcol_offset[i] + bc_size;
  }
  blockcol_offset[blockcols] = B->columns;

  DEBUG(puts("Block offsets calculated"));

  Coo_to_Csbr(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

template <class T, class IT, template<typename, typename> class COO>
void Coo_to_Blocked(Csbr<T,IT> * A, COO<T,IT> * B,
                    IT * blockrow_offset, IT blockrows,
                    IT * blockcol_offset, IT blockcols)
{
    Coo_to_Csbr(A, B, blockrow_offset, blockrows, blockcol_offset, blockcols);
}

/*
 * Print information about the matrix
 */
template <class T, class IT>
void print_info(Csbr<T, IT> A)
{
  printf("Csbr matrix : \n");
  printf("\t%ld rows x %ld columns\n", (long) A.rows, (long) A.columns);
  printf("\t%ld block-rows x %ld block-columns\n", (long) A.blockrows, (long) A.blockcols);
  printf("\t%ld non-zeros in %ld blocks\n", (long) A.nnz, (long) A.nnzblocks);
  printf("\t%.4f%% non-zeros\n", 100 * (float) A.nnz / ( (float) A.rows * (float) A.columns));
  printf("\t%.4f%% non-zero blocks\n", 100 * (float) A.nnzblocks / ( (float) A.blockrows * (float) A.blockcols));
}


/*
 * Release SOME memory allocated for the matrix. This function only releases
 * memory allocated by the Coo_to_Csbr function.
 */
template <class T, class IT>
void release(Csbr<T, IT> A)
{
    for (IT i=0; i<A.nnzblocks; i++) {
        delete [] A.blocks[i].row_ptr;
    }

    // Block-row and block-column offsets may point to the same array
    if (A.blockcol_offset == A.blockrow_offset) {
        free(A.blockrow_offset);
    } else {
        free(A.blockcol_offset);
        free(A.blockrow_offset);
    }

    free(A.blockcol_ind);
    free(A.blocks);
    free(A.blockrow_ptr);
    free(A.col_ind);
    free(A.val);
}

#endif
