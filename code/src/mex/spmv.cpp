#include <stdint.h>

#include "mex.h"
#include "typedefs.h"

void mxArray_to_Cgbr(Cgbr<double,uint32_t,uint16_t> * A, const mxArray * B);

template <class T, class IT, class SIT>
void spmv(const Cgbr<T,IT,SIT> * const A, const T * x, T * y);

template <class T, class IT, class SIT>
void spmv_serial(const Cgbr<T,IT,SIT> * const A, const T * x, T * y);

template <class T, class IT, class SIT>
void release(Cgbr<T, IT, SIT> A);

/* The gateway function. */ 
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[])
{
    double *x, *y;
    mwSize D, N, M;
    Cgbr<double,uint32_t,uint16_t> A;

    /* Check for proper number of arguments */
    if (nrhs != 2) {
        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargin",
                          "SPMV requires two input arguments.");
    }

    if (nlhs != 1) {
        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargout",
                          "SPMV requires one output argument.");
    }

    // Input 0 should be a 2D cell array of matrix blocks
    if (mxGetNumberOfDimensions(prhs[0]) != 2 || 
        !mxIsCell(prhs[0])) {
        mexErrMsgIdAndTxt("MATLAB:mexcpp:typeargin",
                          "First argument has to be a 2D cell array");
    }

    // Input 1 should be a Mx1 (not 1XM) vector where M is the second dimension
    // of the matrix contained by input 0 (the full matrix).
    if (!mxIsDouble(prhs[1]) ||                                   // not double
        mxIsComplex(prhs[1])) {                                   // or complex
        mexErrMsgIdAndTxt("MATLAB:mexcpp:typeargin",
                          "Second argument has to be a vector of type 'double'.");
    }

    // Extract CGBR matrix
    mxArray_to_Cgbr(&A, prhs[0]);
    
    // Perform dimensionality checks
    M = A.rows;
    N = A.columns;
    if (N != mxGetM(prhs[1])) {
        mexErrMsgIdAndTxt("MATLAB:mexcpp:typeargin", "Dimensional mis-match.");
    }

    // Allocate memory for result
    plhs[0] = mxCreateDoubleMatrix(M, 1, mxREAL);

    // Go
    y = mxGetPr(plhs[0]);
    x = mxGetPr(prhs[1]);
    spmv(&A, x, y);

    // Cleanup
    release(A);
}
