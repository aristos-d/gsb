#ifndef _PERMUTE_H_
#define _PERMUTE_H_

#include <cilk/cilk.h>

// This function is copied and slightly modified from SuiteSParse library
// http://faculty.cse.tamu.edu/davis/suitesparse.html
template <typename IT>
IT * invert_permutation (IT const * p, IT size)
{    
    IT * pinv = new IT[size];        /* allocate result */
    
    cilk_for (IT k=0; k<size; k++) {
        pinv [p [k]] = k ;/* invert the permutation */
    }

    return pinv;
}

/*
 * Applies row/column permutation to a matrix if indeces holds the row/column
 * indeces of the matrix non-zeros (COO format)
 */
template <typename IT>
void permute_indeces(IT * indeces, IT * permutation, IT size)
{
    cilk_for(IT i=0; i<size; i++) {
        indeces[i] = permutation[indeces[i]];
    }
}

template <typename NONZERO, typename IT>
void permute_indeces(NONZERO * matrix, IT nnz, IT * permutation)
{
    cilk_for(IT i=0; i<nnz; i++) {
        matrix[i].row = permutation[matrix[i].row];
        matrix[i].col = permutation[matrix[i].col];
    }
}

#endif
