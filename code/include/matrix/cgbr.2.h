#ifndef _CGBR_2_H_
#define _CGBR_2_H_

#include <cilk/cilk.h>

#include "typedefs.h"
#include "utils.h"
#include "partition.h"
#include "matrix/blocks.h"
#include "matrix/gsb.h"

// --------------------- Constructor --------------------- 

template <class T, class IT, class SIT>
Cgbr2<T,IT,SIT>::Cgbr2(Element<T,IT> * array, IT _rows, IT _columns, IT nnz,
        IT * _blockrow_offset, IT _blockrows,
        IT * _blockcol_offset, IT _blockcols,
        BlockFactory<T,IT> & factory) : 
    rows(_rows), columns(_columns), BlockBase<T,IT>(nnz),
    blockrow_offset(_blockrow_offset), blockrows(_blockrows),
    blockcol_offset(_blockcol_offset), blockcols(_blockcols)
{
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
}

template <typename T, typename IT, typename SIT>
int Coo_to_Cgbr (Cgbr2<T,IT,SIT> * A, Coo3<T,IT> * B,
                  IT * blockrow_offset, IT blockrows,
                  IT * blockcol_offset, IT blockcols,
                  BlockFactory<T,IT> & factory )
{
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT block_index;
  IT b;
  MatrixType block_type;

  // Sort non-zeros according to block-row, block-column, row, column
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
  A->blocks = new BlockBase<T,IT>*[A->nnzblocks];
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
    block_type = factory.create(A->blocks + block_index, 
                                B->elements + i, br_size, bc_size, j-i);
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

  // Now create block-row partitioning
  partition_init(A);

  return 0;
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
