#ifndef _IO_BLOCKED_H_
#define _IO_BLOCKED_H_

#include <assert.h>
#include <stdio.h>
#include <limits.h>

#include "common.h"
#include "matrix/coo.h"
#include "matrix/csr.h"
#include "matrix/csbr.h"
#include "matrix/cgbr.h"

/*
 * Function to perform initial steps of reading a matrix in blocked format.
 * BLOCKEDTYPE must have the following fields:
 *   rows
 *   columns
 *   nnz
 *   blockrows
 *   blockcols
 *   nnzblocks
 *   blockrow_offset
 *   blockcol_offset
 */
template <class BLOCKEDTYPE, class IT>
FILE * read_bin_blocked_common(BLOCKEDTYPE * A, const char * filename, IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  size_t n;

  // Open file for reading
  FILE * f = fopen(filename, "rb");
  if(f == NULL){
    fprintf(stderr, "Cannot open input file.\n");
    exit(1);
  }

  // Read header
  n  = fread(&(A->rows), sizeof(IT), 1, f);
  n += fread(&(A->columns), sizeof(IT), 1, f);
  n += fread(&(A->nnz), sizeof(IT), 1, f);
  n += fread(&(A->blockrows), sizeof(IT), 1, f);
  n += fread(&(A->blockcols), sizeof(IT), 1, f);
  n += fread(&(A->nnzblocks), sizeof(IT), 1, f);

  A->blockrow_offset = blockrow_offset;
  A->blockcol_offset = blockcol_offset;

  // Sanity check
  assert(n == 6);
  assert(A->blockrows == blockrows);
  assert(A->blockcols == blockcols);
  assert(A->blockrow_offset[A->blockrows] == A->rows);
  assert(A->blockcol_offset[A->blockcols] == A->columns);

  DEBUG(printf("\nReading header done\n"));
  return f;
}

/*
 * Read a blocked sparse matrix in CSBR format from a binary (.block) file.
 */
template <class T, class IT>
int read_bin_blocked(Csbr<T, IT> * A, const char * filename, 
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  IT block_index;
  IT br, br_offset, br_size;
  IT bc, bc_offset;
  IT b, prev_b;
  IT cur_row, cur_col;
  T cur_val;
  size_t fr;

  // Open file, read header and validate
  FILE * f = read_bin_blocked_common<Csbr<T, IT>, IT>(A, filename, blockrow_offset, blockrows, blockcol_offset, blockcols);

  // Allocate memory
  A->blocks = (Csr<T,IT> *) malloc( A->nnzblocks * sizeof(Csr<T,IT>));
  A->blockcol_ind = (IT *) malloc( A->nnzblocks * sizeof(IT));
  A->blockrow_ptr = (IT *) calloc( A->blockrows + 1 , sizeof(IT));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));
  
  if(A->blocks==NULL || A->blockcol_ind==NULL || A->blockrow_ptr==NULL || A->col_ind==NULL || A->val==NULL){
    fprintf(stderr, "Memory allocation failed.");
    return -3;
  }

  DEBUG(puts("Memory allocation done"));

  block_index = UINT_MAX;  // Assuming UINT_MAX + 1 = 0
  prev_b = UINT_MAX;

  // Read data section
  for(IT i=0; i<A->nnz; i++){
    fr  = fread(&b, sizeof(IT), 1, f);
    fr += fread(&cur_row, sizeof(IT), 1, f);
    fr += fread(&cur_col, sizeof(IT), 1, f);
    fr += fread(&cur_val, sizeof(T), 1, f);
    if(fr != 4){
      fprintf(stderr, "Error while reading data section.\n");
      return -4;
    }

    // Check if this is the begining of a new block
    if( b != prev_b){

      // Make sure blocks are sorted correctly
      assert(b > prev_b || prev_b == UINT_MAX);

      block_index++;
      prev_b = b;
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;

      // Block meta data
      A->blockrow_ptr[br+1]++;
      A->blockcol_ind[block_index] = bc;
      
      // CSR block initialize
      A->blocks[block_index].rows = br_size;
      A->blocks[block_index].val = A->val + i;
      A->blocks[block_index].col_ind = A->col_ind + i;
      A->blocks[block_index].row_ptr = (IT *) calloc(br_size + 1, sizeof(IT));
      if(A->blocks[block_index].row_ptr == NULL){
        fprintf(stderr, "Could not allocate memory for block.\n");
        return -5;
      }
    }

    assert(cur_row >= br_offset);
    assert(cur_row  < br_size + br_offset);
    assert(cur_col >= bc_offset);

    A->blocks[block_index].row_ptr[cur_row - br_offset + 1]++;  // Count non-zeros per row
    A->col_ind[i] = cur_col - bc_offset;
    A->val[i] = cur_val;
  }

  fclose(f);

  DEBUG(puts("Reading data section done"));

  // Get row pointers from row non-zero counters
  for (IT bi=0; bi<A->nnzblocks; bi++) {
    for (IT i=0; i<A->blocks[bi].rows; i++) {
      A->blocks[bi].row_ptr[i+1] = A->blocks[bi].row_ptr[i+1] + A->blocks[bi].row_ptr[i];
    }
  }

  for (br=0; br<A->blockrows; br++) {
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }

  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);
  return 0;
}

