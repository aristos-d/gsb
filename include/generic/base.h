#ifndef _BASE_H_
#define _BASE_H_

/*
 * Abstract block
 */
template <typename T, typename IT>
class BlockBase
{
    protected:
        BlockBase() {}
        BlockBase(IT _nnz) : nnz(_nnz) {}
    public:
        virtual ~BlockBase() {}
        virtual void spmv (T const * const x, T * const y) const = 0;

        IT nnz;
        IT nonzeros() const { return nnz; }
};

#endif
