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
 * Structure representing a dense matrix, stored in classic C way.
 */
template <class T, class IT>
struct Dense {
  T * val;
  IT rows;
  IT columns;
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