/*
 * Read a blocked sparse matrix in CSBR(2) format from a binary (.block) file.
 */
template <class T, class IT>
int read_bin_blocked(Csbr2<T, IT> * A, const char * filename, 
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  IT block_index;
  IT br, br_offset, br_size;
  IT bc, bc_offset;
  IT b, prev_b;
  IT cur_row, cur_col;
  T cur_val;
  size_t fr;

  // Open file, read header and validate
  FILE * f = read_bin_blocked_common<Csbr2<T, IT>, IT>(A, filename, blockrow_offset, blockrows, blockcol_offset, blockcols);

  // Allocate memory
  A->blockrow_ptr = (IT *) calloc( A->blockrows + 1 , sizeof(IT));
  A->row_ptr = (IT **) malloc( A->nnzblocks * sizeof(IT *));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  
  if(A->blockrow_ptr==NULL || A->col_ind==NULL || A->val==NULL || A->row_ptr==NULL){
    fprintf(stderr, "Memory allocation failed.");
    return -3;
  }

  DEBUG(puts("Memory allocation done"));

  block_index = UINT_MAX;  // Assuming UINT_MAX + 1 = 0
  prev_b = UINT_MAX;

  // Read data section
  for(IT i=0; i<A->nnz; i++){
    fr  = fread(&b, sizeof(IT), 1, f);
    fr += fread(&cur_row, sizeof(IT), 1, f);
    fr += fread(&cur_col, sizeof(IT), 1, f);
    fr += fread(&cur_val, sizeof(T), 1, f);
    if(fr != 4){
      fprintf(stderr, "Error while reading data section.\n");
      return -4;
    }

    // Check if this is the begining of a new block
    if( b != prev_b){

      // Make sure blocks are sorted correctly
      assert(b > prev_b || prev_b == UINT_MAX);

      block_index++;
      prev_b = b;
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;

      // Block meta data
      A->blockrow_ptr[br+1]++;
      
      // CSR block initialize
      A->row_ptr[block_index] = (IT *) calloc(br_size + 1, sizeof(IT));
      if(A->row_ptr[block_index] == NULL){
        fprintf(stderr, "Could not allocate memory for block.\n");
        return -5;
      }
    }

    assert(cur_row >= br_offset);
    assert(cur_row  < br_size + br_offset);
    assert(cur_col >= bc_offset);

    A->row_ptr[block_index][cur_row - br_offset + 1]++;  // Count non-zeros per row
    A->col_ind[i] = cur_col;
    A->val[i] = cur_val;
  }

  fclose(f);

  DEBUG(puts("Reading data section done"));

  // Accumulate block-row counters to get block-row pointers
  for(br=0; br<A->blockrows; br++){
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }
  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);

  // Accumulate row counters to get row pointers
  IT nnz_count = 0;
  for(IT br=0; br<blockrows; br++){
    IT block_size;
    IT blockrow_start, blockrow_end;

    block_size = A->blockrow_offset[br+1] - A->blockrow_offset[br];
    blockrow_start = A->blockrow_ptr[br];
    blockrow_end = A->blockrow_ptr[br+1];

    for(IT k=blockrow_start; k<blockrow_end; k++){
      A->row_ptr[k][0] = nnz_count;
      for(IT i=0; i<block_size; i++){
        A->row_ptr[k][i+1] = A->row_ptr[k][i+1] + A->row_ptr[k][i];
      }
      nnz_count = A->row_ptr[k][block_size];
    }
  }
  
  return 0;
}

