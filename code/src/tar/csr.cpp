#include <stdint.h>
#include <cilk/cilk.h>

void serial(const IT * row_ptr, const IT * col_ind, const VT * val, const IT M, 
        const VT * x, VT * y)
{   
  IT row_start, row_end;
  VT res;
  
  row_end = row_ptr[0];

  // For every row
  for (IT i=0; i<M; i++) {
    row_start = row_end;
    row_end = row_ptr[i+1];
    res = y[i];
    for (IT j=row_start; j < row_end; j++)
        res += val[j] * x[col_ind[j]];
    y[i] = res;
  }
}
    
void parallel(const IT * row_ptr, const IT * col_ind, const VT * val, const IT M,
        const VT * x, VT * y)
{
    // For every row, in parallel
    cilk_for (IT i=0; i<M; i++) {
      IT row_start = row_ptr[i];
      IT row_end = row_ptr[i+1];
      VT res = y[i];
      for (IT j=row_start; j<row_end; j++)
          res += val[j] * x[col_ind[j]];
      y[i] = res;
    }
}
