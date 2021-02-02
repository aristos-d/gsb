#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include "params.h"

template <class T, class IT>
struct NonZero {
  T val;
  IT index;
};

template <class T, class IT>
struct Triplet {
  IT row;
  IT col;
  T val;
};

template <class T, class IT>
struct Element {
  IT row;
  IT col;
  T val;
  IT block;
};

/*
 * Structure representing a dense matrix, stored in clasic C way.
 */
template <class T, class IT>
struct Dense {
  T * val;
  IT rows;
  IT columns;
};

/*
 * CSR matrix represented as an array of row pointers and two arrays for
 * non-zero values and column indexes.
 */
template <class T, class IT>
struct Csr {
  T * val;
  IT * col_ind;
  IT * row_ptr;
  IT rows, columns;
  // In 64-bit machine, with 32-bit indeces, adding column information does not
  // increase struct size due to padding. It is not technically neccesary.
};

/*
 * CSR matrix represented as an array of row pointers an array of value -
 * column index pairs.
 */
template <class T, class IT>
struct Csr2 {
  NonZero<T, IT> * nonzeros;
  IT * row_ptr;
  IT rows, columns;
};

/*
 * CSR matrix represented as an array of row pointers and two arrays for
 * non-zero values and column indexes.
 */
template <class T, class IT, class SIT>
struct BlockCsr {
  T * val;
  SIT * col_ind;
  IT * row_ptr;
  IT rows, columns;
  // In 64-bit machine, with 32-bit indeces, adding column information does not
  // increase struct size due to padding. It is not technically neccesary.
};

/*
 * Enumeration of all matrix types used for blocks.
 */
enum MatrixType {
  BLOCK_COO             = 0,
  BLOCK_CSR             = 1,
  BLOCK_DENSE           = 2,
  BLOCK_CSC             = 3,
  BLOCK_CSR_PAR         = 4,
  BLOCK_COMP_ROW_CSR    = 5,
  BLOCK_CUSTOM_1        = 6,
  BLOCK_CUSTOM_2        = 7,
  BLOCK_CUSTOM_3        = 8,
  BLOCK_CUSTOM_4        = 9
};
#define MATRIX_TYPE_NUM 10

template <class IT>
struct BlockRowPartition {
  IT * chunks;
  IT size;     // Size of array allocated
  IT nchunks;  // Number of chunks in partition ( nchunks <= size )
};

/*
 * CSR matrix containing CSR blocks of variable size or Compressed Sparse
 * Block-Rows.
 */
template <class T, class IT>
struct Csbr {
  Csr<T,IT> * blocks;
  IT * blockrow_ptr;         // Indeces for blocks array
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
};

/*
 * CSR matrix containing CSR blocks of variable size.
 */
template <class T, class IT>
struct Csbr2 {
  IT * blockrow_ptr;         // Indexes for row_ptr array

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
 * Dense matrix of CSR blocks of variable size or Blocked Compressed Sparse
 * Rows
 */
template <class T, class IT>
struct Bcsr {
  // Block information
  IT * blockrow_offset;
  IT * blockcol_offset;
  IT blockrows;
  IT blockcols;

  // Partitioning information
  BlockRowPartition<IT> * partition;
  bool balanced;

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
};

/*
 * CSR matrix containing blocks of variable size and type BUT ONLY ONE TYPE or
 * Static Generalized Sparse Blocks
 */
template <typename T, typename IT, typename SIT,
          template <typename, typename, typename> class BLOCK>
struct Sgsb {
  BLOCK<T,IT,SIT> * blocks;
  IT * blockrow_ptr;         // Indexes for blocks array
  IT * blockcol_ind;

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

#endif
