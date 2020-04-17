#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include <iostream>
#include <algorithm>
#include <chrono>
#include <math.h>

#include "typedefs.h"
#include "common.h"

#define BENCH_VERBOSE( EXP, TIMES, NNZ, METH) \
{\
    double * t = new double[TIMES]; \
    EXP; \
    for(unsigned i=0; i<TIMES; i++){ \
        auto start = std::chrono::high_resolution_clock::now(); \
        EXP; \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> lap = end - start; \
        t[i] = lap.count(); \
    } \
    std::sort(t, t+TIMES); \
    print_timing(METH, "min", NNZ, t[0]);  \
    print_timing(METH, "median", NNZ, t[TIMES/2]); \
    print_timing(METH, "mean", NNZ, std::accumulate(t, t+(TIMES), 0.0) / (TIMES) ); \
    print_timing(METH, "max", NNZ, t[TIMES-1]); \
    delete [] t; \
}

#define BENCH_CSV( EXP, TIMES, NNZ, METH) \
{\
    double t[TIMES]; \
    EXP; \
    for(unsigned i=0; i<TIMES; i++){ \
        auto start = std::chrono::high_resolution_clock::now(); \
        EXP; \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> lap = end - start; \
        t[i] = lap.count(); \
    } \
    std::sort(t, t+TIMES); \
    printf("%s,%.0f,%.0f,%.0f,%.0f\n", \
            METH, \
            (2.0 * NNZ / t[0]), \
            (2.0 * NNZ / t[TIMES/2]), \
            (2.0 * NNZ / (std::accumulate(t, t+(TIMES), 0.0) / (TIMES))), \
            (2.0 * NNZ / t[TIMES-1])); \
}

#define BENCH_AVG( EXP, TIMES, T_AVG ) \
{\
    EXP; \
    auto start = std::chrono::high_resolution_clock::now(); \
    for(unsigned i=0; i<TIMES; i++){ EXP; } \
    auto end = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double> lap = end - start; \
    T_AVG = lap.count() / (TIMES);\
}

#ifndef FORMAT_CSV
#define BENCH BENCH_VERBOSE
#else
#define BENCH BENCH_CSV
#endif

template <class T>
void show(T * x, int size)
{
  for(int i=0; i<size; i++){
    std::cout << i << ": " << x[i] << '\n';
  }
}

template <class T, class IT>
void show(Triplet<T, IT> x)
{   
  std::cout << "(" << x.row << ", " << x.col << ", " << x.val << ")\n";
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <class T, class IT>
void show(Csr<T, IT> A)
{
  if(A.rows > 20){
    std::cout << "Printing a matrix with more than 20 rows is a bad idea.\n";
  }else{
    for(IT i=0; i<A.rows; i++){
      std::cout << i << ": ";
      IT row_start, next_row;
      row_start = A.row_ptr[i];
      next_row = A.row_ptr[i+1];
      for(IT k=row_start; k < next_row; k++){
          std::cout << '(' << A.col_ind[k] << ',' << A.val[k] << ')' << '\t';
      }
      std::cout << '\n';
    }
  }
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <class T, class IT>
void show(Coo<T, IT> A)
{
  if(A.nnz > 20){
    std::cout << "Printing a matrix with more than 20 non-zeros is a bad idea.\n";
  }else{
    for(IT i=0; i<A.nnz; i++)
        std::cout << '(' << A.I[i] << ", " << A.J[i] << ", " << A.val[i] << ')' << '\n';
    std::cout << '\n';
  }
}

/*
 * Prints the matrix in a readable format. For debugging purposes only.
 */
template <class T, class IT>
void show(Coo2<T, IT> A)
{
  if(A.nnz > 20){
    std::cout << "Printing a matrix with more than 20 non-zeros is a bad idea.\n";
  }else{
    for(IT i=0; i<A.nnz; i++)
        show(A.triplets[i]);
    std::cout << '\n';
  }
}

/*
 * Compute the mean value of data in an array
 */
template <class T, class IT>
T mean(T * data, IT n)
{
    T sum = 0;
    for (IT i=0; i<n; i++)
        sum += data[i];
    return sum / (T) n;
}

/*
 *
 */
template <class T, class IT>
T median(T * data, IT n)
{
    std::sort(data, data + n);
    if(n % 2)
        return data[n/2];
    else
        return (data[(n/2)-1] + data[n/2]) / 2.0;
}

/*
 * Compute standard deviation of data in an array
 */
template <class T, class IT>
T standard_deviation(T * data, IT n)
{
    T m = mean(data, n);
    T sum = 0;
    for (IT i=0; i<n; i++)
        sum += (data[i] - m)*(data[i] - m);
    return sqrt(sum/n);
}

/*
 * Check if two result vectors are equal. Prints a message in both cases.
 * Returns 0 if results are identical, -1 otherwise.
 */
template <class T, class IT>
int check_results(T * vec1, T * vec2, IT size)
{
    T diff, mse, nmse;
    T max_diff = (T) 0;
    T sq_error = (T) 0;
    T norm = (T) 0;
    IT wrong = 0;

    printf("Checking result...\n");

    // Examine vectors
    for (IT i = 0; i<size; i++) {
        if (vec1[i] != vec2[i]) {
            wrong++;
            diff = vec1[i] - vec2[i];
            sq_error += diff * diff;
            if (std::abs(diff) > max_diff) max_diff = std::abs(diff);
        }
        norm += vec1[i] * vec1[i];
    }
      
    mse = sq_error / size;
    nmse = mse / norm;

    // Print results
    if (wrong>0) {
        printf("\tResults differ in %d elements\n", wrong);
        printf("\tMSE  : %g\n", mse);
        printf("\tNMSE : %g\n", nmse);
        printf("\tMaximum error : %g\n", max_diff);
        return -1;
    } else {
        printf("Calculations produced the same result\n");
        return 0;
    }
}

#endif
