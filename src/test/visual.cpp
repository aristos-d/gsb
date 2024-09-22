#include <cstdint>
#include <cstdio>

#include "matrix/coo.1.h"
#include "matrix/csr.1.h"
#include "matrix/csbr.h"
#include "matrix/csbr.2.h"
#include "io/input.h"
#include "test/utils.h"

int main(int argc, char* argv[])
{
  Coo2<float, uint32_t> A;
  Csr<float, uint32_t> B;
  Csbr<float, uint32_t> C;
  Coo3<float, uint32_t> D;
  Csbr2<float, uint32_t> E;

  constexpr uint32_t M = 8, N = 10, NNZ = 11;
  uint32_t I[] = {1, 1, 2, 3, 4,  2, 4, 5, 0, 4, 7};
  uint32_t J[] = {1, 3, 2, 0, 4,  9, 0, 5, 6, 9, 2};
  float val[] = {1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 3.1};

  uint32_t * br = new uint32_t[4];
  uint32_t * bc = new uint32_t[5];
  br[0] = 0;  br[1] = 3;  br[2] = 4;  br[3] = 8;
  bc[0] = 0;  bc[1] = 2;  bc[2] = 3;  bc[3] = 7; bc[4] = 10;

  float x[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float y[M];

  Coo_to_Coo(&A, val, I, J, M, N, NNZ);
  Coo_to_Coo(&D, val, I, J, M, N, NNZ);

  printf("-------- A (COO) --------\n");
  show(A);

  // Conversion COO -> CSR
  Coo_to_Csr(&B, &A);

  printf("-------- A (sorted) --------\n");
  show(A);

  printf("-------- B (CSR) --------\n");
  show(B);

  for(int i=0;i<M;i++) y[i] = 0.0f;
  B.spmv(x, y);
  printf("-------- B * x --------\n");
  show(y, M);

  // Conversion COO -> CSBR (COO is no longer usable)
  Coo_to_Csbr(&C, &A, br, (uint32_t) 3, bc, (uint32_t) 4);

  for(int i=0;i<M;i++) y[i] = 0.0f;
  C.spmv(x, y);
  printf("-------- C (CSBR) * x --------\n");
  show(y, M);

  Coo_to_Csbr(&E, &D, br, (uint32_t) 3, bc, (uint32_t) 4);

  for(int i=0;i<M;i++) y[i] = 0.0f;
  E.spmv(x, y);
  printf("-------- E (CSBR(2)) * x --------\n");
  show(y, M);

  release(A);
  release(B);
  release(C);
  return 0;
}
