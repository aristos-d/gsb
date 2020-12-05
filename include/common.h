#ifndef _CSBR_COMMON_
#define _CSBR_COMMON_

#include <stdint.h>
#include <stdlib.h>

// Timer API.
void tick();
double tock();

// Aligned memory API
void * aligned_malloc(size_t size);
void aligned_free(void *ptr);

void print_timing(char const *mode, unsigned long nnz, double tm);
void print_timing(char const *method, char const *mode, unsigned long nnz, double tm);
void print_timing_verbose (char const *method, unsigned long nnz, unsigned long iter, double *t);
void print_timing_csv (char const *method, unsigned long nnz, unsigned long iter, double *t);

int string_ends_with(const char * str, const char * suffix);
size_t get_file_size(const char * filename);

uint64_t next_power_of_two(uint64_t n);

unsigned int highest_bit_set(uint64_t v);
int64_t highest_bit_set(int64_t v);
unsigned int highest_bit_set(unsigned int v);
int highest_bit_set(int v);

/*
 * Vector initialization with random data
 */
template <class T, class IT>
void vector_random (T **x, IT size)
{
    *x = (T *) aligned_malloc(size * sizeof(T));

    for (IT i=0; i<size; i++)
        (*x)[i] = ((T) rand()) / ((T) RAND_MAX);
}

template <class T, class IT>
void vector_zero (T **x, IT size)
{
    *x = (T *) aligned_malloc(size * sizeof(T));

    for (IT i=0; i<size; i++)
        // This is slower than memset but safer in case we have to deal with a
        // data type whose 0 representation in not all-zero bytes.
        (*x)[i] = (T) 0;
}

template <class T>
void vector_release (T *x)
{
    aligned_free(x);
}
#endif
