#ifndef _PARTITION_H_
#define _PARTITION_H_

#include <stdlib.h>

#include "typedefs.h"
#include "rt.h"
#include "matrix/csbr.h"

/*
 * Partition the blockrow into chunks. Return number of chunks
 */
template <class BLOCKMATRIX, class IT>
IT partition_blockrow(const BLOCKMATRIX * A, BlockRowPartition<IT> * partition, IT start, IT end)
{
    IT b, c = 0, nnz = 0, size = INITPARTITION;
    const IT min_nnz = PARTITION_FACTOR * A->nnz / (PARTITION_FACTOR_INV * RT_WORKERS);

    partition->chunks = (IT *) malloc(INITPARTITION * sizeof(IT));
    partition->chunks[0] = start;

    for (b=start; b<end; b++) {
      nnz += block_nonzeros(A, b);
      
      if (nnz>min_nnz && b<end-1) {  // Break chunk

        c++;
        nnz = 0;

        // Check if a larger array is nedded. If so, reallocate.
        if (c == size) {
          size = 2 * size;
          partition->chunks = (IT *) realloc(partition->chunks, size * sizeof(IT));
        }

        // Previous chunk ends. New chunk begins
        partition->chunks[c] = b;
      }
    }
    
    partition->chunks[c+1] = end;
    partition->nchunks = c+1;
    partition->size = size;
    return c+1;
}

/*
 *  Debugging function to visually inspect block-row partitioning
 */
template <class IT>
void partition_dump(BlockRowPartition<IT> * partition, IT blockrows)
{
    for (IT br=0; br<blockrows; br++) {
        printf("%u,%u:\t", br, partition[br].nchunks);
        for (IT i=0; i<partition[br].nchunks; i++) {
            printf("%u ", partition[br].chunks[i+1] - partition[br].chunks[i]);
        }
        printf("\n");
    }
}

/*
 * Initialize partition datastruct for GSB matrix
 */
template <typename T, typename IT, typename SIT,
          template<typename, typename, typename> class GSB>
void partition_init(GSB<T,IT,SIT> * A)
{
    A->partition = (BlockRowPartition<IT> *) malloc(A->blockrows * sizeof(BlockRowPartition<IT>));
    for (IT br=0; br<A->blockrows; br++) {
        partition_blockrow(A, A->partition + br, A->blockrow_ptr[br], A->blockrow_ptr[br+1]);
    }
}

/*
 * Initialize partition datastruct for CSBR matrix
 */
template <typename T, typename IT,
          template<typename, typename> class GSB>
void partition_init(GSB<T,IT> * A)
{
    A->partition = (BlockRowPartition<IT> *) malloc(A->blockrows * sizeof(BlockRowPartition<IT>));
    for (IT br=0; br<A->blockrows; br++) {
        partition_blockrow(A, A->partition + br, A->blockrow_ptr[br], A->blockrow_ptr[br+1]);
    }
}

/*
 * Clean-up functions
 */
template <typename T, typename IT, typename SIT,
          template<typename, typename, typename> class GSB>
void partition_destroy(GSB<T, IT, SIT> * A)
{
    for (IT br=0; br<A->blockrows; br++) {
        partition_destroy(A->partition + br);
    }
    free(A->partition);
}

template <typename T, typename IT,
          template<typename, typename> class GSB>
void partition_destroy(GSB<T, IT> * A)
{
    for (IT br=0; br<A->blockrows; br++) {
        partition_destroy(A->partition + br);
    }
    free(A->partition);
}

template <class IT>
void partition_destroy(BlockRowPartition<IT> * partition)
{
  free(partition->chunks);
}

template <class IT>
void partition_destroy(BlockRowPartition<IT> partition)
{
  free(partition.chunks);
}

#endif
