#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <numeric>
#include <limits>
#include <cilk/cilk.h>

#include "typedefs.h"
#include "pss/parallel_stable_sort.h"

template <class T, class IT>
IT pick_block_size(IT m, IT n, int workers)
{
    bool ispar = (workers > 1);
    IT roundrowup = next_power_of_two(m);
    IT roundcolup = next_power_of_two(n);
    
    // if indices are negative, highestbitset returns -1, 
    // but that will be caught by the sizereq below
    IT rowbits = highest_bit_set(roundrowup);
    IT colbits = highest_bit_set(roundcolup);
    bool sizereq;
    if (ispar) {
    	sizereq = ( (1 << rowbits) > SLACKNESS * workers) &&
    		      ( (1 << colbits) > SLACKNESS * workers);
    } else {
    	sizereq = ((rowbits > 1) && (colbits > 1));
    }
    
    if (!sizereq) {
    	return 0;
    }
    
    IT rowlowbits = rowbits-1;	
    IT collowbits = colbits-1;	
    IT inf = std::numeric_limits<IT>::max();
    IT maxbits = highest_bit_set(inf);
    
    IT rowhighbits = rowbits-rowlowbits;	// # higher order bits for rows (has at least one bit)
    IT colhighbits = colbits-collowbits;	// # higher order bits for cols (has at least one bit)
    if(ispar) {
    	while( (1 << rowhighbits) < SLACKNESS * workers) {
    		rowhighbits++;
    		rowlowbits--;
    	}
    }
    
    // calculate the space that suby occupies in L2 cache
    IT yL2 = (1 << rowlowbits) * sizeof(T);
    while(yL2 > L2SIZE)
    {
    	yL2 /= 2;
    	rowhighbits++;
    	rowlowbits--;
    }
    
    // calculate the space that subx occupies in L2 cache
    IT xL2 = (1 << collowbits) * sizeof(T);
    while(xL2 > L2SIZE)
    {
    	xL2 /= 2;
    	colhighbits++;
    	collowbits--;
    }
    
    // blocks need to be square for correctness (maybe generalize this later?) 
    while(rowlowbits+collowbits > maxbits)
    {
    	if(rowlowbits > collowbits)
    	{
    		rowhighbits++;
    		rowlowbits--;
    	}
    	else
    	{
    		colhighbits++;
    		collowbits--;
    	}
    }
    while(rowlowbits > collowbits)
    {
    	rowhighbits++;
    	rowlowbits--;
    }
    while(rowlowbits < collowbits)
    {
    	colhighbits++;
    	collowbits--;
    }
    assert (collowbits == rowlowbits);
    
    IT lowrowmask = (1 << rowlowbits) - 1;
    IT lowcolmask = (1 << collowbits) - 1;
    
    double sqrtn = sqrt(sqrt(static_cast<double>(m) * static_cast<double>(n)));
    IT logbeta = static_cast<IT>(ceil(log2(sqrtn))) + 2;
    if(rowlowbits > logbeta)
    {
        rowlowbits = collowbits = logbeta;
        lowrowmask = lowcolmask = ( 1 << logbeta) -1;
        rowhighbits = rowbits-rowlowbits;
        colhighbits = colbits-collowbits;
    }
    return lowrowmask + 1;
}

/*
 * Helping function to convert a row or column index to its corresponding block
 * row or block clumn index.
 */
template <class IT>
inline IT index_to_blockindex(IT * offset, IT size, IT index)
{
  IT * ub = std::upper_bound(offset, offset + size, index);
  return (IT) (ub - offset - 1);
}

/*
 * Convert an array of block sizes to block offsets. Offset vector must be of
 * size (size + 1) because first element is always zero.
 */
template <class IT>
void size_to_offset(IT * offsets, const IT * sizes, IT size)
{
    offsets[0] = 0;
    std::partial_sum(sizes, sizes + size, offsets + 1);
}

/*
 * Sort a triplet array in either row-major or column-major order
 */
