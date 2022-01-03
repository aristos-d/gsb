#ifndef _BASE_H_
#define _BASE_H_

/*
 * Abstract block
 */
template <typename T, typename IT>
class BlockBase {
    protected:
        BlockBase() {}
        BlockBase(IT _nnz) : nnz(_nnz) {}
    public:
        virtual ~BlockBase() {}
        virtual void spmv_block (T const * const x, T * const y) const = 0;
        IT nnz;
};

template <typename T, typename IT>
inline void spmv (BlockBase<T,IT> const * const block, T const * const x, T * const y)
{
    block->spmv_block(x, y);
}

template <typename T, typename IT>
inline void spmv_serial (BlockBase<T,IT> const * const block, T const * const x, T * const y)
{
    // Serial vs parallel SpMV is determined when the block is constructed.
    block->spmv_block(x, y);
}

#endif