/*
 * Read a blocked sparse matrix in BCSR format from a binary (.block) file.
 */
template <class T, class IT>
int read_bin_blocked(Bcsr<T, IT> * A, const char * filename, 
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  IT br, br_offset, br_size;
  IT bc, bc_offset;
  IT b, prev_b;
  IT cur_row, cur_col;
  T cur_val;
  size_t fr;

  // Open file, read header and validate
  FILE * f = read_bin_blocked_common<Bcsr<T, IT>, IT>(A, filename, blockrow_offset, blockrows, blockcol_offset, blockcols);

  // Allocate memory
  A->blocks = (Csr<T, IT> *) malloc( A->blockrows * A->blockcols * sizeof(Csr<T, IT>));
  A->col_ind = (IT *) malloc( A->nnz * sizeof(IT));
  A->val = (T *) malloc( A->nnz * sizeof(T));
  
  if(A->blocks==NULL || A->col_ind==NULL || A->val==NULL){
    fprintf(stderr, "Memory allocation failed.");
    return -3;
  }

  DEBUG(puts("Memory allocation done"));

  // Mark all blocks as having 0 rows to filter out empty ones
  for(IT i=0; i<A->blockrows*A->blockcols; i++){
    A->blocks[i].rows = 0;
  }

  prev_b = UINT_MAX;

  // Read data section
  for(IT i=0; i<A->nnz; i++){
    fr  = fread(&b, sizeof(IT), 1, f);
    fr += fread(&cur_row, sizeof(IT), 1, f);
    fr += fread(&cur_col, sizeof(IT), 1, f);
    fr += fread(&cur_val, sizeof(T), 1, f);
    if(fr != 4){
      fprintf(stderr, "Error while reading data section.\n");
      return -4;
    }

    // Check if this is the begining of a new block
    if( b != prev_b){
      assert(b > prev_b || prev_b == UINT_MAX);
      prev_b = b;
      br = b / blockcols;
      bc = b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;
      
      // CSR block initialize
      A->blocks[b].row_ptr = (IT *) calloc(br_size + 1, sizeof(IT));
      if(A->blocks[b].row_ptr == NULL){
        fprintf(stderr, "Could not allocate memory for block.\n");
        return -5;
      }
    }

    assert(cur_row >= br_offset);
    assert(cur_row  < br_size + br_offset);
    assert(cur_col >= bc_offset);

    A->blocks[b].row_ptr[cur_row - br_offset + 1]++;  // Count non-zeros per row
    A->col_ind[i] = cur_col - bc_offset;
    A->val[i] = cur_val;
  }

  fclose(f);

  DEBUG(puts("Reading data section done"));

  // Get row pointers from row non-zero counters
  for(IT bi=0; bi<A->nnzblocks; bi++){
    for(IT i=0; i<A->blocks[bi].rows; i++){
      A->blocks[bi].row_ptr[i+1] = A->blocks[bi].row_ptr[i+1] + A->blocks[bi].row_ptr[i];
    }
  }
  
  return 0;
}

/*
 * Read a blocked sparse matrix in CGBR format from a binary (.block) file.
 */