template <class T, class IT>
void sort_triplets(Triplet<T, IT> * array, IT size, bool row_major)
{
  // Define function to compare row-major
  auto compare_row = [](Triplet<T, IT> first, Triplet<T, IT> last){
    return (first.row<last.row || (first.row==last.row && first.col<last.col));
  };

  // Define function to compare column-major
  auto compare_col = [](Triplet<T, IT> first, Triplet<T, IT> last){
    return (first.col<last.col || (first.col==last.col && first.row<last.row));
  };

  // Sort
  if (row_major) {
    pss::parallel_stable_sort(array, array + size, compare_row);
  } else {
    pss::parallel_stable_sort(array, array + size, compare_col);
  }
}

/*
 * Sort an element array in either row-major or column-major order
 */
template <class T, class IT>
void sort_triplets(Element<T, IT> * array, IT size, bool row_major)
{
  // Define function to compare row-major
  auto compare_row = [](Element<T, IT> first, Element<T, IT> last){
    return (first.row<last.row || (first.row==last.row && first.col<last.col));
  };

  // Define function to compare column-major
  auto compare_col = [](Element<T, IT> first, Element<T, IT> last){
    return (first.col<last.col || (first.col==last.col && first.row<last.row));
  };

  // Sort
  if (row_major) {
    pss::parallel_stable_sort(array, array + size, compare_row);
  } else {
    pss::parallel_stable_sort(array, array + size, compare_col);
  }
}

/*
 * Interleave bits by Binary Magic Numbers
 * https://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
 * WARNING : This will not work for any index type. Use with caution.
 */
template <class IT>
IT bit_interleave(IT row, IT col)
{
    static const uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static const uint32_t S[] = {1, 2, 4, 8};
    uint32_t x = (uint32_t) row;
    uint32_t y = (uint32_t) col;

    x = (x | (x << S[3])) & B[3];
    x = (x | (x << S[2])) & B[2];
    x = (x | (x << S[1])) & B[1];
    x = (x | (x << S[0])) & B[0];
    
    y = (y | (y << S[3])) & B[3];
    y = (y | (y << S[2])) & B[2];
    y = (y | (y << S[1])) & B[1];
    y = (y | (y << S[0])) & B[0];
    
    return (IT) (x | (y << 1));
}

/*
 * Sort a Triplet array in Morton order
 */
template <class T, class IT, class SIT>
void sort_triplets_morton(Triplet<T, SIT> * array, IT size)
{
    // Comparison function 
    auto compare = [](Triplet<T, SIT> first, Triplet<T, SIT> last){
        return bit_interleave(first.row, first.col) < bit_interleave(last.row, last.col);
    };

    // Sort
    pss::parallel_stable_sort(array, array + size, compare);
}

/*
 * Sort a Triplet array in Morton order
 */
template <class T, class IT, class SIT>
void sort_triplets_morton(Element<T,SIT> * array, IT size)
{
    // Comparison function 
    auto compare = [](Element<T, SIT> first, Element<T, SIT> last){
        return bit_interleave(first.row, first.col) < bit_interleave(last.row, last.col);
    };

    // Sort
    pss::parallel_stable_sort(array, array + size, compare);
}

/*
 * Sort a triplet array in row-major order
 */
template <class T, class IT>
void sort_triplets(Triplet<T, IT> * array, IT size)
{
  sort_triplets(array, size, true);
}

template <class T, class IT>
void sort_triplets(Element<T, IT> * array, IT size)
{
  sort_triplets(array, size, true);
}

/*
 * Sort a triplet array according to the blocks corresponding to the row and
 * column indeces it holds. Blocks are defined by each block offset and number
 * of blocks along each dimension.
 * NOTE: This is slow and should be avoided. Use Element array instead.
 */
