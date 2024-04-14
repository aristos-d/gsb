#ifndef _PARTITION_H_
#define _PARTITION_H_

#include <cstdio>
#include <vector>

#include "rt.h"

/*
 * A data structure used to partition block-rows of blocked sparse matrices so
 * that we can have parallelism within the block-row.
 */
template <class IT>
struct BlockRowPartition
{
    std::vector<IT> chunks;
    IT nchunks() { return chunks.size() - 1; }
};

/*
 * Partition the blockrow into chunks. Return number of chunks
 */
template <class Matrix_t, class Index_t>
Index_t partition_blockrow(const Matrix_t * A, BlockRowPartition<Index_t> * partition, Index_t start, Index_t end)
{
    const Index_t min_nnz = PARTITION_FACTOR * A->nnz / (PARTITION_FACTOR_INV * RT_WORKERS);
    Index_t nnz = 0;

    partition->chunks.clear();
    partition->chunks.push_back(start);

    for (Index_t b = start; b < end; b++)
    {
      nnz += A->block_nonzeros(b);

      if (nnz > min_nnz && b < end-1)
      {
        // Previous chunk ends. New chunk begins
        nnz = 0;
        partition->chunks.push_back(b);
      }
    }

    partition->chunks.push_back(end);
    return partition->chunks.size();
}

/*
 *  Debugging function to visually inspect block-row partitioning
 */
template <class Index_t>
void partition_dump(BlockRowPartition<Index_t> * partition, Index_t blockrows)
{
    for (Index_t br=0; br<blockrows; br++)
    {
        printf("%u,%u:\t", br, partition[br].nchunks());
        for (Index_t i=1; i<partition[br].chunks.size(); i++)
        {
            printf("%u ", partition[br].chunks[i] - partition[br].chunks[i-1]);
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
    A->partition = new BlockRowPartition<IT>[A->blockrows];
    for (IT br=0; br<A->blockrows; br++)
    {
        partition_blockrow(A, A->partition + br, A->blockrow_ptr[br], A->blockrow_ptr[br+1]);
    }
}

/*
 * Initialize partition datastruct for CSBR matrix
 */
template <typename T, typename Index_t,
          template<typename, typename> class GSB>
void partition_init(GSB<T,Index_t> * A)
{
    A->partition = new BlockRowPartition<Index_t>[A->blockrows];
    for (Index_t br=0; br<A->blockrows; br++)
    {
        partition_blockrow(A, A->partition + br, A->blockrow_ptr[br], A->blockrow_ptr[br+1]);
    }
}

/*
 * Clean-up
 */
template <typename Matrix_t>
void partition_destroy(Matrix_t * A)
{
    delete [] A->partition;
}

#endif
