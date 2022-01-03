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

#endif
