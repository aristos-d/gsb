#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_
#include <iostream>
#include <algorithm>
#include <chrono>
#include <math.h>

#include "typedefs.h"
#include "common.h"

#define BENCH_AVG( EXP, TIMES, T_AVG ) \
{\
    EXP; \
    auto start = std::chrono::high_resolution_clock::now(); \
    for(unsigned i=0; i<TIMES; i++){ EXP; } \
    auto end = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double> lap = end - start; \
    T_AVG = lap.count() / (TIMES);\
}

template <class T>
void show (T * x, int size)
{
  for (int i=0; i<size; i++) {
    std::cout << i << ": " << x[i] << '\n';
  }
}

template <class T, class IT>
void show (Triplet<T, IT> x)
{   
  std::cout << "(" << x.row << ", " << x.col << ", " << x.val << ")\n";
}

/*
 * Compute some statistics on timing data
 */
template <class T, class IT>
T mean (T * data, IT n)
{
    T sum = 0;
    for (IT i=0; i<n; i++)
        sum += data[i];
    return sum / (T) n;
}

template <class T, class IT>
T median (T * data, IT n)
{
    std::sort(data, data + n);
    if(n % 2)
        return data[n/2];
    else
        return (data[(n/2)-1] + data[n/2]) / 2.0;
}

template <class T, class IT>
T standard_deviation (T * data, IT n)
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
int check_results (T * vec1, T * vec2, IT size)
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
