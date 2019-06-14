#ifndef _EXPAND_H_
#define _EXPAND_H_

#include <stdint.h>
#include <assert.h>

#include "typedefs.h"
#include "utils.h"

/*
 * Calculate the new constant block size as the smallest power of two that is
 * larger than all the existing block sizes.
 */
template <class IT>
IT expand_block_size(IT * block_size, IT size)
{
  IT max_block = 0;

  for(IT i=0; i<size; i++)
    if(block_size[i] > max_block)
      max_block = block_size[i];

  return (IT) next_power_of_two(max_block);
}

/*
 * Calculate the new index of a non-zero if all blocks expand to size
 * block_size
 */
template <class IT>
IT expand_index(IT * offset, IT size, IT block_size, IT index)
{
  IT block_index, new_index;

  block_index = index_to_blockindex(offset, size, index);
  new_index = index;
  new_index -= offset[block_index];
  new_index += block_index * block_size;
  return new_index;
}

template <class IT>
IT expand_indeces(IT * indeces, IT size, IT * block_size, IT blocks )
{
  IT * block_offset;
  IT new_block_size;

  new_block_size = expand_block_size(block_size, blocks);
  block_offset = (IT *) malloc((blocks+1) * sizeof(IT));
  size_to_offset(block_offset, block_size, blocks);

  cilk_for(IT i=0; i<size; i++){
    indeces[i] = expand_index(block_offset, blocks, new_block_size, indeces[i]);
  }

  free(block_offset);
  return new_block_size;
}

template <class T, class IT>
IT expand_indeces(Element<T, IT> * indeces, IT size, IT * block_sizes, IT * block_offsets, IT blocks )
{
  IT new_block_size = expand_block_size(block_sizes, blocks);
  
  cilk_for(IT i=0; i<size; i++){
    IT br, bc;
    br = indeces[i].block / blocks;
    bc = indeces[i].block % blocks;
    assert(br * new_block_size >= block_offsets[br]);
    assert(bc * new_block_size >= block_offsets[bc]);
    indeces[i].row = (br * new_block_size - block_offsets[br]) + indeces[i].row;
    indeces[i].col = (bc * new_block_size - block_offsets[bc]) + indeces[i].col;
  }

  return new_block_size;
}

#endif
