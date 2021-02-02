#include <chrono>

#include "test/utils.h"
#include "common.h"

template <class MATRIX, class T>
void benchmark_spmv_core (
        MATRIX A, unsigned iterations,
        char const *method,
        void (*print)(char const *, unsigned long, unsigned long, double *))
{
    T *x, *y;
    double *t;
    
    vector_random(&x, A.columns);
    vector_zero(&y, A.rows);
    t = new double[iterations];
    
    spmv(&A, x, y); 
    for (unsigned i=0; i<iterations; i++) { 
        auto start = std::chrono::high_resolution_clock::now();
        spmv(&A, x, y);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> lap = end - start;
        t[i] = lap.count();
    }

    print(method, nonzeros(A), iterations, t);

    // Free memory
    vector_release(y);
    vector_release(x);
    delete[] t;
}

template <class T, class IT,
          template <typename, typename> class MATRIX>
void benchmark_spmv (MATRIX<T,IT> const A, unsigned iterations,
                     char const *method)
{
    benchmark_spmv_core<MATRIX<T,IT>,T>(A, iterations, method, print_timing_verbose);
}

template <class T, class IT, class SIT,
          template <typename, typename, typename> class MATRIX>
void benchmark_spmv (MATRIX<T,IT,SIT> const A, unsigned iterations,
                     char const *method)
{
    benchmark_spmv_core<MATRIX<T,IT,SIT>,T>(A, iterations, method, print_timing_verbose);
}

template <class T, class IT,
          template <typename, typename> class MATRIX>
void benchmark_spmv_csv (MATRIX<T,IT> const A, unsigned iterations,
                         char const *method)
{
    benchmark_spmv_core<MATRIX<T,IT>,T>(A, iterations, method, print_timing_csv);
}

template <class T, class IT, class SIT,
          template <typename, typename, typename> class MATRIX>
void benchmark_spmv_csv (MATRIX<T,IT,SIT> const A, unsigned iterations,
                         char const *method)
{
    benchmark_spmv_core<MATRIX<T,IT,SIT>,T>(A, iterations, method, print_timing_csv);
}

