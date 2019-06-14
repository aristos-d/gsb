#include <stdint.h>
#include "mex.h"
#include "matrix.h"
#include "matrix/cgbr.h"
#include "partition.h"

#define T double
#define IT uint32_t
#define SIT uint16_t

template
void spmv(const Cgbr<T, IT, SIT> * const A,
          const T * __restrict x, T * __restrict y);

template
void spmv_serial(const Cgbr<T, IT, SIT> * const A,
          const T * __restrict x, T * __restrict y);


IT nonzeros(const mxArray * array)
{
    if (mxIsSparse(array)) {
        mwIndex * jc = mxGetJc(array);
        mwSize n = mxGetN(array);
        return (IT) jc[n];
    } else {
        return (IT) (mxGetM(array) * mxGetN(array));
    }
}

bool isValidCell(const mxArray * cell)
{
    return (cell != NULL && 
            mxGetNumberOfDimensions(cell) == 2 &&
            nonzeros(cell) > 0);
}

void mxArraySparse_to_Coo(BlockCoo<T,IT,SIT> * A, mxArray * csc)
{   
    const IT nnz = (IT) nonzeros(csc);
    const IT columns = (IT) mxGetN(csc);
    const mwIndex * jc = mxGetJc(csc);
    const mwIndex * ir = mxGetIr(csc);
    double * val = mxGetPr(csc);
    IT i;

    allocate(A, nnz);
    
    i = 0;
    for (IT c=0; c<columns; c++) {
        for (IT j=jc[c]; j<jc[c+1]; j++) {
            A->I[i] = ir[j];
            A->J[i] = c;
            A->val[i] = val[j];
            i++;
        }
    }

    assert(i == A->nnz);
}

void mxArraySparse_to_Csr(BlockCsr<T,IT,SIT> * A, mxArray * csc)
{  

}

void mxArrayDense_to_Dense(Dense<T,IT> * A, mxArray * dense)
{
    T * data = mxGetPr(dense);
        
    A->rows = (IT) mxGetM(dense);
    A->columns = (IT) mxGetN(dense);
    A->val = (T *) malloc (A->rows * A->columns * sizeof(T));

    // NOTE: Consider re-using original data or at least to NOT trasnpose the
    //       matrix and keep it column-major.
    for(IT i=0; i<A->rows; i++)
        for(IT j=0; j<A->columns; j++)
            A->val[i*A->columns + j] = data[i + j*A->rows];

}

MatrixType mxArray_to_MatrixBlock(MatrixBlock<T,IT,SIT> * block, mxArray * cell)
{
    if (mxIsSparse(cell)) {

        float nnzratio = ((float) nonzeros(cell)) / (mxGetM(cell) * mxGetN(cell));
        
        if (nnzratio < 0.01f || true) {
            mxArraySparse_to_Coo(&(block->matrix.coo), cell);
            block->type = BLOCK_COO;
        } else {
            mxArraySparse_to_Csr(&(block->matrix.csr), cell);
            block->type = BLOCK_CSR;
        }

    } else {
        mxArrayDense_to_Dense(&(block->matrix.dense), cell);
        block->type = BLOCK_DENSE;
    }

    return block->type;
}

void mxArray_to_Cgbr(Cgbr<T, IT, SIT> * A, const mxArray * B)
{
    const mwSize * dims = mxGetDimensions(B);
    mxArray * cell;
    IT cellIndex, blockIndex;
    IT rows, columns;
    
    A->rows = 0;
    A->columns = 0;
    A->nnz = 0;
    A->blockrows = (IT) dims[0];
    A->blockcols = (IT) dims[1];
    A->blockrow_offset = (IT *) calloc (A->blockrows + 1, sizeof(IT));
    A->blockcol_offset = (IT *) calloc (A->blockcols + 1, sizeof(IT));
    A->nnzblocks = 0;

    // Iterate over every cell to calculate number of non-zero blocks and size
    // of blocks.
    for(IT br=0; br<A->blockrows; br++){
        for(IT bc=0; bc<A->blockcols; bc++){
            cellIndex = bc*A->blockrows + br;
            cell = mxGetCell(B, cellIndex); // Column-major indexing!
            if (isValidCell(cell)) {
                rows = mxGetM(cell);
                columns = mxGetN(cell);
                if (rows > A->blockrow_offset[br+1]) A->blockrow_offset[br+1] = rows;
                if (columns > A->blockcol_offset[bc+1]) A->blockcol_offset[bc+1] = columns;
                A->nnzblocks++;
            }   
        }
    }

    // Accumulate block size to get block offsets
    for(IT br=0; br<A->blockrows; br++){
        A->blockrow_offset[br+1] += A->blockrow_offset[br];
    }
    for(IT bc=0; bc<A->blockcols; bc++){
        A->blockcol_offset[bc+1] += A->blockcol_offset[bc];
    }

    A->rows = A->blockrow_offset[A->blockrows];
    A->columns = A->blockcol_offset[A->blockcols];
    
    // Allocate MatrixBlock array
    A->blocks = (MatrixBlock<T,IT,SIT> *) malloc (A->nnzblocks * sizeof(MatrixBlock<T,IT,SIT>));
    A->blockcol_ind = (IT *) malloc(A->nnzblocks * sizeof(IT));
    A->blockrow_ptr = (IT *) calloc(A->blockrows + 1, sizeof(IT));

    if (A->blocks == NULL ||
        A->blockcol_ind == NULL ||
        A->blockrow_ptr == NULL) {
        mexPrintf("ERROR in memory allocation\n");
    }

    // Iterate over every cell to fill the blocks
    blockIndex = 0;
    for(IT br=0; br<A->blockrows; br++){
        for(IT bc=0; bc<A->blockcols; bc++){
            cellIndex = bc*A->blockrows + br;
            cell = mxGetCell(B, cellIndex); // Column-major indexing!
            if (isValidCell(cell)) { 
                
                mxArray_to_MatrixBlock(A->blocks + blockIndex, cell);

                A->blockrow_ptr[br+1]++;
                A->blockcol_ind[blockIndex] = bc;
                A->nnz += nonzeros(cell);
                blockIndex++;
            }   
        }
    }

    for (IT br=0; br<A->blockrows; br++) {
        A->blockrow_ptr[br+1] += A->blockrow_ptr[br];
    }

    partition_init(A);

    DEBUG( mexPrintf("CGBR:\t(%u, %u, %u)\n", A->rows, A->columns, A->nnz) );
    DEBUG( mexPrintf("\t(%u, %u, %u)\n", A->blockrows, A->blockcols, A->nnzblocks) );
}

template
void release(MatrixBlock<T, IT, SIT> *A);

template
void release(Cgbr<T, IT, SIT> A);
