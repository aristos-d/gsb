#ifndef _CSR_H_
#define _CSR_H_

#include <stdio.h>

#include "matrix/coo.1.h"
#include "matrix/coo.2.h"
#include "matrix/coo.3.h"
#include "spmv/csr.h"
#include "spmv/omp/csr.h"

/*
 * Constructor wrappers
 */
template <typename T, typename IT,
          template <typename, typename> class CSR>
int Coo_to_Csr(CSR<T, IT> *A, Coo2<T, IT> * B)
{
    return Coo_to_Csr(A, B->triplets, B->rows, B->columns, B->nnz, false, true);
}

template <typename T, typename IT,
          template <typename, typename> class CSR>
int Coo_to_Csr(CSR<T, IT> *A, Coo3<T, IT> * B)
{
    return Coo_to_Csr(A, B->elements, B->rows, B->columns, B->nnz, false, true);
}

/*
 * Print information about the matrix
 */
template <typename CSRTYPE>
void print_info(CSRTYPE A)
{
    printf("CSR matrix : ");
    printf("%lu rows, %lu non-zeros\n", (unsigned long) A.rows, (unsigned long) nonzeros(A));
}

#endif