template <class T, class IT, class SIT>
int read_bin_blocked(Cgbr<T, IT, SIT> * A, const char * filename, 
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
  Element<T, IT> * temp_array;
  MatrixType block_type;
  IT block_index, block_nnz, block_start;
  IT br, br_offset, br_size;
  IT bc, bc_offset, bc_size;
  IT b, prev_b;
  IT cur_row, cur_col;
  T cur_val;
  size_t fr;

  // Open file, read header and validate
  FILE * f = read_bin_blocked_common<Cgbr<T, IT, SIT>, IT>(A, filename, blockrow_offset, blockrows, blockcol_offset, blockcols);

  // Allocate memory
  A->blocks = new MatrixBlock<T, IT, SIT>[A->nnzblocks];
  A->blockcol_ind = new IT[A->nnzblocks];
  A->blockrow_ptr = new IT[A->blockrows + 1]();
  
  for(int i=0; i<MATRIX_TYPE_NUM; i++){
    A->type_block_count[i] = 0;
    A->type_nnz_count[i] = 0;
  }

  DEBUG(puts("Memory allocation done"));

  block_index = 0;
  block_start = 0;

  fread(&prev_b, sizeof(IT), 1, f);
  fread(&cur_row, sizeof(IT), 1, f);
  fread(&cur_col, sizeof(IT), 1, f);
  fread(&cur_val, sizeof(T), 1, f);

  // Read data section
  for(IT i=0; i<A->nnz; i++){
    fr =  fread(&b, sizeof(IT), 1, f);
    fr += fread(&cur_row, sizeof(IT), 1, f);
    fr += fread(&cur_col, sizeof(IT), 1, f);
    fr += fread(&cur_val, sizeof(T), 1, f);
    
    // Check if this is the begining of a new block
    if( b != prev_b || fr == 0){
      br = prev_b / blockcols;
      bc = prev_b % blockcols;
      br_offset = blockrow_offset[br];
      bc_offset = blockcol_offset[bc];
      br_size = blockrow_offset[br + 1] - br_offset;
      bc_size = blockcol_offset[bc + 1] - bc_offset;

      // Block meta data
      A->blockrow_ptr[br+1]++;
      A->blockcol_ind[block_index] = bc;
      
      // Read new block in memory
      block_nnz = i - block_start;
      temp_array = new Element<T,IT>[block_nnz];
      if(fr == 4)
        fseek(f, - ((long) ((block_nnz + 1) * (3*sizeof(IT) + sizeof(T)))), SEEK_CUR);
      else
        fseek(f, - ((long) (block_nnz * (3*sizeof(IT) + sizeof(T)))), SEEK_CUR);
    
      for(IT j=0; j<block_nnz; j++){
        fread(&(temp_array[j].block), sizeof(IT), 1, f);
        fread(&(temp_array[j].row), sizeof(IT), 1, f);
        fread(&(temp_array[j].col), sizeof(IT), 1, f);
        fread(&(temp_array[j].val), sizeof(T), 1, f);
 
        assert(temp_array[j].block == prev_b);
        assert(temp_array[j].row   >= br_offset);
        assert(temp_array[j].row   <  br_offset + br_size);
        assert(temp_array[j].col   >= bc_offset);
        assert(temp_array[j].col   <  bc_offset + bc_size);

        temp_array[j].row -= br_offset;
        temp_array[j].col -= bc_offset;
      }

      // Initialize new block
      block_type = Coo_to_MatrixBlock(A->blocks + block_index, br_size, bc_size, temp_array, block_nnz);
      A->type_block_count[block_type]++;
      A->type_nnz_count[block_type] += block_nnz;

      // Prepare for next block
      delete [] temp_array;
      fseek(f, (long)  (3*sizeof(IT) + sizeof(T)), SEEK_CUR);
      block_index++;
      block_start = i;
      prev_b = b;
    }
  }

  fclose(f);

  DEBUG(puts("Reading data section done"));

  for(br=0; br<A->blockrows; br++){
    A->blockrow_ptr[br+1] = A->blockrow_ptr[br+1] + A->blockrow_ptr[br];
  }
  assert(A->blockrow_ptr[blockrows] == A->nnzblocks);
  
  partition_init(A);

  return 0;
}

#endif
