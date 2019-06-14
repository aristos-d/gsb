#ifndef _CSBR_COMMON_
#define _CSBR_COMMON_

#include <stdint.h>

// Timer API.
void tick();
double tock();

// Aligned memory API
#ifndef _NO_CSBR_UTILS_
void * aligned_malloc( size_t size );
void aligned_free( void *ptr );
#endif

void print_timing(char *mode, unsigned nnz, int iterations, double ts, double te);
void print_timing(char *mode, unsigned nnz, double tm);
void print_timing(char *method, char *mode, unsigned nnz, double tm);

int string_ends_with(const char * str, const char * suffix);
size_t get_file_size(const char * filename);

uint64_t next_power_of_two(uint64_t n);

unsigned int highest_bit_set(uint64_t v);
int64_t highest_bit_set(int64_t v);
unsigned int highest_bit_set(unsigned int v);
int highest_bit_set(int v);

#endif
