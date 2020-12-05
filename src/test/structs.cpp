#include <stdint.h>
#include <stdio.h>

#include "typedefs.h"
#include "matrix/crcsr.h"

int main(int argc, char * argv[])
{
  printf("Size of uint32 : %lu\n", sizeof(uint32_t));
  printf("Size of int32 : %lu\n", sizeof(int32_t));
  printf("Size of int : %lu\n", sizeof(int));
  printf("Size of long : %lu\n", sizeof(long));
  printf("Size of float : %lu\n", sizeof(float));
  printf("Size of double : %lu\n", sizeof(double));
  printf("Size of NonZero<float, uint32_t> : %lu\n", sizeof(NonZero<float, uint32_t>));
  printf("Size of NonZero<double, uint32_t> : %lu\n", sizeof(NonZero<double, uint32_t>));
  printf("Size of Triplet<float, uint32_t> : %lu\n", sizeof(Triplet<float, uint32_t>));
  printf("Size of Triplet<float, uint16_t> : %lu\n", sizeof(Triplet<float, uint16_t>));
  printf("Size of Triplet<double, uint32_t> : %lu\n", sizeof(Triplet<double, uint32_t>));
  printf("Size of Triplet<double, uint16_t> : %lu\n", sizeof(Triplet<double, uint16_t>));
  printf("Size of Element<float, uint32_t> : %lu\n", sizeof(Element<float, uint32_t>));
  printf("Size of Element<double, uint32_t> : %lu\n", sizeof(Element<double, uint32_t>));
  printf("Size of Coo<float, uint32_t> : %lu\n", sizeof(Coo<float, uint32_t>));
  printf("Size of Coo<double, uint32_t> : %lu\n", sizeof(Coo<double, uint32_t>));
  printf("Size of BlockCoo<float, uint32_t> : %lu\n", sizeof(BlockCoo<float, uint32_t, uint32_t>));
  printf("Size of BlockCoo<double, uint32_t> : %lu\n", sizeof(BlockCoo<double, uint32_t, uint32_t>));
  printf("Size of Csr<float, uint32_t> : %lu\n", sizeof(Csr<float, uint32_t>));
  printf("Size of Csr<double, uint32_t> : %lu\n", sizeof(Csr<double, uint32_t>));
  printf("Size of Csr2<float, uint32_t> : %lu\n", sizeof(Csr2<float, uint32_t>));
  printf("Size of Csr2<double, uint32_t> : %lu\n", sizeof(Csr2<double, uint32_t>));
  printf("Size of CompRowCsr<float, uint32_t> : %lu\n", sizeof(CompRowCsr<float, uint32_t>));
  printf("Size of CompRowCsr<double, uint32_t> : %lu\n", sizeof(CompRowCsr<double, uint32_t>));
  printf("Size of MatrixBlock<float, uint32_t, uint32_t> : %lu\n",
          sizeof(MatrixBlock<float, uint32_t, uint32_t>));
  printf("Size of MatrixBlock<float, uint32_t, uint16_t> : %lu\n",
          sizeof(MatrixBlock<float, uint32_t, uint16_t>));
  printf("Size of MatrixBlock<float, uint32_t, uint8_t> : %lu\n",
          sizeof(MatrixBlock<float, uint32_t, uint8_t>));
  printf("Size of MatrixBlock<double, uint32_t, uint32_t> : %lu\n",
          sizeof(MatrixBlock<double, uint32_t, uint32_t>));
}