template <class T, class IT>
void sort_triplets_blocks(Triplet<T, IT> * array, IT size,
  IT * brow_offset, IT blockrows, IT * bcol_offset, IT blockcols)
{
  IT * brow_ptr;
  IT i;

  // Define function to compare block columns
  auto block_compare = [bcol_offset, blockcols]
                        (Triplet<T, IT> first, Triplet<T, IT> last){
    IT first_bcol = index_to_blockindex(bcol_offset, blockcols, first.col);
    IT last_bcol = index_to_blockindex(bcol_offset, blockcols, last.col);
    return first_bcol < last_bcol ;
  };

  // Define function to compare rows (NOT block-rows)
  auto row_compare = [](Triplet<T, IT> first, Triplet<T, IT> last){
      return  first.row < last.row ;
  };

  // First, sort according to row
  pss::parallel_stable_sort(array, array + size, row_compare);

  // Locate start of each blockrow
  brow_ptr = new IT[blockrows + 1];
  brow_ptr[0] = 0;
  i = 0;
  for (IT br=0; br<blockrows; br++) {
    while(array[i].row < brow_offset[br+1]) i++;
    brow_ptr[br+1] = i;
  }
  brow_ptr[blockrows] = size;

  // Sort every block row according to block-column, row and column
  cilk_for (IT b=0; b<blockrows; b++) {
    IT start = brow_ptr[b];
    IT end = brow_ptr[b+1];
    std::sort(array + start, array + end, block_compare);
  }

  // Clean up
  delete [] brow_ptr;
}

/*
 * Sort an array of Element<T, IT> according to block-id, row and column.
 * If block-id is selected as (block-row) * (number of block-rows) +
 * (block-column), then the array is sorted as in any blocked matrix format.
 */
template <class T, class IT>
void sort_elements_blocks(Element<T, IT> * array, IT size)
{
  auto compare = [](Element<T, IT> first, Element<T, IT> last)
  {
    return ( first.block<last.block || 
           ( first.block==last.block && first.row < last.row ) || 
           ( first.block==last.block && first.row == last.row && first.col<last.col ));
  };

  pss::parallel_stable_sort(array, array + size, compare);
}

/*
 * Sort a triplet array according to wide-row, column and row (in this order).
 * A wide row is a group of `width` rows.
 */
template <class T, class IT>
void sort_triplets_widerows(Triplet<T, IT> * array, IT size, IT width)
{
  auto compare = [width](Triplet<T, IT> first, Triplet<T, IT> last)
  {
    IT wr1, wr2;
    wr1 = first.row / width;
    wr2 = last.row / width;
    return ( wr1 < wr2  || 
           ( wr1 == wr2 && first.col < last.col ) || 
           ( wr1 == wr2 && first.col == last.col && first.row < last.row ));
  };

  pss::parallel_stable_sort(array, array + size, compare);
}

/*
 * Sort an element array according to wide-row, column and row (in this order).
 * A wide row is a group of `width` rows.
 */
template <class T, class IT>
void sort_triplets_widerows(Element<T, IT> * array, IT size, IT width)
{
  auto compare = [width](Element<T, IT> first, Element<T, IT> last)
  {
    IT wr1, wr2;
    wr1 = first.row / width;
    wr2 = last.row / width;
    return ( wr1 < wr2  || 
           ( wr1 == wr2 && first.col < last.col) || 
           ( wr1 == wr2 && first.col == last.col && first.row < last.row ));
  };

  pss::parallel_stable_sort(array, array + size, compare);
}

/*
 * Calculate block id for every element
 */
template <class T, class IT>
void calculate_block_id(Element<T, IT> * elements, IT size,
  IT * blockrow_offset, IT blockrows, IT * blockcol_offset, IT blockcols)
{
    cilk_for (IT i=0; i<size; i++) {
        IT br = index_to_blockindex(blockrow_offset, blockrows, elements[i].row );
        IT bc = index_to_blockindex(blockcol_offset, blockcols, elements[i].col );
        elements[i].block = br * blockcols + bc;
    }
}

/*
 * Operates on a SORTED array of Element<T, IT> and returns the number of
 * non-zero blocks.
 */
template <class T, class IT>
IT count_blocks(Element<T, IT> * array, IT size)
{
  IT current = array[0].block;
  IT count = 1;

  for(IT i=0; i<size; i++){
    if(array[i].block > current){
      count++;
      current = array[i].block;
    }
  }

  return count;
}

/*
 * Operates on a SORTED array of type NONZERO and returns the number of
 * non-zero rows. nnz has to be at greater or equal to 1. NNZERO type must have
 * a 'row' field
 */
template <class NONZERO, class IT>
IT count_nnz_rows(NONZERO * source, IT nnz)
{
  IT count=1;
  IT cur_row = source[0].row;

  for(IT i=0; i<nnz; i++){
    if(source[i].row != cur_row){
      cur_row = source[i].row;
      count++;
    }
  }
  return count;
}

#endif
