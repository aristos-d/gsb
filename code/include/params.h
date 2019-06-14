#ifndef _PARAMS_H_
#define _PARAMS_H_

#include <stdint.h>
#include <math.h>

// Type parameters
#ifndef VALUETYPE
#define VALUETYPE float
#endif

#ifndef INDEXTYPE
#define INDEXTYPE uint32_t
#endif

#ifndef SMALLINDEXTYPE
#define SMALLINDEXTYPE uint16_t
#endif

#ifndef ITERATIONS
#ifdef BENCH_THREADS
#define ITERATIONS 50
#else
#define ITERATIONS 1000
#endif
#endif

// Used to calculate minimum number of non-zeros in a subblockrow to use Cilk
#ifndef PARTITION_FACTOR
#define PARTITION_FACTOR 1
#endif
#ifndef PARTITION_FACTOR_INV
#define PARTITION_FACTOR_INV 2
#endif

// Initial size of array allocated for partition (not actual number of chunks)
#ifndef INITPARTITION
#define INITPARTITION 4
#endif

// Block dimension as a function of original dimension
#ifndef BLOCK_SIZE_PARAM
#define BLOCK_SIZE_PARAM 4
#endif
#ifndef BLOCK_SIZE
#define BLOCK_SIZE(m, n) (BLOCK_SIZE_PARAM * round(sqrt(sqrt(m * n))))
#endif

// Memory allignment, first argument of alligned_alloc
#define MEMALLIGN 32
#define alligned_malloc(s) aligned_alloc(MEMALLIGN, s)

#define DEFAULT_CSWR_WIDTH 32 

// L2 cache size for tuning
#define KBYTE 1024
#ifndef L2SIZE
#define L2SIZE (256 * KBYTE)
#endif
#ifndef SLACKNESS
#define SLACKNESS 8
#endif

// Uncomment for debugging messages
// #define DEBUG_MODE
// #define DEBUG_MODE_MKL

#ifdef DEBUG_MODE
#define DEBUG(expression) expression
#else
#define DEBUG(expression)
#endif

#ifdef DEBUG_MODE_MKL
#define DEBUG_USE_MKL(expression) expression
#else
#define DEBUG_USE_MKL(expression)
#endif

#endif
