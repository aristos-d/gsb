#ifndef GENERIC_CSR_H
#define GENERIC_CSR_H

#include <iostream>

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
int Coo_to_Csr(CSR<T,IT> *A, Coo<T,IT,IT> *B)
{
    return Coo_to_Csr(A, B->val, B->I, B->J, B->rows, B->columns, B->nnz);
}

template <typename T, typename IT,
          template <typename, typename> class CSR>
int Coo_to_Csr(CSR<T,IT> *A, Coo2<T,IT> *B)
{
    return Coo_to_Csr(A, B->triplets, B->rows, B->columns, B->nnz, false, true);
}

template <typename T, typename IT,
          template <typename, typename> class CSR>
int Coo_to_Csr(CSR<T,IT> *A, Coo3<T,IT> *B)
{
    return Coo_to_Csr(A, B->elements, B->rows, B->columns, B->nnz, false, true);
}

/*
 * Print information about the matrix
 */
template <typename CSRTYPE>
void print_info(CSRTYPE const& A)
{
    std::cout << "CSR matrix : " << A.rows << " rows, " << A.nonzeros() << " non-zeros\n";
}

#endif /* GENERIC_CSR_H */
