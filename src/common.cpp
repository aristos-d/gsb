#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <algorithm>
#include <numeric>

/*
 * Timer code
 */
static std::chrono::time_point<std::chrono::high_resolution_clock> mark;

void tick()
{
    mark = std::chrono::high_resolution_clock::now();
}

double tock()
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - mark;
    return diff.count();
}

/*
 * Print execution time and FLOPs/second
 */
static float mflops (unsigned long nnz, float t)
{
    return 2.0 * nnz / (t * 1000000.0);
}

void print_timing (char const *mode, unsigned long nnz, float t)
{
    printf("Execution time %s : %.6f\n", mode, t);
    printf("MFLOP/s        %s : %.0f\n", mode, mflops(nnz, t));
}

void print_timing (char const *method, char const *mode, unsigned long nnz, float t)
{
    printf("Execution time %s (%s): %.6f\n", method, mode, t);
    printf("MFLOP/s        %s (%s): %.0f\n", method, mode, mflops(nnz, t));
}

void print_timing_verbose (char const *method, unsigned long nnz, unsigned long iter, double *t)
{
    std::sort(t, t+iter);
    print_timing(method, "best", nnz, t[0]);
    print_timing(method, "median", nnz, t[iter/2]);
    print_timing(method, "mean", nnz, std::accumulate(t, t + iter, 0.0) / iter);
    print_timing(method, "worst", nnz, t[iter-1]);
}

void print_timing_csv (char const *method, unsigned long nnz, unsigned long iter, double *t)
{
    std::sort(t, t+iter);
    // The format is: method, best, median, mean, worst in MFLOP/s
    printf("%s,%.0f,%.0f,%.0f,%.0f\n",
            method,
            mflops(nnz, t[0]),
            mflops(nnz, t[iter/2]),
            mflops(nnz, std::accumulate(t, t+iter, 0.0) / iter),
            mflops(nnz, t[iter-1]));
}

/*
 * Copied from
 * https://stackoverflow.com/questions/10347689/how-can-i-check-whether-a-string-ends-with-csv-in-c
 */
int string_ends_with(const char * str, const char * suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    return
      (str_len >= suffix_len) &&
      (0 == strcmp(str + (str_len-suffix_len), suffix));
}

/*
 * Return the size of a file in bytes
 */
size_t get_file_size(const char * filename)
{
  size_t size;
  FILE * f;

  f = fopen(filename, "rb");
  if(f == NULL){
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fclose(f);

  return size;
}

/*
 * Calculate the smallest power of two that is equal or greater than given
 * number.
 */
uint64_t next_power_of_two(uint64_t n)
{
  --n;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;
  ++n;
  return n;
}

/*
 * 64-bit version
 * note: least significant bit is the "zeroth" bit
 * pre: v > 0
 */
unsigned int highest_bit_set(uint64_t v)
{
	// b in binary is {10,1100, 11110000, 1111111100000000 ...}
	const uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
	const unsigned int S[] = {1, 2, 4, 8, 16, 32};
	int i;

	unsigned int r = 0; // result of log2(v) will go here
	for (i = 5; i >= 0; i--) {
		if (v & b[i]) { // highestbitset is on the left half (i.e. v > S[i] for sure)
			v >>= S[i];
			r |= S[i];
		}
	}
	return r;
}

int64_t highest_bit_set(int64_t v)
{
	if(v < 0) {
		return -1;
	} else {
		uint64_t uv = static_cast<uint64_t>(v);
		uint64_t ur = highest_bit_set(uv);
		return static_cast<int64_t> (ur);
	}
}

/*
 * 32-bit version
 * note: least significant bit is the "zeroth" bit
 * pre: v > 0
 */
unsigned int highest_bit_set(unsigned int v)
{
	// b in binary is {10,1100, 11110000, 1111111100000000 ...}
	const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
	const unsigned int S[] = {1, 2, 4, 8, 16};
	int i;

	unsigned int r = 0;
	for (i = 4; i >= 0; i--)
	{
		if (v & b[i])	// highestbitset is on the left half (i.e. v > S[i] for sure)
		{
			v >>= S[i];
			r |= S[i];
		}
	}
	return r;
}

int highest_bit_set(int v)
{
	if (v < 0) {
		return -1;
	} else {	
		unsigned int uv = static_cast<unsigned int> (v);
		unsigned int ur = highest_bit_set(uv);
		return static_cast<int> (ur);
	}
}
